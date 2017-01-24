cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)


set(CPM_MODULE_NAME lpc_usb_lib)

#-----------------------------------------------------------------------
# Required CPM Setup - no need to modify - See: https://github.com/iauns/cpm
#-----------------------------------------------------------------------
set(CPM_LIB_TARGET_NAME ${CPM_MODULE_NAME})

if ((DEFINED CPM_DIR) AND (DEFINED CPM_UNIQUE_ID) AND (DEFINED CPM_TARGET_NAME))
    set(CPM_LIB_TARGET_NAME ${CPM_TARGET_NAME})
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CPM_DIR})
    include(CPM)
else()
    set (CPM_DIR "${CMAKE_CURRENT_BINARY_DIR}/cpm-packages" CACHE TYPE STRING)
    find_package(Git)
    if(NOT GIT_FOUND)
        message(FATAL_ERROR "CPM requires Git.")
    endif()
    if (NOT EXISTS ${CPM_DIR}/CPM.cmake)
        message(STATUS "Cloning repo (https://github.com/iauns/cpm)")
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" clone https://github.com/iauns/cpm ${CPM_DIR}
            RESULT_VARIABLE error_code
            OUTPUT_QUIET ERROR_QUIET)
        if(error_code)
            message(FATAL_ERROR "CPM failed to get the hash for HEAD")
        endif()
    endif()
    include(${CPM_DIR}/CPM.cmake)
endif()

#------------------------------------------------------------------------------
# Dependencies
#------------------------------------------------------------------------------
#
# Depends on variables:
#
# MCU_PLATFORM      A supported microcontroller platform.
#                   For example '43xx_m4' or '43xx_m0'.

if(NOT DEFINED MCU_PLATFORM)
    message(FATAL_ERROR "${CPM_MODULE_NAME}: \
    please define variable 'MCU_PLATFROM'")
endif()


if("${MCU_PLATFORM}" STREQUAL "43xx_m4")
    message(STATUS "lpc_usb_lib: Platform '43xx_m4' detected")

    CPM_AddModule("lpc_chip_43xx_m4"
        GIT_REPOSITORY "https://github.com/JitterCompany/lpc_chip_43xx_m4.git"
        GIT_TAG "1.0")

elseif("${MCU_PLATFORM}" STREQUAL "43xx_m0")
    message(STATUS "lpc_usb_lib: Platform '43xx_m0' detected")

    CPM_AddModule("lpc_chip_43xx_m0"
        GIT_REPOSITORY "https://github.com/JitterCompany/lpc_chip_43xx_m0.git"
        GIT_TAG "1.0.1")

elseif("${MCU_PLATFORM}" STREQUAL "11uxx_m0")
    message(STATUS "lpc_usb_lib: Platform '11uxx_m0' detected")
    message(FATAL_ERROR "lpc_usb_lib: Platform '11uxx_m0' not supported yet:\
    \n Chip library not available")

else()
    message(FATAL_ERROR "lpc_usb_lib: platform '${MCU_PLATFORM}' not supported")
endif()



CPM_InitModule(${CPM_MODULE_NAME})

#-----------------------------------------------------------------------
# Project sources / settings
#-----------------------------------------------------------------------

# multiple versions of the usb lib makes no sense
CPM_ForceOnlyOneModuleVersion()

file(GLOB_RECURSE SOURCES 
    "*.c"
    )
include_directories("${CPM_MODULE_NAME}")
include_directories("${CPM_MODULE_NAME}/Common")
CPM_ExportAdditionalIncludeDir("${CPM_MODULE_NAME}/Common")

add_library(${CPM_LIB_TARGET_NAME} ${SOURCES})
target_link_libraries(${CPM_LIB_TARGET_NAME} ${CPM_LIBRARIES})

