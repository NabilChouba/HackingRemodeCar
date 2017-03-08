
/**
 *  @file l2cap.h
 *
 *  This is Module Header File for L2CAP Layer.
 */

/*
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_L2CAP_
#define _H_L2CAP_

/* ---------------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* ---------------------------------------------- Global Definitions */
/** L2CAP Header Size */
#define L2CAP_HDR_LEN                                   4
#define L2CAP_GROUP_HDR_LEN                             6

/** L2CAP Connect Results */
#define L2CAP_CONNECTION_SUCCESSFUL                     0x0000
#define L2CAP_CONNECTION_PENDING                        0x0001
#define L2CAP_CONNECTION_REFUSED_PSM_UNSUPPORTED        0x0002
#define L2CAP_CONNECTION_REFUSED_SECURITY_BLOCK         0x0003
#define L2CAP_CONNECTION_REFUSED_NO_RESOURCE            0x0004

/** L2CAP Connect Status */
#define L2CAP_CONNECTION_NO_FURTHER_INFORMATION         0x0000
#define L2CAP_CONNECTION_AUTHENTICATION_PENDING         0x0001
#define L2CAP_CONNECTION_AUTHORIZATION_PENDING          0x0002

/** L2CAP Configuration Results */
#define L2CAP_CONFIG_SUCCESSFUL                         0x0000
#define L2CAP_CONFIG_UNACCEPTABLE_PARAMETERS            0x0001
#define L2CAP_CONFIG_REJECTED                           0x0002
#define L2CAP_CONFIG_UNKNOWN_OPTION                     0x0003

/** L2CAP Information Request Results */
#define L2CAP_INFO_REQ_SUCCESSFUL                       0x0000
#define L2CAP_INFO_REQ_NOT_SUPPORTED                    0x0001

/** L2CAP Parameter Default Values */
#define L2CAP_MIN_MTU                                   48
#define L2CAP_SIG_MTU                                   L2CAP_MIN_MTU
#define L2CAP_CONNECTIONLESS_MTU                        L2CAP_MIN_MTU
#define L2CAP_MTU_DEFAULT                               672
#define L2CAP_FLUSH_TO_DEFAULT                          0xFFFF
#define L2CAP_FLOW_SPEC_SERVICE_TYPE_DEFAULT            0x01
#define L2CAP_FLOW_SPEC_TOKEN_RATE_DEFAULT              0x00000000
#define L2CAP_FLOW_SPEC_TOKEN_BKT_SIZE_DEFAULT          0x00000000
#define L2CAP_FLOW_SPEC_PEAK_BANDWIDTH_DEFAULT          0x00000000
#define L2CAP_FLOW_SPEC_LATENCY_DEFAULT                 0xFFFFFFFF
#define L2CAP_FLOW_SPEC_DELAY_VARIATION_DEFAULT         0xFFFFFFFF

#ifdef BT_ENH_L2CAP
/* L2CAP FCS Options in non-Basic mode */
#define L2CAP_FCS_OPTION_DEFAULT                        0x01
#define L2CAP_FCS_OPTION_NO_FCS                         0x00
#endif /* BT_ENH_L2CAP */

/* L2CAP Information Request Type Definitions */
#define L2CAP_INFO_REQ_CONNECTIONLESS_MTU               0x0001
#define L2CAP_INFO_REQ_EXTENDED_FEATURES                0x0002


#ifdef BT_ENH_L2CAP
/** L2CAP Operation Modes */
#define L2CAP_MODE_BASIC                                0x00
#define L2CAP_MODE_RTM                                  0x01
#define L2CAP_MODE_FC                                   0x02
#define L2CAP_MODE_ERTM                                 0x03
#define L2CAP_MODE_SM                                   0x04

/* TODO: Temporary: this should not be required long term */
#define L2CAP_MODE_MAX                         L2CAP_MODE_SM

/** L2CAP FEC Mode - Transmission Flow Control */
#define L2CAP_FEC_TX_FLOW_OFF                           0x01
#define L2CAP_FEC_TX_FLOW_ON                            0x00

/** L2CAP FEC Mode - Reception Flow Control */
#define L2CAP_FEC_RX_FLOW_OFF                           0x01
#define L2CAP_FEC_RX_FLOW_ON                            0x00
#endif /* BT_ENH_L2CAP */


/** L2CAP ACL Transmission Queue Flow Control */
#define L2CAP_TX_QUEUE_FLOW_OFF                         0x01
#define L2CAP_TX_QUEUE_FLOW_ON                          0x00

#ifdef BT_PBF
/** L2CAP ACL Flushable or Non-Flushable Identifier */
#define L2CAP_USE_NON_FLUSHABLE_ACL_PACKET              0x00
#define L2CAP_USE_FLUSHABLE_ACL_PACKET                  0x02
#endif /* BT_PBF */


/** L2CAP Extended Feature Mask values */
#define L2CAP_FEATURE_FC                                0x00000001
#define L2CAP_FEATURE_RTM                               0x00000002
#define L2CAP_FEATURE_QOS                               0x00000004
#define L2CAP_FEATURE_ERTM                              0x00000008
#define L2CAP_FEATURE_SM                                0x00000010
#define L2CAP_FEATURE_FCS                               0x00000020

#define L2CAP_FEATURE_NONE                              0x00000000
#define L2CAP_FEATURE_ALL                               0x0000003F


/** Enhanced L2CAP Configuration Mode Preference */
#define L2CAP_CONFIG_PREF_BASIC                         0x00
#define L2CAP_CONFIG_PREF_ERTM_ONLY                     0x01
#define L2CAP_CONFIG_PREF_ERTM_OPTIONAL                 0x02
#define L2CAP_CONFIG_PREF_SM_ONLY                       0x03
#define L2CAP_CONFIG_PREF_SM_OPTIONAL                   0x04


/* ---------------------------------------------- Structures/Data Types */

/**
 *   The Data Type representing the L2CAP Flow Specification
 */
typedef struct
{
    /** Service Type - Guaranteed or Best Effort */
    UCHAR service_type;

    /** Token Rate */
    UINT32 token_rate;

    /** Token Bucket Size */
    UINT32 token_bucket_size;

    /** Peak Bandwidth */
    UINT32 peak_bandwidth;

    /** Latency */
    UINT32 latency;

    /** Delay Variation */
    UINT32 delay_variation;

} FLOW_SPEC, L2CAP_FLOW_SPEC;


#ifdef BT_ENH_L2CAP

/**
 *  The Data Type representing L2CAP FEC Options
 */
typedef struct
{
    /** Value in milliseconds of the retransmission time-out */
    UINT16   retx_timeout;

    /** Value in milliseconds of the retransmission time-out */
    UINT16   monitor_timeout;

    /**
     *  Maximum size of paylod data L2CAP entity is
     *  capable of accepting
     */
    UINT16   mps;

    /**
     *  Maximum number of retransmissions of a single
     *  I-frame in retransmission mode
     */
    UCHAR    max_transmit;

    /** Size of Transmission WIndow for FEC mode */
    UCHAR    tx_window;

    /** Mode of the Link */
    UCHAR    mode;

} L2CAP_FEC_OPTION;

#endif /* BT_ENH_L2CAP */


/**
 *  The Data Type representing L2CAP Configuration Options
 */
typedef struct
{
    /** Flow Specification */
    FLOW_SPEC *           flow_spec;

#ifdef BT_ENH_L2CAP
    /** FEC Options */
    L2CAP_FEC_OPTION *    fec;
#endif /* BT_ENH_L2CAP */

    /** L2CAP MTU - Maximum Transmission Unit */
    UINT16                mtu;

    /** Flush Timeout */
    UINT16                flush_to;

#ifdef BT_ENH_L2CAP
    /** FCS Options */
    UCHAR                 fcs;
#endif /* BT_ENH_L2CAP */

} L2CAP_CONFIG_OPTION;


/**
 *  The structure representing the PSMs. It stores all the callback
 *  functions. The upper layer uses this structure to register itself
 *  with L2CAP.
 */
typedef struct
{
#ifdef BT_ENH_L2CAP
    /* Bitmap to indicate support for L2CAP extended features */
    UINT32 feature_mask;
#endif /* BT_ENH_L2CAP */

    /** Callback for l2ca_connect_ind (BDADDR, CID, PSM) */
    API_RESULT (* l2ca_connect_ind_cb)(UCHAR *, UINT16, UINT16);

    /** Callback for l2ca_connect_cnf (BDADDR, CID, Result, Status) */
    API_RESULT (* l2ca_connect_cnf_cb)(UCHAR *, UINT16, UINT16, UINT16);

    /** Callback for l2ca_config_ind (CID, L2CAP_CONFIG_OPTION) */
    API_RESULT (* l2ca_config_ind_cb)(UINT16, L2CAP_CONFIG_OPTION *);

    /** Callback for l2ca_config_cnf (CID, Result, L2CAP_CONFIG_OPTION) */
    API_RESULT (* l2ca_config_cnf_cb)(UINT16, UINT16, L2CAP_CONFIG_OPTION *);

    /** Callback for l2ca_disconnect_ind (CID) */
    API_RESULT (* l2ca_disconnect_ind_cb)(UINT16);

    /** Callback for l2ca_disconnect_cnf (CID, Result) */
    API_RESULT (* l2ca_disconnect_cnf_cb)(UINT16, UINT16);

    /** Callback function for l2ca_data_read (CID, Buffer, Length) */
    API_RESULT (* l2ca_data_read_cb)(UINT16, UCHAR *, UINT16);

    /** Callback for l2ca_qos_violation_ind (BD_ADDR) */
    API_RESULT (* l2ca_qos_violation_ind_cb)(UCHAR *);

#ifdef BT_ENH_L2CAP
    /** Callback for l2ca_tx_flow_ind (CID, Flow On/Off) */
    API_RESULT (* l2ca_tx_flow_ind_cb)(UINT16, UCHAR);

    /** Callback for l2ca_get_fec_params (CID, FEC) */
    API_RESULT (* l2ca_get_fec_params_cb) (UINT16, L2CAP_FEC_OPTION *);
#endif /* BT_ENH_L2CAP */

    /** The PSM value for the Protocol */
    UINT16 psm;

#ifdef L2CAP_HAVE_GROUP_SUPPORT
    /**
     *  Boolean Flag to indicate whether Connectionless Traffic
     *  is enabled for this PSM.
     */
    UCHAR clt;
#endif /* L2CAP_HAVE_GROUP_SUPPORT */

} PSM_STRUCT, L2CAP_PSM;


/** Common Callback structure for Ping and Info */
typedef struct
{
#ifdef L2CAP_HAVE_PING_INFO_SUPPORT

    /** Callback for Ping, called on reception of L2CAP ECHO RSP */
    API_RESULT (* l2ca_ping_cnf)(UCHAR *, UINT16, UCHAR*, UINT16);
    API_RESULT (* l2ca_getinfo_cnf)(UCHAR *, UINT16, UCHAR*, UINT16);

#else  /* L2CAP_HAVE_PING_INFO_SUPPORT */

   /**
    *  If L2CAP_HAVE_PING_INFO_SUPPORT is not defined compiler will
    *  generate warnings due to the empty srtucure. To avoid this a
    *  dummy variable is declared, which serves no purpose.
    */
   UCHAR dummy;

#endif /* L2CAP_HAVE_PING_INFO_SUPPORT */

} L2CAP_COMMON_CB;


/* ---------------------------------------------- Macros */


/* ---------------------------------------------- API Declarations */
/** L2CAP Initialisation & Shutdown */
void l2cap_init (void);
void l2cap_bt_init (void);
void l2cap_bt_shutdown ( void );

#ifdef __cplusplus
extern "C"{
#endif

/** Initialization of L2CAP_PSM Structure */
void l2cap_init_psm
     (
         UINT16       psm,
         L2CAP_PSM *  l2cap_psm
     );

/** Registration of Upper Layer PSM to L2CAP */
API_RESULT l2cap_register_psm
           (
               IN L2CAP_PSM *proto
           );

/** De-registration of Upper Layer PSM to L2CAP */
API_RESULT l2cap_unregister_psm ( IN UINT16 psm );

/** Registration of Common Callbacks (L2CA_PingCNF & L2CA_InfoCNF) */
API_RESULT l2cap_register_common_cb ( IN L2CAP_COMMON_CB *cb_struct );


#ifdef L2CAP_TX_QUEUE_FLOW
/** Registration of Global ACL Transmission Queue Flow Control Callback */
API_RESULT l2cap_register_tx_queue_flow_cb
           (
               API_RESULT (* callback_fn) (UCHAR, UINT16)
           );

/** To get number of available buffers in ACL Transmission Queue */
API_RESULT l2cap_get_tx_queue_num_buffer
           (
               UINT32 *    num_buffer
           );

/** To set LWM & HWM for ACL Transmission Queue Flow Control */
API_RESULT l2cap_set_tx_queue_flow_limits
           (
               UINT32    tx_queue_lwm,
               UINT32    tx_queue_hwm
           );
#endif /* L2CAP_TX_QUEUE_FLOW */


/** LP Callbacks */
#ifdef BT_ARCH_1_2

API_RESULT l2cap_lp_connect_ind
           (
               IN UCHAR *  bd_addr,
               IN UINT16   handle
           );

API_RESULT l2cap_lp_disconnect_ind
           (
               IN UCHAR *  bd_addr,
               IN UCHAR    reason
           );

API_RESULT l2cap_lp_data_read
           (
               IN UCHAR *  acl_header,
               IN UCHAR *  packet,
               IN UINT16   packet_len
           );

API_RESULT l2cap_lp_mode_change_ind
           (
               IN UINT16  handle,
               IN UCHAR   mode
           );

void l2cap_lp_acl_buffer_size (IN UINT16, IN UINT16);
void l2cap_lp_num_completed_packets (IN UINT16, IN UINT16);

#else  /* BT_ARCH_1_2 */

API_RESULT l2cap_lp_connect_ind
           (
               IN UCHAR *  bd_addr
           );

API_RESULT l2cap_lp_disconnect_ind
           (
               IN UCHAR *  bd_addr,
               IN UCHAR    reason
           );

API_RESULT l2cap_lp_data_read
           (
               IN UCHAR *  bd_addr,
               IN UCHAR *  data_buf,
               IN UINT16   length,
               IN UCHAR    pb,
               IN UCHAR    bc
           );

API_RESULT lp_device_mode_change_ind
           (
               IN UCHAR * bd_addr,
               IN UCHAR   mode
           );

#endif /* BT_ARCH_1_2 */

#ifdef BT_ENH_L2CAP
API_RESULT l2cap_lp_set_flush_to_ind
           (
               IN UINT16  handle,
               IN UINT16  flush_to
           );
#endif /* BT_ENH_L2CAP */


/**
 *  'l2ca_' APIs are those called from the upper layer to send a relevant
 *  command from L2CAP to the peer. These are the interface functions
 *  L2CAP and the upper layer.
 */

#ifdef BT_ENH_L2CAP

/** L2CA Connect Request */
API_RESULT l2ca_connect_req_ex
           (
               IN  UINT16     local_psm,
               IN  UINT16     remote_psm,
               IN  UCHAR *    bd_addr,
               IN  UCHAR      config_pref
           );

#define l2ca_connect_req(p1, p2, p3) \
        l2ca_connect_req_ex(p1, p2, p3, L2CAP_CONFIG_PREF_BASIC)

/** L2CA Connect Response */
API_RESULT l2ca_connect_rsp_ex
           (
               IN  UINT16     local_cid,
               IN  UINT16     response,
               IN  UINT16     status,
               IN  UCHAR      config_pref
           );

#define l2ca_connect_rsp(p1, p2, p3) \
        l2ca_connect_rsp_ex(p1, p2, p3, L2CAP_CONFIG_PREF_BASIC)

#else  /* BT_ENH_L2CAP */

/** L2CA Connect Request */
API_RESULT l2ca_connect_req
           (
               IN  UINT16     local_psm,
               IN  UINT16     remote_psm,
               IN  UCHAR *    bd_addr
           );

/** L2CA Connect Response */
API_RESULT l2ca_connect_rsp
           (
               IN  UINT16     local_cid,
               IN  UINT16     response,
               IN  UINT16     status
           );

#endif /* BT_ENH_L2CAP */

API_RESULT l2cap_sm_access_response
           (
               UCHAR *bd_addr,
               UINT16 psm,
               UINT32 proto_data,
               UINT16 result
           );

/** L2CA Config Request - CID, Config Options */
API_RESULT l2ca_config_req
           (
               IN UINT16,
               IN L2CAP_CONFIG_OPTION *
           );

/** L2CA Config Response - CID, Config Options */
API_RESULT l2ca_config_rsp
           (
               IN UINT16,
               IN L2CAP_CONFIG_OPTION *
           );

/** L2CA Disconnect Request - CID */
API_RESULT l2ca_disconnect_req ( IN UINT16 );


/** L2CA Data Write - CID, Length, Buffer, Length Written */
API_RESULT l2ca_data_write ( IN UINT16,
                             IN UINT16,
                             IN UCHAR *,
                             IN UINT16 * );

/** L2CA Ping/Echo Request - BD_ADDR, Echo Data, Data Length */
API_RESULT l2ca_ping_req ( IN UCHAR *,
                           IN UCHAR *,
                           IN UINT16 );

/** L2CA Get Information Request - BD_ADDR, Info Type */
API_RESULT l2ca_getinfo_req ( IN UCHAR *,
                              IN UINT16 );

#ifdef BT_ENH_L2CAP
/** L2CA Rx Flow REQ */
API_RESULT l2ca_rx_flow_req
           (
               IN  UINT16   lcid,
               IN  UCHAR    flow
           );
#endif /* BT_ENH_L2CAP */

#ifdef BT_PBF
/** To set flushability of L2CAP packets transmitted for a CID */
API_RESULT l2cap_set_flushability
           (
               IN UINT16    lcid,
               IN UCHAR     flushable
           );
#endif /* BT_PBF */

#ifdef L2CAP_HAVE_GROUP_SUPPORT

/** L2CA Group Create - PSM, CID */
API_RESULT l2ca_group_create ( IN  UINT16,
                               OUT UINT16 * );

/** L2CA Group Add Member - CID, BD_ADDR */
API_RESULT l2ca_group_add_member ( IN UINT16,
                                   IN UCHAR * );

/** L2CA Group Remove Member  - CID, BD_ADDR */
API_RESULT l2ca_group_remove_member ( IN UINT16,
                                      IN UCHAR * );

/** L2CA Group Destroy - CID */
API_RESULT l2ca_group_destroy ( IN UINT16 );

/** L2CA Group Get Membership - CID, Buffer_len, Buffer */
API_RESULT l2ca_group_get_membership ( IN    UINT16,
                                       INOUT UINT16 *,
                                       IN    UCHAR * );

/** L2CA Group Enable CLT - PSM */
API_RESULT l2ca_group_enable_clt ( IN UINT16 );

/** L2CA Group Disable CLT - PSM */
API_RESULT l2ca_group_disable_clt ( IN UINT16 );

#endif /* L2CAP_HAVE_GROUP_SUPPORT */


/** L2CA Utility APIs */
API_RESULT l2cap_set_config_timeout (IN UINT16 seconds);
void l2cap_init_config_option (L2CAP_CONFIG_OPTION * ptr);
void l2cap_init_flow_spec_default (FLOW_SPEC *);

#ifdef BT_ENH_L2CAP
/** Utility API to Initialize FEC Option */
void l2cap_init_fec_option_default (L2CAP_FEC_OPTION *fec);
#endif /* BT_ENH_L2CAP */


/* ---------------------------------------------- Platform Definitions */


/* ---------------------------------------------- Feature Definitions */


/* ---------------------------------------------- Test Hooks */
#ifdef L2CAP_TEST_PACKET_DROP

/*
 *  Application hooks for dropping packets during transmission
 *  and/or reception for I/S-frames. L2CAP does not implement
 *  these functions, rather the application willing to test
 *  L2CAP with simulated packet drops must implement these.
 *
 *  Return value: 1 - Drop the packet, do not process
 *                0 - process the packet as usual
 *
 *  Currently, no packet drop is implemented for received I/S-frames,
 *  though l2cap_test_packet_drop_rx() is called - so, the appl can
 *  print what is being received, but returning 1 from this function
 *  will be ignored.
 *
 *  Also, l2cap_test_packet_drop_rx() is called only if FCS check passes
 *  for the received frame.
 */
UCHAR l2cap_test_packet_drop_tx (UCHAR *header, UCHAR header_len);
UCHAR l2cap_test_packet_drop_rx (UCHAR *header, UCHAR header_len);

/* Application hook to report FCS error */
void l2cap_test_packet_fcs_error
     (UINT16 cid, UINT16 fcs_recv, UINT16 fcs_calc);

#endif /* L2CAP_TEST_PACKET_DROP */

void l2cap_test_drop_config (UCHAR flag);
void l2cap_test_check_up (void);

#ifdef L2CAP_TEST_UPF
API_RESULT l2cap_upf_i_frame_rx (UCHAR tx_seq);
#endif /* L2CAP_TEST_UPF */


#ifdef __cplusplus
};
#endif

#endif /* _H_L2CAP_ */


