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
/*    _lx_nand_flash_driver_block_erased_verify           PORTABLE C      */
/*                                                           6.2.1       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Microsoft Corporation                             */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function calls the driver to verify the block was erased.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    nand_flash                            NAND flash instance           */
/*    block                                 Block number                  */
/*    page                                  Page number                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    (lx_nand_flash_driver_block_erased_verify)                          */
/*                                          Driver verify page erased     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal LevelX                                                     */
/*                                                                        */
/**************************************************************************/
UINT  _lx_nand_flash_driver_block_erased_verify(LX_NAND_FLASH *nand_flash, ULONG block)
{

UINT    status;

    /* Increment the block erased verify count.  */
    nand_flash -> lx_nand_flash_diagnostic_block_erased_verifies++;

    /* Call driver block erased verify function.  */
#ifdef LX_NAND_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
    status =  (nand_flash -> lx_nand_flash_driver_block_erased_verify)(nand_flash, block);
#else
    status =  (nand_flash -> lx_nand_flash_driver_block_erased_verify)(block);
#endif
    /* Return status.  */
    return(status);
}


