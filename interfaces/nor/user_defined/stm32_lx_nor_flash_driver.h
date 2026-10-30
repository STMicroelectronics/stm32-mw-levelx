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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32_LX_NOR_CUSTOM_DRIVER_H
#define STM32_LX_NOR_CUSTOM_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Define the NOR flash memory properties. */
#error "[This error was thrown on purpose] : define the constants below related to the NOR Flash memory used, then remove this line"

#define CUSTOM_NOR_FLASH_BASE_ADDRESS             0             /* Base address of the NOR flash. */
#define CUSTOM_NOR_TOTAL_BLOCKS                   0             /* Number of erase blocks. */
#define CUSTOM_NOR_WORDS_PER_BLOCK                0             /* Number of words per block. */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
UINT lx_stm32_nor_custom_driver_initialize(LX_NOR_FLASH *nor_flash);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* STM32_LX_NOR_CUSTOM_DRIVER_H */
