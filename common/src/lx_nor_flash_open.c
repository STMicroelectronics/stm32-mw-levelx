/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Portion Copyright (c) 2025-2026 STMicroelectronics
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
/**   NOR Flash                                                           */
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
/***************************************************************************/
/*                                                                         */
/*  FUNCTION                                               RELEASE         */
/*                                                                         */
/*    _lx_nor_flash_open                                  PORTABLE C       */
/*                                                           6.3.0         */
/*  AUTHOR                                                                 */
/*                                                                         */
/*    William E. Lamie, Microsoft Corporation                              */
/*                                                                         */
/*  DESCRIPTION                                                            */
/*                                                                         */
/*    This function opens a NOR flash instance and ensures the NOR flash   */
/*    is in a coherent state.                                              */
/*                                                                         */
/*  INPUT                                                                  */
/*                                                                         */
/*    nor_flash                             NOR flash instance             */
/*    name                                  Name of NOR flash instance     */
/*    nor_driver_initialize                 Driver initialize              */
/*                                                                         */
/*  OUTPUT                                                                 */
/*                                                                         */
/*    return status                                                        */
/*                                                                         */
/*  CALLS                                                                  */
/*  lx_nor_flash_open_extended()                                           */
/*                                                                         */
/*  CALLED BY                                                              */
/*                                                                         */
/*    Application Code                                                     */
/*                                                                         */
/***************************************************************************/
UINT  _lx_nor_flash_open(LX_NOR_FLASH  *nor_flash, CHAR *name, UINT (*nor_driver_initialize)(LX_NOR_FLASH *))
{
UINT status = LX_ERROR;

    status = _lx_nor_flash_open_extended(nor_flash, name, nor_driver_initialize, NULL);

    return status;
}
