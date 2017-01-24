/*
 * @brief USB Controller definitions for the LPC microcontrollers
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#define  __INCLUDE_FROM_USB_DRIVER
#include "USBController.h"

volatile uint8_t USB_CurrentMode[MAX_USB_CORE];
volatile bool Mem_IsInitialized = false;


void USB_Init(uint8_t corenum, uint8_t mode)
{
	USB_CurrentMode[corenum] = mode;
	HAL_USBInit(corenum);
	USB_ResetInterface(corenum, mode);
	USB_IsInitialized = true;
}

void USB_Disable(uint8_t corenum, uint8_t mode)
{
	if (mode != USB_MODE_Device) {
        return;
    }

	USB_IsInitialized = false;
    HAL_USBConnect(corenum, 0);
    HAL_USBDeInit(corenum, mode);
}

void USB_ResetInterface(uint8_t corenum, uint8_t mode)
{
	if (mode != USB_MODE_Device) {
        return;
    }
	USB_DeviceState[corenum]        = DEVICE_STATE_Unattached;
	USB_Device_ConfigurationNumber  = 0;

	#if !defined(NO_DEVICE_REMOTE_WAKEUP)
	USB_Device_RemoteWakeupEnabled  = false;
	#endif

	#if !defined(NO_DEVICE_SELF_POWER)
	USB_Device_CurrentlySelfPowered = false;
	#endif

	Endpoint_ConfigureEndpoint(corenum, ENDPOINT_CONTROLEP, EP_TYPE_CONTROL,
							   ENDPOINT_DIR_OUT, USB_Device_ControlEndpointSize,
							   ENDPOINT_BANK_SINGLE);
	HAL_EnableUSBInterrupt(corenum);
	HAL_USBConnect(corenum, 1);
}

