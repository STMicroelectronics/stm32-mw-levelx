/**
  *************************************************************************
    * Copyright (c) 2024 Microsoft Corporation
    * Copyright (c) 2026 STMicroelectronics
    *
    * This program and the accompanying materials are made available under the
    * terms of the MIT License which is available at
    * https://opensource.org/licenses/MIT.
    *
    * SPDX-License-Identifier: MIT
    *************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include "stm32_lx_nand_flash_driver.h"

#ifndef LX_NAND_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#error "LX_NAND_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE must be defined in the lx_user.h"
#endif /* LX_NAND_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static UINT lx_nand_custom_driver_pages_read(LX_NAND_FLASH *nand_flash, ULONG block, ULONG page,
                                             UCHAR *main_buffer, UCHAR *spare_buffer, ULONG pages);
static UINT lx_nand_custom_driver_pages_write(LX_NAND_FLASH *nand_flash, ULONG block, ULONG page,
                                              UCHAR *main_buffer, UCHAR *spare_buffer, ULONG pages);
static UINT lx_nand_custom_driver_pages_copy(LX_NAND_FLASH *nand_flash, ULONG source_block, ULONG source_page,
                                             ULONG destination_block, ULONG destination_page, ULONG pages,
                                             UCHAR *data_buffer);
static UINT lx_nand_custom_driver_block_status_get(LX_NAND_FLASH *nand_flash, ULONG block, UCHAR *bad_block_flag);
static UINT lx_nand_custom_driver_block_status_set(LX_NAND_FLASH *nand_flash, ULONG block, UCHAR bad_block_flag);
static UINT lx_nand_custom_driver_block_erase(LX_NAND_FLASH *nand_flash, ULONG block, ULONG erase_count);

/* Private variables ---------------------------------------------------------*/

/**
  * @brief Entry function for the LevelX custom NAND driver.
  * @param nand_flash a pointer to a LX_NAND_FLASH object
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
UINT lx_stm32_nand_custom_driver_initialize(LX_NAND_FLASH *nand_flash)
{
  UINT ret = LX_SUCCESS;

  /* Setup the NAND flash memory properties. */
  nand_flash->lx_nand_flash_total_blocks       = CUSTOM_NAND_TOTAL_BLOCKS;
  nand_flash->lx_nand_flash_pages_per_block    = CUSTOM_NAND_PAGES_PER_BLOCK;
  nand_flash->lx_nand_flash_bytes_per_page     = CUSTOM_NAND_BYTES_PER_PAGE;
  nand_flash->lx_nand_flash_spare_total_length = CUSTOM_NAND_SPARE_TOTAL_LENGTH;
  nand_flash->lx_nand_flash_spare_data1_offset = CUSTOM_NAND_SPARE_DATA1_OFFSET;
  nand_flash->lx_nand_flash_spare_data1_length = CUSTOM_NAND_SPARE_DATA1_LENGTH;
  nand_flash->lx_nand_flash_spare_data2_offset = CUSTOM_NAND_SPARE_DATA2_OFFSET;
  nand_flash->lx_nand_flash_spare_data2_length = CUSTOM_NAND_SPARE_DATA2_LENGTH;

  /* Setup the function pointers for the NAND flash services. */
  nand_flash->lx_nand_flash_driver_pages_read       = lx_nand_custom_driver_pages_read;
  nand_flash->lx_nand_flash_driver_pages_write      = lx_nand_custom_driver_pages_write;
  nand_flash->lx_nand_flash_driver_pages_copy       = lx_nand_custom_driver_pages_copy;
  nand_flash->lx_nand_flash_driver_block_status_get = lx_nand_custom_driver_block_status_get;
  nand_flash->lx_nand_flash_driver_block_status_set = lx_nand_custom_driver_block_status_set;
  nand_flash->lx_nand_flash_driver_block_erase      = lx_nand_custom_driver_block_erase;

  return ret;
}

/**
  * @brief read data from the flash memory into specified buffers.
  * @param nand_flash Pointer to an LX_NAND_FLASH object.
  * @param block Block number from which to read the data.
  * @param page Page number within the block from which to read the data.
  * @param main_buffer Pointer to the buffer where the main data will be stored.
  * @param spare_buffer Pointer to the buffer where the spare data will be stored.
  * @param pages Number of pages to read.
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nand_custom_driver_pages_read(LX_NAND_FLASH *nand_flash, ULONG block, ULONG page,
                                             UCHAR *main_buffer, UCHAR *spare_buffer, ULONG pages)
{
  UINT ret = LX_SUCCESS;

  return ret;
}

/**
  * @brief write a data buffer on the flash memory
  * @param nand_flash Pointer to an LX_NAND_FLASH object
  * @param block the block number where to write the data
  * @param page the page number within the block fromwhere to write the data
  * @param main_buffer a pointer on the main data to be written
  * @param spare_buffer a pointer on the spare data to be written
  * @param pages number of pages to read.
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nand_custom_driver_pages_write(LX_NAND_FLASH *nand_flash, ULONG block, ULONG page,
                                              UCHAR *main_buffer, UCHAR *spare_buffer, ULONG pages)
{
  UINT ret = LX_SUCCESS;

  return ret;
}

/**
  * @brief Copy pages from a source block to a destination block in NAND flash memory.
  * @param nand_flash Pointer to an LX_NAND_FLASH object.
  * @param source_block Block number of the source from which pages will be copied.
  * @param source_page Page number within the source block to start copying from.
  * @param destination_block Block number of the destination where pages will be copied to.
  * @param destination_page Page number within the destination block to start copying to.
  * @param pages Number of pages to copy.
  * @param data_buffer Pointer to a buffer used for temporary data storage during the copy process.
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nand_custom_driver_pages_copy(LX_NAND_FLASH *nand_flash, ULONG source_block, ULONG source_page,
                                             ULONG destination_block, ULONG destination_page, ULONG pages,
                                             UCHAR *data_buffer)
{
  UINT ret = LX_SUCCESS;

  return ret;
}

/**
  * @brief Retrieves the status of a specified block in the NAND flash memory
  * @param nand_flash Pointer to an LX_NAND_FLASH object
  * @param block The block number whose status is to be checked.
  * @param bad_block_flag Pointer to a variable where the bad block status will be stored.
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nand_custom_driver_block_status_get(LX_NAND_FLASH *nand_flash, ULONG block, UCHAR *bad_block_flag)
{
  UINT ret = LX_SUCCESS;

  return ret;
}

/**
  * @brief Set the status of a block in the NAND flash memory.
  * @param nand_flash Pointer to an LX_NAND_FLASH object.
  * @param block Block number whose status is to be set.
  * @param bad_block_flag Indicator of the block status
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nand_custom_driver_block_status_set(LX_NAND_FLASH *nand_flash, ULONG block, UCHAR bad_block_flag)
{
  UINT ret = LX_SUCCESS;

  return ret;
}

/**
  * @brief Erase a block
  * @param nand_flash LX_NAND_FLASH object pointer
  * @param block the index of the block to erase
  * @param erase_count the number of times the block has been erased
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
static UINT lx_nand_custom_driver_block_erase(LX_NAND_FLASH *nand_flash, ULONG block, ULONG erase_count)
{
  UINT ret = LX_SUCCESS;

  return ret;
}
