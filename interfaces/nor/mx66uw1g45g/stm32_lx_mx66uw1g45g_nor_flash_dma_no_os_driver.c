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

#include "stm32_lx_mx66uw1g45g_nor_flash_dma_no_os_driver.h"

#ifndef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#error "LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE must be defined in the lx_user.h"
#endif /* LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE */
#ifdef LX_DIRECT_READ
#error "LX_DIRECT_READ feature is not supported. It must be disabled"
#endif /* LX_DIRECT_READ */

static inline UINT nor_flash_check_status(mx66uw1g45g_obj_t *pobj, ULONG max_timeout);

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
  uint32_t start = HAL_GetTick();

  while (HAL_GetTick() - start < max_timeout)
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
  * @brief Entry function for the LevelX Driver
  * @param nor_flash a pointer to a LX_NOR_FLASH object
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */

UINT stm32_lx_mx66uw1g45g_nor_flash_dma_no_os_driver_initialize(LX_NOR_FLASH *nor_flash)
{
  UINT ret = LX_SUCCESS;
  mx66uw1g45g_info_t nor_flash_info;
  mx66uw1g45g_obj_t *p_mx66uw1g45g_obj;

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
  p_mx66uw1g45g_obj = (mx66uw1g45g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* register DMA R/W callbacks */
  mx66uw1g45g_register_read_cplt_callback(p_mx66uw1g45g_obj, mx66uw1g45g_read_cplt_callback,
                                          (void *)&nor_ctx->data_rx_cplt);
  mx66uw1g45g_register_write_cplt_callback(p_mx66uw1g45g_obj, mx66uw1g45g_write_cplt_callback,
                                           (void *)&nor_ctx->data_tx_cplt);

  /* get the memory part characteristics */
  mx66uw1g45g_get_info(p_mx66uw1g45g_obj, &nor_flash_info);

  /* to customize the flash configuration use the fields in the STM32_LX_NOR_FLASH_DRIVER_CONTEXT */
  nor_flash->lx_nor_flash_base_address = (ULONG *)0;
  nor_flash->lx_nor_flash_total_blocks = nor_flash_info.erase_block_number;

  nor_flash->lx_nor_flash_words_per_block = (nor_flash_info.erase_block_size) / sizeof(ULONG);

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
  ULONG timeout_start;
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
    nor_ctx->data_rx_cplt = 0;

    /* perform the read operation */
    status = mx66uw1g45g_read_dma(p_mx66uw1g45g_obj, MX66UW1G45G_4BYTES_SIZE, (uint8_t *) destination,
                                  (uint32_t)flash_address, words * sizeof(ULONG));
    if (status != MX66UW1G45G_OK)
    {
      return LX_ERROR;
    }

    timeout_start = HAL_GetTick();
    while (HAL_GetTick() - timeout_start < nor_ctx->nor_flash_op_timeout)
    {
      if (nor_ctx->data_rx_cplt == 1)
      {
        status = LX_SUCCESS;
        break;
      }
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
  ULONG timeout_start;
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
    nor_ctx->data_tx_cplt = 0;

    /* perform the write operation */
    status = mx66uw1g45g_write_dma(p_mx66uw1g45g_obj, MX66UW1G45G_4BYTES_SIZE, (uint8_t *) source,
                                   (uint32_t)flash_address, words * sizeof(ULONG));
    if (status != MX66UW1G45G_OK)
    {
      return LX_ERROR;
    }

    timeout_start = HAL_GetTick();
    while (HAL_GetTick() - timeout_start < nor_ctx->nor_flash_op_timeout)
    {
      if (nor_ctx->data_tx_cplt == 1)
      {
        status = LX_SUCCESS;
        break;
      }
    }

    status = nor_flash_check_status(p_mx66uw1g45g_obj, nor_ctx->nor_flash_op_timeout);
  }

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
  ULONG timeout_start;
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
                                             (void *)&nor_ctx->erase_cplt);

    nor_ctx->erase_cplt = 0;

    /* perform the block erase operation */
    status = mx66uw1g45g_erase_it(p_mx66uw1g45g_obj, (block * MX66UW1G45G_BLOCK_SIZE), MX66UW1G45G_ERASE_BLOCK);

    if (status != MX66UW1G45G_OK)
    {
      return LX_ERROR;
    }

    timeout_start = HAL_GetTick();
    while (HAL_GetTick() - timeout_start < nor_ctx->nor_flash_op_timeout)
    {
      if (nor_ctx->erase_cplt == 1)
      {
        status = LX_SUCCESS;
        break;
      }
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

  /* enable memory mapped mode to be able to read the block content without any need for intermediate buffers */
  status = mx66uw1g45g_enable_memory_mapped(p_mx66uw1g45g_obj, MX66UW1G45G_4BYTES_SIZE);
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
  volatile uint32_t *flag = arg;
  *flag = 1;
}

/**
  * @brief write completion callback
  * @param pobj mx66uw1g45g_obj_t pointer
  * @arg void user data pointer
  */
static void mx66uw1g45g_write_cplt_callback(mx66uw1g45g_obj_t *pobj, void *arg)
{
  volatile uint32_t *flag = arg;
  *flag = 1;
}

/**
  * @brief Callback to signal the completion of the erase operation.
  * @param pobj Pointer to the NOR flash memory object.
  * @param arg Pointer to a flag that is incremented upon completion.
  *
  * This function is called when the mass operation is complete. It increments the
  * value of the flag pointed to by the arg parameter, signaling that the operation has finished.
  */
static void mx66uw1g45g_erase_cplt_callback(mx66uw1g45g_obj_t *pobj, void *arg)
{
  volatile uint32_t *flag = arg;
  *flag = 1;
}
