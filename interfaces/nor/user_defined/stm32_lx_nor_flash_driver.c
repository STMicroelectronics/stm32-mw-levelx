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

/* Private includes ----------------------------------------------------------*/
#include "stm32_lx_nor_flash_driver.h"

#ifndef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#error "LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE must be defined in the lx_user.h"
#endif /* LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE */
#ifdef LX_DIRECT_READ
#error "LX_DIRECT_READ feature is not supported. It must be disabled"
#endif /* LX_DIRECT_READ */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static UINT lx_nor_custom_driver_read(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *destination, ULONG words);
static UINT lx_nor_custom_driver_write(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *source, ULONG words);
static UINT lx_nor_custom_driver_block_erase(LX_NOR_FLASH *nor_flash, ULONG block, ULONG erase_count);
static UINT lx_nor_custom_driver_block_erased_verify(LX_NOR_FLASH *nor_flash, ULONG block);

/* Private variables ---------------------------------------------------------*/
static ULONG nor_sector_memory[LX_NOR_SECTOR_SIZE];

/**
  * @brief Entry function for the LevelX custom NOR driver.
  * @param nor_flash a pointer to a LX_NOR_FLASH object
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
UINT lx_stm32_nor_custom_driver_initialize(LX_NOR_FLASH *nor_flash)
{
  UINT ret = LX_SUCCESS;

  /* Setup the NOR flash memory properties. */
  nor_flash->lx_nor_flash_base_address    = (ULONG *)CUSTOM_NOR_FLASH_BASE_ADDRESS;
  nor_flash->lx_nor_flash_total_blocks    = CUSTOM_NOR_TOTAL_BLOCKS;
  nor_flash->lx_nor_flash_words_per_block = CUSTOM_NOR_WORDS_PER_BLOCK;

  /* Setup the function pointers for the NOR flash services. */
  nor_flash->lx_nor_flash_driver_read                = lx_nor_custom_driver_read;
  nor_flash->lx_nor_flash_driver_write               = lx_nor_custom_driver_write;
  nor_flash->lx_nor_flash_driver_block_erase         = lx_nor_custom_driver_block_erase;
  nor_flash->lx_nor_flash_driver_block_erased_verify = lx_nor_custom_driver_block_erased_verify;

  /* Setup the local buffer for NOR flash operations. */
  nor_flash->lx_nor_flash_sector_buffer = nor_sector_memory;

  return ret;
}

/**
  * @brief Read data from the NOR flash memory into a destination buffer.
  * @param nor_flash a pointer to a LX_NOR_FLASH object
  * @param flash_address the flash address to read from
  * @param destination the buffer to hold the read data
  * @param words the number of words to read
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nor_custom_driver_read(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *destination, ULONG words)
{
  UINT ret = LX_SUCCESS;

  return ret;
}

/**
  * @brief Write a source buffer into the NOR flash memory.
  * @param nor_flash a pointer to a LX_NOR_FLASH object
  * @param flash_address the flash address to write into
  * @param source the data buffer to be written
  * @param words the number of words to write
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nor_custom_driver_write(LX_NOR_FLASH *nor_flash, ULONG *flash_address, ULONG *source, ULONG words)
{
  UINT ret = LX_SUCCESS;

  return ret;
}

/**
  * @brief Erase the specified block of the NOR flash memory.
  * @param nor_flash a pointer to a LX_NOR_FLASH object
  * @param block the block to be erased
  * @param erase_count the erase count of the block
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nor_custom_driver_block_erase(LX_NOR_FLASH *nor_flash, ULONG block, ULONG erase_count)
{
  UINT ret = LX_SUCCESS;

  return ret;
}

/**
  * @brief Verify that the specified block was actually erased.
  * @param nor_flash a pointer to a LX_NOR_FLASH object
  * @param block the block to be checked
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nor_custom_driver_block_erased_verify(LX_NOR_FLASH *nor_flash, ULONG block)
{
  UINT ret = LX_SUCCESS;

  return ret;
}
