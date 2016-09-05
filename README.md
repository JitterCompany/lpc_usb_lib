# lpc_usb_lib
CPM package: USB library for LPC microcontrollers based on NXP LPCUSBlib.

## usage
Make sure to set the variable `MCU_PLATFORM` in your top level CMakeLists.txt before adding the lpc_usb_lib module.
This is used to add library-specific defines and to depend on the correct version of the lpc_chip_xxx library.

To use the library add `#include <lpc_usb_lib/USB.h>` to your code.
