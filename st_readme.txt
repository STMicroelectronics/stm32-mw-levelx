  /**
  ******************************************************************************
  * @file    st_readme.txt
  * @brief   This file lists the main changes done by STMicroelectronics on
  *          LevelX for STM32 devices.
  /*****************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  *
  * This program and the accompanying materials are made available under the
  * terms of the MIT License which is available at
  * https://opensource.org/licenses/MIT.
  *
  * SPDX-License-Identifier: MIT
  *****************************************************************************/

### V6.4.1 (12-11-2025) ###
=================================
- Revise copyright information in file headers.
- Enhance LX_MUTEX and LX_SEMAPHORE implementations for proper LevelX RTOS Agnostic synchronization.
- Add open state checks at the start of user NOR APIs to prevent operations when flash memory is closed.

### V6.4.1 (30-05-2025) ###
============================
- Implement RTOS agnostic support features:
  - Add lx_port.h to the ports/freertos/inc directory.
  - Add lx_port.h to the ports/generic/inc directory, ensuring it is not specific to the FreeRTOS port.
  - Remove related and non related ThreadX macros from lx_api.h and add the non related ones to lx_port.h.
  - Add APIs for the management of mutexes in lx_port.h under the ports/freertos/inc directory. This includes functions to:
    - Create mutexes
    - Delete mutexes
    - Get (lock) mutexes
    - Put (unlock) mutexes
- Add LevelX 6.4.1 from Eclipse ThreadX