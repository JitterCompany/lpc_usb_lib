
TOOLCHAIN_PREFIX=arm-none-eabi-
CC=$(TOOLCHAIN_PREFIX)gcc
AR=$(TOOLCHAIN_PREFIX)ar
LD=$(TOOLCHAIN_PREFIX)ld
SZ=$(TOOLCHAIN_PREFIX)size
RM := rm -rf

BUILD_DIR_BASE = ./build/
TARGET_PREFIX_LIB = lib
TARGET_PREFIX_FILE = LPCUSBlib_
TARGET_SUFFIX = .a
TARGET_ARCHS = 11uxx 43xx_m0

11uxx: CORE = m0
11uxx: BUILD_DIR = $(BUILD_DIR_BASE)11uxx/
11uxx: OBJS_F = $(addprefix $(BUILD_DIR), $(OBJS))
11uxx: ILIBS = -I./Common -I./../lpc_chip_11uxx/inc
11uxx: GLOBAL_DEFS = -D__LPC11U1X__ -DUSB_DEVICE_ONLY -DCORE_M0 

43xx_m0: CORE = m0
43xx_m0: BUILD_DIR = $(BUILD_DIR_BASE)43xx_m0/
43xx_m0: OBJS_F = $(addprefix $(BUILD_DIR), $(OBJS))
43xx_m0: ILIBS = -I./Common -I./../lpc_chip_43xx_m0/inc
43xx_m0: GLOBAL_DEFS = -D__LPC43XX__ -DUSB_DEVICE_ONLY -DCORE_M0 

PWD = $(shell pwd)

CFLAGS = -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -std=gnu99 -mthumb -MMD -MP -fdebug-prefix-map=$(PWD)=../libs/lpc_usb_lib/

test:
	@echo $(CFLAGS)

OBJS := \
Drivers/USB/Core/HCD/OHCI/OHCI.o \
Drivers/USB/Core/HCD/EHCI/EHCI.o \
Drivers/USB/Core/HCD/HCD.o \
Drivers/USB/Core/DCD/USBROM/usbd_adcuser.o \
Drivers/USB/Core/DCD/USBROM/usbd_cdc.o \
Drivers/USB/Core/DCD/USBROM/usbd_hid.o \
Drivers/USB/Core/DCD/USBROM/usbd_msc.o \
Drivers/USB/Core/DCD/USBROM/usbd_rom.o \
Drivers/USB/Core/ConfigDescriptor.o \
Drivers/USB/Core/Device.o \
Drivers/USB/Core/DeviceStandardReq.o \
Drivers/USB/Core/Endpoint.o \
Drivers/USB/Core/EndpointStream.o \
Drivers/USB/Core/Events.o \
Drivers/USB/Core/Host.o \
Drivers/USB/Core/HostStandardReq.o \
Drivers/USB/Core/Pipe.o \
Drivers/USB/Core/PipeStream.o \
Drivers/USB/Core/USBController.o \
Drivers/USB/Core/USBMemory.o \
Drivers/USB/Core/USBTask.o \
Drivers/USB/Class/Device/AudioClassDevice.o \
Drivers/USB/Class/Device/CDCClassDevice.o \
Drivers/USB/Class/Device/HIDClassDevice.o \
Drivers/USB/Class/Device/MIDIClassDevice.o \
Drivers/USB/Class/Device/MassStorageClassDevice.o \
Drivers/USB/Class/Device/RNDISClassDevice.o \
Drivers/USB/Class/Common/HIDParser.o \
Drivers/USB/Core/DCD/LPC11UXX/Endpoint_LPC11Uxx.o \
Drivers/USB/Core/HAL/LPC11UXX/HAL_LPC11Uxx.o \
Drivers/USB/Core/DCD/LPC17XX/Endpoint_LPC17xx.o \
Drivers/USB/Core/HAL/LPC17XX/HAL_LPC17xx.o \
Drivers/USB/Core/DCD/LPC18XX/Endpoint_LPC18xx.o \
Drivers/USB/Core/HAL/LPC18XX/HAL_LPC18xx.o \
Drivers/USB/Core/HAL/LPC18XX/lpc43xx_cgu.o \
Drivers/USB/Core/HAL/LPC18XX/lpc43xx_scu.o \


all: post-build


.SECONDEXPANSION:
$(TARGET_ARCHS): $$(OBJS_F)
	@mkdir -p "$(BUILD_DIR)"
	@mkdir -p "./$(TARGET_PREFIX_FILE)$@"
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Archiver'
	 
	$(AR) -r  "./$(TARGET_PREFIX_FILE)$@/$(TARGET_PREFIX_LIB)$(TARGET_PREFIX_FILE)$@$(TARGET_SUFFIX)" $(OBJS_F) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	#$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BUILD_DIR_BASE)
	-$(RM) "./$(TARGET_PREFIX_FILE)11uxx/$(TARGET_PREFIX_LIB)$(TARGET_PREFIX_FILE)11uxx$(TARGET_SUFFIX)"
	-$(RM) "./$(TARGET_PREFIX_FILE)43xx_m0/$(TARGET_PREFIX_LIB)$(TARGET_PREFIX_FILE)43xx_m0$(TARGET_SUFFIX)"
	-@echo ' '


post-build: $(TARGET_ARCHS)
	-@echo 'Performing post-build steps'
	$(SZ) "./$(TARGET_PREFIX_FILE)11uxx/$(TARGET_PREFIX_LIB)$(TARGET_PREFIX_FILE)11uxx$(TARGET_SUFFIX)"
	-@echo ' '
	-@echo ' '
	$(SZ) "./$(TARGET_PREFIX_FILE)43xx_m0/$(TARGET_PREFIX_LIB)$(TARGET_PREFIX_FILE)43xx_m0$(TARGET_SUFFIX)"
	-@echo ' '

./build/11uxx/Drivers/USB/%.o: ./Drivers/USB/%.c
	mkdir -p '$(dir $@)'
	@echo 'Building file: $@ in $(BUILD_DIR) from $<'
	@echo 'Invoking: MCU C Compiler'
	$(CC) $(GLOBAL_DEFS) $(ILIBS) -mcpu=cortex-$(CORE) $(CFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

./build/43xx_m0/Drivers/USB/%.o: ./Drivers/USB/%.c
	mkdir -p '$(dir $@)'
	@echo 'Building file: $@ in $(BUILD_DIR) from $<'
	@echo 'Invoking: MCU C Compiler'
	$(CC) $(GLOBAL_DEFS) $(ILIBS) -mcpu=cortex-$(CORE) $(CFLAGS) -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

.PHONY: all post-build clean dependents
.SECONDARY:

