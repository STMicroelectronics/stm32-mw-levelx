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

#include "stm32_lx_w25n01gvxx_nand_flash_polling_driver.h"

#ifndef LX_NAND_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
#error "LX_NAND_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE must be defined in the lx_user.h"
#endif /* LX_NAND_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE */

static UINT stm32_lx_nand_flash_driver_block_status_get(LX_NAND_FLASH *nand_flash, ULONG block,
                                                        UCHAR *bad_block_flag);
static UINT stm32_lx_nand_flash_driver_block_status_set(LX_NAND_FLASH *nand_flash, ULONG block,
                                                        UCHAR bad_block_flag);
static UINT stm32_lx_nand_flash_driver_pages_read(LX_NAND_FLASH *nand_flash, ULONG block, ULONG page,
                                                  UCHAR* main_buffer, UCHAR* spare_buffer, ULONG pages);
static UINT stm32_lx_nand_flash_driver_pages_write(LX_NAND_FLASH *nand_flash, ULONG block, ULONG page,
                                                   UCHAR* main_buffer, UCHAR* spare_buffer, ULONG pages);
static UINT stm32_lx_nand_flash_driver_pages_copy(LX_NAND_FLASH *nand_flash, ULONG source_block, ULONG source_page,
                                                  ULONG destination_block, ULONG destination_page, ULONG pages,
                                                  UCHAR *data_buffer);
static UINT stm32_lx_nand_flash_driver_block_erase(LX_NAND_FLASH *nand_flash, ULONG block, ULONG erase_count);

/**
  * @brief Entry function for the LevelX Driver
  * @param nand_flash a pointer to a LX_NAND_FLASH object
  * @retval LX_SUCCESS on success, LX_ERROR otherwise
  */
UINT stm32_lx_w25n01gvxx_nand_flash_polling_driver_initialize(LX_NAND_FLASH *nand_flash)
{
  UINT status = LX_SUCCESS;
  w25n01gvxx_info_t nand_flash_info;
  w25n01gvxx_obj_t *p_w25n01gvxx_obj;

  /* initialize the LX_NAND_FLASH object with relevant data */
  STM32_LX_NAND_FLASH_DRIVER_CONTEXT *nand_ctx = (STM32_LX_NAND_FLASH_DRIVER_CONTEXT *) nand_flash->lx_nand_flash_driver_info_ptr;

  /* check if context is valid to prevent a hard fault.  */
  if (nand_ctx == NULL)
  {
    return LX_ERROR;
  }
  /* check that the nand_flash_get_driver_object function pointer is correctly set */
  if (nand_ctx->nand_flash_get_driver_object == NULL)
  {
    return LX_ERROR;
  }

  /* retrieve the memory part object */
  p_w25n01gvxx_obj = (w25n01gvxx_obj_t *)nand_ctx->nand_flash_get_driver_object();

  /* get the memory part characteristics */
  nand_flash_info = w25n01gvxx_get_info(p_w25n01gvxx_obj);

  /* to customize the flash configuration use the fields in the STM32_LX_NAND_FLASH_DRIVER_CONTEXT */

  nand_flash->lx_nand_flash_spare_data1_length = W25N01GVXX_SPARE_USER_DATA1_SIZE;
  nand_flash->lx_nand_flash_spare_data1_offset = W25N01GVXX_SPARE_USER_DATA1_OFFSET;
  nand_flash->lx_nand_flash_spare_data2_length = W25N01GVXX_SPARE_USER_DATA2_SIZE;
  nand_flash->lx_nand_flash_spare_data2_offset = W25N01GVXX_SPARE_USER_DATA2_OFFSET;

  nand_flash->lx_nand_flash_pages_per_block = nand_flash_info.block_size/nand_flash_info.page_size;
  nand_flash->lx_nand_flash_bytes_per_page = nand_flash_info.page_size;
  nand_flash->lx_nand_flash_spare_total_length = nand_flash_info.spare_area_size;
  if (nand_ctx->nand_flash_flags & STM32_LX_NAND_FLAG_TOTAL_BLOCKS)
  {
    nand_flash->lx_nand_flash_total_blocks = nand_ctx->nand_flash_total_blocks;
  }
  else if (nand_ctx->nand_flash_flags & STM32_LX_NAND_FLAG_START_BLOCK)
  {
    nand_flash->lx_nand_flash_total_blocks = nand_flash_info.block_count - nand_ctx->nand_flash_start_block;
  }
  else
    nand_flash->lx_nand_flash_total_blocks = nand_flash_info.block_count;

  /* initialize the fops pointers */
  nand_flash->lx_nand_flash_driver_pages_read = stm32_lx_nand_flash_driver_pages_read;
  nand_flash->lx_nand_flash_driver_pages_write = stm32_lx_nand_flash_driver_pages_write;
  nand_flash->lx_nand_flash_driver_pages_copy = stm32_lx_nand_flash_driver_pages_copy;
  nand_flash->lx_nand_flash_driver_block_status_get = stm32_lx_nand_flash_driver_block_status_get;
  nand_flash->lx_nand_flash_driver_block_status_set = stm32_lx_nand_flash_driver_block_status_set;
  nand_flash->lx_nand_flash_driver_block_erase = stm32_lx_nand_flash_driver_block_erase;

  return status;
}

/**
  * @brief read data from the flash memory into specified buffers.
  * @param nand_flash Pointer to an LX_NAND_FLASH object.
  * @param block Block number from which to read the data.
  * @param page Page number within the block from which to read the data.
  * @param main_buffer Pointer to the buffer where the main data will be stored.
  * @param spare_buffer Pointer to the buffer where the spare data will be stored.
  * @param pages Number of pages to read.
  * @retval LX_SUCCESS if the operation is successful, LX_ERROR otherwise.
  */
static UINT stm32_lx_nand_flash_driver_pages_read(LX_NAND_FLASH *nand_flash, ULONG block, ULONG page,
                                                  UCHAR* main_buffer, UCHAR* spare_buffer, ULONG pages)
{
  UINT status;
  w25n01gvxx_obj_t *p_w25n01gvxx_obj;

  /* initialize the LX_NAND_FLASH object with relevant data */
  STM32_LX_NAND_FLASH_DRIVER_CONTEXT *nand_ctx = (STM32_LX_NAND_FLASH_DRIVER_CONTEXT *) nand_flash->lx_nand_flash_driver_info_ptr;

  /* check if context is valid to prevent a hard fault.  */
  if (nand_ctx == NULL)
  {
    return LX_ERROR;
  }
  /* check that the nand_flash_get_driver_object function pointer is correctly set */
  if (nand_ctx->nand_flash_get_driver_object == NULL)
  {
    return LX_ERROR;
  }
  /* Check if the NAND flash flags indicate the use of a start block offset */
  if (nand_ctx->nand_flash_flags & STM32_LX_NAND_FLAG_START_BLOCK)
  {
    block+= nand_ctx->nand_flash_start_block;
  }
  /* retrieve the memory part object */
  p_w25n01gvxx_obj = (w25n01gvxx_obj_t *)nand_ctx->nand_flash_get_driver_object();

  /* perform the read operation for main buffer */
  if (main_buffer != NULL)
  {
    status = w25n01gvxx_read(p_w25n01gvxx_obj, (uint16_t)block, (uint8_t)page, (uint8_t *)main_buffer, W25N01GVXX_DATA_PAGE_SIZE);
    if (status != W25N01GVXX_OK)
    {
      return LX_ERROR;
    }
  }

  /* perform the read operation for spare buffer */
  status = w25n01gvxx_read_spare_area(p_w25n01gvxx_obj, (uint16_t)block, (uint8_t) page, 1, (uint8_t *)spare_buffer);
  if (status != W25N01GVXX_OK)
  {
    return LX_ERROR;
  }
  return LX_SUCCESS;
}

/**
  * @brief write a data buffer on the flash memory
  * @param nand_flash Pointer to an LX_NAND_FLASH object
  * @param block the block number where to write the data
  * @param page the page number within the block fromwhere to write the data
  * @param main_buffer a pointer on the main data to be written
  * @param spare_buffer a pointer on the spare data to be written
  * @param pages number of pages to read.
  * @retval LX_SUCCESS if the operation is successful, LX_ERROR otherwise
  */
static UINT stm32_lx_nand_flash_driver_pages_write(LX_NAND_FLASH *nand_flash, ULONG block, ULONG page,
                                                   UCHAR* main_buffer, UCHAR* spare_buffer, ULONG pages)
{
  UINT status;
  w25n01gvxx_obj_t *p_w25n01gvxx_obj;

  /* initialize the LX_NAND_FLASH object with relevant data */
  STM32_LX_NAND_FLASH_DRIVER_CONTEXT *nand_ctx = (STM32_LX_NAND_FLASH_DRIVER_CONTEXT *) nand_flash->lx_nand_flash_driver_info_ptr;

  /* check if context is valid to prevent a hard fault.  */
  if (nand_ctx == NULL)
  {
    return LX_ERROR;
  }
  /* check that the nor_flash_get_driver_object function pointer is correctly set */
  if (nand_ctx->nand_flash_get_driver_object == NULL)
  {
    return LX_ERROR;
  }
  /* Check if the NAND flash flags indicate the use of a start block offset */
  if (nand_ctx->nand_flash_flags & STM32_LX_NAND_FLAG_START_BLOCK)
  {
    block+= nand_ctx->nand_flash_start_block;
  }
  /* retrieve the memory part object */
  p_w25n01gvxx_obj = (w25n01gvxx_obj_t *)nand_ctx->nand_flash_get_driver_object();

  /* perform the write operation for main buffer */
  status = w25n01gvxx_write(p_w25n01gvxx_obj, block, page, (uint8_t *)main_buffer, W25N01GVXX_DATA_PAGE_SIZE);
  if (status != W25N01GVXX_OK)
  {
    return LX_ERROR;
  }

  /* perform the write operation for spare buffer */
  status = w25n01gvxx_write_spare_area(p_w25n01gvxx_obj, block, page, 1, (uint8_t *)spare_buffer);
  if(status != W25N01GVXX_OK)
  {
    return LX_ERROR;
  }
  return LX_SUCCESS;
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
  * @retval LX_SUCCESS if the pages are successfully copied, LX_ERROR otherwise.
  */
static UINT stm32_lx_nand_flash_driver_pages_copy(LX_NAND_FLASH *nand_flash, ULONG source_block, ULONG source_page,
                                                  ULONG destination_block, ULONG destination_page, ULONG pages,
                                                  UCHAR *data_buffer)
{
  UINT status;
  w25n01gvxx_obj_t *p_w25n01gvxx_obj;

  /* initialize the LX_NAND_FLASH object with relevant data */
  STM32_LX_NAND_FLASH_DRIVER_CONTEXT *nand_ctx = (STM32_LX_NAND_FLASH_DRIVER_CONTEXT *) nand_flash->lx_nand_flash_driver_info_ptr;

  /* check if context is valid to prevent a hard fault.  */
  if (nand_ctx == NULL)
  {
    return LX_ERROR;
  }

  /* check that the nor_flash_get_driver_object function pointer is correctly set */
  if (nand_ctx->nand_flash_get_driver_object == NULL)
  {
    return LX_ERROR;
  }
  /* Check if the NAND flash flags indicate the use of a start block offset */
  if (nand_ctx->nand_flash_flags & STM32_LX_NAND_FLAG_START_BLOCK)
  {
    source_block+= nand_ctx->nand_flash_start_block;
    destination_block+= nand_ctx->nand_flash_start_block;
  }
  /* retrieve the memory part object */
  p_w25n01gvxx_obj = (w25n01gvxx_obj_t *)nand_ctx->nand_flash_get_driver_object();

  /* execute the page copy operation */
  status = w25n01gvxx_page_copy(p_w25n01gvxx_obj, source_block, source_page, destination_block, destination_page);

  if(status != W25N01GVXX_OK)
  {
    return LX_ERROR;
  }
  return LX_SUCCESS;
}

/**
  * @brief Retrieves the status of a specified block in the NAND flash memory
  * @param nand_flash Pointer to an LX_NAND_FLASH object
  * @param block The block number whose status is to be checked.
  * @param bad_block_flag Pointer to a variable where the bad block status will be stored.
  * @retval LX_SUCCESS if the block status is successfully retrieved, LX_ERROR otherwise.
  */
static UINT stm32_lx_nand_flash_driver_block_status_get(LX_NAND_FLASH *nand_flash, ULONG block,
                                                        UCHAR *bad_block_flag)
{
  UINT status;
  w25n01gvxx_obj_t *p_w25n01gvxx_obj;
  UCHAR  spare_buffer[W25N01GVXX_PAGE_SPARE_SIZE] = {0};

  /* initialize the LX_NAND_FLASH object with relevant data */
  STM32_LX_NAND_FLASH_DRIVER_CONTEXT *nand_ctx = (STM32_LX_NAND_FLASH_DRIVER_CONTEXT *) nand_flash->lx_nand_flash_driver_info_ptr;

  /* check if context is valid to prevent a hard fault.  */
  if (nand_ctx == NULL)
  {
    return LX_ERROR;
  }
  /* check that the nor_flash_get_driver_object function pointer is correctly set */
  if (nand_ctx->nand_flash_get_driver_object == NULL)
  {
    return LX_ERROR;
  }
  /* Check if the NAND flash flags indicate the use of a start block offset */
  if (nand_ctx->nand_flash_flags & STM32_LX_NAND_FLAG_START_BLOCK)
  {
    block+= nand_ctx->nand_flash_start_block;
  }
  /* retrieve the memory part object */
  p_w25n01gvxx_obj = (w25n01gvxx_obj_t *)nand_ctx->nand_flash_get_driver_object();

  /* need to read spare area and check the first byte equal to LX_NAND_GOOD_BLOCK or no  */
  status = w25n01gvxx_read_spare_area(p_w25n01gvxx_obj, (uint16_t)block, 0, 0, spare_buffer);

  if (status != W25N01GVXX_OK)
  {
    return LX_ERROR;
  }

  *bad_block_flag = spare_buffer[0];

  return LX_SUCCESS;
}

/**
  * @brief Set the status of a block in the NAND flash memory.
  * @param nand_flash Pointer to an LX_NAND_FLASH object.
  * @param block Block number whose status is to be set.
  * @param bad_block_flag Indicator of the block status
  * @retval LX_SUCCESS if the block status is successfully set, LX_ERROR otherwise.
  */
static UINT stm32_lx_nand_flash_driver_block_status_set(LX_NAND_FLASH *nand_flash, ULONG block,
                                                        UCHAR bad_block_flag)
{
  UINT status;
  w25n01gvxx_obj_t *p_w25n01gvxx_obj;
  UCHAR  spare_buffer[W25N01GVXX_PAGE_SPARE_SIZE/4] = {0};

  /* initialize the LX_NAND_FLASH object with relevant data */
  STM32_LX_NAND_FLASH_DRIVER_CONTEXT *nand_ctx = (STM32_LX_NAND_FLASH_DRIVER_CONTEXT *) nand_flash->lx_nand_flash_driver_info_ptr;

  /* check if context is valid to prevent a hard fault.  */
  if (nand_ctx == NULL)
  {
    return LX_ERROR;
  }
  /* check that the nor_flash_get_driver_object function pointer is correctly set */
  if (nand_ctx->nand_flash_get_driver_object == NULL)
  {
    return LX_ERROR;
  }
  /* Check if the NAND flash flags indicate the use of a start block offset */
  if (nand_ctx->nand_flash_flags & STM32_LX_NAND_FLAG_START_BLOCK)
  {
    block+= nand_ctx->nand_flash_start_block;
  }

  /* retrieve the memory part object */
  p_w25n01gvxx_obj = (w25n01gvxx_obj_t *)nand_ctx->nand_flash_get_driver_object();

  /* Read spare area sector 0 */
  status = w25n01gvxx_read_spare_area(p_w25n01gvxx_obj, block, 0,  0, spare_buffer);

  if(status != W25N01GVXX_OK)
  {
    return LX_ERROR;
  }

  spare_buffer[0] = bad_block_flag;

  status = w25n01gvxx_write_spare_area(p_w25n01gvxx_obj, block, 0, 0, (uint8_t *) spare_buffer);

  if(status != W25N01GVXX_OK)
  {
    return LX_ERROR;
  }
  return LX_SUCCESS;
}

/**
  * @brief Erase a block
  * @param nand_flash LX_NAND_FLASH object pointer
  * @param block the index of the block to erase
  * @param erase_count the number of times the block has been erased
  * @retval LX_SUCCESS if the memory part is ready, LX_ERROR otherwise
  */
static UINT stm32_lx_nand_flash_driver_block_erase(LX_NAND_FLASH *nand_flash, ULONG block, ULONG erase_count)
{
  w25n01gvxx_obj_t *p_w25n01gvxx_obj;
  UINT status = LX_ERROR;

  /* initialize the LX_NAND_FLASH object with relevant data */
  STM32_LX_NAND_FLASH_DRIVER_CONTEXT *nand_ctx = (STM32_LX_NAND_FLASH_DRIVER_CONTEXT *) nand_flash->lx_nand_flash_driver_info_ptr;

  /* check if context is valid to prevent a hard fault.  */
  if (nand_ctx == NULL)
  {
    return LX_ERROR;
  }
  /* check that the nor_flash_get_driver_object function pointer is correctly set */
  if (nand_ctx->nand_flash_get_driver_object == NULL)
  {
    return LX_ERROR;
  }
  /* Check if the NAND flash flags indicate the use of a start block offset */
  if (nand_ctx->nand_flash_flags & STM32_LX_NAND_FLAG_START_BLOCK)
  {
    block+= nand_ctx->nand_flash_start_block;
  }
  /* retrieve the memory part object */
  p_w25n01gvxx_obj = (w25n01gvxx_obj_t *)nand_ctx->nand_flash_get_driver_object();

  /* erase one bleck from the W25N01GVXX NAND flash memory */
  status = w25n01gvxx_erase_blocks(p_w25n01gvxx_obj, block, 1);

  if (status != W25N01GVXX_OK)
  {
    return LX_ERROR;
  }
  return LX_SUCCESS;
}
