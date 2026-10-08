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
/*    _lx_nand_flash_extended_cache_enable                PORTABLE C      */
/*                                                           6.2.1       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Microsoft Corporation                             */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets up the extended NAND cache for block status,     */
/*    page extra bytes, and page 0 contents. The routine will enable as   */
/*    many cache capabilities as possible until the supplied memory is    */
/*    exhausted.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    nand_flash                            NAND flash instance           */
/*    memory                                Pointer to memory for caches  */
/*    size                                  Size of memory in bytes       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal LevelX                                                     */
/*                                                                        */
/**************************************************************************/
UINT  _lx_nand_flash_extended_cache_enable(LX_NAND_FLASH  *nand_flash, VOID *memory, ULONG size)
{

    LX_PARAMETER_NOT_USED(nand_flash);
    LX_PARAMETER_NOT_USED(memory);
    LX_PARAMETER_NOT_USED(size);

    /* Return not supported.  */
    return(LX_NOT_SUPPORTED);
}

