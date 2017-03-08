
/**
 * Copyright 2008 Texas Instruments, Inc.
 * \file    hal_board.c
 * \brief   Contains functions related to the HAL
 */

#include "hal_MSP430F5438.h"
#include "sdk_pin_config.h"

/**
 * \fn      halBoardSetVCoreUp
 * \brief   Increments the VCore setting.
 * \param   level The target VCore setting
 * \return  void
 */
static void halBoardSetVCoreUp(unsigned char level)
{
    // Open PMM module registers for write access
    PMMCTL0_H = 0xA5;

    // Set SVS/M high side to new level
    SVSMHCTL =
        (SVSMHCTL & ~(SVSHRVL0 * 3 + SVSMHRRL0)) | (SVSHE + SVSHRVL0 * level +
                                                    SVMHE + SVSMHRRL0 * level);

    // Set SVM new Level
    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
    // Set SVS/M low side to new level
    SVSMLCTL = (SVSMLCTL & ~(SVSMLRRL_3)) | (SVMLE + SVSMLRRL0 * level);

    while ((PMMIFG & SVSMLDLYIFG) == 0);    // Wait till SVM is settled (Delay)
    PMMCTL0_L = PMMCOREV0 * level;  // Set VCore to x
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);  // Clear already set flags

    if ((PMMIFG & SVMLIFG))
        while ((PMMIFG & SVMLVLRIFG) == 0); // Wait till level is reached

    // Set SVS/M Low side to new level
    SVSMLCTL =
        (SVSMLCTL & ~(SVSLRVL0 * 3 + SVSMLRRL_3)) | (SVSLE + SVSLRVL0 * level +
                                                     SVMLE + SVSMLRRL0 * level);

    // Lock PMM module registers from write access
    PMMCTL0_H = 0x00;
}

/**
 * \fn      halBoardSetVCoreDown
 * \brief   Decrements the VCore setting.
 * \param   level The target VCore.
 * \return  void
 */
static void halBoardSetVCoreDown(unsigned char level)
{
    // Open PMM module registers for write access
    PMMCTL0_H = 0xA5;

    // Set SVS/M low side to new level
    SVSMLCTL =
        (SVSMLCTL & ~(SVSLRVL0 * 3 + SVSMLRRL_3)) | (SVSLRVL0 * level + SVMLE +
                                                     SVSMLRRL0 * level);

    while ((PMMIFG & SVSMLDLYIFG) == 0);    // Wait till SVM is settled (Delay)
    PMMCTL0_L = (level * PMMCOREV0);    // Set VCore to new level
    // Lock PMM module registers for write access

    PMMCTL0_H = 0x00;
}

/**
 * \fn      halBoardGetSystemClockSettings
 * \brief   Get function for the DCORSEL, VCORE, and DCO multiplier settings
 *          that map to a given clock speed.
 * \param  systemClockSpeed Target DCO frequency - SYSCLK_xxMHZ.
 * \param  setDcoRange      Pointer to the DCO range select bits.
 * \param  setVCore         Pointer to the VCore level bits.
 * \param  setMultiplier    Pointer to the DCO multiplier bits.
 * \return  void
 */
static void halBoardGetSystemClockSettings(unsigned char systemClockSpeed,
                                           unsigned char *setDcoRange,
                                           unsigned char *setVCore,
                                           unsigned int *setMultiplier)
{
    switch (systemClockSpeed) {
    case SYSCLK_1MHZ:
        *setDcoRange = DCORSEL_1MHZ;
        *setVCore = VCORE_1MHZ;
        *setMultiplier = DCO_MULT_1MHZ;
        break;
    case SYSCLK_4MHZ:
        *setDcoRange = DCORSEL_4MHZ;
        *setVCore = VCORE_4MHZ;
        *setMultiplier = DCO_MULT_4MHZ;
        break;
    case SYSCLK_8MHZ:
        *setDcoRange = DCORSEL_8MHZ;
        *setVCore = VCORE_8MHZ;
        *setMultiplier = DCO_MULT_8MHZ;
        break;
    case SYSCLK_12MHZ:
        *setDcoRange = DCORSEL_12MHZ;
        *setVCore = VCORE_12MHZ;
        *setMultiplier = DCO_MULT_12MHZ;
        break;
    case SYSCLK_16MHZ:
        *setDcoRange = DCORSEL_16MHZ;
        *setVCore = VCORE_16MHZ;
        *setMultiplier = DCO_MULT_16MHZ;
        break;
    case SYSCLK_18MHZ:
        *setDcoRange = DCORSEL_18MHZ;
        *setVCore = VCORE_18MHZ;
        *setMultiplier = DCO_MULT_18MHZ;
        break;
    case SYSCLK_20MHZ:
        *setDcoRange = DCORSEL_20MHZ;
        *setVCore = VCORE_20MHZ;
        *setMultiplier = DCO_MULT_20MHZ;
        break;
    case SYSCLK_25MHZ:
        *setDcoRange = DCORSEL_25MHZ;
        *setVCore = VCORE_25MHZ;
        *setMultiplier = DCO_MULT_25MHZ;
        break;
    }
}



/**
 * \fn      halBoardSetVCore
 * \brief   Set function for the PMM core voltage (PMMCOREV) setting
 * \param   level Target VCore setting
 * \return  void
 */
void halBoardSetVCore(unsigned char level)
{
    unsigned char actLevel;

    /* F5438 VCore level is set to level 2 and should never change. F5438A
     * begins at default level zero and should change according to frequency */
    if (Get_Device_Type() == F5438A) {
        do {
            actLevel = PMMCTL0_L & PMMCOREV_3;
            if (actLevel < level)
                halBoardSetVCoreUp(++actLevel); // Set VCore (step by step)
            if (actLevel > level)
                halBoardSetVCoreDown(--actLevel);   // Set VCore (step by step)
        } while (actLevel != level);
    }
}

/**
 * \fn      halBoardDisableSVS
 * \brief   Disables all supply voltage supervision and monitoring.
 * \param   void
 * \return  void
 */
void halBoardDisableSVS(void)
{
    // Open PMM module registers for write access
    PMMCTL0_H = 0xA5;

    SVSMLCTL &= ~(SVMLE + SVSLE + SVSLFP + SVMLFP); // Disable Low side SVM
    SVSMHCTL &= ~(SVMHE + SVSHE + SVSHFP + SVMHFP); // Disable High side SVM
    PMMCTL1 = PMMREFMD;

    // Lock PMM module registers for write access
    PMMCTL0_H = 0x00;
}

/**
 * \fn      halBoardEnableSVS
 * \brief   Enables all supply voltage supervision and monitoring
 * \param   void
 * \return  void
 */
void halBoardEnableSVS(void)
{
    // Open PMM module registers for write access
    PMMCTL0_H = 0xA5;

  /*-----------
   * NOTE: To attain the expected < 6 us wakeup from LPM modes, the following
   * two lines must be commented out due to the fact that the PMM will hold
   * the CPU until the reference is fully settled.
   *----------*/
    SVSMHCTL &= ~(SVSHFP + SVMHFP); // Disable full-performance mode
    SVSMLCTL &= ~(SVSLFP + SVMLFP); // Disable full-performance mode
    SVSMLCTL |= (SVMLE + SVSLE);    // Enable Low side SVM
    SVSMHCTL |= (SVMHE + SVSHE);    // Enable High side SVM
    PMMCTL1 &= ~PMMREFMD;

    // Lock PMM module registers for write access
    PMMCTL0_H = 0x00;
}

/**
 * \fn      halBoardStartXT1
 * \brief   Initialization routine for XT1.
 *          Sets the necessary internal capacitor values and loops until
 *          all ocillator fault flags remain cleared.
 * \param   void
 * \return  void
 */
void halBoardStartXT1(void)
{
    // Set up XT1 Pins to analog function, and to lowest drive
    P7SEL |= 0x03;

    /* Set internal cap values */
#ifdef EZ430_PLATFORM
    UCSCTL6 |= XCAP_1;          /* match EZ430 XTAL 6.5pF each pin, ~ 7pF XTAL
                                 * req. */
#else /* MSP-EXPF5438 platform */
    UCSCTL6 |= XCAP_3;          // Set internal cap values
#endif /* EZ430_PLATFORM */

    while (SFRIFG1 & OFIFG) {   // Check OFIFG fault flag
        while ((SFRIFG1 & OFIFG))   // Check OFIFG fault flag
        {
            // Clear OSC fault flags
            UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT1HFOFFG + XT2OFFG);
            SFRIFG1 &= ~OFIFG;  // Clear OFIFG fault flag
        }
        UCSCTL6 &= ~(XT1DRIVE1_L + XT1DRIVE0);  // Reduce the drive strength
    }
}

/**
 * \fn      halBoardSetSystemClock
 * \brief   Set function for MCLK frequency.
 * \param   systemClockSpeed    Intended frequency of operation - SYSCLK_xxMHZ.
 * \return  void
 */
void halBoardSetSystemClock(unsigned char systemClockSpeed)
{
    unsigned char setDcoRange, setVCore;
    unsigned int setMultiplier;

    halBoardGetSystemClockSettings(systemClockSpeed, &setDcoRange, &setVCore,
                                   &setMultiplier);

    halBoardSetVCore(setVCore);

    __bis_SR_register(SCG0);    // Disable the FLL control loop
    UCSCTL0 = 0x00;             // Set lowest possible DCOx, MODx
    UCSCTL1 = setDcoRange;      // Select suitable range

    UCSCTL2 = setMultiplier + FLLD_1;   // Set DCO Multiplier
    UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV;
    __bic_SR_register(SCG0);    // Enable the FLL control loop

    // Loop until XT1,XT2 & DCO fault flag is cleared
    do {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
        // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;      // Clear fault flags
    } while (SFRIFG1 & OFIFG);  // Test oscillator fault flag

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_FLL_reference. See UCS chapter in 5xx UG
    // for optimization.
    // 32 x 32 x / f_FLL_reference (32,768 Hz) = .03125 = t_DCO_settle
    // t_DCO_settle / (1 / 25 MHz) = 781250 = counts_DCO_settle
    __delay_cycles(781250);
}

/**
 * \fn      halBoardOutputSystemClock
 * \brief   Initializes the CLK outputs in the system
 * \param  void
 * \return void
 */
void halBoardOutputSystemClock(void)
{
    CLK_PORT_DIR |= ALL_CLK_PINS;
    CLK_PORT_SEL |= ALL_CLK_PINS;
}

/**
 * \fn      halBoardStopOutputSystemClock
 * \brief   Stops the output of CLK in the system
 * \param  void
 * \return void
 */
void halBoardStopOutputSystemClock(void)
{
    CLK_PORT_OUT &= ~ALL_CLK_PINS;
    CLK_PORT_DIR |= ALL_CLK_PINS;
    CLK_PORT_SEL &= ~ALL_CLK_PINS;
}

/**
 * \fn      halBoardInit
 * \brief   Initializes all GPIO configurations.
 * \param  void
 * \return void
 */
void halBoardInit(void)
{
    // Tie unused ports
    PAOUT = 0;
    PADIR = 0xFFFF;
    PASEL = 0;
    PBOUT = 0;
    PBDIR = 0xFFFF;
    PBSEL = 0;
    PCOUT = 0;
    PCDIR = 0xFFFF;
    PCSEL = 0;
    PDOUT = 0;
    PDDIR = 0xFFFF;
    PDSEL = 0;
    PEOUT = 0;
#ifdef EZ430_PLATFORM
    PEDIR = 0xFFF5;             /* P9.1 and P9.3 --> RF2560 Rev 1.1 */
#else /* MSP-EXPF5438 platform */
    PEDIR = 0xFEFF;             /* P10.0 to USB RST pin, if enabled with J5 */
#endif /* EZ430_PLATFORM */
    PESEL = 0;
    P11OUT = 0;
    P11DIR = 0xFF;
    PJOUT = 0;
    PJDIR = 0xFF;
    P11SEL = 0;

#ifndef EZ430_PLATFORM
    AUDIO_PORT_OUT = AUDIO_OUT_PWR_PIN;
#endif /* EZ430_PLATFORM */

    USB_PORT_DIR &= ~USB_PIN_RXD;   /* USB RX Pin, Input with PD Resistor */
    USB_PORT_OUT &= ~USB_PIN_RXD;
    USB_PORT_REN |= USB_PIN_RXD;
}
