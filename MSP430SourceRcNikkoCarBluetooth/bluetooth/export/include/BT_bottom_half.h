
/**
 *  @file BT_bottom_half.h
 *
 *  This file lists all the Write Task Bottom Half function handlers
 *  under EtherMind Protocol Suite.
 */

/*
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_BOTTOM_HALF_
#define _H_BT_BOTTOM_HALF_

/*
 *  Theory:
 *  There are three kinds of Bottom Halves, as provided by the EtherMind
 *  Write Task module:
 *    1. User Data Bottom Halves (eg, RFCOMM/BNEP Write)
 *    2. User API Bottom Halves
 *    3. Internal (eg, Timer/Internal Loopback handlers)
 *
 *  As required by the Write Task module, all EtherMind Bottom Halves
 *  have the same format:
 *      void bh_handler_function (void *args, UINT16 length);
 */

/* Macro to declare a Bottom Half handler */
#define BT_BOTTOM_HALF(x) \
        void x (void *, UINT16)


/* ============================================= User Data Bottom Halves */
/* --------------------------------------------- Section 'HCI' */
BT_BOTTOM_HALF ( hci_sco_data_bh );

/* --------------------------------------------- Section 'RFCOMM' */
BT_BOTTOM_HALF ( rfcomm_write_bh );
BT_BOTTOM_HALF ( rfcomm_read_bh );

/* --------------------------------------------- Section 'TCS' */
BT_BOTTOM_HALF ( tcs_msg_handler_bh );

/* --------------------------------------------- Section 'BNEP' */
BT_BOTTOM_HALF ( bnep_write_bh );

/* --------------------------------------------- Section 'AVDTP' */
BT_BOTTOM_HALF ( avdtp_media_write_bh );

/* --------------------------------------------- Section 'AVCTP' */
BT_BOTTOM_HALF ( avctp_send_message_bh );

/* --------------------------------------------- Section 'MCAP' */
BT_BOTTOM_HALF ( mcap_md_write_bh );

/* ===================================================================== */


/* ============================================= User API Bottom Halves  */
/* --------------------------------------------- Section 'HCI' */
BT_BOTTOM_HALF ( hci_common_api_bh );

/* --------------------------------------------- Section 'SM' */
BT_BOTTOM_HALF ( sm_ui_notification_bh );

/* --------------------------------------------- Section 'SDP' */
BT_BOTTOM_HALF ( sdp_request_bh );

/* --------------------------------------------- Section 'RFCOMM' */
BT_BOTTOM_HALF ( rfcomm_open_bh );
BT_BOTTOM_HALF ( rfcomm_close_bh );
BT_BOTTOM_HALF ( rfcomm_accept_bh );
BT_BOTTOM_HALF ( rfcomm_send_pn_bh );
BT_BOTTOM_HALF ( rfcomm_send_msc_bh );
BT_BOTTOM_HALF ( rfcomm_send_rpn_bh );
BT_BOTTOM_HALF ( rfcomm_send_rls_bh );
BT_BOTTOM_HALF ( rfcomm_session_fc_bh );
BT_BOTTOM_HALF ( rfcomm_session_test_bh );
BT_BOTTOM_HALF ( rfcomm_credit_tx_bh );
BT_BOTTOM_HALF ( rfcomm_session_open_bh );
BT_BOTTOM_HALF ( rfcomm_session_close_bh );
BT_BOTTOM_HALF ( rfcomm_dlc_open_bh );
BT_BOTTOM_HALF ( rfcomm_dlc_close_bh );

/* --------------------------------------------- Section 'TCS' */
BT_BOTTOM_HALF ( tcs_open_bh );
BT_BOTTOM_HALF ( tcs_close_bh );

/* --------------------------------------------- Section 'BNEP' */
BT_BOTTOM_HALF ( bnep_connect_bh );
BT_BOTTOM_HALF ( bnep_disconnect_bh );
BT_BOTTOM_HALF ( bnep_setup_bh );
BT_BOTTOM_HALF ( bnep_setup_rsp_bh );
BT_BOTTOM_HALF ( bnep_filter_bh );

/* --------------------------------------------- Section 'AVDTP' */
BT_BOTTOM_HALF ( avdtp_connect_req_bh );
BT_BOTTOM_HALF ( avdtp_disconnect_req_bh );
BT_BOTTOM_HALF ( avdtp_common_req_bh_set_1 );
BT_BOTTOM_HALF ( avdtp_common_req_bh_set_2 );
BT_BOTTOM_HALF ( avdtp_common_req_bh_set_3 );

/* --------------------------------------------- Section 'AVCTP' */
BT_BOTTOM_HALF ( avctp_connect_req_bh );
BT_BOTTOM_HALF ( avctp_disconnect_req_bh );

/* --------------------------------------------- Section 'MCAP' */
BT_BOTTOM_HALF ( mcap_md_command_bh );
BT_BOTTOM_HALF ( mcap_mc_bh );
BT_BOTTOM_HALF ( mcap_sync_bh );

/* ===================================================================== */


/* ============================================= Internal Bottom Halves  */
/* --------------------------------------------- Section 'HCI Transport' */
BT_BOTTOM_HALF ( hci_transport_data_read_bh );

/* --------------------------------------------- Section 'BCSP' */
BT_BOTTOM_HALF ( bcsp_link_establishment_bh );
BT_BOTTOM_HALF ( bcsp_sequence_tx_bh );
BT_BOTTOM_HALF ( bcsp_sequence_ack_bh );

/* --------------------------------------------- Section 'HCI' */
BT_BOTTOM_HALF ( hci_data_tx_bh );
BT_BOTTOM_HALF ( hci_command_tx_bh );

/* --------------------------------------------- Section 'L2CAP' */
BT_BOTTOM_HALF ( l2cap_data_transmitter_bh );
BT_BOTTOM_HALF ( l2cap_fec_handle_ack_bh );
BT_BOTTOM_HALF ( l2cap_fec_signal_handler_bh );
BT_BOTTOM_HALF ( l2cap_fec_data_receiver_bh );

/* ===================================================================== */

#endif /* _H_BT_BOTTOM_HALF_ */


