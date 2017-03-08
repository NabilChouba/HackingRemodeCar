/* 
 *  Copyright (C) 2009-2010. MindTree Ltd.
 *  All rights reserved.
 */

/**
 *
 *  This header file contains function/datatype declaration for Default
 *  Accelerometer Application
 */


#ifndef _H_APPL_SDK_
#define _H_APPL_SDK_

/* ----------------------------------------------- Header File Inclusion */
#include "sdk_pl.h"
#include "BT_api.h"
#include "appl_spp.h"
#include "sdk_common.h"

#define MAX_USER_BUF            20

/* ----------------------------------------- Macros */
/* Unit key types */
#define COMBINATION_KEY         0x00
#define LOCAL_UNIT_KEY          0x01
#define REMOTE_UNIT_KEY         0x02

/* Device new role type */
#define MASTER_ROLE             0x00
#define SLAVE_ROLE              0x01

/* ----------------------------------------- Structures/Data Types */
/* Structure to store remote bluetooth devices info found during inquiry */
typedef struct {
    /* Remote Device Bluetooth address */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    /* Remote Device Clock offset */
    UINT16 clock_offset;

    /* Remote Device page scan mode */
    UCHAR page_scan_rep_mode;

} SDK_REM_BD_DEVICES;

/* Structure which stores the status of the SPP Connections if any */
typedef struct {
    /* SPP Connection handle */
    UINT16 spp_connection_handle;

    /* ACL Connection handle */
    UINT16 acl_connection_handle;

    /* Bluetooth Device Address of peer */
    UCHAR peer_bd_addr[BT_BD_ADDR_SIZE];

    /* Bluetooth Connection Status Flag */
    UCHAR connect_switch;

    /* Bluetooth Data Send Status Flag */
    UCHAR sdk_data_sending;
#ifdef SDK_ENABLE_SNIFF_MODE
    /* SDK ACL Link status */
    UCHAR link_state;
#endif                          /* SDK_ENABLE_SNIFF_MODE */
    /* Flag to check if data transfer was halted because of l2cap buffer
     * overflow */
    UCHAR appl_spp_data_to_be_sent;

} SDK_SPP_CONNECTION_STATUS;
/* ----------------------------------------------- Functions */
#ifdef __cplusplus
extern "C" {
#endif

    /* Initialize the sdk status flags */
    void sdk_appl_init(void);

    void appl_specific_init(void);

    API_RESULT sdk_bluetooth_on_complete(void);

    API_RESULT sdk_hci_event_indication_callback(UCHAR event_type,
                                                 UCHAR * event_data,
                                                 UCHAR event_datalen);

    API_RESULT sdk_sm_ui_notify_cb(UCHAR event_type, UCHAR * bd_addr,
                                   UCHAR * event_data);

    void appl_dump_bytes(UCHAR * buffer, UINT16 length);
    void appl_bluetooth_on_complete_event_handler(void);

    void sdk_bluetooth_menu_handler(UCHAR input);

    /* Function to read from sensor and send over SPP */
    void appl_send_spp_data(UCHAR rem_bt_dev_index);

    void appl_acl_connection_complete_event(UCHAR * bd_addr, UCHAR status,
                                            UINT16 connection_handle);

    /* Function to handle connection complete event handler */
    void hci_connection_complete_event_handler(UCHAR * event_data);

    /* Function to handle disconnection complete event handler */
    void hci_disconnection_complete_event_handler(UCHAR * event_data);

    /* Function to handle command complete event handler */
    void hci_command_complete_event_handler(UCHAR * event_data);

    /* Function to handle command status event handler */
    void hci_command_status_event_handler(UCHAR * event_data);

    /* Function to handle mode change event handler */
    void hci_mode_change_event_handler(UCHAR * event_data);

    /* Function to handle inquiry complete event handler */
    void hci_inquiry_complete_event_handler(UCHAR * event_data);

    /* Function to handle inquiry result complete event handler */
    void hci_inquiry_result_event_handler(UCHAR * event_data);

    /* Function to handle remote name request complete event handler */
    void hci_remote_name_request_complete_event_handler(UCHAR * event_data,
                                                        UCHAR event_datalen);

    /* Function to handle hci vendor specific debug event */
    void hci_vendor_specific_debug_event_handler(UCHAR * event_data,
                                                 UCHAR event_datalen);

    /* Function to handle role change complete event handler */
    void hci_role_change_event_handler(UCHAR * event_data);

    /* Function to handle link key notification event handler */
    void hci_link_key_notification_event_handler(UCHAR * event_data);

#ifdef __cplusplus
};
#endif


#endif /* _H_APPL_ACCELEROMETER_ */
