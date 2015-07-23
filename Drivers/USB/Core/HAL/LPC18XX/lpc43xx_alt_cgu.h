#ifndef LPC43xx_ALT_CGU
#define LPC43xx_ALT_CGU

/*
 * This header provides LPC_ALT_CGU. It is the same as LPC_CGU
 * from the chip library, but with different names to match LPCUSBlib
 *
 */

// alternate struct member names to match LPCUSBlib
typedef struct {                            /*!< (@ 0x40050000) CGU Structure          */
    __I  uint32_t  RESERVED0[5];
    __IO uint32_t  FREQ_MON;                  /*!< (@ 0x40050014) Frequency monitor register */
    __IO uint32_t  XTAL_OSC_CTRL;             /*!< (@ 0x40050018) Crystal oscillator control register */
    __I  uint32_t  PLL0USB_STAT;              /*!< (@ 0x4005001C) PLL0 (USB) status register */
    __IO uint32_t  PLL0USB_CTRL;              /*!< (@ 0x40050020) PLL0 (USB) control register */
    __IO uint32_t  PLL0USB_MDIV;              /*!< (@ 0x40050024) PLL0 (USB) M-divider register */
    __IO uint32_t  PLL0USB_NP_DIV;            /*!< (@ 0x40050028) PLL0 (USB) N/P-divider register */
    __I  uint32_t  PLL0AUDIO_STAT;            /*!< (@ 0x4005002C) PLL0 (audio) status register */
    __IO uint32_t  PLL0AUDIO_CTRL;            /*!< (@ 0x40050030) PLL0 (audio) control register */
    __IO uint32_t  PLL0AUDIO_MDIV;            /*!< (@ 0x40050034) PLL0 (audio) M-divider register */
    __IO uint32_t  PLL0AUDIO_NP_DIV;          /*!< (@ 0x40050038) PLL0 (audio) N/P-divider register */
    __IO uint32_t  PLL0AUDIO_FRAC;            /*!< (@ 0x4005003C) PLL0 (audio)           */
    __I  uint32_t  PLL1_STAT;                 /*!< (@ 0x40050040) PLL1 status register   */
    __IO uint32_t  PLL1_CTRL;                 /*!< (@ 0x40050044) PLL1 control register  */
    __IO uint32_t  IDIVA_CTRL;                /*!< (@ 0x40050048) Integer divider A control register */
    __IO uint32_t  IDIVB_CTRL;                /*!< (@ 0x4005004C) Integer divider B control register */
    __IO uint32_t  IDIVC_CTRL;                /*!< (@ 0x40050050) Integer divider C control register */
    __IO uint32_t  IDIVD_CTRL;                /*!< (@ 0x40050054) Integer divider D control register */
    __IO uint32_t  IDIVE_CTRL;                /*!< (@ 0x40050058) Integer divider E control register */
    __IO uint32_t  BASE_SAFE_CLK;             /*!< (@ 0x4005005C) Output stage 0 control register for base clock BASE_SAFE_CLK */
    __IO uint32_t  BASE_USB0_CLK;             /*!< (@ 0x40050060) Output stage 1 control register for base clock BASE_USB0_CLK */
    __IO uint32_t  BASE_PERIPH_CLK;           /*!< (@ 0x40050064) Output stage 2 control register for base clock BASE_PERIPH_CLK */
    __IO uint32_t  BASE_USB1_CLK;             /*!< (@ 0x40050068) Output stage 3 control register for base clock BASE_USB1_CLK */
    __IO uint32_t  BASE_M4_CLK;               /*!< (@ 0x4005006C) Output stage BASE_M4_CLK control register */
    __IO uint32_t  BASE_SPIFI_CLK;            /*!< (@ 0x40050070) Output stage BASE_SPIFI_CLK control register */
    __IO uint32_t  BASE_SPI_CLK;              /*!< (@ 0x40050074) Output stage BASE_SPI_CLK control register */
    __IO uint32_t  BASE_PHY_RX_CLK;           /*!< (@ 0x40050078) Output stage BASE_PHY_RX_CLK control register */
    __IO uint32_t  BASE_PHY_TX_CLK;           /*!< (@ 0x4005007C) Output stage BASE_PHY_TX_CLK control register */
    __IO uint32_t  BASE_APB1_CLK;             /*!< (@ 0x40050080) Output stage BASE_APB1_CLK control register */
    __IO uint32_t  BASE_APB3_CLK;             /*!< (@ 0x40050084) Output stage BASE_APB3_CLK control register */
    __IO uint32_t  BASE_LCD_CLK;              /*!< (@ 0x40050088) Output stage BASE_LCD_CLK control register */
    __I  uint32_t  RESERVED2;
    __IO uint32_t  BASE_SDIO_CLK;             /*!< (@ 0x40050090) Output stage BASE_SDIO_CLK control register */
    __IO uint32_t  BASE_SSP0_CLK;             /*!< (@ 0x40050094) Output stage BASE_SSP0_CLK control register */
    __IO uint32_t  BASE_SSP1_CLK;             /*!< (@ 0x40050098) Output stage BASE_SSP1_CLK control register */
    __IO uint32_t  BASE_UART0_CLK;            /*!< (@ 0x4005009C) Output stage BASE_UART0_CLK control register */
    __IO uint32_t  BASE_UART1_CLK;            /*!< (@ 0x400500A0) Output stage BASE_UART1_CLK control register */
    __IO uint32_t  BASE_UART2_CLK;            /*!< (@ 0x400500A4) Output stage BASE_UART2_CLK control register */
    __IO uint32_t  BASE_UART3_CLK;            /*!< (@ 0x400500A8) Output stage BASE_UART3_CLK control register */
    __IO uint32_t  BASE_OUT_CLK;              /*!< (@ 0x400500AC) Output stage 20 control register for base clock BASE_OUT_CLK */
    __I  uint32_t  RESERVED3[4];
    __IO uint32_t  BASE_APLL_CLK;             /*!< (@ 0x400500C0) Output stage 25 control register for base clock BASE_APLL_CLK */
    __IO uint32_t  BASE_CGU_OUT0_CLK;         /*!< (@ 0x400500C4) Output stage 25 control register for base clock BASE_CGU_OUT0_CLK */
    __IO uint32_t  BASE_CGU_OUT1_CLK;         /*!< (@ 0x400500C8) Output stage 25 control register for base clock BASE_CGU_OUT1_CLK */
} LPC_ALT_CGU_T;

// LPCUSBlib-compatible version of LPC_CGU
#define LPC_ALT_CGU ((LPC_ALT_CGU_T*) LPC_CGU_BASE)

#endif

