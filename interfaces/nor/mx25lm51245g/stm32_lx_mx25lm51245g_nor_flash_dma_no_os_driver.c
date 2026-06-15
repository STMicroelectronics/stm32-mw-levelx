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

#include "stm32_lx_mx25lm51245g_nor_flash_dma_no_os_driver.h"

#ifndef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#error "LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE must be defined in the lx_user.h"
#endif /* LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE */
#ifdef LX_DIRECT_READ
#error "LX_DIRECT_READ feature is not supported. It must be disabled"
#endif /* LX_DIRECT_READ */

static inline UINT nor_flash_check_status(mx25lm51245g_obj_t *pobj, ULONG max_timeout);
static inline mx25lm51245g_addr_size_t stm32_lx_mx25lm51245g_get_addr_size(mx25lm51245g_obj_t *pobj,
                                                                            uint32_t address,
                                                                            uint32_t size_byte);

static void mx25lm51245g_read_cplt_callback(mx25lm51245g_obj_t *pobj, void *arg);
static void mx25lm51245g_write_cplt_callback(mx25lm51245g_obj_t *pobj, void *arg);
static void mx25lm51245g_erase_cplt_callback(mx25lm51245g_obj_t *pobj, void *arg);

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
  * @brief Select the address size according to current interface and transfer range.
  * @param pobj mx25lm51245g_obj_t pointer
  * @param address operation start address
  * @param size_byte operation size in bytes
  * @retval Address size to use
  * @note In SPI mode, 3-byte addressing is used only when the transfer fits in the 24-bit range.
  * @note In OPI mode, 4-byte addressing is always used.
  */
static inline mx25lm51245g_addr_size_t stm32_lx_mx25lm51245g_get_addr_size(mx25lm51245g_obj_t *pobj,
                                                                            uint32_t address,
                                                                            uint32_t size_byte)
{
  mx25lm51245g_interface_t interface_mode;

  interface_mode = mx25lm51245g_get_interface_cfg(pobj);

  if (interface_mode == MX25LM51245G_OPI_MODE)
  {
    return MX25LM51245G_4BYTES_SIZE;
  }

  /* In SPI mode, keep 3-byte addressing only if the whole transfer stays below 16 MBytes. */
  if ((interface_mode == MX25LM51245G_SPI_MODE) &&
      (address <= 0x00FFFFFFU) &&
      (size_byte <= (0x01000000U - address)))
  {
    return MX25LM51245G_3BYTES_SIZE;
  }

  return MX25LM51245G_4BYTES_SIZE;
}

/**
  * @brief Entry function for the LevelX Driver
  * @param nor_flash a pointer to a LX_NOR_FLASH object
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */

UINT stm32_lx_mx25lm51245g_nor_flash_dma_no_os_driver_initialize(LX_NOR_FLASH *nor_flash)
{
  UINT ret = LX_SUCCESS;
  mx25lm51245g_info_t nor_flash_info;
  mx25lm51245g_obj_t *p_mx25lm51245g_obj;
  ULONG block_size;
  ULONG start_address = 0U;
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
  p_mx25lm51245g_obj = (mx25lm51245g_obj_t *)nor_ctx->nor_flash_get_driver_object();

  /* register DMA R/W callbacks */
  mx25lm51245g_register_read_cplt_callback(p_mx25lm51245g_obj, mx25lm51245g_read_cplt_callback,
                                           (void *)&nor_ctx->data_rx_cplt);
  mx25lm51245g_register_write_cplt_callback(p_mx25lm51245g_obj, mx25lm51245g_write_cplt_callback,
                                            (void *)&nor_ctx->data_tx_cplt);

  /* get the memory part characteristics */
  mx25lm51245g_get_info(p_mx25lm51245g_obj, &nor_flash_info);

  /* customize the flash start address */
  if (nor_ctx->nor_flash_flags & STM32_LX_NOR_FLAG_START_ADDRESS)
  {
    /* ensure base address is within the flash address range. */
    if (nor_ctx->nor_flash_start_address >= nor_flash_info.erase_block_number)
    {
      return LX_ERROR;
    }

    start_address = nor_ctx->nor_flash_start_address;
  }

  /* compute the available flash size from the selected start address to the end of the NOR flash */
  block_size = nor_flash_info.erase_block_size;
  available_flash_size = nor_flash_info.flash_size - (start_address * block_size);

  /* customize the flash size limit. */
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
  ULONG timeout_start;
  mx25lm51245g_obj_t *p_mx25lm51245g_obj;
  mx25lm51245g_addr_size_t addr_size;
  uint32_t size_byte;

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
    nor_ctx->data_rx_cplt = 0;
    size_byte = (uint32_t)(words * sizeof(ULONG));
    addr_size = stm32_lx_mx25lm51245g_get_addr_size(p_mx25lm51245g_obj, (uint32_t)flash_address, size_byte);

    /* perform the read operation */
    status = mx25lm51245g_read_dma(p_mx25lm51245g_obj, addr_size, (uint8_t *) destination,
                                   (uint32_t)flash_address, size_byte);
    if (status != MX25LM51245G_OK)
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

  status = nor_flash_check_status(p_mx25lm51245g_obj, nor_ctx->nor_flash_op_timeout);

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
  mx25lm51245g_obj_t *p_mx25lm51245g_obj;
  mx25lm51245g_addr_size_t addr_size;
  uint32_t size_byte;

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
    nor_ctx->data_tx_cplt = 0;
    size_byte = (uint32_t)(words * sizeof(ULONG));
    addr_size = stm32_lx_mx25lm51245g_get_addr_size(p_mx25lm51245g_obj, (uint32_t)flash_address, size_byte);

    /* perform the write operation */
    status = mx25lm51245g_write_dma(p_mx25lm51245g_obj, addr_size, (uint8_t *) source,
                                    (uint32_t)flash_address, size_byte);
    if (status != MX25LM51245G_OK)
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

    status = nor_flash_check_status(p_mx25lm51245g_obj, nor_ctx->nor_flash_op_timeout);
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
    /* register block erase callback */
    mx25lm51245g_register_erase_cplt_callback(p_mx25lm51245g_obj, mx25lm51245g_erase_cplt_callback,
                                                    (void *)&nor_ctx->erase_cplt);

    nor_ctx->erase_cplt = 0;

    /* perform the block erase operation */
    status = mx25lm51245g_erase_it(p_mx25lm51245g_obj, (block * MX25LM51245G_BLOCK_64K), MX25LM51245G_ERASE_BLOCK);

    if (status != MX25LM51245G_OK)
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

  status = nor_flash_check_status(p_mx25lm51245g_obj, nor_ctx->nor_flash_op_timeout);

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
  mx25lm51245g_addr_size_t addr_size;
  uint32_t verify_addr;
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

  verify_addr = (uint32_t)(block * MX25LM51245G_BLOCK_64K);
  addr_size = stm32_lx_mx25lm51245g_get_addr_size(p_mx25lm51245g_obj, verify_addr, MX25LM51245G_BLOCK_64K);

  /* enable memory mapped mode to be able to read the block content without any need for intermediate buffers */
  status = mx25lm51245g_enable_memory_mapped(p_mx25lm51245g_obj, addr_size);
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

/**
  * @brief read completion callback
  * @param pobj mx25lm51245g_obj_t pointer
  * @arg void user data pointer
  */
static void mx25lm51245g_read_cplt_callback(mx25lm51245g_obj_t *pobj, void *arg)
{
  volatile uint32_t *flag = arg;
  *flag = 1;
}

/**
  * @brief write completion callback
  * @param pobj mx25lm51245g_obj_t pointer
  * @arg void user data pointer
  */
static void mx25lm51245g_write_cplt_callback(mx25lm51245g_obj_t *pobj, void *arg)
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
static void mx25lm51245g_erase_cplt_callback(mx25lm51245g_obj_t *pobj, void *arg)
{
  volatile uint32_t *flag = arg;
  *flag = 1;
}
