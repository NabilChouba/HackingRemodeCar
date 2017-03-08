
/**
 * Copyright (C) 2009 MindTree Ltd.  All rights reserved.
 *
 * \file    appl_bt_rf.h
 * \brief   This file contains functions declarations related to ehcill protocol implementation
 */

#ifndef _H_APPL_BT_RF_
#define _H_APPL_BT_RF_

/* Flag to enable ehcill mode */
#define SDK_EHCILL_MODE
#define SDK_EHCILL_INACTIVITY_TIMEOUT       0x0010  /* (x * 1.25 milli secs) */
#define SDK_EHCILL_RETRANSMISSION_TIMEOUT   0x0050  /* (x * 1.25 milli secs) */
#define SDK_EHCILL_RTS_PULSE_WIDTH          0x96    /* (x micro secs) */

/* States pertaining to ehcill mode */
#define SDK_BT_RF_SLEEP_IND                 0x30
#define SDK_BT_RF_SLEEP_ACK                 0x31
#define SDK_BT_RF_WAKE_UP_IND               0x32
#define SDK_BT_RF_WAKE_UP_ACK               0x33

#define SDK_MSP430_SLEEP_STATE              0x0
#define SDK_MSP430_AWAKE_STATE              0x1

#define BT_RF_NO_EHCILL_DATA                0xFF

#define BT_RF_SLEEP_IND_SENT                SDK_BT_RF_SLEEP_IND
#define BT_RF_SLEEP_ACK_SENT                SDK_BT_RF_SLEEP_ACK
#define BT_RF_WAKE_UP_IND_SENT              SDK_BT_RF_WAKE_UP_IND
#define BT_RF_WAKE_UP_ACK_SENT              SDK_BT_RF_WAKE_UP_ACK

#define BT_RF_SLEEP_IND_RCVD                SDK_BT_RF_SLEEP_IND
#define BT_RF_SLEEP_ACK_RCVD                SDK_BT_RF_SLEEP_ACK
#define BT_RF_WAKE_UP_IND_RCVD              SDK_BT_RF_WAKE_UP_IND
#define BT_RF_WAKE_UP_ACK_RCVD              SDK_BT_RF_WAKE_UP_ACK


#ifdef __cplusplus
extern "C" {
#endif

    /* Function to configure deep sleep mode */
    API_RESULT sdk_deep_sleep_config(void);

    /* Function to enable deep sleep mode */
    API_RESULT sdk_deep_sleep_enable(void);

    /* Function to disable deep sleep mode */
    API_RESULT sdk_deep_sleep_disable(void);

    /* Function to handle ehcill data transmission */
    void ehcill_tx_handler(void);

    /* Function to handle ehcill data reception */
    void ehcill_rx_handler(void);

    /**
     * \fn      init_ehcill_params
     * \brief   Initialize EHCILL related parameters
     * \param   void
     * \return  void
     */
    void init_ehcill_params(void);

#ifdef __cplusplus
};
#endif


#endif /* _H_APPL_BT_RF_ */
