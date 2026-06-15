/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026-present Eclipse ThreadX contributors
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
/*    _lx_nand_flash_driver_block_status_get              PORTABLE C      */
/*                                                           6.2.1       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Microsoft Corporation                             */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calls the driver to get the block status and          */
/*    updates the internal cache.                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    nand_flash                            NAND flash instance           */
/*    block                                 Block number                  */
/*    bad_block_flag                        Pointer to Bad block flag     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    (lx_nand_flash_driver_block_status_get)                             */
/*                                          NAND flash block status get   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal LevelX                                                     */
/*                                                                        */
/**************************************************************************/
UINT  _lx_nand_flash_driver_block_status_get(LX_NAND_FLASH *nand_flash, ULONG block, UCHAR *bad_block_flag)
{

UINT    status;


    /* Increment the block status get count.  */
    nand_flash -> lx_nand_flash_diagnostic_block_status_gets++;

    /* Call driver block status get function.  */
#ifdef LX_NAND_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
    status =  (nand_flash -> lx_nand_flash_driver_block_status_get)(nand_flash, block, bad_block_flag);
#else
    status =  (nand_flash -> lx_nand_flash_driver_block_status_get)(block, bad_block_flag);
#endif

    /* Return status.  */
    return(status);
}


