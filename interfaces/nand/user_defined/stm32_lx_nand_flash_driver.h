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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_LX_NAND_CUSTOM_DRIVER_H
#define STM32_LX_NAND_CUSTOM_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Define the NAND flash memory properties. */
#error "[This error was thrown on purpose] : define the constants below related to the NAND Flash memory used, then remove this line"

#define CUSTOM_NAND_TOTAL_BLOCKS                  0             /* Number of total blocks. */
#define CUSTOM_NAND_PAGES_PER_BLOCK               0             /* Number of pages per block. */
#define CUSTOM_NAND_BYTES_PER_PAGE                0             /* Number of bytes per page. */
#define CUSTOM_NAND_SPARE_TOTAL_LENGTH            0             /* Number of spare bytes per page. */
#define CUSTOM_NAND_SPARE_DATA1_OFFSET            0             /* Offset of spare data 1. */
#define CUSTOM_NAND_SPARE_DATA1_LENGTH            0             /* Length of spare data 1. */
#define CUSTOM_NAND_SPARE_DATA2_OFFSET            0             /* Offset of spare data 2. */
#define CUSTOM_NAND_SPARE_DATA2_LENGTH            0             /* Length of spare data 2. */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
UINT lx_stm32_nand_custom_driver_initialize(LX_NAND_FLASH *nand_flash);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* STM32_LX_NAND_CUSTOM_DRIVER_H */
