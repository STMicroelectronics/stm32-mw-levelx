/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2025-2026 STMicroelectronics
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** LevelX Component                                                      */
/**                                                                       */
/**   NAND Flash                                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define LX_SOURCE_CODE


/* Disable ThreadX error checking.  */

#ifndef LX_DISABLE_ERROR_CHECKING
#define LX_DISABLE_ERROR_CHECKING
#endif


/* Include necessary system files.  */

#include "lx_api.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _lx_nand_flash_open                                 PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Xiuwen Cai, Microsoft Corporation                                   */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function opens a NAND flash instance and ensures the           */
/*    NAND flash is in a coherent state.                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    nand_flash                            NAND flash instance           */
/*    name                                  Name of NAND flash instance   */
/*    nand_driver_initialize                Driver initialize             */
/*    memory_ptr                            Pointer to memory used by the */
/*                                            LevelX for this NAND.       */
/*    memory_size                           Size of memory - must at least*/
/*                                            7 * total block count +     */
/*                                            2 * page size               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    return status                                                       */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    (nand_driver_initialize)              Driver initialize             */
/*    _lx_nand_flash_memory_initialize      Initialize buffer             */
/*    _lx_nand_flash_driver_block_status_get                              */
/*                                          Get block status              */
/*    lx_nand_flash_driver_pages_read       Read pages                    */
/*    _lx_nand_flash_free_block_list_add    Add free block to list        */
/*    _lx_nand_flash_mapped_block_list_add  Add mapped block to list      */
/*    _lx_nand_flash_system_error           System error handler          */
/*    lx_os_mutex_create                    Create thread-safe mutex      */
/*    lx_nand_flash_open_extended           Pass NULL as driver_info_ptr  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  03-08-2023     Xiuwen Cai               Initial Version 6.2.1         */
/*  10-31-2023     Xiuwen Cai               Modified comment(s),          */
/*                                            avoided clearing user       */
/*                                            extension in flash control  */
/*                                            block,                      */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT  _lx_nand_flash_open(LX_NAND_FLASH  *nand_flash, CHAR *name, UINT (*nand_driver_initialize)(LX_NAND_FLASH *),
                          ULONG* memory_ptr, UINT memory_size)
{
UINT status = LX_ERROR;

    status = _lx_nand_flash_open_extended(nand_flash, name, nand_driver_initialize, NULL, memory_ptr, memory_size);

    return status;
}
