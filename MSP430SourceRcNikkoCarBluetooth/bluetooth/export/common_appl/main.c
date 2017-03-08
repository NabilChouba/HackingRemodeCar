
/**
 * Copyright (c) 2009 MindTree Ltd.  All rights reserved.
 * \file    main.c
 * \brief   This file contains 'main()' entry point of the
 *          application and low level initialization function
 */


/* Header File Inclusion */
#include "appl_sdk.h"
#include "arch_defines.h"

/* Functions */

#ifdef __IAR_SYSTEMS_ICC__	/* Toolchain specific low level initialization */

/**
 * \fn      __low_level_init
 * \brief   The function __low_level_init is called by the start-up code
 *          before doing the normal initialization of data segments.
 *          If the return value is zero, initialization is not performed.
 *          In the run-time library there is a dummy __low_level_init, which
 *          does nothing but return 1. This means that the start-up routine
 *          proceeds with initialization of data segments. To replace this
 *          dummy, compile a customized version (like the example below) and
 *          link it with the rest of your code.
 * \param   void
 * \return   0 to omit segment initialization
 *           1 to run segment initialization
 */
int __low_level_init(void)
{
    /* Insert your low-level initializations here */
    WDTCTL = WDTPW + WDTHOLD;   /* Stop Watchdog timer */
    return (1);
}
#elif defined(__TI_COMPILER_VERSION__)

/**
 * \fn       _system_pre_init
 * \brief    The function is called by the CCS start-up code
 * \param    void
 * \return   0 to omit segment initialization
 *           1 to run segment initialization
 */

int _system_pre_init(void)
{
    /* Insert your low-level initializations here */
    WDTCTL = WDTPW + WDTHOLD;   /* Stop Watchdog timer */
    return (1);
}
#else
#warning "Insert toolchain specific low level initialization code"
#endif /* Toolchain specific low level initialization */


/**
 * \fn      main
 * \brief   Entry point to start the FreeRTOS scheduler.
 * \param   void
 * \return  int
 */

int main(void)
{
    sdk_start_scheduler();      /* Start OS scheduler */

    return 0;                   /* Should never reach here */
}

/**
 * \fn      sdkPlatformInit
 * \brief   This is called from the idle task.
 *          This sets up the peripherals and initializes the EtherMind SDK
 * \param   void
 * \return  void
 */
void sdkPlatformInit(void)
{
    /* Common hardware initialisations */
    common_init_bsp();

    /* Initialize application specific peripherals */
    sdk_init_bsp();

    /* Initializes read & write semaphores and configures memory for Bluetooth
     * operations. */
    BT_os_init();

    /* Configure the EtherMind stack */
    BT_ethermind_init();

    /* Init SPP */
    if (API_SUCCESS != BT_spp_init(appl_spp_notify_cb)) {
        sdk_display("Failed to initialize SPP\n");
    }

    /* enable the Interrupts */
    __bis_SR_register(GIE);

    /* Application specific initialisations */
    appl_specific_init();

    /* Initialize the sdk status flags */
    sdk_appl_init();

}

/**
 * \fn      configTimer1_A3
 * \brief   This Function configures Timer1_A3, which is used by application as
 *          reference timer to handle menu and LED related functionalities.
 *          Timer1_A3 is configured for up mode, thus the timer overflows when
 *          TAR counts to CCR0 and interrupt is generated. ACLK(32768Hz) is used
 *          as clock source and CCR0 is set to 0x8000. So, output Time period
 *          is = 1sec.
 * \param   void
 * \return  void
 */
void configTimer1_A3(void)
{
    TA1CCR0 = 0x8000;
    TA1CCTL0 = CCIE;
    TA1CTL = TASSEL_1 + MC_1 + TACLR;
}
