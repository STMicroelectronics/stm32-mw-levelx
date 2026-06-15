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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _lx_nor_flash_system_error                          PORTABLE C      */
/*                                                           6.2.1       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    William E. Lamie, Microsoft Corporation                             */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function handles system errors in the NOR flash.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    nor_flash                             NOR flash instance            */
/*    error_code                            System error code             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    (lx_nor_flash_driver_system_error)    Driver system error handler   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal LevelX                                                     */
/*                                                                        */
/**************************************************************************/
VOID  _lx_nor_flash_system_error(LX_NOR_FLASH *nor_flash, UINT error_code)
{

    /* Increment the system error counter.  */
    nor_flash -> lx_nor_flash_diagnostic_system_errors++;

    /* Save the most recent system error code.  */
    nor_flash -> lx_nor_flash_diagnostic_system_error =  error_code;

    /* Determine if the driver has setup a system error handler.  */
    if (nor_flash -> lx_nor_flash_driver_system_error)
    {

        /* Yes, call the driver's system error handler.  */
#ifdef LX_NOR_ENABLE_CONTROL_BLOCK_FOR_DRIVER_INTERFACE
        (nor_flash -> lx_nor_flash_driver_system_error)(nor_flash, error_code);
#else
        (nor_flash -> lx_nor_flash_driver_system_error)(error_code);
#endif
    }
}

