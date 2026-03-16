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
#ifndef STM32_LX_W25Q128J_NOR_FLASH_POLLING_DRIVER_H
#define STM32_LX_W25Q128J_NOR_FLASH_POLLING_DRIVER_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "w25q128j.h"
#include "stm32_lx_nor_flash_driver_context.h"

UINT stm32_lx_w25q128j_nor_flash_polling_driver_initialize(LX_NOR_FLASH *nor_flash);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* STM32_LX_W25Q128J_NOR_FLASH_POLLING_DRIVER_H */
