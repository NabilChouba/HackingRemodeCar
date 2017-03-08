
/**
 * Copyright (C) 2010. MindTree Ltd.  All rights reserved.
 *
 * \file    appl_bt_rf.c
 * \brief   This file contains functions related to ehcill mode.
 */


/* Header File Inclusion */
#include "BT_common.h"
#include "sdk_bluetooth_config.h"
#include "appl_bt_rf.h"
#include "sdk_pl.h"
#include "vendor_specific_init.h"
#include "msp430_uart.h"
#include "bt_sdk_error.h"


/* Extern Function Declarations */
extern API_RESULT BT_hci_vendor_specific_command(UINT16 ocf, UCHAR * params,
                                                 UCHAR params_length);

/* Extern variables */
extern UINT32 sdk_error_code;

#ifdef SDK_EHCILL_MODE
volatile UINT32 msp430_state = SDK_MSP430_AWAKE_STATE;
volatile UINT32 ehcill_rx_state = BT_RF_NO_EHCILL_DATA;
volatile UINT32 ehcill_tx_state = BT_RF_NO_EHCILL_DATA;
volatile UINT32 ehcill_data = 0x0;

#define UPDATE_EHCILL_DATA()  { \
    UINT32 temp; \
    temp = (msp430_state << 16); \
    temp |= (ehcill_rx_state << 8); \
    temp |= (ehcill_tx_state); \
    ehcill_data = temp; \
}


/**
 * \fn      init_ehcill_params
 * \brief   Initialize EHCILL related parameters
 * \param   void
 * \return  void
 */
void init_ehcill_params(void)
{
    msp430_state = SDK_MSP430_AWAKE_STATE;
    ehcill_rx_state = BT_RF_NO_EHCILL_DATA;
    ehcill_tx_state = BT_RF_NO_EHCILL_DATA;
    UPDATE_EHCILL_DATA();
}



/**
 * \fn      uart_enable_cts_interrupt
 * \brief   Function used to set CTS in interrupt mode
 * \param   void
 * \return  void
 */
void uart_enable_cts_interrupt(void)
{

    /* Enabling Interrupt for high to low transistion */
    *(bt_uart_config.uart_cts_ies) |= BT_UART_CTS_PIN;
    /* Clearing Interrupt flag */
    *(bt_uart_config.uart_cts_ifg) &= ~BT_UART_CTS_PIN;
    /* Enabling Interrupt for CTS Pin */
    *(bt_uart_config.uart_cts_ie) |= BT_UART_CTS_PIN;
}

/**
 * \fn      uart_disable_cts_interrupt
 * \brief   Function used to set CTS pin to normal GPIO
 * \param   void
 * \return  void
 */
void uart_disable_cts_interrupt(void)
{
    /* Clearing IFG Flag */
    *(bt_uart_config.uart_cts_ifg) &= ~BT_UART_CTS_PIN;
    /* Clearing Interrupt flag */
    *(bt_uart_config.uart_cts_ie) &= ~BT_UART_CTS_PIN;
}



/**
 * \fn      BT_CTS_PIN_VECTOR_ISR
 * \brief   The ISR is used for the ehcill wake up from CTS functionality
 *          Before going to sleep CTS pin of the host is configured as
 *          interrpt pin. The contoller issues a pulse on the CTS signal
 *          to wake up the host. If CTS is connected to PORT 1 pin, the
 *          pulse is detected as PORT 1 interrupt
 * \param   void
 * \return  void
 */
#ifdef __TI_COMPILER_VERSION__
#pragma CODE_SECTION(BT_CTS_PIN_VECTOR_ISR,".text:_isr");
#endif /* __TI_COMPILER_VERSION__ */
#pragma vector=BT_CTS_PIN_VECTOR
__interrupt void BT_CTS_PIN_VECTOR_ISR(void)
{
    volatile UINT16 int_vect;
    int_vect = P1IV;


    if (BT_UART_CTS_REG_PXIV == (BT_UART_CTS_REG_PXIV & int_vect)) {

        ENABLE_UART();
        UART_ENABLE_BT_UART_RTS();

    }
}




/**
 * \fn      ehcill_tx_handler
 * \brief   Function to handle data ehcill transmission
 * \param   void
 * \return  void
 */
void ehcill_tx_handler(void)
{
    UPDATE_EHCILL_DATA();
    switch (ehcill_data) {

    /** MSP430_state  = Awake = 0x1
      * ehcill_rx_state = BT_RF_SLEEP_IND_RCVD
      * ehcill_tx_state = BT_RF_SLEEP_ACK_SENT
      */
    case 0x13031:
        uart_enable_cts_interrupt();
        ehcill_rx_state = BT_RF_NO_EHCILL_DATA;
        ehcill_tx_state = BT_RF_NO_EHCILL_DATA;
        msp430_state = SDK_MSP430_SLEEP_STATE;
        UART_TRANSMIT(SDK_BT_RF_SLEEP_ACK);
        while ((*(bt_uart_config.uart_reg_ucaxstat)) & UCBUSY);
        DISABLE_UART();
        break;


    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_NO_EHCILL_DATA
      * ehcill_tx_state = BT_RF_NO_EHCILL_DATA
      */
    case 0x0FFFF:
        ehcill_tx_state = BT_RF_WAKE_UP_IND_SENT;
        UART_ENABLE_BT_UART_RTS();
        UART_TRANSMIT(SDK_BT_RF_WAKE_UP_IND);
        break;

    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_NO_EHCILL_DATA
      * ehcill_tx_state = BT_RF_WAKE_UP_IND_SENT
      */
    case 0x0FF32:
        /* Ignore */
        break;

    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_SLEEP_IND_RCVD
      * ehcill_tx_state = BT_RF_WAKE_UP_IND_SENT
      */
    case 0x03032:              /* Collision Case 2 */
        sdk_error_code = ((ehcill_data << 8) | SDK_EHCILL_COLLISION_CASE_2_TX);
        sdk_error_handler();
        break;

    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_WAKE_UP_IND_RCVD
      * ehcill_tx_state = BT_RF_WAKE_UP_IND_SENT
      */
    case 0x03232:
        /* Collision Case 1 */
        ehcill_rx_state = BT_RF_NO_EHCILL_DATA;
        ehcill_tx_state = BT_RF_NO_EHCILL_DATA;
        msp430_state = SDK_MSP430_AWAKE_STATE;
        uart_disable_cts_interrupt();
        break;

    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_WAKE_UP_IND_RCVD
      * ehcill_tx_state = BT_RF_WAKE_UP_ACK_SENT
      */
    case 0x03233:
        ehcill_rx_state = BT_RF_NO_EHCILL_DATA;
        ehcill_tx_state = BT_RF_NO_EHCILL_DATA;
        msp430_state = SDK_MSP430_AWAKE_STATE;
        UART_TRANSMIT(SDK_BT_RF_WAKE_UP_ACK);
        uart_disable_cts_interrupt();
        break;

    default:
        sdk_error_code = ((ehcill_data << 8) | SDK_EHCILL_TX_DEFAULT_CASE);
        sdk_error_handler();
        break;
    }
    UPDATE_EHCILL_DATA();
}

/**
 * \fn      ehcill_rx_handler
 * \brief   Function to handle data ehcill reception
 * \param   void
 * \return  void
 */
void ehcill_rx_handler(void)
{
    UPDATE_EHCILL_DATA();

    switch (ehcill_data) {

    /** MSP430_state  = Awake = 0x1
      * ehcill_rx_state = BT_RF_SLEEP_IND_RCVD
      * ehcill_tx_state = BT_RF_NO_EHCILL_DATA
      */
    case 0x130FF:
        ehcill_tx_state = BT_RF_SLEEP_ACK_SENT;
        while ((*(bt_uart_config.uart_reg_ucaxstat)) & UCBUSY);
        *(bt_uart_config.uart_reg_ucaxifg) |= UCTXIFG;
        break;

    case 0x132FF:
        /* If we rx wake up indication, in awake state; This scenario is
         * normally seen during debugging */
        ehcill_rx_state = BT_RF_NO_EHCILL_DATA;
        break;

    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_SLEEP_IND_RCVD
      * ehcill_tx_state = BT_RF_WAKE_UP_IND_SENT
      */
    case 0x03032:              /* Collision Case 2 */
        sdk_error_code = ((ehcill_data << 8) | SDK_EHCILL_COLLISION_CASE_2_RX);
        sdk_error_handler();
        break;

    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_WAKE_UP_IND_RCVD
      * ehcill_tx_state = BT_RF_NO_EHCILL_DATA
      */
    case 0x032FF:
        ehcill_rx_state = BT_RF_WAKE_UP_IND_RCVD;
        ehcill_tx_state = BT_RF_WAKE_UP_ACK_SENT;
        while ((*(bt_uart_config.uart_reg_ucaxstat)) & UCBUSY);
        *(bt_uart_config.uart_reg_ucaxifg) |= UCTXIFG;
        break;

    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_WAKE_UP_IND_RCVD
      * ehcill_tx_state = BT_RF_WAKE_UP_IND_SENT
      */
    case 0x03232:
        /* Collision Case 1 */
        ehcill_rx_state = BT_RF_NO_EHCILL_DATA;
        ehcill_tx_state = BT_RF_NO_EHCILL_DATA;
        msp430_state = SDK_MSP430_AWAKE_STATE;
        uart_disable_cts_interrupt();
        break;

    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_WAKE_UP_IND_RCVD
      * ehcill_tx_state = BT_RF_WAKE_UP_ACK_SENT
      */
    case 0x03233:
        /* Since we have acknowledged wakeup , we need change the state to
         * awake */
        ehcill_rx_state = BT_RF_NO_EHCILL_DATA;
        ehcill_tx_state = BT_RF_NO_EHCILL_DATA;
        msp430_state = SDK_MSP430_AWAKE_STATE;
        uart_disable_cts_interrupt();
        break;

    /** MSP430_state  = Sleep = 0x0
      * ehcill_rx_state = BT_RF_WAKE_UP_ACK_RCVD
      * ehcill_tx_state = BT_RF_WAKE_UP_IND_SENT
      */
    case 0x03332:
        ehcill_rx_state = BT_RF_NO_EHCILL_DATA;
        ehcill_tx_state = BT_RF_NO_EHCILL_DATA;
        msp430_state = SDK_MSP430_AWAKE_STATE;
        uart_disable_cts_interrupt();
        while ((*(bt_uart_config.uart_reg_ucaxstat)) & UCBUSY);
        *(bt_uart_config.uart_reg_ucaxifg) |= UCTXIFG;

        break;

    default:
        sdk_error_code = ((ehcill_data << 8) | SDK_EHCILL_RX_DEFAULT_CASE);
        sdk_error_handler();
        break;
    }
    UPDATE_EHCILL_DATA();
}


#endif /* SDK_EHCILL_MODE */


/**
 * \fn      sdk_deep_sleep_config
 * \brief   Function to configure deep sleep mode
 * \param   void
 * \return  API_RESULT      API_SUCCESS/API_FAILURE
 */
API_RESULT sdk_deep_sleep_config(void)
{
    API_RESULT retval = API_FAILURE;

    /**
     * inactivity_timeout (x * 1.25 milli secs),
     * retransmit_timeout (x * 1.25 milli secs),
     * rts_pulse_width (micro secs)
     */
    const UCHAR sleep_config_param[] = {
        (UCHAR) SDK_EHCILL_INACTIVITY_TIMEOUT,
        (UCHAR) (SDK_EHCILL_INACTIVITY_TIMEOUT >> 8),
        (UCHAR) SDK_EHCILL_RETRANSMISSION_TIMEOUT,
        (UCHAR) (SDK_EHCILL_RETRANSMISSION_TIMEOUT >> 8),
        (UCHAR) SDK_EHCILL_RTS_PULSE_WIDTH,
        (UCHAR) (SDK_EHCILL_RTS_PULSE_WIDTH >> 8)
    };


#ifdef SDK_EHCILL_MODE

    retval =
        BT_hci_vendor_specific_command(SDK_BT_RF_CONFIG_SLEEP_MODE,
                                       (UCHAR *) sleep_config_param,
                                       sizeof(sleep_config_param)
        );
#endif
    return retval;
}

/**
 * \fn      sdk_deep_sleep_enable
 * \par     Function to enable deep sleep mode
 * \param   void
 * \return  API_RESULT      API_SUCCESS/API_FAILURE
 */

API_RESULT sdk_deep_sleep_enable(void)
{
    API_RESULT retval = API_FAILURE;

    /**
     * Enable Big sleep, Enable Deep Sleep, HCILL Mode, Do not change,
     * Do not change, Do not change, Do not change, Host_Wake deassertion timer
     */
    const UCHAR sleep_enable_param[] =
        { 0x01, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x64, 0x00 };


#ifdef SDK_EHCILL_MODE
    retval =
        BT_hci_vendor_specific_command(SDK_BT_RF_SET_SLEEP_MODE,
                                       (UCHAR *) sleep_enable_param,
                                       sizeof(sleep_enable_param)
        );
#endif /* SDK_EHCILL_MODE */
    return retval;
}

/**
 * \fn      sdk_deep_sleep_disable
 * \brief   Function to disable deep sleep mode
 * \param   void
 * \return  API_RESULT      API_SUCCESS/API_FAILURE
 */
API_RESULT sdk_deep_sleep_disable(void)
{
    API_RESULT retval = API_FAILURE;

    /* Disable deep sleep param */
    const UCHAR sleep_disable_param[] =
        { 0x01, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x64, 0x00 };

#ifdef SDK_EHCILL_MODE

    retval =
        BT_hci_vendor_specific_command(SDK_BT_RF_SET_SLEEP_MODE,
                                       (UCHAR *) sleep_disable_param,
                                       sizeof(sleep_disable_param)
        );
#endif /* SDK_EHCILL_MODE */
    return retval;
}
