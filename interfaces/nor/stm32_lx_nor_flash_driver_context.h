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
#ifndef STM32_LX_NOR_FLASH_DRIVER_CONTEXT_H
#define STM32_LX_NOR_FLASH_DRIVER_CONTEXT_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "lx_api.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  STM32_LX_NOR_FLAG_NONE               = 0x00000000,
  STM32_LX_NOR_FLAG_START_ADDRESS      = 0x00000001,
  STM32_LX_NOR_FLAG_BLOCK_SIZE         = 0x00000002,
  STM32_LX_NOR_FLAG_FLASH_SIZE         = 0x00000004,
  STM32_LX_NOR_FLAG_READ_BUFFER        = 0x00000008,
  STM32_LX_NOR_FLAG_CACHE_MAINTENANCE  = 0x00000010,
  STM32_LX_NOR_FLAG_ALL                = 0x0000001F,

} STM32_LX_NOR_FLASH_DRIVER_FLAGS;

typedef VOID *nor_flash_get_object(void);
typedef struct STM32_LX_NOR_FLASH_DRIVER_CONTEXT_STRUCT
{
  STM32_LX_NOR_FLASH_DRIVER_FLAGS nor_flash_flags;
  ULONG                           nor_flash_start_address;
  ULONG                           nor_flash_block_size;
  ULONG                           nor_flash_size;
  ULONG                           nor_flash_op_timeout;
#ifdef LX_STANDALONE_ENABLE
  volatile UINT                   data_rx_cplt;
  volatile UINT                   data_tx_cplt;
  volatile UINT                   erase_cplt;
#else
  LX_SEMAPHORE                    data_rx_cplt_semaphore;
  LX_SEMAPHORE                    data_tx_cplt_semaphore;
  LX_SEMAPHORE                    erase_cplt_semaphore;

#endif /* LX_STANDALONE_ENABLE */
  ULONG                           *nor_flash_read_buffer;
  nor_flash_get_object            *nor_flash_get_driver_object;

} STM32_LX_NOR_FLASH_DRIVER_CONTEXT;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32_LX_NOR_FLASH_DRIVER_CONTEXT_H */
