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
#ifndef STM32_LX_NAND_FLASH_DRIVER_CONTEXT_H
#define STM32_LX_NAND_FLASH_DRIVER_CONTEXT_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  STM32_LX_NAND_FLAG_NONE               = 0x00000000,
  STM32_LX_NAND_FLAG_START_BLOCK        = 0x00000001,
  STM32_LX_NAND_FLAG_TOTAL_BLOCKS       = 0x00000002,
  STM32_LX_NAND_FLAG_CACHE_MAINTENANCE  = 0x00000004,
  STM32_LX_NAND_FLAG_ALL                = 0x00000007,

} STM32_LX_NAND_FLASH_DRIVER_FLAGS;

typedef VOID *nand_flash_get_object(void);
typedef struct STM32_LX_NAND_FLASH_DRIVER_CONTEXT_STRUCT
{
  STM32_LX_NAND_FLASH_DRIVER_FLAGS nand_flash_flags;
  ULONG                            nand_flash_start_block;
  ULONG                            nand_flash_total_blocks;
  ULONG                            nand_flash_op_timeout;
#ifdef LX_STANDALONE_ENABLE
  volatile UINT                   main_data_rx_cplt;
  volatile UINT                   main_data_tx_cplt;
  volatile UINT                   spare_data_rx_cplt;
  volatile UINT                   spare_data_tx_cplt;
#else
  LX_SEMAPHORE                    main_data_rx_cplt_semaphore;
  LX_SEMAPHORE                    main_data_tx_cplt_semaphore;
  LX_SEMAPHORE                    spare_data_rx_cplt_semaphore;
  LX_SEMAPHORE                    spare_data_tx_cplt_semaphore;

#endif /* LX_STANDALONE_ENABLE */
  nand_flash_get_object           *nand_flash_get_driver_object;

} STM32_LX_NAND_FLASH_DRIVER_CONTEXT;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32_LX_NOR_FLASH_DRIVER_CONTEXT_H */
