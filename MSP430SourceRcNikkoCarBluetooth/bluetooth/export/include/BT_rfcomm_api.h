
/**
 *  \file BT_rfcomm_api.h
 *
 *  \brief EtherMind RFCOMM API Header File.
 *
 *  This header file contains declaration of RFCOMM APIs, various Macros and
 *  Constant Definitions for use in upper layer profiles/applications.
 */

/*
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_RFCOMM_API_
#define _H_BT_RFCOMM_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* --------------------------------------------- Global Definitions */
/** RFCOMM RPN "Request" Types */
#define RFCOMM_RPN_REQUEST_OPTION               0x01
#define RFCOMM_RPN_COMMAND_OPTION               0x02
#define RFCOMM_RPN_RESPONSE_OPTION              0x03

/** RFCOMM Port Parameters - Baud Rates */
#define RFCOMM_PORT_BAUD_RATE_2400              0x00
#define RFCOMM_PORT_BAUD_RATE_4800              0x01
#define RFCOMM_PORT_BAUD_RATE_7200              0x02
#define RFCOMM_PORT_BAUD_RATE_9600              0x03
#define RFCOMM_PORT_BAUD_RATE_19200             0x04
#define RFCOMM_PORT_BAUD_RATE_38400             0x05
#define RFCOMM_PORT_BAUD_RATE_57600             0x06
#define RFCOMM_PORT_BAUD_RATE_115200            0x07
#define RFCOMM_PORT_BAUD_RATE_230400            0x08

/** RFCOMM Port Parameters - Data Bits */
#define RFCOMM_PORT_DATA_BITS_5                 0x00
#define RFCOMM_PORT_DATA_BITS_6                 0x02
#define RFCOMM_PORT_DATA_BITS_7                 0x01
#define RFCOMM_PORT_DATA_BITS_8                 0x03

/** RFCOMM Port Parameters - Stop Bit */
#define RFCOMM_PORT_STOP_BIT_1                  0x00
#define RFCOMM_PORT_STOP_BIT_1_5                0x01

/** RFCOMM Port Parameters - Parity */
#define RFCOMM_PORT_PARITY_NONE                 0x00
#define RFCOMM_PORT_PARITY_SET                  0x01

/** RFCOMM Port Parameters - Parity Type */
#define RFCOMM_PORT_PARITY_TYPE_ODD             0x00
#define RFCOMM_PORT_PARITY_TYPE_EVEN            0x02
#define RFCOMM_PORT_PARITY_TYPE_MARK            0x01
#define RFCOMM_PORT_PARITY_TYPE_SPACE           0x03

/** RFCOMM Port Parameters - Flow Control */
#define RFCOMM_PORT_FLC_X_ON_X_OFF_INPUT        0x01
#define RFCOMM_PORT_FLC_X_ON_X_OFF_OUTPUT       0x02
#define RFCOMM_PORT_FLC_RTR_INPUT               0x04
#define RFCOMM_PORT_FLC_RTR_OUTPUT              0x08
#define RFCOMM_PORT_FLC_RTC_INPUT               0x10
#define RFCOMM_PORT_FLC_RTC_OUTPUT              0x20

/** RFCOMM Port Parameters - Default Port Settings */
#define RFCOMM_DEFAULT_PORT_BAUD_RATE           RFCOMM_PORT_BAUD_RATE_9600
#define RFCOMM_DEFAULT_PORT_DATA_BITS           RFCOMM_PORT_DATA_BITS_8
#define RFCOMM_DEFAULT_PORT_STOP_BIT            RFCOMM_PORT_STOP_BIT_1
#define RFCOMM_DEFAULT_PORT_PARITY              RFCOMM_PORT_PARITY_NONE
#define RFCOMM_DEFAULT_PORT_PARITY_TYPE         0x00
#define RFCOMM_DEFAULT_PORT_FLOW_CONTROL        0x00
#define RFCOMM_DEFAULT_PORT_XON_CHAR            0x11
#define RFCOMM_DEFAULT_PORT_XOFF_CHAR           0x13

/**
 *  RFCOMM API Contexts or RFCOMM Command Owners.
 *  This has the following purposes:
 *  - Identify the User Context (Blocking Mode)
 *  - Identify the User API (Non-Blocking & Blocking Modes)
 *  - Event Identifiers for RFCOMM Notification Callback (Non-Blocking Mode)
 */
#define RFCOMM_NO_OPERATION                     0x00
#define RFCOMM_OPEN                             0x01
#define RFCOMM_CLOSE                            0x02
#define RFCOMM_ACCEPT                           0x03
#define RFCOMM_READ                             0x04
#define RFCOMM_WRITE                            0x05

#define RFCOMM_SEND_PN                          0x11
#define RFCOMM_SEND_MSC                         0x12
#define RFCOMM_SEND_RPN                         0x13
#define RFCOMM_SEND_RLS                         0x14

#define RFCOMM_SEND_FC_OFF                      0x21
#define RFCOMM_SEND_FC_ON                       0x22
#define RFCOMM_SEND_TEST                        0x23

/* Asynchronous Events for Non-Blocking Mode */
#define RFCOMM_RESET                            0x41
#define RFCOMM_RECVD_MSC                        0x51
#define RFCOMM_RECVD_RLS                        0x52

/** RFCOMM Credit Transmission Enable & Disable - Internal Flags */
#define RFCOMM_CREDIT_TX_ENABLED                0x01
#define RFCOMM_CREDIT_TX_DISABLED               0x00

/** RFCOMM Reading Local & Remote Credit Information - Internal Flags */
#define RFCOMM_CREDIT_LOCAL                     0x01
#define RFCOMM_CREDIT_REMOTE                    0x00



/* --------------------------------------------- Structures/Data Types */

/**
 *  The RFCOMM Handle
 */
typedef struct rfcomm_handle
{
    /* RFCOMM Event Notification Callback (Non-Blocking Mode) */
    API_RESULT (* notify_cb)
               (
                   UCHAR                      /* Event Identifier */,
                   struct rfcomm_handle *     /* RFCOMM Handle */,
                   UINT16                     /* Result/Response */,
                   UCHAR *                    /* Returned Data */,
                   UINT16                     /* Returned Data Length */
               );

    /* The Server Channel for the Data Link Connection */
    UCHAR server_channel;

    /* The Data Link Connection Identifier */
    UCHAR dlci;

    /* The Remote Bluetooth Device Address */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

} RFCOMM_HANDLE;



/**
 *  The RFCOMM Event Notification Callback Data Type (Non-Blocking Mode)
 */
typedef API_RESULT (* RFCOMM_NOTIFY_CB)
                   (
                       UCHAR                /* Event Identifier */,
                       RFCOMM_HANDLE *      /* RFCOMM Handle */,
                       UINT16               /* Result/Response */,
                       UCHAR *              /* Returned Data */,
                       UINT16               /* Returned Data Length */
                   );



/**
 *  The RFCOMM Parameter Negotiation (PN) Data Type
 */
typedef struct
{
    /* Frame Size of the Data Link Connection */
    UINT16 frame_size;

    /* The Data Link Connection Identifier */
    UCHAR dlci;

    /*
     *  The TS 07.10 Convergence Layer.
     *
     *  In RFCOMM Specification v1.1 onwards it is used to claim
     *  configure Credit based Flow Control.
     */
    UCHAR convergence_layer;

    /* The Priority of the Data Link Connection */
    UCHAR priority;

    /*
     *  The TS 07.10 Window Size.
     *
     *  In RFCOMM Specification v1.1 onwards it is used to convey
     *  initial local Credit.
     */
    UCHAR window_size;

} RFCOMM_PN;



/**
 *  The RFCOMM Remote Port Negotiation (RPN) Data Type
 */
typedef struct
{
    /* To indicate whether the RPN is Request, Command or Response */
    UCHAR request;

    /* The Data Link Connection Identifier */
    UCHAR dlci;

    /* RFCOMM Port Setting - Baud Rate */
    UCHAR baud_rate;

    /* RFCOMM Port Setting - Data Bits */
    UCHAR data_bits;

    /* RFCOMM Port Setting - Stop Bit */
    UCHAR stop_bit;

    /* RFCOMM Port Setting - Parity */
    UCHAR parity;

    /* RFCOMM Port Setting - Parity Type */
    UCHAR parity_type;

    /* RFCOMM Port Setting - Flow Control */
    UCHAR flc;

    /* RFCOMM Port Setting - XON Character */
    UCHAR x_on;

    /* RFCOMM Port Setting - XOFF Character */
    UCHAR x_off;

    /*
     *  The Parameter Masks - used to indicate which parameters in
     *  the RPN Frame are negotiated.
     *
     *  For a RPN "Command":
     *      0 -> No Change
     *      1 -> Change
     *
     *  For a RPN "Response":
     *      0 -> Not accepted proposal
     *      1 -> Accepted proposal, and new values are used
     *
     *  Bitmask for the 1st octet of Parameter Mask:
     *      Bit 0 -> Bit/Baud Rate
     *      Bit 1 -> Data Bits
     *      Bit 2 -> Stop Bit
     *      Bit 3 -> Parity
     *      Bit 4 -> Parity Type
     *      Bit 5 -> XON Character
     *      Bit 6 -> XOFF Character
     *      Bit 7 -> (Reserved)
     *
     *  Bitmask for the 2nd octet of Parameter Mask:
     *      Bit 0 -> XON/XOFF in input
     *      Bit 1 -> XON/XOFF in output
     *      Bit 2 -> RTR on input
     *      Bit 3 -> RTR on output
     *      Bit 4 -> RTC on input
     *      Bit 5 -> RTC on output
     *      Bit 6 -> (Reserved)
     *      Bit 7 -> (Reserved)
     */
    UCHAR mask [2];

} RFCOMM_RPN;



/**
 *  The RFCOMM Modem Status Command (MSC) Data Type
 */
typedef struct
{
    /* The Data Link Connection Identifier */
    UCHAR dlci;

    /* Modem Status Parameter - Flow Control */
    UCHAR fc;

    /* Modem Status Parameter - Ready to Communicate */
    UCHAR rtc;

    /* Modem Status Parameter - Ready to Receive */
    UCHAR rtr;

    /* Modem Status Parameter - Incoming Call */
    UCHAR ic;

    /* Modem Status Parameter - Data valid */
    UCHAR dv;

    /* Boolean Flag to indicate if Break Signal is present */
    UCHAR flag;

    /* To indicate that the Octet encodes a valid Break Signal */
    UCHAR break_present;

    /* Length of Break in units of 200 ms */
    UCHAR break_length;

} RFCOMM_MSC;



/**
 *  The RFCOMM Remote Line Status (RLS) Data Type
 */
typedef struct
{
    /* The Data Link Connection Identifier */
    UCHAR dlci;

    /*
     *  The Line Status
     *
     *  7   6   5   4   3   2   1   0
     *  X   X   X   X   X   X   X   0  =>  No Error
     *  X   X   X   X   0   0   1   1  =>  Overrun Error
     *  X   X   X   X   0   1   0   X  =>  Parity Error
     *  X   X   X   X   1   0   0   X  =>  Framing Error
     *
     *  X = Don't Care
     */
    UCHAR line_status;

} RFCOMM_RLS;



/**
 *  The RFCOMM Non Supported Command (NSC) Data Type
 */
typedef struct
{
    /* C/R-Bit of the Non Supported Command */
    UCHAR cr_bit;

    /* Command Type of the Non Supported Command */
    UCHAR command_type;

} RFCOMM_NSC;



/**
 *  The RFCOMM Test (TEST) Data Type
 */
typedef struct
{
    /* The Test Data Pattern */
    UCHAR *    data;

    /* Size of the Test Data Pattern */
    UINT16     datalen;

} RFCOMM_TEST;



/* --------------------------------------------- Macros */

/**
 *  This utility macro initializes fields of a RFCOMM Handle.
 *
 *  \param hdl (IN)
 *         The RFCOMM Handle to be initialized.
 */
#define RFCOMM_RESET_HANDLE(hdl) \
        (hdl)->notify_cb = NULL; \
        (hdl)->dlci = 0x0; \
        (hdl)->server_channel = 0x0; \
        memset((hdl)->bd_addr, 0x0, BT_BD_ADDR_SIZE);


/**
 *  This utility macro populates required field of a RFCOMM handle,
 *  as given by the parameters.
 *
 *  \param hdl (IN)
 *         The RFCOMM Handle to be initialized
 *  \param bd (IN)
 *         The remote Bluetooth Device Address
 *  \param ch (IN)
 *         The RFCOMM Server Channel
 *  \param cb (IN)
 *         The RFCOMM Event Notification (Non-Blocking Mode)
 */
#define RFCOMM_SET_HANDLE(hdl, bd, ch, cb) \
        (hdl)->notify_cb = (cb); \
        (hdl)->server_channel = (ch); \
        BT_COPY_BD_ADDR((hdl)->bd_addr, (bd));


/**
 *  This utility macro populates required field of a RFCOMM handle,
 *  for calling RFCOMM Accept
 *
 *  \param hdl (IN)
 *         The RFCOMM Handle to be initialized
 *  \param ch (IN)
 *         The RFCOMM Server Channel
 */
#define RFCOMM_SET_HANDLE_ACCEPT(hdl, ch, cb) \
        (hdl)->notify_cb = (cb); \
        (hdl)->server_channel = (ch);


/**
 *  This utility macro compares two RFCOMM Handles.
 *
 *  \param hdl_1 (IN)
 *  \param hdl_2 (IN)
 *         The RFCOMM Handles to be compared.
 */
#define BT_rfcomm_compare_handles(hdl_1, hdl_2) \
        (((hdl_1)->dlci == (hdl_2)->dlci) && \
         ((hdl_1)->server_channel == (hdl_2)->server_channel) && \
         (memcmp ((hdl_1)->bd_addr, (hdl_2)->bd_addr, BT_BD_ADDR_SIZE) == 0))


/**
 *  This utility macro sets the Notification Callback in a RFCOMM Handle
 *
 *  \param hdl (IN)
 *         The RFCOMM Handle to be initialized
 *  \param cb (IN)
 *         The RFCOMM Event Notification (Non-Blocking Mode)
 */
#define RFCOMM_SET_HANDLE_NOTIFY_CB(hdl, cb) \
        (hdl)->notify_cb = (cb);


/** RFCOMM Credit Transmission Enable */
#define BT_rfcomm_credit_tx_enable(hdl) \
        BT_rfcomm_credit_tx((hdl), RFCOMM_CREDIT_TX_ENABLED)


/** RFCOMM Credit Transmission Disable */
#define BT_rfcomm_credit_tx_disable(hdl) \
        BT_rfcomm_credit_tx((hdl), RFCOMM_CREDIT_TX_DISABLED)


/** RFCOMM Read Local Credit */
#define BT_rfcomm_read_local_credit(hdl, cr) \
        BT_rfcomm_read_credit((hdl), (cr), RFCOMM_CREDIT_LOCAL)


/** RFCOMM Read Remote Credit */
#define BT_rfcomm_read_remote_credit(hdl, cr) \
        BT_rfcomm_read_credit((hdl), (cr), RFCOMM_CREDIT_REMOTE)


/** To Send Session Flow ON */
#define BT_rfcomm_send_fc_on(hdl) \
        BT_rfcomm_send_fc(hdl, RFCOMM_SEND_FC_ON)


/** To Send Session Flow OFF */
#define BT_rfcomm_send_fc_off(hdl) \
        BT_rfcomm_send_fc(hdl, RFCOMM_SEND_FC_OFF)



#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------- Internal Functions */

/** RFCOMM Credit Transmission Enable & Disable - Internal Functions */
API_RESULT BT_rfcomm_credit_tx
           (
               IN  RFCOMM_HANDLE *    handle,
               IN  UCHAR              flag
           );

/** RFCOMM Read Local & Remote Credit Information - Internal Functions */
API_RESULT BT_rfcomm_read_credit
           (
               IN  RFCOMM_HANDLE *    handle,
               IN  UINT16 *           credit,
               IN  UCHAR              flag
           );

/** To Send Session Flow ON/OFF */
API_RESULT BT_rfcomm_send_fc
           (
               IN  RFCOMM_HANDLE *    handle,
               IN  UCHAR              operation
           );


/* --------------------------------------------- API Declarations */

/** To Accept Connection on a RFCOMM Server Channel */
API_RESULT BT_rfcomm_accept
           (
               INOUT  RFCOMM_HANDLE *    handle,
               IN     UCHAR              server_channel
           );

/** To Cancel accepting Connection on a RFCOMM Server Channel */
API_RESULT BT_rfcomm_accept_cancel
           (
               IN  UCHAR    server_channel
           );

/** To Open a RFCOMM Connection for a Server Channel */
API_RESULT BT_rfcomm_open
           (
               IN     UCHAR *            bd_addr,
               IN     UCHAR              server_channel,
               INOUT  RFCOMM_HANDLE *    handle
           );

/** To Close a RFCOMM Connection for a Server Channel */
API_RESULT BT_rfcomm_close
           (
               IN  RFCOMM_HANDLE *    handle
           );

/** To Query State of a RFCOMM Connection for a Server Channel */
API_RESULT BT_rfcomm_query_state
           (
               IN  RFCOMM_HANDLE *    handle
           );

/** To Write Data on a RFCOMM Connection */
API_RESULT BT_rfcomm_write
           (
               IN  RFCOMM_HANDLE *    handle,
               IN  UCHAR *            data,
               IN  UINT16             length,
               OUT UINT16 *           actual
           );

/** To Test a RFCOMM Session */
API_RESULT BT_rfcomm_send_test
           (
               IN  RFCOMM_HANDLE *    handle,
               IN  RFCOMM_TEST *      test
           );

/** To initiate Parameter Negotiation for a RFCOMM Connection */
API_RESULT BT_rfcomm_send_pn
           (
               IN  RFCOMM_HANDLE *    handle,
               IN  RFCOMM_PN *        pn
           );

/** To initiate Remote Port Negotiation for a RFCOMM Connection */
API_RESULT BT_rfcomm_send_rpn
           (
               IN     RFCOMM_HANDLE *    handle,
               INOUT  RFCOMM_RPN *       rpn
           );

/** To Send Modem Status Command for a RFCOMM Connection */
API_RESULT BT_rfcomm_send_msc
           (
               IN  RFCOMM_HANDLE *    handle,
               IN  RFCOMM_MSC *       msc
           );

/** To Send Remote Line Status for a RFCOMM Connection */
API_RESULT BT_rfcomm_send_rls
           (
               IN  RFCOMM_HANDLE *    handle,
               IN  RFCOMM_RLS *       rls
           );

/** To Get Local Port Settings for a RFCOMM Connection */
API_RESULT BT_rfcomm_get_local_rpn
           (
               IN  RFCOMM_HANDLE *    handle,
               OUT RFCOMM_RPN *       rpn
           );

/** To Set Local Port Settings for a RFCOMM Connection */
API_RESULT BT_rfcomm_set_local_rpn
           (
               IN  RFCOMM_HANDLE *    handle,
               IN  RFCOMM_RPN *       rpn
           );

/** To Get PN Parameters for a RFCOMM Connection */
API_RESULT BT_rfcomm_get_local_pn
           (
               IN  RFCOMM_HANDLE *    handle,
               OUT RFCOMM_PN *        pn
           );

/** To Get Local MSC Parameters for a RFCOMM Connection */
API_RESULT BT_rfcomm_get_local_msc
           (
               IN  RFCOMM_HANDLE *    handle,
               OUT RFCOMM_MSC *       msc
           );

/** To Register L2CAP In-MTU for RFCOMM */
API_RESULT BT_rfcomm_register_mtu
           (
               IN  UINT16    mtu
           );

#ifdef __cplusplus
};
#endif


#endif /* _H_BT_RFCOMM_API_ */


