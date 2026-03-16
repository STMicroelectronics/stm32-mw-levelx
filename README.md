

# Middleware LevelX Component

![tag](https://img.shields.io/badge/tag-2.0.0-brightgreen.svg)
[![release note](https://img.shields.io/badge/release_note-view_html-gold.svg)](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/stm32-mw-levelx/blob/hal2/ST_Release_Notes.html)

## Overview

The LevelX middleware component is an STM32-tailored fork of the [Eclipse ThreadX LevelX](https://github.com/eclipse-threadx/levelx).

LevelX provides NAND and NOR flash wear-leveling functionality, exposing an array of logical sectors mapped to the underlying flash memory. LevelX can be used as a standalone wear-leveling layer or in conjunction with FileX.

The main goal of this component is to provide STM32-ready low-level flash interfaces through the STM32 HAL component drivers.


## RTOS support

This LevelX fork provides both RTOS and bare-metal support. It is decoupled from hardcoded RTOS dependencies.

The **ports** folder provides a reference implementation on top of FreeRTOS™.

## Low level drivers

The **interfaces** folder contains a set of ready-to-use low-level drivers for each flash memory type and component.

| Media type (folder) | Memory Part | Driver flavor | Description |
|---|---|---|---|
| NOR flash (`interfaces/nor`) | `mx25lm51245g` | Polling | Blocking accesses using the MX25LM51245G part driver component. |
| NOR flash (`interfaces/nor`) | `mx25lm51245g` | DMA (RTOS) | DMA-based accesses with RTOS synchronization . |
| NOR flash (`interfaces/nor`) | `mx25lm51245g` | DMA (No-OS) | DMA-based accesses without RTOS objects; waits for transfer completion using polling/timeouts. |
| NOR flash (`interfaces/nor`) | `mx66uw1g45g` | Polling | Blocking accesses using the MX66UW1G45G part driver component. |
| NOR flash (`interfaces/nor`) | `mx66uw1g45g` | DMA (RTOS) | DMA-based accesses with RTOS synchronization . |
| NOR flash (`interfaces/nor`) | `mx66uw1g45g` | DMA (No-OS) | DMA-based accesses without RTOS objects; waits for transfer completion using polling/timeouts. |
| NOR flash (`interfaces/nor`) | `w25q128j` | Polling | Blocking accesses using the W25Q128J part driver component. |
| NOR flash (`interfaces/nor`) | `w25q128j` | DMA (RTOS) | DMA-based accesses with RTOS synchronization . |
| NOR flash (`interfaces/nor`) | `w25q128j` | DMA (No-OS) | DMA-based accesses without RTOS objects; waits for transfer completion using polling/timeouts. |
| NAND flash (`interfaces/nand`) | `w25n01gvxx` | Polling | Blocking accesses using the W25N01GVXX part driver component. |
| NAND flash (`interfaces/nand`) | `w25n01gvxx` | DMA (RTOS) | DMA-based accesses with RTOS synchronization . |
| NAND flash (`interfaces/nand`) | `w25n01gvxx` | DMA (No-OS) | DMA-based accesses without RTOS objects; waits for transfer completion using polling/timeouts. |

See `interfaces/readme.txt` for the low-level drivers changelog.
