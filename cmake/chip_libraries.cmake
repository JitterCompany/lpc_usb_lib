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

elseif("${MCU_PLATFORM}" STREQUAL "11uxx")
    message(STATUS "lpc_usb_lib: Platform '11uxx_m0' detected")

    message(FATAL_ERROR "lpc_usb_lib: platform '11uxx' not supported yet..")

else()
    message(FATAL_ERROR "lpc_usb_lib: platform '${MCU_PLATFORM}' not supported")
endif()

