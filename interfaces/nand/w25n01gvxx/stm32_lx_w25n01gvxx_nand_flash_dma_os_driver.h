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
#ifndef STM32_LX_W25N01GVXX_FLASH_DMA_OS_H
#define STM32_LX_W25N01GVXX_FLASH_DMA_OS_H
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "w25n01gvxx.h"
#include "stm32_lx_nand_flash_driver_context.h"

UINT stm32_lx_w25n01gvxx_nand_flash_dma_os_driver_initialize(LX_NAND_FLASH *nand_flash);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* STM32_LX_W25N01GVXX_FLASH_DMA_OS_H */
