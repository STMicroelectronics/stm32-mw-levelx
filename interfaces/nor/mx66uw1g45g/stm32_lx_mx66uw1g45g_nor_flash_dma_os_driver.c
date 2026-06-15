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

#include "stm32_lx_mx66uw1g45g_nor_flash_dma_os_driver.h"

#ifndef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#error "LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE must be defined in the lx_user.h"
#endif /* LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE */
#ifdef LX_DIRECT_READ
#error "LX_DIRECT_READ feature is not supported. It must be disabled"
#endif /* LX_DIRECT_READ */

static inline UINT nor_flash_check_status(mx66uw1g45g_obj_t *pobj, ULONG max_timeout);
static inline mx66uw1g45g_addr_size_t stm32_lx_mx66uw1g45g_get_addr_size(mx66uw1g45g_obj_t *pobj,
                                                                          uint32_t address,
                                                                          uint32_t size_byte);

static void mx66uw1g45g_read_cplt_callback(mx66uw1g45g_obj_t *pobj, void *arg);
static void mx66uw1g45g_write_cplt_callback(mx66uw1g45g_obj_t *pobj, void *arg);
static void mx66uw1g45g_erase_cplt_callback(mx66uw1g45g_obj_t *pobj, void *arg);

static UINT stm32_lx_nor_flash_driver_read(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *destination,
                                           ULONG words);
static UINT stm32_lx_nor_flash_driver_write(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *source, ULONG words);

static UINT stm32_lx_nor_flash_driver_block_erase(LX_NOR_FLASH *nor_flash, ULONG block, ULONG erase_count);
static UINT stm32_lx_nor_flash_driver_block_erased_verify(LX_NOR_FLASH *nor_flash, ULONG block);

/**
  * @brief Check the status of the memory part
  * @param pobj mx66uw1g45g_obj_t pointer
  * @retval LX_SUCCESS if the memory part is ready, LX_ERROR otherwise
  */
static inline UINT nor_flash_check_status(mx66uw1g45g_obj_t *pobj, ULONG max_timeout)
{
  mx66uw1g45g_status_t mx66uw1g45g_status = MX66UW1G45G_ERROR;
  TickType_t start = xTaskGetTickCount();

  while ((xTaskGetTickCount() - start) < pdMS_TO_TICKS(max_timeout))
  {
    /* get current status of the flash memory */
    mx66uw1g45g_status = mx66uw1g45g_get_status(pobj);
    if (mx66uw1g45g_status == MX66UW1G45G_OK)
    {
      return LX_SUCCESS;
    }
  }

  return LX_ERROR;
}

/**
  * @brief Select the address size according to current interface and transfer range.
  * @param pobj mx66uw1g45g_obj_t pointer
  * @param address operation start address
  * @param size_byte operation size in bytes
  * @retval Address size to use
  * @note In SPI mode, 3-byte addressing is used only when the transfer fits in the 24-bit range.
  * @note In OPI mode, 4-byte addressing is always used.
  */
static inline mx66uw1g45g_addr_size_t stm32_lx_mx66uw1g45g_get_addr_size(mx66uw1g45g_obj_t *pobj,
                                                                          uint32_t address,
                                                                          uint32_t size_byte)
{
  mx66uw1g45g_interface_t interface_mode;

  interface_mode = mx66uw1g45g_get_interface_cfg(pobj);

  if (interface_mode == MX66UW1G45G_OPI_MODE)
  {
    return MX66UW1G45G_4BYTES_SIZE;
  }

  /* In SPI mode, keep 3-byte addressing only if the whole transfer stays below 16 MBytes. */
  if ((interface_mode == MX66UW1G45G_SPI_MODE) &&
      (address <= 0x00FFFFFFU) &&
      (size_byte <= (0x01000000U - address)))
  {
    return MX66UW1G45G_3BYTES_SIZE;
  }

  return MX66UW1G45G_4BYTES_SIZE;
}

/**
  * @brief Entry function for the LevelX Driver
  * @param nor_flash a pointer to a LX_NOR_FLASH object
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */

UINT stm32_lx_mx66uw1g45g_nor_flash_dma_os_driver_initialize(LX_NOR_FLASH *nor_flash)
{
  UINT ret = LX_SUCCESS;
  mx66uw1g45g_info_t nor_flash_info;
  mx66uw1g45g_obj_t *p_mx66uw1g45g_obj;
  ULONG block_size;
  ULONG start_address = 0U;
  ULONG available_flash_size;

  /* initialize the LX_NOR_FLASH object with relevant data */
  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *)nor_flash->lx_nor_flash_driver_info_ptr;

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
  p_mx66uw1g45g_obj = (mx66uw1g45g_obj_t *)nor_ctx->nor_flash_get_driver_object();

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

  /* create semaphore for erase notification if not already created */
  if (nor_ctx->erase_cplt_semaphore.semaphore_handle == NULL)
  {
    nor_ctx->erase_cplt_semaphore.semaphore_handle = xSemaphoreCreateBinary();
    if (nor_ctx->erase_cplt_semaphore.semaphore_handle == NULL)
    {
      return LX_ERROR;
    }
  }

  /* register DMA R/W callbacks */
  mx66uw1g45g_register_read_cplt_callback(p_mx66uw1g45g_obj, mx66uw1g45g_read_cplt_callback,
                                          (void *)&nor_ctx->data_rx_cplt_semaphore);
  mx66uw1g45g_register_write_cplt_callback(p_mx66uw1g45g_obj, mx66uw1g45g_write_cplt_callback,
                                           (void *)&nor_ctx->data_tx_cplt_semaphore);

  /* get the memory part characteristics */
  mx66uw1g45g_get_info(p_mx66uw1g45g_obj, &nor_flash_info);

  /* customize the flash start address. */
  if (nor_ctx->nor_flash_flags & STM32_LX_NOR_FLAG_START_ADDRESS)
  {
    /* ensure base address is within the flash address range. */
    if (nor_ctx->nor_flash_start_address >= nor_flash_info.erase_block_number)
    {
      return LX_ERROR;
    }

    start_address = nor_ctx->nor_flash_start_address;
  }

  /* compute the available flash size from the selected start address to the end of the NOR flash. */
  block_size = nor_flash_info.erase_block_size;
  available_flash_size = nor_flash_info.flash_size - (start_address * block_size);

  /* customize the flash size limit. */
  if (nor_ctx->nor_flash_flags & STM32_LX_NOR_FLAG_FLASH_SIZE)
  {
    /* ensure the configured sub-range is non-zero, block-aligned, and does not exceed the available space. */
    if ((nor_ctx->nor_flash_size == 0U) ||
        ((nor_ctx->nor_flash_size % block_size) != 0U) ||
        (nor_ctx->nor_flash_size > available_flash_size))
    {
      return LX_ERROR;
    }

    /* restrict the managed region to the requested size. */
    available_flash_size = nor_ctx->nor_flash_size;
  }

  /* set the flash base address. */
  nor_flash->lx_nor_flash_base_address = (ULONG *)(start_address * block_size);
  /* set the total number of managed flash blocks. */
  nor_flash->lx_nor_flash_total_blocks = available_flash_size / block_size;
  /* set the number of ULONG words per flash block. */
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
  mx66uw1g45g_obj_t *p_mx66uw1g45g_obj;
  mx66uw1g45g_addr_size_t addr_size;
  uint32_t size_byte;

  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_mx66uw1g45g_obj = (mx66uw1g45g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_mx66uw1g45g_obj, nor_ctx->nor_flash_op_timeout);

  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {
    size_byte = (uint32_t)(words * sizeof(ULONG));
    addr_size = stm32_lx_mx66uw1g45g_get_addr_size(p_mx66uw1g45g_obj, (uint32_t)flash_address, size_byte);

    /* perform the read operation */
    status = mx66uw1g45g_read_dma(p_mx66uw1g45g_obj, addr_size, (uint8_t *) destination,
                                  (uint32_t)flash_address, size_byte);
    if (status != MX66UW1G45G_OK)
    {
      return LX_ERROR;
    }

    status = xSemaphoreTake(nor_ctx->data_rx_cplt_semaphore.semaphore_handle, pdMS_TO_TICKS(nor_ctx->nor_flash_op_timeout));
    if (status != pdTRUE)
    {
      return LX_ERROR;
    }
  }
  status = nor_flash_check_status(p_mx66uw1g45g_obj, nor_ctx->nor_flash_op_timeout);

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
  mx66uw1g45g_obj_t *p_mx66uw1g45g_obj;
  mx66uw1g45g_addr_size_t addr_size;
  uint32_t size_byte;

  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_mx66uw1g45g_obj = (mx66uw1g45g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_mx66uw1g45g_obj, nor_ctx->nor_flash_op_timeout);

  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {
    size_byte = (uint32_t)(words * sizeof(ULONG));
    addr_size = stm32_lx_mx66uw1g45g_get_addr_size(p_mx66uw1g45g_obj, (uint32_t)flash_address, size_byte);

    /* perform the write operation */
    status = mx66uw1g45g_write_dma(p_mx66uw1g45g_obj, addr_size, (uint8_t *) source,
                                   (uint32_t)flash_address, size_byte);
    if (status != MX66UW1G45G_OK)
    {
      return LX_ERROR;
    }

    status = xSemaphoreTake(nor_ctx->data_tx_cplt_semaphore.semaphore_handle, pdMS_TO_TICKS(nor_ctx->nor_flash_op_timeout));
    if (status != pdTRUE)
    {
      return LX_ERROR;
    }
  }

  status = nor_flash_check_status(p_mx66uw1g45g_obj, nor_ctx->nor_flash_op_timeout);

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
  mx66uw1g45g_obj_t *p_mx66uw1g45g_obj;
  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_mx66uw1g45g_obj = (mx66uw1g45g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_mx66uw1g45g_obj, nor_ctx->nor_flash_op_timeout);

  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {
    /* register block erase callback */
    mx66uw1g45g_register_erase_cplt_callback(p_mx66uw1g45g_obj, mx66uw1g45g_erase_cplt_callback,
                                             (void *)&nor_ctx->erase_cplt_semaphore);

    /* perform the block erase operation */
    status = mx66uw1g45g_erase_it(p_mx66uw1g45g_obj, (block * MX66UW1G45G_BLOCK_SIZE), MX66UW1G45G_ERASE_BLOCK);

    if (status != MX66UW1G45G_OK)
    {
      return LX_ERROR;
    }

    status = xSemaphoreTake(nor_ctx->erase_cplt_semaphore.semaphore_handle, pdMS_TO_TICKS(nor_ctx->nor_flash_op_timeout));
    if (status != pdTRUE)
    {
      return LX_ERROR;
    }
  }

  status = nor_flash_check_status(p_mx66uw1g45g_obj, nor_ctx->nor_flash_op_timeout);

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
  ULONG *block_address;
  mx66uw1g45g_obj_t *p_mx66uw1g45g_obj;
  mx66uw1g45g_addr_size_t addr_size;
  uint32_t verify_addr;
  ULONG i;

  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_mx66uw1g45g_obj = (mx66uw1g45g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_mx66uw1g45g_obj, nor_ctx->nor_flash_op_timeout);
  if (status != LX_SUCCESS)
  {
    return status;
  }

  verify_addr = (uint32_t)(block * MX66UW1G45G_BLOCK_SIZE);
  addr_size = stm32_lx_mx66uw1g45g_get_addr_size(p_mx66uw1g45g_obj, verify_addr, MX66UW1G45G_BLOCK_SIZE);

  /* enable memory mapped mode to be able to read the block content without any need for intermediate buffers */
  status = mx66uw1g45g_enable_memory_mapped(p_mx66uw1g45g_obj, addr_size);
  if (status != MX66UW1G45G_OK)
  {
    return LX_ERROR;
  }

  /* calculate the block address */
  block_address = (ULONG *)(mx66uw1g45g_get_memory_mapped_base_address(p_mx66uw1g45g_obj) + (block * MX66UW1G45G_BLOCK_SIZE));

  /* verify that all bytes in the block are 0xFFFFFFFF */
  for (i = 0; i < MX66UW1G45G_BLOCK_SIZE / 4; i++)
  {
    if (block_address[i] != 0xFFFFFFFF)
    {
      status = LX_ERROR;
      break;
    }
  }

  /* disable the memory mapped mode otherwise the write operations will fail */
  mx66uw1g45g_disable_memory_mapped(p_mx66uw1g45g_obj);

  return status;
}

/**
  * @brief read completion callback
  * @param pobj mx66uw1g45g_obj_t pointer
  * @arg void user data pointer
  */
static void mx66uw1g45g_read_cplt_callback(mx66uw1g45g_obj_t *pobj, void *arg)
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
  * @param pobj mx66uw1g45g_obj_t pointer
  * @arg void user data pointer
  */
static void mx66uw1g45g_write_cplt_callback(mx66uw1g45g_obj_t *pobj, void *arg)
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
  * @brief Callback to signal the completion of the erase operation.
  * @param pobj Pointer to the NOR flash memory object.
  * @param arg Pointer to a flag that is incremented upon completion.
  */
static void mx66uw1g45g_erase_cplt_callback(mx66uw1g45g_obj_t *pobj, void *arg)
{
  LX_SEMAPHORE *semaphore = (LX_SEMAPHORE *)arg;

  if (semaphore->semaphore_handle != NULL)
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(semaphore->semaphore_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}
