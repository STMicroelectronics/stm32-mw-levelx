# origin-pack: STMicroelectronics::levelx@2.0.0
# file-format: 1.0.0
project(STMicroelectronics_levelx_2_0_0)
cmake_minimum_required(VERSION 3.20)
add_library(STMicroelectronics_levelx_2_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package
if(NOT DEFINED CMSIS_Dname)
  set(CMSIS_Dname "CMSIS_Dname not set")
  message(DEBUG "CMSIS_Dname is not set to any value")
endif()

if(NOT DEFINED CMSIS_Dvendor)
  set(CMSIS_Dvendor "CMSIS_Dvendor not set")
  message(DEBUG "CMSIS_Dvendor is not set to any value")
endif()


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_STMicroelectronics_levelx_2_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:NOR Flash Core#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:NAND Flash Core#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:No_OS#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx25lm51245g_nor_polling_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx25lm51245g_nor_dma_no_os_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx25lm51245g_nor_dma_os_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25q128j_nor_polling_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25q128j_nor_dma_no_os_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25q128j_nor_dma_os_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25n01gvxx_nand_polling_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25n01gvxx_nand_dma_no_os_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25n01gvxx_nand_dma_os_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx66uw1g45g_nor_polling_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx66uw1g45g_nor_dma_no_os_itf#Cversion:6.4.1")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx66uw1g45g_nor_dma_os_itf#Cversion:6.4.1")
endif()

# All conditions used by this package

# condition: STMicroelectronics.levelx.2.0.0
# description: 
if((CMSIS_Dname MATCHES "STM32.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(STMicroelectronics.levelx.2.0.0 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0 enabled")
else()
  set(STMicroelectronics.levelx.2.0.0 0)
endif()


# condition: STMicroelectronics.levelx.2.0.0:LX Core condition
# description: levelx core dependency
set(STMicroelectronics.levelx.2.0.0_LX_Core_condition "$<OR:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:NOR Flash Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:NAND Flash Core(#.*|$)>,>>>")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_LX_Core_condition enabled")


# condition: STMicroelectronics.levelx.2.0.0:Operating System Used
# description: 
set(STMicroelectronics.levelx.2.0.0_Operating_System_Used "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_Operating_System_Used enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE MX25LM51245G DMA NO OS
# description: LX MX25LM51245G NO OS interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_NO_OS "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_NO_OS enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE MX25LM51245G DMA OS
# description: LX MX25LM51245G DMA OS interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_OS "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_OS enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE MX25LM51245G POLLING
# description: LX MX25LM51245G POLLING interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_POLLING "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_POLLING enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE MX66UW1G45G DMA NO OS
# description: LX MX66UW1G45G DMA NO OS interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_NO_OS "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_NO_OS enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE MX66UW1G45G DMA OS
# description: LX MX66UW1G45G DMA OS interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_OS "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_OS enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE MX66UW1G45G POLLING
# description: LX MX66UW1G45G POLLING interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_POLLING "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_POLLING enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE W25N01GVXX DMA NO OS
# description: LX W25N01GVXX NO OS interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_NO_OS "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_NO_OS enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE W25N01GVXX DMA OS
# description: LX W25N01GVXX DMA OS interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_OS "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_OS enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE W25N01GVXX POLLING
# description: LX W25N01GVXX POLLING interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_POLLING "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_POLLING enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE W25Q128J DMA NO OS
# description: LX W25Q128J DMA NO OS interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_NO_OS "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_NO_OS enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE W25Q128J DMA OS
# description: LX W25Q128J DMA OS interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_OS "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_OS enabled")


# condition: STMicroelectronics.levelx.2.0.0:USE W25Q128J POLLING
# description: LX W25Q128J POLLING interface dependency
set(STMicroelectronics.levelx.2.0.0_USE_W25Q128J_POLLING "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_USE_W25Q128J_POLLING enabled")


# condition: STMicroelectronics.levelx.2.0.0:Use NAND Flash Core
# description: LevelX NAND Flash Core dependency
set(STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core enabled")


# condition: STMicroelectronics.levelx.2.0.0:Use NOR Flash Core
# description: LevelX NOR Flash Core dependency
set(STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core "1")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core enabled")


# condition: STMicroelectronics.levelx.2.0.0:levelx freertos condition
# description: 
set(STMicroelectronics.levelx.2.0.0_levelx_freertos_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:RTOS#.*Cgroup:FreeRTOS#.*Csub:Core(#.*|$)>,>>,$<NOT:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:File System#.*Cgroup:LevelX#.*Csub:No_OS(#.*|$)>,>>>>")
message(DEBUG "CMSIS condition STMicroelectronics.levelx.2.0.0_levelx_freertos_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:NOR Flash Core#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # Component to support NOR flash
  message(DEBUG "Using component File_System_LevelX_NOR_Flash_Core_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:-DCMSIS_USE_File_System_LevelX_NOR_Flash_Core_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/inc>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_block_reclaim.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_close.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_defragment.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_driver_block_erase.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_driver_read.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_driver_write.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_extended_cache_enable.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_format.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_initialize.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_logical_sector_find.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_next_block_to_erase_find.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_open.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_open_extended.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_partial_defragment.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_physical_sector_allocate.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_sector_mapping_cache_invalidate.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_sector_read.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_sector_release.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_sector_write.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nor_flash_system_error.c>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<$<AND:${STMicroelectronics.levelx.2.0.0_Use_NOR_Flash_Core},${STMicroelectronics.levelx.2.0.0_levelx_freertos_condition}>:${CMAKE_CURRENT_LIST_DIR}/ports/freertos/inc/>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:NAND Flash Core#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # Component to support NAND flash
  message(DEBUG "Using component File_System_LevelX_NAND_Flash_Core_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:-DCMSIS_USE_File_System_LevelX_NAND_Flash_Core_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/inc>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_256byte_ecc_check.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_256byte_ecc_compute.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_block_allocate.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_block_data_move.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_block_find.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_block_mapping_set.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_block_status_set.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_close.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_data_page_copy.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_defragment.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_driver_block_erase.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_driver_block_erased_verify.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_driver_block_status_get.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_driver_block_status_set.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_driver_page_erased_verify.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_erase_count_set.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_extended_cache_enable.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_format.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_format_extended.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_free_block_list_add.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_initialize.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_mapped_block_list_add.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_mapped_block_list_get.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_mapped_block_list_remove.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_memory_initialize.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_metadata_allocate.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_metadata_build.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_metadata_write.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_open.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_open_extended.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_page_ecc_check.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_page_ecc_compute.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_partial_defragment.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_sector_read.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_sector_release.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_sector_write.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_sectors_read.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_sectors_release.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_sectors_write.c>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core}:${CMAKE_CURRENT_LIST_DIR}/common/src/lx_nand_flash_system_error.c>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<$<AND:${STMicroelectronics.levelx.2.0.0_Use_NAND_Flash_Core},${STMicroelectronics.levelx.2.0.0_levelx_freertos_condition}>:${CMAKE_CURRENT_LIST_DIR}/ports/freertos/inc/>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:No_OS#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # LevelX Standalone Mode 
  message(DEBUG "Using component File_System_LevelX_No_OS_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "-DCMSIS_USE_File_System_LevelX_No_OS_6_4_1=1")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/ports/generic/inc/")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx25lm51245g_nor_polling_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # Polling interface for NOR flash mx25lm51245g
  message(DEBUG "Using component File_System_LevelX_mx25lm51245g_nor_polling_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_POLLING}:-DCMSIS_USE_File_System_LevelX_mx25lm51245g_nor_polling_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx25lm51245g>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx25lm51245g/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx25lm51245g/stm32_lx_mx25lm51245g_nor_flash_polling_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx25lm51245g_nor_dma_no_os_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # DMA no os interface for NOR flash mx25lm51245g
  message(DEBUG "Using component File_System_LevelX_mx25lm51245g_nor_dma_no_os_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_NO_OS}:-DCMSIS_USE_File_System_LevelX_mx25lm51245g_nor_dma_no_os_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx25lm51245g>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx25lm51245g/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx25lm51245g/stm32_lx_mx25lm51245g_nor_flash_dma_no_os_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx25lm51245g_nor_dma_os_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # DMA os interface for NOR flash mx25lm51245g
  message(DEBUG "Using component File_System_LevelX_mx25lm51245g_nor_dma_os_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_OS}:-DCMSIS_USE_File_System_LevelX_mx25lm51245g_nor_dma_os_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx25lm51245g>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx25lm51245g/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX25LM51245G_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx25lm51245g/stm32_lx_mx25lm51245g_nor_flash_dma_os_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25q128j_nor_polling_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # Polling interface for NOR flash w25q128j
  message(DEBUG "Using component File_System_LevelX_w25q128j_nor_polling_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_POLLING}:-DCMSIS_USE_File_System_LevelX_w25q128j_nor_polling_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/w25q128j>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/w25q128j/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/w25q128j/stm32_lx_w25q128j_nor_flash_polling_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25q128j_nor_dma_no_os_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # DMA no os interface for NOR flash w25q128j
  message(DEBUG "Using component File_System_LevelX_w25q128j_nor_dma_no_os_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_NO_OS}:-DCMSIS_USE_File_System_LevelX_w25q128j_nor_dma_no_os_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/w25q128j>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/w25q128j/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/w25q128j/stm32_lx_w25q128j_nor_flash_dma_no_os_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25q128j_nor_dma_os_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # DMA os interface for NOR flash w25q128j
  message(DEBUG "Using component File_System_LevelX_w25q128j_nor_dma_os_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_OS}:-DCMSIS_USE_File_System_LevelX_w25q128j_nor_dma_os_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/w25q128j>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/w25q128j/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25Q128J_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/w25q128j/stm32_lx_w25q128j_nor_flash_dma_os_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25n01gvxx_nand_polling_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # Polling interface for NAND flash w25n01gvxx
  message(DEBUG "Using component File_System_LevelX_w25n01gvxx_nand_polling_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_POLLING}:-DCMSIS_USE_File_System_LevelX_w25n01gvxx_nand_polling_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/w25n01gvxx>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/w25n01gvxx/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/w25n01gvxx/stm32_lx_w25n01gvxx_nand_flash_polling_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25n01gvxx_nand_dma_no_os_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # DMA no os interface for NAND flash w25n01gvxx
  message(DEBUG "Using component File_System_LevelX_w25n01gvxx_nand_dma_no_os_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_NO_OS}:-DCMSIS_USE_File_System_LevelX_w25n01gvxx_nand_dma_no_os_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/w25n01gvxx>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/w25n01gvxx/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/w25n01gvxx/stm32_lx_w25n01gvxx_nand_flash_dma_no_os_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:w25n01gvxx_nand_dma_os_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # DMA os interface for NAND flash w25n01gvxx
  message(DEBUG "Using component File_System_LevelX_w25n01gvxx_nand_dma_os_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_OS}:-DCMSIS_USE_File_System_LevelX_w25n01gvxx_nand_dma_os_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/w25n01gvxx>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/w25n01gvxx/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_W25N01GVXX_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nand/w25n01gvxx/stm32_lx_w25n01gvxx_nand_flash_dma_os_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx66uw1g45g_nor_polling_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # Polling interface for NOR flash mx66uw1g45g
  message(DEBUG "Using component File_System_LevelX_mx66uw1g45g_nor_polling_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_POLLING}:-DCMSIS_USE_File_System_LevelX_mx66uw1g45g_nor_polling_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx66uw1g45g>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx66uw1g45g/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_POLLING}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx66uw1g45g/stm32_lx_mx66uw1g45g_nor_flash_polling_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx66uw1g45g_nor_dma_no_os_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # DMA no os interface for NOR flash mx66uw1g45g
  message(DEBUG "Using component File_System_LevelX_mx66uw1g45g_nor_dma_no_os_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_NO_OS}:-DCMSIS_USE_File_System_LevelX_mx66uw1g45g_nor_dma_no_os_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx66uw1g45g>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx66uw1g45g/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_NO_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx66uw1g45g/stm32_lx_mx66uw1g45g_nor_flash_dma_no_os_driver.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:File System#Cgroup:LevelX#Csub:mx66uw1g45g_nor_dma_os_itf#Cversion:6.4.1" IN_LIST CMSIS_COMPONENTS_LIST)  # DMA os interface for NOR flash mx66uw1g45g
  message(DEBUG "Using component File_System_LevelX_mx66uw1g45g_nor_dma_os_itf_6_4_1")
  target_compile_definitions(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_OS}:-DCMSIS_USE_File_System_LevelX_mx66uw1g45g_nor_dma_os_itf_6_4_1=1>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx66uw1g45g>")
  target_include_directories(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx66uw1g45g/>")
  target_sources(STMicroelectronics_levelx_2_0_0 INTERFACE "$<${STMicroelectronics.levelx.2.0.0_USE_MX66UW1G45G_DMA_OS}:${CMAKE_CURRENT_LIST_DIR}/interfaces/nor/mx66uw1g45g/stm32_lx_mx66uw1g45g_nor_flash_dma_os_driver.c>")
endif()

