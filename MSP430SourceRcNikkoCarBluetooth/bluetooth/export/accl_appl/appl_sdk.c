
/**
 * Copyright (c) 2009 MindTree Ltd.  All rights reserved.
 * \file    appl_sdk.c
 * \brief   This file contains initialization,
 *          BT menu handlers and connection related functions
 */


/* Header File Inclusion */
#include "appl_sdk.h"
#include "l2cap.h"
#include "appl_bt_rf.h"

/* Extern Variables */

/* User selected remote BT device info */
extern UCHAR rem_dev_num, rem_dev_index;
extern UCHAR sdk_connect_in_progress;

/* spp connections status information */
extern SDK_SPP_CONNECTION_STATUS sdk_status[];
/* Bluetooth Power On/Off Status Flag */
extern UCHAR sdk_bt_power;

/* BT Discovery On/Off Status */
extern UCHAR sdk_bt_visible;
/* Static Variables */
static UCHAR appl_spp_data_buffer[4];

/* Global Variables */
UCHAR sdk_initiator = FALSE;

/* Flag to indicated that ACL connection is initiated by peer */
UCHAR peer_initiated_acl_connection = FALSE;

/* Used to Store Inquiry Results */
SDK_REM_BD_DEVICES rem_bt_dev[SDK_INQ_MAX_DEVICES];

/* Flag to indicate sniff mode is requested after the SPP connection */
UCHAR sdk_sniff_mode_requested = FALSE;

void appl_specific_init(void)
{
    /* Creating User Task */
    init_user_task();
}

/* Functions */

/**
 * \fn      sdk_appl_init
 * \brief   Function to initialize the sdk status flags
 * \param   void
 * \return  void
 */
void sdk_appl_init(void)
{
    UCHAR index;

    /* Bluetooth Power On/Off Status Flag */
    sdk_bt_power = SDK_BT_OFF;

    /* Bluetooth Discoverable On/Off Status Flag */
    sdk_bt_visible = SDK_DISC_OFF;

    for (index = 0; index < SPP_MAX_ENTITY; index++) {
        /* Initialize the spp connection state */
        SDK_SPP_CHANGE_STATE(index, SDK_DISCONNECTED);
        /* Initialize the spp data transfer state */
        SDK_SPP_CHANGE_TX_STATE(index, SDK_SPP_TX_OFF);
#ifdef SDK_ENABLE_SNIFF_MODE
        /* Change link state */
        SDK_SPP_CHANGE_LINK_STATE(index, SDK_OFF);
#endif /* SDK_ENABLE_SNIFF_MODE */
        /* Change data to be sent flag state */
        SDK_SPP_CHANGE_DATA_STATE(index, FALSE);
    }
#ifdef SDK_EHCILL_MODE
    init_ehcill_params();
#endif /* SDK_EHCILL_MODE */

}

/**
 * \fn      sdk_bluetooth_menu_handler
 * \brief   Handles the Bluetooth menu options
 * \param   input [IN]  menu option selected
 * \return  void
 * \see     appl_cb.c, user_task.c
 */
void sdk_bluetooth_menu_handler(UCHAR input)
{
    API_RESULT retval;

    /**
     * Only option will be SPP Connect, Which initiates Inquiry looks for
     * remote BT Device with "BlueMSP430Demo" name and connects with it.
     */
    switch (input) {
    case OP_PEER_CONNECT:      /* Connect/Disconnect Button Pressed */
        if (SDK_IS_BT_POWERED_ON()) {
            /* Check if SPP is already connected */
            if (SDK_IS_DISCONNECTED(0)) {
                /* SPP not connected, so initiate connection */
                /* Set the initiator flag */
                sdk_initiator = TRUE;
                /* SPP Connection started, set the flag */
                sdk_connect_in_progress = TRUE;
                /* Initiate Inquiry */
                retval =
                    BT_hci_inquiry(SDK_INQUIRY_LAP, SDK_INQUIRY_LEN,
                                   SDK_NUM_RESPONSES);
                if (retval != API_SUCCESS) {
                    sdk_display((const UCHAR *)"Failed to initiate Inquiry\n");
                } else {
                    rem_dev_index = rem_dev_num = 0;
                    sdk_display((const UCHAR *)
                                "Inquiry Started...Wait for Completion\n");
                }
            } else if (SDK_IS_SPP_CONNECTED(0)) {
                /* SPP connected, so initiate disconnection */
#ifdef SDK_ENABLE_SNIFF_MODE
                /* Check if sniff mode is active */
                if (SDK_IS_IN_SNIFF_MODE(0)) {
                    /* Exit the sniff mode for disconnection */
                    retval =
                        BT_hci_exit_sniff_mode(sdk_status[0].
                                               acl_connection_handle);
                    if (retval != API_SUCCESS) {
                        sdk_display((const UCHAR *)
                                    "Exit sniff mode attempt failed\n");
                    } else {
                        sdk_display((const UCHAR *)"Exiting sniff mode\n");

                        /* Change spp connection state */
                        SDK_SPP_CHANGE_STATE(0, SDK_IN_SPP_DISCONNECTION);
                        break;
                    }
                }
#endif /* SDK_ENABLE_SNIFF_MODE */
                /* Initiate Disconnection */
                appl_spp_disconnect(0);

                /* On Success */
                SDK_SPP_CHANGE_STATE(0, SDK_IN_SPP_DISCONNECTION);
            }
        }
        break;
    case OP_PEER_DATASEND:
    {
        /* Dtasend stop and pause toggle button presses */
        if (SDK_IS_SPP_CONNECTED(0)) {
            if (SDK_IS_SPP_TX_STARTED(0)) {
                /* Stop Sending Data */
                SDK_SPP_CHANGE_TX_STATE(0, SDK_SPP_TX_OFF);

            } else {
                /* Start Sending Data */
                SDK_SPP_CHANGE_TX_STATE(0, SDK_SPP_TX_ON);
                appl_send_spp_data(0);
            }

        } else {
            sdk_display((const UCHAR *)"SPP is not connected\n");
        }
    }
        break;
    }

}

/**
 * \fn      appl_bluetooth_on_complete_event_handler
 * \brief   Function to handle BT on complete event.
 * 		    Application can perform specific profile initialization,
 * 		    setting various bluetooth controller parameters like
 * 		    Class Of Device, default link supervision timeout etc.
 *		    For TI processor the init script also has to be sent.
 * \param   void
 * \return  void
 */
void appl_bluetooth_on_complete_event_handler(void)
{
    API_RESULT retval;

    /* Bluetooth ON Completed */
    sdk_bt_power = SDK_BT_ON;
    appl_bluetooth_on_indication();

    sdk_display((const UCHAR *)"Bluetooth ON Initialization Completed.\n");

    retval = BT_hci_write_scan_enable(0x03);

    if (API_SUCCESS != retval) {
        sdk_display((const UCHAR *)"Failed to turn on visibility\n");
    } else {

        /**
         * LED: Inquiry and page scan mode enabled, Indicated by
         * flashing of LED2 (Red)
         */
        sdk_bt_visible = SDK_DISC_ON;
    }

    /* Start the SPP Profile */
    appl_spp_start();
}

/**
 * \fn      appl_acl_connection_complete_event
 * \brief   Function to handle ACL connection complete event
 * \param   bd_addr     BT address
 * \param   status      Status of the connection
 * \param   connection_handle       Handle for the connection
 * \return  void
 */
void appl_acl_connection_complete_event(UCHAR * bd_addr, UCHAR status,
                                        UINT16 connection_handle)
{
    UCHAR dev_index;
    API_RESULT retval;

    /* Check if the connection is initiated from local device */
    /* If locally initiated, check if the status is success */
    if (API_SUCCESS == appl_get_status_instance_bd_addr(&dev_index, bd_addr)) {
        /* If Success, Initiate SDP Query */
        if (0x00 == status) {
            /* Set the link super vision timeout */
            retval =
                BT_hci_write_link_supervision_timeout(connection_handle,
                                                      SDK_CONFIG_LINK_SUPERVISION_TIMEOUT);

            if (SDK_IS_IN_ACL_CONNECTION(dev_index)) {
                /* Store the ACL Handle */
                sdk_status[dev_index].acl_connection_handle = connection_handle;

#ifdef SDK_ENABLE_SNIFF_MODE
                SDK_SPP_CHANGE_LINK_STATE(dev_index, SDK_ACTIVE)
#endif /* SDK_ENABLE_SNIFF_MODE */
                    /* Start the SDP Query */
                    appl_spp_sdp_query(dev_index);

                SDK_SPP_CHANGE_STATE(dev_index, SDK_IN_SDP_QUERY);
            }

        }
    } else {
        /* Peer initiated acl connection, if success, note the connection info */
        if (0x00 == status) {
            /* Remote Device initiated connection, suffice if resources are
             * available */
            peer_initiated_acl_connection = TRUE;
            appl_get_free_status_instance(&dev_index);
            BT_mem_copy(sdk_status[dev_index].peer_bd_addr, bd_addr,
                        BT_BD_ADDR_SIZE);

            sdk_status[dev_index].acl_connection_handle = connection_handle;
            SDK_SPP_CHANGE_STATE(dev_index, SDK_ACL_CONNECTED);
#ifdef SDK_ENABLE_SNIFF_MODE
            SDK_SPP_CHANGE_LINK_STATE(dev_index, SDK_ACTIVE);
#endif /* SDK_ENABLE_SNIFF_MODE */
            retval =
                BT_hci_remote_name_request(sdk_status[dev_index].peer_bd_addr,
                                           0x00, 0x00, 0x00);

            if (retval != API_SUCCESS) {
                peer_initiated_acl_connection = FALSE;
                sdk_display
                    ("Remote Name Request FAILED !! Error Code = 0x%04x\n",
                     retval);
            }
        }
    }
}

/**
 * \fn      appl_send_spp_data
 * \brief   Function to read data from the sensor and send over SPP
 * \param   dev_index Index used to denote the device
 * \return  void
 */
void appl_send_spp_data(UCHAR dev_index)
{
  //nabil 
    //sdk_spp_data_read(appl_spp_data_buffer, sizeof(appl_spp_data_buffer)
    //    );

    //appl_spp_write(dev_index, appl_spp_data_buffer, sizeof(appl_spp_data_buffer)
    //    );
}
