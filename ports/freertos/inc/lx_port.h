/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2025 STMicroelectronics
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/
#ifndef LX_PORT_H
#define LX_PORT_H

#ifdef LX_INCLUDE_USER_DEFINE_FILE

/* include the user defines in lx_user.h. The defines in this file may
   alternately be defined on the command line.
*/

#include "lx_user.h"
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* Define compiler library include files.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef VOID_DEFINED
#define VOID_DEFINED
typedef void                                    VOID;
#endif

#ifndef CHAR_DEFINED
#define CHAR_DEFINED
typedef char                                    CHAR;
#endif

#ifndef BOOL_DEFINED
#define BOOL_DEFINED
typedef char                                    BOOL;
#endif

#ifndef UCHAR_DEFINED
#define UCHAR_DEFINED
typedef unsigned char                           UCHAR;
#endif

#ifndef INT_DEFINED
#define INT_DEFINED
typedef int                                     INT;
#endif

#ifndef UINT_DEFINED
#define UINT_DEFINED
typedef unsigned int                            UINT;
#endif

#ifndef LONG_DEFINED
#define LONG_DEFINED
typedef long                                    LONG;
#endif

#ifndef ULONG_DEFINED
#define ULONG_DEFINED
typedef unsigned long                           ULONG;
#endif

#ifndef SHORT_DEFINED
#define SHORT_DEFINED
typedef short                                   SHORT;
#endif

#ifndef USHORT_DEFINED
#define USHORT_DEFINED
typedef unsigned short                          USHORT;
#endif

#ifndef ULONG64_DEFINED
#define ULONG64_DEFINED
typedef unsigned long long                      ULONG64;
#endif

/* Define basic alignment type used in block and byte pool operations. This data type must
   be at least 32-bits in size and also be large enough to hold a pointer type.  */

#ifndef ALIGN_TYPE_DEFINED
#define ALIGN_TYPE_DEFINED
#define ALIGN_TYPE                              ULONG
#endif

/* Define the LX_MEMSET macro to the standard library function, if not already defined.  */
#ifndef LX_MEMSET
#define LX_MEMSET(a,b,c)                        memset((a),(b),(c))
#endif

#ifndef LX_MEMCPY
#define LX_MEMCPY(a,b,c)                        memcpy((a),(b),(c))
#endif

#define LX_INTERRUPT_SAVE_AREA
#define LX_DISABLE                              taskDISABLE_INTERRUPTS();
#define LX_RESTORE                              taskENABLE_INTERRUPTS();

/* Define LevelX RTOS structures */
typedef struct
{
  SemaphoreHandle_t semaphore_handle;
} lx_semaphore_t;

typedef struct
{
  SemaphoreHandle_t mutex_handle;
} lx_mutex_t;

#define LX_MUTEX                                lx_mutex_t
#define LX_SEMAPHORE                            lx_semaphore_t

static inline UINT lx_os_mutex_create(LX_MUTEX *mutex, CHAR* mutex_name)
{
  mutex->mutex_handle = xSemaphoreCreateMutex();
  if (mutex->mutex_handle == NULL)
  {
    return 1;
  }
  else
  {
#if (configQUEUE_REGISTRY_SIZE > 0)
    if (mutex_name != NULL)
    {
      vQueueAddToRegistry((QueueHandle_t) mutex->mutex_handle, mutex_name);
    }
#endif
    return 0;
  }
}

static inline VOID lx_os_mutex_delete(LX_MUTEX *mutex)
{
#if (configQUEUE_REGISTRY_SIZE > 0)
  vQueueUnregisterQueue((QueueHandle_t) mutex->mutex_handle);
#endif
  vSemaphoreDelete(mutex->mutex_handle);
  mutex->mutex_handle = NULL;
}

static inline VOID lx_os_mutex_get(LX_MUTEX *mutex)
{
  xSemaphoreTake(mutex->mutex_handle, portMAX_DELAY);
}

static inline VOID lx_os_mutex_put(LX_MUTEX *mutex)
{
  xSemaphoreGive(mutex->mutex_handle);
}


#endif /* lX_PORT_H */
