/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026-present Eclipse ThreadX contributors
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
/*  RELEASE HISTORY                                                        */
/*                                                                         */
/*    DATE              NAME                      DESCRIPTION              */
/*                                                                         */
/*  05-19-2020     William E. Lamie         Initial Version 6.0            */
/*  09-30-2020     William E. Lamie         Modified comment(s),           */
/*                                            resulting in version 6.1     */
/*  11-09-2020     William E. Lamie         Modified comment(s),           */
/*                                            fixed compiler warnings,     */
/*                                            resulting in version 6.1.2   */
/*  12-30-2020     William E. Lamie         Modified comment(s),           */
/*                                            fixed compiler warnings,     */
/*                                            resulting in version 6.1.3   */
/*  06-02-2021     Bhupendra Naphade        Modified comment(s), and       */
/*                                            updated product constants    */
/*                                            resulting in version 6.1.7   */
/*  03-08-2023     Xiuwen Cai               Modified comment(s),           */
/*                                            added new driver interface,  */
/*                                            resulting in version 6.2.1   */
/*  10-31-2023     Xiuwen Cai               Modified comment(s),           */
/*                                            added count for minimum      */
/*                                            erased blocks, added         */
/*                                            obsolete count cache,        */
/*                                            avoided clearing user        */
/*                                            extension in flash control   */
/*                                            block,                       */
/*                                            resulting in version 6.3.0   */
/* 10-21-2025                              Modified comment(s)             */
/*                                            call the extended equivalent */
/*                                              API                        */

UINT  _lx_nor_flash_open(LX_NOR_FLASH  *nor_flash, CHAR *name, UINT (*nor_driver_initialize)(LX_NOR_FLASH *))
{
UINT status = LX_ERROR;

    status = _lx_nor_flash_open_extended(nor_flash, name, nor_driver_initialize, NULL);

    return status;
}
