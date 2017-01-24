/*
 * @brief HAL USB functions for the LPC18xx microcontrollers
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

#if defined(__LPC18XX__) || defined(__LPC43XX__)

#include "lpc43xx_cgu.h"
#include "../HAL.h"
#include "../../USBTask.h"

#if defined(USB_CAN_BE_DEVICE)
#include "../../Device.h"

void HAL_USBConnect(uint8_t corenum, uint32_t con)
{
#if defined(USB_DEVICE_ROM_DRIVER)
	if (con) {
		USBD_API->hw->Connect(UsbHandle, 1);
	}
	else {
		USBD_API->hw->Connect(UsbHandle, 0);
	}
#else
	if (con) {
		USB_REG(corenum)->USBCMD_D |= (1 << 0);
	}
	else {
		USB_REG(corenum)->USBCMD_D &= ~(1 << 0);
	}
#endif
}

#endif

LPC_USBHS_T * const USB_REG_BASE_ADDR[LPC18_43_MAX_USB_CORE] = {LPC_USB0, LPC_USB1};

// enable state for the USB0 core
static bool coreEnabled[1];

void HAL_USBInit(uint8_t corenum)
{
    if(corenum)
    {
        return; // USB1 not supported
    }
    if(!coreEnabled[corenum]) {

        Chip_Clock_DisablePLL(CGU_USB_PLL);

        // setup and enable 480Mhz USB pll
        const CGU_USBAUDIO_PLL_SETUP_T pll_setup = {
            .ctrl = (1 << 2) | (1 << 3) | (1 << 4), // DIRECTI, DIRECTO, CLKEN
            .mdiv = 0x06167FFA,                     // See user manual 13.8.3
            .ndiv = 0x00202062,                     // WTF: Not used? see DIRECTI/O
            .fract = 0,                             // Not used for USB
            .freq = 480000000
        };
        Chip_Clock_SetupPLL(CLKIN_CRYSTAL, CGU_USB_PLL, &pll_setup);
        while (!(Chip_Clock_GetPLLStatus(CGU_USB_PLL) & CGU_PLL_LOCKED));


        // enable USB0 base clock
        Chip_Clock_EnableBaseClock(CLK_BASE_USB0);
        Chip_Clock_EnableOpts(CLK_MX_USB0, true, true, 1);

        // Turn on the USB phy
        Chip_CREG_EnableUSB0Phy();
        coreEnabled[corenum] = true;
    }

    // reset the controller
    USB_REG(corenum)->USBCMD_D = USBCMD_D_Reset;

    // wait for reset to complete
    while (USB_REG(corenum)->USBCMD_D & USBCMD_D_Reset);

    /* Program the controller to be the USB device controller */
    USB_REG(corenum)->USBMODE_D = USBMODE_D_CM1_0_DEVICE;

    // NOTE: do NOT use OTGSC_VBUS_DISCHARGE if using a voltage divider on vbus
    LPC_USB0->OTGSC = OTGSC_OTG_TERMINATION;
#if (USB_FORCED_FULLSPEED)
    LPC_USB0->PORTSC1_D |= PORTSC_D_PortForceFullspeedConnect;
#endif
    HAL_Reset(corenum);
}

void HAL_USBDeInit(uint8_t corenum, uint8_t mode)
{
	HAL_DisableUSBInterrupt(corenum);
	if (mode == USB_MODE_Device) {
		#if defined(USB_CAN_BE_HOST)
		USB_REG(corenum)->USBSTS_H = 0xFFFFFFFF;				/* clear all current interrupts */
		USB_REG(corenum)->PORTSC1_H &= ~(1 << 12);					/* clear port power */
		USB_REG(corenum)->USBMODE_H =   (1 << 0);				/* set USB mode reserve */
		#endif
	}
	else if (mode == USB_MODE_Host) {
		#if defined(USB_CAN_BE_DEVICE)
		/* Clear all pending interrupts */
		USB_REG(corenum)->USBSTS_D   = 0xFFFFFFFF;
		USB_REG(corenum)->ENDPTNAK   = 0xFFFFFFFF;
		USB_REG(corenum)->ENDPTNAKEN = 0;
		USB_REG(corenum)->ENDPTSETUPSTAT = USB_REG(corenum)->ENDPTSETUPSTAT;
		USB_REG(corenum)->ENDPTCOMPLETE  = USB_REG(corenum)->ENDPTCOMPLETE;
		while (USB_REG(corenum)->ENDPTPRIME) ;						/* Wait until all bits are 0 */
		USB_REG(corenum)->ENDPTFLUSH = 0xFFFFFFFF;
		while (USB_REG(corenum)->ENDPTFLUSH) ;		/* Wait until all bits are 0 */
		#endif
	}

	/* Disable USB PHY if both USB cores are disabled */
	if (coreEnabled[1 - corenum]) {
		/* Turn off the phy (prior to PLL disabled) */
		Chip_CREG_DisableUSB0Phy();
	}

	/* Power down USB clocking */
	if (corenum == 0) {
		Chip_Clock_Disable(CLK_MX_USB0);
		Chip_Clock_DisableBaseClock(CLK_BASE_USB0);
	}
	else {
		Chip_Clock_Disable(CLK_MX_USB1);
		Chip_Clock_DisableBaseClock(CLK_BASE_USB1);
	}

	/* Disable USB PLL if both USB cores are disabled */
	if (!coreEnabled[1 - corenum]) {
		/* Disable USB PLL */
		Chip_Clock_DisablePLL(CGU_USB_PLL);
	}

	coreEnabled[corenum] = false;
}

void HAL_EnableUSBInterrupt(uint8_t corenum)
{
	NVIC_EnableIRQ((corenum) ? USB1_IRQn : USB0_IRQn);	//  enable USB interrupts
}

void HAL_DisableUSBInterrupt(uint8_t corenum)
{
	NVIC_DisableIRQ((corenum) ? USB1_IRQn : USB0_IRQn);	//  disable USB interrupts
}

void USB0_IRQHandler(void)
{
	if (USB_CurrentMode[0] == USB_MODE_Host) {
		#ifdef USB_CAN_BE_HOST
		HcdIrqHandler(0);
		#endif
	}
	if (USB_CurrentMode[0] == USB_MODE_Device) {
		#ifdef USB_CAN_BE_DEVICE
			#ifdef USB_DEVICE_ROM_DRIVER
		UsbdRom_IrqHandler();
			#else
		DcdIrqHandler(0);
			#endif
		#endif
	}
}

void USB1_IRQHandler(void)
{
	if (USB_CurrentMode[1] == USB_MODE_Host) {
		#ifdef USB_CAN_BE_HOST
		HcdIrqHandler(1);
		#endif
	}
	if (USB_CurrentMode[1] == USB_MODE_Device) {
		#ifdef USB_CAN_BE_DEVICE
			#ifdef USB_DEVICE_ROM_DRIVER
		UsbdRom_IrqHandler();
			#else
		DcdIrqHandler(1);
			#endif
		#endif
	}
}

#endif /*__LPC18XX__*/
