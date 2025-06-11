  /**
  ******************************************************************************
  * @file    st_readme.txt
  * @author  GPM Application Team
  * @brief   This file lists the main changes done by STMicroelectronics on
  *          LevelX for STM32 devices.
  ******************************************************************************
  */

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