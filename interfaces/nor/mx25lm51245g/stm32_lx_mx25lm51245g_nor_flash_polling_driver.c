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

#include "stm32_lx_mx25lm51245g_nor_flash_polling_driver.h"

#include <stdio.h>
#ifndef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#error "LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE must be defined in the lx_user.h"
#endif /* LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE */
#ifdef LX_DIRECT_READ
#error "LX_DIRECT_READ feature is not supported. It must be disabled"
#endif /* LX_DIRECT_READ */

static inline UINT nor_flash_check_status(mx25lm51245g_obj_t *pobj, ULONG max_timeout);

static UINT stm32_lx_nor_flash_driver_read(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *destination,
                                           ULONG words);
static UINT stm32_lx_nor_flash_driver_write(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *source, ULONG words);

static UINT stm32_lx_nor_flash_driver_block_erase(LX_NOR_FLASH *nor_flash, ULONG block, ULONG erase_count);
static UINT stm32_lx_nor_flash_driver_block_erased_verify(LX_NOR_FLASH *nor_flash, ULONG block);
/**
  * @brief Check the status of the memory part
  * @param pobj mx25lm51245g_obj_t pointer
  * @retval LX_SUCCESS if the memory part is ready, LX_ERROR otherwise
  */
static inline UINT nor_flash_check_status(mx25lm51245g_obj_t *pobj, ULONG max_timeout)
{
  mx25lm51245g_status_t mx25lm51245g_status = MX25LM51245G_ERROR;
  uint32_t start = HAL_GetTick();

  while (HAL_GetTick() - start < max_timeout)
  {
    /* get current status of the flash memory */
    mx25lm51245g_get_status(pobj, &mx25lm51245g_status);
    if (mx25lm51245g_status == MX25LM51245G_OK)
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

UINT stm32_lx_mx25lm51245g_nor_flash_polling_driver_initialize(LX_NOR_FLASH *nor_flash)
{
  UINT ret = LX_SUCCESS;
  mx25lm51245g_info_t nor_flash_info;
  mx25lm51245g_obj_t *p_mx25lm51245g_obj;

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
  p_mx25lm51245g_obj = (mx25lm51245g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* get the memory part characteristics */
  mx25lm51245g_get_info(p_mx25lm51245g_obj, &nor_flash_info);

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
  mx25lm51245g_obj_t *p_mx25lm51245g_obj;

  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_mx25lm51245g_obj = (mx25lm51245g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_mx25lm51245g_obj, nor_ctx->nor_flash_op_timeout);

  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {
    /* perform the read operation */
    status = mx25lm51245g_read(p_mx25lm51245g_obj, MX25LM51245G_4BYTES_SIZE, (uint8_t *) destination,
                               (uint32_t)flash_address, words * sizeof(ULONG));
    if (status != MX25LM51245G_OK)
    {
      return LX_ERROR;
    }
  }

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
  mx25lm51245g_obj_t *p_mx25lm51245g_obj;

  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_mx25lm51245g_obj = (mx25lm51245g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_mx25lm51245g_obj, nor_ctx->nor_flash_op_timeout);

  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {
    /* perform the write operation */
    status = mx25lm51245g_write(p_mx25lm51245g_obj, MX25LM51245G_4BYTES_SIZE, (uint8_t *) source,
                                (uint32_t)flash_address, words * sizeof(ULONG));
    if (status != MX25LM51245G_OK)
    {
      return LX_ERROR;
    }
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
  mx25lm51245g_obj_t *p_mx25lm51245g_obj;
  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_mx25lm51245g_obj = (mx25lm51245g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_mx25lm51245g_obj, nor_ctx->nor_flash_op_timeout);

  if (status != LX_SUCCESS)
  {
    return status;
  }
  else
  {
    /* perform the block erase operation */
    status = mx25lm51245g_erase(p_mx25lm51245g_obj, (block * MX25LM51245G_BLOCK_64K), MX25LM51245G_ERASE_BLOCK);
    if (status != MX25LM51245G_OK)
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
  ULONG *block_address;
  mx25lm51245g_obj_t *p_mx25lm51245g_obj;
  ULONG i;

  STM32_LX_NOR_FLASH_DRIVER_CONTEXT *nor_ctx = (STM32_LX_NOR_FLASH_DRIVER_CONTEXT *) nor_flash->lx_nor_flash_driver_info_ptr;

  /* retrieve the memory part object */
  p_mx25lm51245g_obj = (mx25lm51245g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* check that the memory part is ready */
  status = nor_flash_check_status(p_mx25lm51245g_obj, nor_ctx->nor_flash_op_timeout);
  if (status != LX_SUCCESS)
  {
    return status;
  }

  /* enable memory mapped mode to be able to read the block content without any need for intermediate buffers */
  status = mx25lm51245g_enable_memory_mapped(p_mx25lm51245g_obj, MX25LM51245G_4BYTES_SIZE);
  if (status != MX25LM51245G_OK)
  {
    return LX_ERROR;
  }

  /* calculate the block address */
  block_address = (ULONG *)(mx25lm51245g_get_memory_mapped_base_address(p_mx25lm51245g_obj) + (block * MX25LM51245G_BLOCK_64K));

  /* verify that all bytes in the block are 0xFFFFFFFF */
  for (i = 0; i < MX25LM51245G_BLOCK_64K / 4; i++)
  {
    if (block_address[i] != 0xFFFFFFFF)
    {
      status = LX_ERROR;
      break;
    }
  }

  /* disable the memory mapped mode otherwise the write operations will fail */
  mx25lm51245g_disable_memory_mapped(p_mx25lm51245g_obj);

  return status;
}
