

# Release Notes for
# <mark>LevelX SW Pack</mark>


# Purpose

STM32Cube enables developers to achieve design success. With a comprehensive suite of professional development tools and embedded software components, STM32Cube allows developers to differentiate products, streamline design cycles, and reduce costs. STM32Cube ecosystem supports all design steps, including selection, configuration, development, debugging, programming, and monitoring.

The STM32Cube embedded software offer provides ready-to-use software components that can be added to a project. It includes STM32 peripheral driver APIs with two levels of abstraction, middleware, board drivers, and examples. There are several distribution channels, including the STM32CubeMX2 tool, the ST website, and GitHub. All embedded software comes with enhanced online documentation, with flowcharts and user sequences.

**LevelX** is an open-source library part of Eclipse ThreadX project maintained by the Eclipse foundation.
It offers services to access NOR and NAND flash memories including a built-in wear leveling and bad block management.
LevelX is not intended to provide Filesystem APIs but only, low-level API to read, write, erase sectors in flash memories, but combined with FileX it is possible to use NAND and NOR flash memories as media storage devices.

More documentation is available at [LevelX SW Pack online documentation](https://dev.st.com/stm32cube-docs/mw-levelx/latest/en/index.html).

# Update history

<label for="collapse-section-2.1.0" aria-hidden="true">__2.1.0 / 12-June-2026__</label>
<div>

## Main changes

Release of LevelX SW Pack update:

  - Bug fixing.
  - Miscellaneous update of .config files.

This pack is based on LevelX [V6.5.0 version](https://github.com/eclipse-threadx/levelx/releases/tag/v6.5.0.202601_rel)

## Contents

- **LevelX** is RTOS-agnostic (can be used with FreeRTOS or other RTOSs)
- Supported interfaces: **NOR** and **NAND** flash

## Known limitations

- Cache is not supported on NOR and NAND interfaces.
- Block size is not customizable on NOR interfaces.
- Multi-instance (creating multiple interfaces) is not supported.

## Development toolchains and compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V9.60.3 + ST-LINK
- MDK-ARM Keil uVision V5.42
- STM32CubeIDE for Visual Studio Code (GCC13 compiler)
- STM32CubeMX2 V1.0.1

## Supported devices and boards

- STM32C5 series

## Backward compatibility

- None

## Dependencies

- STM32C5xx HAL Drivers V2.0.0
- FreeRTOS SW Pack V2.0.0 (required when selecting RTOS mode)
- W25Q128J Part Drivers SW Pack V2.0.0
- W25N01GVXX Part Drivers SW Pack V2.0.0
</div>

<label for="collapse-section-2.0.0" aria-hidden="true">__2.0.0 / 13-March-2026__</label>
<div>

## Main changes

First Official release of LevelX SW Pack.

This pack is based on LevelX [V6.4.1 version](https://github.com/eclipse-threadx/levelx/releases/tag/v6.4.1_rel)

## Contents

- **LevelX** is RTOS-agnostic (can be used with FreeRTOS or other RTOSs)
- Supported interfaces: **NOR** and **NAND** flash

## Known limitations

- None

## Development toolchains and compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V9.60.3 + ST-LINK
- MDK-ARM Keil uVision V5.42
- STM32CubeIDE for Visual Studio Code (GCC13 compiler)

## Supported devices and boards

- STM32C5 series

## Backward compatibility

- None

## Dependencies

- STM32C5xx HAL Drivers V2.0.0
- FreeRTOS SW Pack V2.0.0 (required when selecting RTOS mode)
</div>


For complete documentation on STM32 microcontrollers,
visit: [www.st.com/stm32](http://www.st.com/stm32)
<abbr title="Based on template cx566953 version 2.1">Info</abbr>

This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.