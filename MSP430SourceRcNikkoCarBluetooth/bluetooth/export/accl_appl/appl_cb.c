
/**
 * Copyright (c) 2009 MindTree Ltd.  All rights reserved.
 * \file    appl_bt_on_cb.c
 * \brief This file contains BT_bluetooth_on completion callback handler
 */


/* Header File Inclusion */
#include "appl_sdk.h"
#include "appl_bt_rf.h"
#include "sdk_bluetooth_config.h"
#include "vendor_specific_init.h"
#include "bt_sdk_error.h"

/* Extern Variables */

extern UCHAR sdk_bt_visible;    /* Bluetooth Discoverable On/Off Status */

/* spp connections status information */
extern SDK_SPP_CONNECTION_STATUS sdk_status[SPP_MAX_ENTITY];

/* Flag to check if the connection is initated by the device */
extern UCHAR sdk_initiator;

/* Flag to indicated that ACL connection is initiated by peer */
extern UCHAR peer_initiated_acl_connection;

/* Used to Store Inquiry Results */
extern SDK_REM_BD_DEVICES rem_bt_dev[SDK_INQ_MAX_DEVICES];

extern UCHAR rem_dev_index, rem_dev_num;

/* Flag to indicate sniff mode is requested after the SPP connection */
extern UCHAR sdk_sniff_mode_requested;
extern UINT32 sdk_error_code;

/**
 * \fn      sdk_bluetooth_on_complete
 * \brief   Callback function on completion of Bluetooth being switched on
 * \param   void
 * \return  API_RESULT  API_SUCCESS on success, API_FAILURE otherwise
 */
API_RESULT sdk_bluetooth_on_complete(void)
{
    API_RESULT retval;

    SDK_DEBUG_PRINT_STRING("Bluetooth turned ON\n");

    /* Register SM callback handler */
    sdk_display((const UCHAR *)"Registering UI Notification Callback ... ");
    retval = BT_sm_register_user_interface(sdk_sm_ui_notify_cb);

    /* 
     * All SM Events will be notified to the application
     * using sdk_sm_ui_notify_cb()
     */
    if (retval != API_SUCCESS) {
        sdk_display((const UCHAR *)"Registration with SM FAILED");
    }
#ifdef BT_SSP
    /* Set the Device IO Capability */
    retval = BT_sm_set_local_io_cap(SDK_IO_CAPABILITY);

    if (retval != API_SUCCESS) {
        sdk_display((const UCHAR *)"Set IO Capability failed\0");
    }
#endif /* BT_SSP */

    appl_bluetooth_on_complete_event_handler();

    return API_SUCCESS;
}



/**
 * \fn      hci_connection_complete_event_handler
 * \brief   Event handler for HCI connection completion
 * \param   event_data
 * \return  void
 * \see     common_cb.c
 */
void hci_connection_complete_event_handler(UCHAR * event_data)
{
    UINT16 connection_handle;
    UCHAR status, link_type;
    UCHAR *bd_addr, value_1;
    API_RESULT retval;

    sdk_display((const UCHAR *)"Received HCI_CONNECTION_COMPLETE_EVENT.\n");

    /* Status */
    hci_unpack_1_byte_param(&status, event_data);
    sdk_display("\tStatus: 0x%02X\n", status);
    event_data += 1;

    sdk_display("Status = %02x\n", status);
    /* Connection Handle */
    hci_unpack_2_byte_param(&connection_handle, event_data);
    sdk_display("\tConnection Handle: 0x%04X\n", connection_handle);
    event_data += 2;

    /* BD_ADDR */
    sdk_display((const UCHAR *)"\tBD_ADDR: ");
    appl_hci_print_bd_addr(event_data);
    bd_addr = event_data;
    event_data += 6;

    /* Link Type */
    hci_unpack_1_byte_param(&link_type, event_data);
    sdk_display("\tLink Type: 0x%02X", link_type);
    switch (link_type) {
    case HCI_SCO_LINK:
        sdk_display((const UCHAR *)" -> SCO\n");
        break;
    case HCI_ACL_LINK:
        sdk_display((const UCHAR *)" -> ACL\n");
        break;
#ifdef BT_HCI_1_2
    case HCI_ESCO_LINK:
        sdk_display((const UCHAR *)" -> eSCO\n");
        break;
#endif /* BT_HCI_1_2 */
    default:
        sdk_display((const UCHAR *)" -> ???\n");
        break;
    }
    event_data += 1;

    /* Encryption Mode */
    hci_unpack_1_byte_param(&value_1, event_data);
    sdk_display("\tEcnryption Mode: 0x%02X", value_1);
    switch (value_1) {
    case 0x00:
        sdk_display((const UCHAR *)" -> Encryption OFF\n");
        break;
    case 0x01:
        sdk_display((const UCHAR *)" -> Encryption ON\n");
        break;
    default:
        sdk_display((const UCHAR *)" -> ???\n");
        break;
    }
    event_data += 1;

    if (HCI_ACL_LINK == link_type) {
        if (status != 0x00) {
            SDK_DEBUG_PRINT_STRING("SPP Connection failed\n");
            /* Try Reconnect ACL */
            BT_hci_create_connection(bd_addr, SDK_CONFIG_ACL_PKT_TYPE,
                                     rem_bt_dev[rem_dev_index].
                                     page_scan_rep_mode, 0,
                                     rem_bt_dev[rem_dev_index].clock_offset,
                                     0x01);
        } else {

            /* Make local device invisible */
            retval = BT_hci_write_scan_enable(0x00);

            if (API_SUCCESS != retval) {
                sdk_display((const UCHAR *)"Failed to turn off visibility\n");
            } else {
                sdk_bt_visible = SDK_DISC_OFF;
            }

            appl_acl_connection_complete_event(bd_addr, status,
                                               connection_handle);
        }
    }


}

/**
 * \fn      hci_disconnection_complete_event_handler
 * \brief   Event handler for HCI disconnection completion
 * \param   event_data
 * \return  void
 * \see     common_cb.c
 */
void hci_disconnection_complete_event_handler(UCHAR * event_data)
{

    UINT16 connection_handle;
    UCHAR status;
    UCHAR value_1, dev_index;
    API_RESULT retval;

    sdk_display((const UCHAR *)"Received HCI_DISCONNECTION_COMPLETE_EVENT.\n");

    /* Status */
    hci_unpack_1_byte_param(&status, event_data);
    sdk_display("\tStatus: 0x%02X\n", status);
    event_data += 1;

    /* Connection Handle */
    hci_unpack_2_byte_param(&connection_handle, event_data);
    sdk_display("\tConnection Handle: 0x%04X\n", connection_handle);
    event_data += 2;

    /* Reason */
    hci_unpack_1_byte_param(&value_1, event_data);
    sdk_display("\tReason: 0x%02X\n", value_1);
    event_data += 1;

    if (0x00 == status) {
        appl_get_status_instance_acl(&dev_index, connection_handle);
        SDK_SPP_CHANGE_STATE(dev_index, SDK_DISCONNECTED);

        retval = BT_hci_write_scan_enable(0x03);

        if (API_SUCCESS == retval) {
            sdk_bt_visible = SDK_DISC_ON;

                /**
                 * SPP reconnection not initiated if it is user (Local or
                 * Remote) terminated connection.
                 */
            if ((sdk_initiator) && ((value_1 != 0x16) && (value_1 != 0x13))) {
                sdk_bluetooth_menu_handler(OP_PEER_CONNECT);
            }
        }
    }

    SDK_DEBUG_PRINT_STRING("Disconnected\n");

}



/**
 * \fn      hci_command_complete_event_handler
 * \brief   Event handler for HCI command complete
 * \param   event_data
 * \return  void
 * \see     common_cb.c
 */
void hci_command_complete_event_handler(UCHAR * event_data)
{
    UINT16 value_2;
    UCHAR status;

    sdk_display((const UCHAR *)"Received HCI_COMMAND_COMPLETE_EVENT.\n");

    /* Number of Command Packets */
    event_data += 1;

    /* Command Opcode */
    hci_unpack_2_byte_param(&value_2, event_data);
    sdk_display("\tCommand Opcode: 0x%04X\n", value_2);
    event_data += 2;

    /* Command Status */
    hci_unpack_1_byte_param(&status, event_data);
    sdk_display("\tCommand Status: 0x%02X\n", status);
    event_data += 1;

    if (0x00 != status) {
        sdk_error_code = SDK_WRONG_EVENT_RECEIVED;
        sdk_error_handler();
    }

}

/**
 * \fn      hci_command_status_event_handler
 * \brief   Event handler for HCI command status
 * \param   event_data
 * \return  void
 * \see     common_cb.c
 */
void hci_command_status_event_handler(UCHAR * event_data)
{

    UINT16 value_2;
    UCHAR status;
    /* UCHAR value_1; */
    API_RESULT retval;
    sdk_display((const UCHAR *)"Received HCI_COMMAND_STATUS_EVENT.\n");
    /* Status */
    hci_unpack_1_byte_param(&status, event_data);
    sdk_display("\tCommand Status: 0x%02X\n", status);
    event_data += 1;
    /* Number of Command Packets */
    /* hci_unpack_1_byte_param(&value_1, event_data); */
    /* sdk_display( "\tNum Command Packets: %d (0x%02X)\n", value_1, value_1); */
    event_data += 1;
    /* Command Opcode */
    hci_unpack_2_byte_param(&value_2, event_data);
    sdk_display("\tCommand Opcode: 0x%04X \n", value_2);
    event_data += 2;
#ifdef SDK_ENABLE_SNIFF_MODE
    if (HCI_SNIFF_MODE_OPCODE == value_2) {
        if (0x00 == status) {
            sdk_display((const UCHAR *)"Sniff mode enabled successfully.\n");
        } else {
            sdk_display((const UCHAR *)"Enabling Sniff mode failed.\n");
        }
    } else if (HCI_EXIT_SNIFF_MODE_OPCODE == value_2) {
        if (0x00 == status) {
            sdk_display((const UCHAR *)"Exiting Sniff mode successfully.\n");
        } else {
            sdk_display((const UCHAR *)"Exiting Sniff mode failed.\n");
        }
        return;
    }
#endif /* SDK_ENABLE_SNIFF_MODE */

        /**
         * If remote name request during the inquiry fails for a device,
         * issue remote name request for the next discovered device if any
         */
    if ((API_SUCCESS != status)
        && (HCI_REMOTE_NAME_REQUEST_OPCODE == value_2)) {
        rem_dev_index++;
        if (rem_dev_index < rem_dev_num) {
            /* Device exist for which name request has not been issued */
            retval =
                BT_hci_remote_name_request(rem_bt_dev[rem_dev_index].bd_addr,
                                           rem_bt_dev[rem_dev_index].
                                           page_scan_rep_mode, 0x00,
                                           rem_bt_dev[rem_dev_index].
                                           clock_offset);
            if (retval != API_SUCCESS) {
                sdk_display
                    ("Remote Name Request FAILED !! Error Code = 0x%04x\n",
                     retval);
            }
        } else if (rem_dev_index == rem_dev_num) {
            /* Restart Inquiry as BlueMSP430Demo device is not found */
            retval =
                BT_hci_inquiry(SDK_INQUIRY_LAP, SDK_INQUIRY_LEN,
                               SDK_NUM_RESPONSES);
            if (retval != API_SUCCESS) {
                sdk_display((const UCHAR *)
                            "Failed to initiate Inquiry\n");
            } else {
                rem_dev_index = rem_dev_num = 0;
                sdk_display((const UCHAR *)
                            "Inquiry Started...Wait for Completion\n");
            }
        }
    }
}

/**
 * \fn      hci_mode_change_event_handler
 * \brief   Event handler for HCI mode change
 * \param   event_data
 * \return  void
 * \see     common_cb.c
 */
void hci_mode_change_event_handler(UCHAR * event_data)
{

    UINT16 connection_handle, value_2;
    UCHAR value_1, dev_index, status;
    API_RESULT retval;
    sdk_display((const UCHAR *)
                "Received HCI_MODE_CHANGE_EVENT.\n");
    /* Status */
    hci_unpack_1_byte_param(&status, event_data);
    if (0x0 == status) {
        sdk_display("\tStatus: 0x%02X\n", status);
        event_data += 1;
        /* Connection Handle */
        hci_unpack_2_byte_param(&connection_handle, event_data);
        sdk_display("\tConnection Handle: 0x%04X\n", connection_handle);
        event_data += 2;
        /* Current Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        retval = appl_get_status_instance_acl(&dev_index, connection_handle);
        if (retval != API_SUCCESS) {
            sdk_display("Spurious ACL Connection handle %04X!!!\n",
                        connection_handle);
        } else {
            sdk_display("\tCurrent Mode: 0x%02X", value_1);
            switch (value_1) {
            case 0x00:
                sdk_display((const UCHAR *)" -> Active Mode\n", 0);
#ifdef SDK_ENABLE_SNIFF_MODE
                SDK_SPP_CHANGE_LINK_STATE(dev_index, SDK_ACTIVE);
                if (SDK_IS_SPP_IN_DISCONNECTION(dev_index)) {
                    /* Initiate Disconnection */
                    appl_spp_disconnect(dev_index);
                }
#endif /* SDK_ENABLE_SNIFF_MODE */
                break;
            case 0x01:
                sdk_display((const UCHAR *)" -> Hold Mode\n");
                break;
            case 0x02:
                sdk_display((const UCHAR *)" -> Sniff Mode\n");
#ifdef SDK_ENABLE_SNIFF_MODE
                if (TRUE == sdk_sniff_mode_requested) {
                    sdk_sniff_mode_requested = FALSE;
                    SDK_SPP_CHANGE_LINK_STATE(dev_index, SDK_IN_SNIFF);
                    if (SDK_IS_SPP_TX_STARTED(dev_index)) {
                        /* Start Sending Data */
                        appl_send_spp_data(dev_index);
                    }
                }
#endif /* SDK_ENABLE_SNIFF_MODE */
                break;
            case 0x03:
                sdk_display((const UCHAR *)" -> Park Mode\n");
                break;
            default:
                sdk_display((const UCHAR *)" -> ???\n");
                break;
            }
            event_data += 1;
            /* Interval */
            hci_unpack_2_byte_param(&value_2, event_data);
            sdk_display("\tInterval: 0x%04X\n", value_2);
            event_data += 2;
        }
    }

}

/**
 * \fn      hci_inquiry_complete_event_handler
 * \brief   Event handler for HCI inquiry complete
 * \param   event_data
 * \return  void
 * \see     common_cb.c
 */
void hci_inquiry_complete_event_handler(UCHAR * event_data)
{
    API_RESULT retval;
    UCHAR status;
    /* Status */
    hci_unpack_1_byte_param(&status, event_data);
    if (0x0 == status) {
        /* Inquiry Completed */
        if (0 == rem_dev_num) {
            /* No remote BT device found, so restart inquiry procedure */
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
        } else {
            /* BT devices found, initiate remote name request for the first
             * device */
            retval =
                BT_hci_remote_name_request(rem_bt_dev[0].bd_addr,
                                           rem_bt_dev[0].page_scan_rep_mode,
                                           0x00, rem_bt_dev[0].clock_offset);
            if (retval != API_SUCCESS) {
                sdk_display
                    ("Remote Name Request FAILED !! Error Code = 0x%04x\n",
                     retval);
            } else {
                sdk_display("Remote BT Devices List:\n");
            }
        }

    }

}

/**
 * \fn      hci_inquiry_result_event_handler
 * \brief   Event handler for HCI inquiry result
 * \param   event_data
 * \return  void
 * \see     common_cb.c
 */
void hci_inquiry_result_event_handler(UCHAR * event_data)
{

    UINT16 value_2;
    UCHAR *bd_addr, num_responses, count, value_1, index, flag = 0;
    /* Number of Responses */
    hci_unpack_1_byte_param(&num_responses, event_data);
    event_data += 1;
    /* For each Response, Print the Inquiry Result */
    for (count = 0; count < num_responses; count++) {
        /* BD_ADDR of the Remote Device */
        /* sdk_display("\tBD_ADDR: "); */
        /* appl_hci_print_bd_addr(event_data); */
        /* Note the bd_addr */
        bd_addr = event_data;
        event_data += 6;
        /* Page Scan Repetition Mode */
        hci_unpack_1_byte_param(&value_1, event_data);
        event_data += 6;
        /* Clock Offset */
        hci_unpack_2_byte_param(&value_2, event_data);
        if (SDK_INQ_MAX_DEVICES == rem_dev_num) {
            /* Max remote devices limit reached */
            break;
        }

        for (index = 0; index < rem_dev_num; index++) {
            if (!memcmp(bd_addr, rem_bt_dev[index].bd_addr, BT_BD_ADDR_SIZE)) {
                /* Remote device already noted */
                flag = 1;
                break;
            }
        }
        if (flag) {
            /* Remote device noted, so skip updation */
            break;
        }
        /* Store the remote device details */
        memcpy(rem_bt_dev[rem_dev_num].bd_addr, bd_addr, BT_BD_ADDR_SIZE);
        rem_bt_dev[rem_dev_num].page_scan_rep_mode = value_1;
        rem_bt_dev[rem_dev_num].clock_offset = value_2;
        /* Increase the count of number of devices discovered */
        rem_dev_num++;
    }


}

/**
 * \fn      hci_remote_name_request_complete_event_handler
 * \brief   Event handler for HCI remote request complete
 * \param   event_data
 * \param   event_datalen
 * \return  void
 * \see     common_cb.c
 */
void hci_remote_name_request_complete_event_handler(UCHAR * event_data,
                                                    UCHAR event_datalen)
{

    API_RESULT retval;
    UCHAR *bd_addr, dev_index, rem_dev_name_len, *rem_dev_name;
    UCHAR rem_bt_dev_index;
    event_data += 1;
    /* Note the bd addr */
    bd_addr = event_data;
    event_data += 6;
    if (event_datalen > 7) {
        if ((event_datalen - 7) > SDK_REM_BT_DEV_NAME_MAX_LEN - 1) {
            /* Truncate the rem dev name length */
            rem_dev_name_len = SDK_REM_BT_DEV_NAME_MAX_LEN - 1;
        } else {
            rem_dev_name_len = event_datalen - 7;
        }

        /* Check if inquiry is in progress */
        if (TRUE == peer_initiated_acl_connection) {

                /**
                 * Not in inquiry, remote name request issued for a connected
                 * device, so get the sdk_status instance
                 */
            peer_initiated_acl_connection = FALSE;
            appl_get_status_instance_bd_addr(&dev_index, bd_addr);
            if (0 !=
                strncmp((char *)event_data, (char *)SDK_REM_DEV_NAME_PREFIX,
                        strlen((char *)SDK_REM_DEV_NAME_PREFIX))) {
                /* Disconnect ACL if the connection is requested form a
                 * Non-BlueMSP device */
                retval =
                    BT_hci_disconnect(sdk_status[dev_index].
                                      acl_connection_handle, 0x13);
                if (retval != API_SUCCESS) {
                    sdk_display("** FAILED ** !! Reason Code = 0x%04X\n",
                                retval);
                }
            }
            return;
        }

        /* Store the remote device name */
        bd_addr = rem_bt_dev[rem_dev_index].bd_addr;
        rem_dev_name = event_data;
        rem_dev_name[rem_dev_name_len] = '\0';
        sdk_display("%02X %02X %02X %02X %02X %02X --> ", bd_addr[0],
                    bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        sdk_display("%s\n", rem_dev_name);
        /* Check if "BlueMSP430Demo" is found */
        if (0 ==
            strncmp((char *)rem_dev_name, SDK_REM_DEV_NAME_PREFIX,
                    (strlen((char *)SDK_REM_DEV_NAME_PREFIX)))) {
            /* Allocate free spp instance if available */
            if (API_SUCCESS != appl_get_free_status_instance(&rem_bt_dev_index)) {
                sdk_display("No free SPP connection instance available\n");
                return;
            }
            /* BlueMSP430Demo Found, so initiate connection */
            retval =
                BT_hci_create_connection(bd_addr, SDK_CONFIG_ACL_PKT_TYPE,
                                         rem_bt_dev[rem_dev_index].
                                         page_scan_rep_mode, 0,
                                         rem_bt_dev[rem_dev_index].clock_offset,
                                         0x01);
            /* If already connected */
            if (HCI_STATE_ALREADY_CONNECTED == retval) {
                /* Initiate SDP Query */
                appl_spp_sdp_query(rem_bt_dev_index);
                SDK_SPP_CHANGE_STATE(rem_bt_dev_index, SDK_IN_SDP_QUERY);
            } else if (API_SUCCESS == retval) {
                /* Populate the connection instance with bd_addr */
                BT_mem_copy(sdk_status[rem_bt_dev_index].peer_bd_addr, bd_addr,
                            BT_BD_ADDR_SIZE);
                /* On Success */
                SDK_SPP_CHANGE_STATE(rem_bt_dev_index, SDK_IN_ACL_CONNECTION);
            }
            return;
        }
    }
    /* Start the query for next remote device discovered */
    rem_dev_index++;
    if (rem_dev_index < rem_dev_num) {
        /* Device exist for which name request has not been issued */
        retval =
            BT_hci_remote_name_request(rem_bt_dev[rem_dev_index].bd_addr,
                                       rem_bt_dev[rem_dev_index].
                                       page_scan_rep_mode, 0x00,
                                       rem_bt_dev[rem_dev_index].clock_offset);
        if (retval != API_SUCCESS) {
            sdk_display("Remote Name Request FAILED !! Error Code = 0x%04x\n",
                        retval);
        }
    } else if (rem_dev_index == rem_dev_num) {
        /* Restart Inquiry as BlueMSP430Demo device is not found */
        retval =
            BT_hci_inquiry(SDK_INQUIRY_LAP, SDK_INQUIRY_LEN, SDK_NUM_RESPONSES);
        if (retval != API_SUCCESS) {
            sdk_display((const UCHAR *)"Failed to initiate Inquiry\n");
        } else {
            rem_dev_index = rem_dev_num = 0;
            sdk_display((const UCHAR *)
                        "Inquiry Started...Wait for Completion\n");
        }
    }
}


/**
 * \fn      hci_vendor_specific_debug_event_handler
 * \brief   Event handler for HCI Vendor Specific Debug Event Handler
 * \param   event_data
 * \param   event_datalen
 * \return  void
 * \see     common_cb.c
 */
void hci_vendor_specific_debug_event_handler(UCHAR * event_data,
                                             UCHAR event_datalen)
{
}
