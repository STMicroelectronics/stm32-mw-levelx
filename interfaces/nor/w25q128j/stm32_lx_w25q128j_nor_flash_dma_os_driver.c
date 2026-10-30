/***************************************************************************
  * Copyright (c) 2024 Microsoft Corporation
  * Copyright (c) 2026 STMicroelectronics
  *
  * This program and the accompanying materials are made available under the
  * terms of the MIT License which is available at
  * https://opensource.org/licenses/MIT.
  *
  * SPDX-License-Identifier: MIT
  **************************************************************************/

#include "stm32_lx_w25q128j_nor_flash_dma_os_driver.h"

#ifndef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#error "LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE must be defined in the lx_user.h"
#endif /* LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE */
#ifdef LX_DIRECT_READ
#error "LX_DIRECT_READ feature is not supported. It must be disabled"
#endif /* LX_DIRECT_READ */

static inline UINT nor_flash_check_status(w25q128j_obj_t *pobj, ULONG max_timeout);

static void w25q128j_read_cplt_callback(w25q128j_obj_t *pobj, void *arg);
static void w25q128j_write_cplt_callback(w25q128j_obj_t *pobj, void *arg);

static UINT stm32_lx_nor_flash_driver_read(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *destination,
                                           ULONG words);
static UINT stm32_lx_nor_flash_driver_write(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *source, ULONG words);

static UINT stm32_lx_nor_flash_driver_block_erase(LX_NOR_FLASH *nor_flash, ULONG block, ULONG erase_count);
static UINT stm32_lx_nor_flash_driver_block_erased_verify(LX_NOR_FLASH *nor_flash, ULONG block);

/**
  * @brief Check the status of the memory part
  * @param pobj w25q128j_obj_t pointer
  * @retval LX_SUCCESS if the memory part is ready, LX_ERROR otherwise
  */
static inline UINT nor_flash_check_status(w25q128j_obj_t *pobj, ULONG max_timeout)
{
  TickType_t start = xTaskGetTickCount();

  w25q128j_status_t w25q128j_status = W25Q128J_ERROR;
  w25q128j_status_t p_status = W25Q128J_ERROR;

  while ((xTaskGetTickCount() - start) < pdMS_TO_TICKS(max_timeout))
  {
    /* get current status of the flash memory */
    w25q128j_status = w25q128j_get_status(pobj, &p_status);
    if (w25q128j_status == W25Q128J_OK)
    {
      return LX_SUCCESS;
    }
  }

  return LX_ERROR;
}

/**
  * @brief Entry function for the LevelX Driver
  * @param nor_flash a pointer to a LX_NOR_FLASH object
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */

UINT stm32_lx_w25q128j_nor_flash_dma_os_driver_initialize(LX_NOR_FLASH *nor_flash)
{
  UINT ret = LX_SUCCESS;

  w25q128j_info_t nor_flash_info;
  w25q128j_obj_t *p_w25q128j_obj;
  ULONG block_size;
  ULONG start_address = 0U;
  ULONG max_block_count;
  ULONG available_flash_size;

  /* initialize the LX_NOR_FLASH object with relevant data */
  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* check if context is valid to prevent a hard fault.  */
  if (nor_ctx == NULL)
  {
    return LX_ERROR;
  }
  /* check that the nor_flash_get_driver_object function pointer is correctly set */
  if (nor_ctx->nor_flash_get_driver_object == NULL)
  {
    return LX_ERROR;
  }

  /* retrieve the memory part object */
  p_w25q128j_obj = (w25q128j_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* create semaphores for transfer notification if not already created */
  if (nor_ctx->data_rx_cplt_semaphore.semaphore_handle == NULL)
  {
    nor_ctx->data_rx_cplt_semaphore.semaphore_handle = xSemaphoreCreateBinary();
    if (nor_ctx->data_rx_cplt_semaphore.semaphore_handle == NULL)
    {
      return LX_ERROR;
    }
  }

  if (nor_ctx->data_tx_cplt_semaphore.semaphore_handle == NULL)
  {
    nor_ctx->data_tx_cplt_semaphore.semaphore_handle = xSemaphoreCreateBinary();
    if (nor_ctx->data_tx_cplt_semaphore.semaphore_handle == NULL)
    {
      return LX_ERROR;
    }
  }

  /* get the memory part characteristics */
  w25q128j_get_info(p_w25q128j_obj, &nor_flash_info);

  /* start with the default erase block size reported by the flash driver (64K) */
  block_size = nor_flash_info.erase_block_64k_size;

  /* customize the erase block size */
  if (nor_ctx->nor_flash_flags & STM32_LX_NOR_FLAG_BLOCK_SIZE)
  {
    /* accept only sizes supported by w25q128j memory: sector size (4K), subblock size (32K) or block size (64K) */
    if ((nor_ctx->nor_flash_block_size != nor_flash_info.erase_sector_size) &&
        (nor_ctx->nor_flash_block_size != nor_flash_info.erase_block_32k_size) &&
        (nor_ctx->nor_flash_block_size != nor_flash_info.erase_block_64k_size))
    {
      return LX_ERROR;
    }

    block_size = nor_ctx->nor_flash_block_size;
  }

  /* compute how many logical erase blocks fit in the full flash with the selected block size */
  max_block_count = nor_flash_info.flash_size / block_size;

  /* register DMA R/W callbacks */
  w25q128j_register_read_cplt_callback(p_w25q128j_obj, w25q128j_read_cplt_callback,
                                          (void *)&nor_ctx->data_rx_cplt_semaphore);
  w25q128j_register_write_cplt_callback(p_w25q128j_obj, w25q128j_write_cplt_callback,
                                           (void *)&nor_ctx->data_tx_cplt_semaphore);


  /* customize the flash start address */
  if (nor_ctx->nor_flash_flags & STM32_LX_NOR_FLAG_START_ADDRESS)
  {
    /* ensure base address is within the flash address range */
    if (nor_ctx->nor_flash_start_address >= max_block_count)
    {
      return LX_ERROR;
    }

    start_address = nor_ctx->nor_flash_start_address;
  }

  /* compute the available flash size from the selected start address to the end of the NOR flash */
  available_flash_size = nor_flash_info.flash_size - (start_address * block_size);

  /* customize the flash size limit */
  if (nor_ctx->nor_flash_flags & STM32_LX_NOR_FLAG_FLASH_SIZE)
  {
    /* ensure the configured sub-range is non-zero, block-aligned, and does not exceed the available space */
    if ((nor_ctx->nor_flash_size == 0U) ||
        ((nor_ctx->nor_flash_size % block_size) != 0U) ||
        (nor_ctx->nor_flash_size > available_flash_size))
    {
      return LX_ERROR;
    }

    /* restrict the managed region to the requested size */
    available_flash_size = nor_ctx->nor_flash_size;
  }

  /* set the flash base address */
  nor_flash->lx_nor_flash_base_address = (ULONG *)(start_address * block_size);
  /* set the total number of managed flash blocks */
  nor_flash->lx_nor_flash_total_blocks = available_flash_size / block_size;
  /* set the number of ULONG words per flash block */
  nor_flash->lx_nor_flash_words_per_block = block_size / sizeof(ULONG);

  /* set the intermediate read buffer */
  nor_flash->lx_nor_flash_sector_buffer = nor_ctx->nor_flash_read_buffer;

  /* initialize the fops pointers */
  nor_flash->lx_nor_flash_driver_read = stm32_lx_nor_flash_driver_read;
  nor_flash->lx_nor_flash_driver_write = stm32_lx_nor_flash_driver_write;
  nor_flash->lx_nor_flash_driver_block_erase = stm32_lx_nor_flash_driver_block_erase;
  nor_flash->lx_nor_flash_driver_block_erased_verify = stm32_lx_nor_flash_driver_block_erased_verify;

  return ret;
}

/**
  * @brief Read data from a the flash memory into a data buffer
  * @param nor_flash LX_NOR_FLASH object pointer
  * @param flash_address the address from which to read the data
  * @param destination a pointer on the buffer on which to put the data
  * @param words size of the data to read in words
  * @retval LX_SUCCESS if the memory part is ready, LX_ERROR otherwise
  */
static UINT stm32_lx_nor_flash_driver_read(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *destination,
                                           ULONG words)
{
  UINT status = LX_SUCCESS;

  w25q128j_obj_t *p_w25q128j_obj;

  /* initialize the LX_NOR_FLASH object with relevant data */
  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_w25q128j_obj = (w25q128j_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_w25q128j_obj, nor_ctx->nor_flash_op_timeout);

  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {
    /* perform the read operation */
    status = w25q128j_read_dma(p_w25q128j_obj, (uint8_t *) destination,
                                (uint32_t ) flash_address, words * sizeof(ULONG));
    if (status != W25Q128J_OK)
    {
      return LX_ERROR;
    }

    status = xSemaphoreTake(nor_ctx->data_rx_cplt_semaphore.semaphore_handle, pdMS_TO_TICKS(nor_ctx->nor_flash_op_timeout));
    if (status != pdTRUE)
    {
      return LX_ERROR;
    }
  }
  status = nor_flash_check_status(p_w25q128j_obj, nor_ctx->nor_flash_op_timeout);

  return status;
}

/**
  * @brief write a data buffer on the flash memory
  * @param nor_flash LX_NOR_FLASH object pointer
  * @param flash_address the address where to write the data
  * @param destination a pointer on the data buffer to be written
  * @param words size of the data to read in words
  * @retval LX_SUCCESS if the memory part is ready, LX_ERROR otherwise
  */
static UINT stm32_lx_nor_flash_driver_write(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *source, ULONG words)
{
  UINT status = LX_SUCCESS;

  w25q128j_obj_t *p_w25q128j_obj;

  /* initialize the LX_NOR_FLASH object with relevant data */
  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_w25q128j_obj = (w25q128j_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_w25q128j_obj, nor_ctx->nor_flash_op_timeout);

  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {
    /* perform the write operation */
    status = w25q128j_write_dma_async(p_w25q128j_obj, (uint8_t *) source,
                               (uint32_t)flash_address, words * sizeof(ULONG));
    if (status != W25Q128J_OK)
    {
      return LX_ERROR;
    }

    do
    {
      status = w25q128j_exec_data_handler(p_w25q128j_obj);
    } while (status == W25Q128J_BUSY);
    if (status != W25Q128J_OK)
    {
      return LX_ERROR;
    }

    status = xSemaphoreTake(nor_ctx->data_tx_cplt_semaphore.semaphore_handle, pdMS_TO_TICKS(nor_ctx->nor_flash_op_timeout));
    if (status != pdTRUE)
    {
      return LX_ERROR;
    }
  }

  status = nor_flash_check_status(p_w25q128j_obj, nor_ctx->nor_flash_op_timeout);

  return status;
}

/**
  * @brief Erase a block
  * @param nor_flash LX_NOR_FLASH object pointer
  * @param block the index of the block to erase
  * @param erase_count the number of times the block has been erased
  * @retval LX_SUCCESS if the memory part is ready, LX_ERROR otherwise
  */
static UINT stm32_lx_nor_flash_driver_block_erase(LX_NOR_FLASH *nor_flash, ULONG block, ULONG erase_count)
{
  UINT status = LX_SUCCESS;
  ULONG block_size;
  w25q128j_erase_t erase_mode;

  w25q128j_obj_t *p_w25q128j_obj;

  /* initialize the LX_NOR_FLASH object with relevant data */
  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  block_size = nor_flash->lx_nor_flash_words_per_block * sizeof(ULONG);

  /* match the configured LevelX block size with the part erase command */
  if (block_size == W25Q128J_SECTOR_SIZE)
  {
    erase_mode = W25Q128J_ERASE_SECTOR;
  }
  else if (block_size == W25Q128J_SUBBLOCK_SIZE)
  {
    erase_mode = W25Q128J_ERASE_32K_BLOCK;
  }
  else if (block_size == W25Q128J_BLOCK_SIZE)
  {
    erase_mode = W25Q128J_ERASE_64K_BLOCK;
  }
  else
  {
    return LX_ERROR;
  }

  /* retrieve the memory part object */
  p_w25q128j_obj = (w25q128j_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_w25q128j_obj, nor_ctx->nor_flash_op_timeout);

  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {
    /* perform the block erase operation */
    status = w25q128j_erase(p_w25q128j_obj, (uint32_t)(block * block_size), erase_mode);
    if (status != W25Q128J_OK)
    {
      return LX_ERROR;
    }
  }

  return status;
}

/**
  * @brief Check that a block is actually erased
  * @param nor_flash LX_NOR_FLASH object pointer
  * @param block the index of the block to be checked
  * @retval LX_SUCCESS if the block is erased, LX_ERROR otherwise
  */
static UINT stm32_lx_nor_flash_driver_block_erased_verify(LX_NOR_FLASH *nor_flash, ULONG block)
{
  UINT status = LX_SUCCESS;
  ULONG block_size;
  ULONG start_address, end_address;

  w25q128j_obj_t *p_w25q128j_obj;
  /* initialize the LX_NOR_FLASH object with relevant data */
  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  block_size = nor_flash->lx_nor_flash_words_per_block * sizeof(ULONG);

  /* retrieve the memory part object */
  p_w25q128j_obj = (w25q128j_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_w25q128j_obj, nor_ctx->nor_flash_op_timeout);
  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {

    /* calculate the start and the end block address */
    start_address = block * block_size;
    end_address = start_address + block_size;

    while (start_address < end_address)
    {
      status = w25q128j_read_dma(p_w25q128j_obj, (uint8_t *)nor_ctx->nor_flash_read_buffer, (uint32_t)start_address, LX_NOR_SECTOR_SIZE * sizeof(ULONG));
      if (status != W25Q128J_OK)
      {
        return LX_ERROR;
      }

      status = xSemaphoreTake(nor_ctx->data_rx_cplt_semaphore.semaphore_handle, pdMS_TO_TICKS(nor_ctx->nor_flash_op_timeout));
      if (status != pdTRUE)
      {
        return LX_ERROR;
      }

      for (UINT i = 0; i < LX_NOR_SECTOR_SIZE; i++)
      {
        if (nor_ctx->nor_flash_read_buffer[i] != 0xFFFFFFFF)
        {
          return LX_ERROR;
        }
      }

      start_address += LX_NOR_SECTOR_SIZE * sizeof(ULONG);
    }
  }
  return LX_SUCCESS;
}

/**
  * @brief read completion callback
  * @param pobj w25q128j_obj_t pointer
  * @arg void user data pointer
  */
static void w25q128j_read_cplt_callback(w25q128j_obj_t *pobj, void *arg)
{
  LX_SEMAPHORE *semaphore = (LX_SEMAPHORE *)arg;
  if (semaphore->semaphore_handle != NULL)
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(semaphore->semaphore_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

/**
  * @brief write completion callback
  * @param pobj w25q128j_obj_t pointer
  * @arg void user data pointer
  */
static void w25q128j_write_cplt_callback(w25q128j_obj_t *pobj, void *arg)
{
  LX_SEMAPHORE *semaphore = (LX_SEMAPHORE *)arg;
  if (semaphore->semaphore_handle != NULL)
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(semaphore->semaphore_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}
