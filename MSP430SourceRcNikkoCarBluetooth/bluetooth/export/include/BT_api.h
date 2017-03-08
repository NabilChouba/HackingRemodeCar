
/**
 *  @file BT_api.h
 *
 *  This file contains the API Header Files of the EtherMind Protocols
 */

/* 
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_API_
#define _H_BT_API_

/* ---------------------------------------------- Global Definitions */
#include "BT_common.h"
#include "BT_sm_api.h"

#ifdef SDP
#include "BT_sdp_api.h"
#include "BT_dbase_api.h"
#endif /* SDP */

#ifdef RFCOMM
#include "BT_rfcomm_api.h"
#endif /* RFCOMM */

#ifdef TCS
#include "BT_tcs_api.h"
#endif /* TCS */

#ifdef BNEP
#include "BT_bnep_api.h"
#endif /* BNEP */

#ifdef AVDTP
#include "BT_avdtp_api.h"
#endif /* AVDTP */

#ifdef AVCTP
#include "BT_avctp_api.h"
#endif /* AVCTP */

#ifdef OBEX
#include "BT_irobex_api.h"
#endif /* OBEX */


/* ---------------------------------------------- Global Definitions */


/* ---------------------------------------------- Structures/Data Types */


#ifdef __cplusplus
extern "C" {
#endif

    /* ---------------------------------------------- API Declarations */
    /* To set Serial Port settings for HCI-UART Transport */
    void hci_uart_set_serial_settings(CHAR * device, UINT32 speed);

    /* To set USB Port settings for HCI-USB Transport */
    void hci_usb_set_serial_settings(CHAR * device, UINT32 speed);

#ifdef __cplusplus
};
#endif


#endif /* _H_BT_API_ */
