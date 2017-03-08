/* 
 *  Copyright (C) 2009-2010. MindTree Ltd.
 *  All rights reserved.
 */

/**
 *  @file appl_spp.h
 *
 *  This file contains function/datatype for SPP application
 */


#ifndef _H_APPL_SPP_
#define _H_APPL_SPP_

/* ----------------------------------------------- Header File Inclusion */
#include "BT_spp_api.h"

/* ----------------------------------------------- Macros */

/* ----------------------------------------------- Functions */
#ifdef __cplusplus
extern "C" {
#endif

    void appl_spp_sdp_query(UCHAR rem_bt_dev_index);

    void appl_spp_sdp_callback(UINT8 command, UINT8 * data, UINT16 length,
                               UINT16 status);

    void appl_spp_connect(UCHAR * bd_addr, UCHAR server_ch);
    void appl_spp_disconnect(UCHAR rem_bt_dev_index);

    API_RESULT appl_spp_notify_cb(IN SPP_HANDLE handle,
                                  IN SPP_EVENTS event_type,
                                  IN API_RESULT result, IN void *data,
                                  IN UINT16 datalen);

    void appl_spp_start(void);
    void appl_spp_stop(void);

    API_RESULT appl_spp_write(UCHAR rem_bt_dev_index, UCHAR * data,
                              UINT16 data_len);

    API_RESULT appl_sm_service_cb(UCHAR event_type, UCHAR * bd_addr,
                                  UCHAR * event_data);

    API_RESULT appl_spp_l2cap_tx_queue_flow_cb(UCHAR state, UINT16 count);

#ifdef __cplusplus
};
#endif


#endif /* _H_APPL_SPP_ */
