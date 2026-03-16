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
/*    _lx_nand_flash_format                               PORTABLE C      */ 
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Xiuwen Cai, Microsoft Corporation                                   */
/*                                                                        */
/*  DESCRIPTION                                                           */ 
/*                                                                        */ 
/*    This function erases and formats a NAND flash.                      */ 
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
/*  _lx_nand_flash_format_extended                                        */
/*                                                                        */
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    Application Code                                                    */ 
/*                                                                        */ 
/**************************************************************************/
UINT  _lx_nand_flash_format(LX_NAND_FLASH* nand_flash, CHAR* name,
                            UINT(*nand_driver_initialize)(LX_NAND_FLASH*),
                            ULONG* memory_ptr, UINT memory_size)
{

UINT status;

    status = _lx_nand_flash_format_extended(nand_flash, name, nand_driver_initialize, NULL, memory_ptr, memory_size);

    return status;
}