
/**
 * Copyright (C) 2009 MindTree Ltd.  All rights reserved.
 *
 * \file    appl_cb.c
 * \brief   This file contains the function related to call backs
 */

/* Header File Inclusion */
#include "appl_sdk.h"
#include "appl_bt_rf.h"
#include "sdk_pl.h"
#include "BT_api.h"
#include "vendor_specific_init.h"


/**
 * \fn      sdk_sm_ui_notify_cb
 * \brief   Callback function for SM event notification
 * \param   event_type Type of event
 * \param   bd_addr Address of peer device
 * \param   event_data Data buffer for events
 * \return  API_RESULT      API_SUCCESS/API_FAILURE
 * \see     appl_cb.c
 */

API_RESULT sdk_sm_ui_notify_cb(UCHAR event_type, UCHAR * bd_addr,
                               UCHAR * event_data)
{
#ifdef BT_SSP
    UINT32 numeric_val;
    UCHAR user_reply;
#endif /* BT_SSP */

    API_RESULT retval;
    UCHAR reason, flag;
    UCHAR link_key[BT_LINK_KEY_SIZE];

    UCHAR msg_string[100];

    retval = API_SUCCESS;

    switch (event_type) {
    case SM_ACL_CONNECT_REQUEST_NTF:
        sprintf((char *)msg_string,
                "Received UI Connection Request from SM\n"
                "BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n", bd_addr[0],
                bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

        sdk_display((const UCHAR *)msg_string);

        /* Accept the incoming connection */
        reason = 0x0;

        /* Automatically accepting - in this sample example */

        /* 
         * Application developer might choose to inform the user
         * about an incomming connection.
         *
         * Application should not call the user notification callback
         * from this SM callback context, that will block the entire
         * read task, which will prevent handling of data recevied
         * from the transport and might have unpredictable behaviour.
         */
        retval = BT_sm_connection_request_reply(bd_addr, 0x01, reason);

        if (retval != API_SUCCESS) {
            sdk_display("Replying to UI Connection Request ...  FAILED");

            break;
        } else {
            sdk_display((const UCHAR *)"OK\n");
        }

        break;

    case SM_PIN_CODE_REQUEST_NTF:

        sprintf((char *)msg_string,
                "Received UI PIN Code Request from SM\n"
                "BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n", bd_addr[0],
                bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

        sdk_display((const UCHAR *)msg_string);

        sdk_display((const UCHAR *)
                    "Replying to UI PIN Code Request (+Ve) ... ");

        /* 
         * Application developer has to provide the user interface to
         * get the PIN code from user.
         *
         * For the demonstration we are using the fixed PIN code.
         *
         * Application should not call the user notification callback
         * from this SM callback context, that will block the entire
         * read task, which will prevent handling of data recevied
         * from the transport and might have unpredictable behaviour.
         *
         */
        retval =
            BT_sm_pin_code_request_reply(bd_addr, (UCHAR *) SDK_CONFIG_PIN,
                                         strlen(SDK_CONFIG_PIN)
            );

        if (retval != API_SUCCESS) {
            sprintf((char *)msg_string,
                    (const char *)"FAILED ! Reason = 0x%04X\n", retval);

            sdk_display((const UCHAR *)msg_string);
            break;
        } else {
            sdk_display((const UCHAR *)"OK\n");
        }

        break;

    case SM_LINK_KEY_REQUEST_NTF:
        sprintf((char *)msg_string,
                "Received UI Link Key Request from SM\n"
                "BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n", bd_addr[0],
                bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

        sdk_display((const UCHAR *)msg_string);

        /* Get Link Key from Device Database */
        retval = BT_sm_get_device_link_key(bd_addr, link_key);
        flag = (API_SUCCESS == retval) ? 0x1 : 0x0;

        sprintf((char *)msg_string, "Replying to UI Link Key Request (%s) ... ",
                (API_SUCCESS == retval) ? "+Ve" : "-Ve");

        sdk_display((const UCHAR *)msg_string);
        retval = BT_sm_link_key_request_reply(bd_addr, link_key, flag);

        if (API_SUCCESS != retval) {
            sprintf((char *)msg_string, "FAILED ! Reason = 0x%04X\n", retval);

            sdk_display((const UCHAR *)msg_string);
            break;
        } else {
            sdk_display((const UCHAR *)"OK\n");
        }

        break;

#ifdef BT_SSP
    case SM_USER_CONF_REQUEST_NTF:
        sprintf((char *)msg_string,
                "Received UI User Conf Request from SM\n"
                "BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n", bd_addr[0],
                bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

        sdk_display((const UCHAR *)msg_string);


        if ((SDK_IO_CAPABILITY == SM_IO_CAPABILITY_NO_INPUT_NO_OUTPUT)
            || (SDK_IO_CAPABILITY == SM_IO_CAPABILITY_DISPLAY_ONLY)) {
            /* Device has no Input Capability, so directly accept the
             * connection */
            retval = BT_sm_user_conf_request_reply(bd_addr, 0x01);
            if (retval != API_SUCCESS) {
                sprintf((char *)msg_string,
                        (const char *)"FAILED ! Reason = 0x%04X\n", retval);

                sdk_display((const UCHAR *)msg_string);
            } else {
                sdk_display((const UCHAR *)"OK\n");
            }
        } else {

            /* Get Numeric Value */
            numeric_val = event_data[3];
            numeric_val <<= 8;
            numeric_val |= event_data[2];
            numeric_val <<= 8;
            numeric_val |= event_data[1];
            numeric_val <<= 8;
            numeric_val |= event_data[0];

            sprintf((char *)msg_string, "Numeric Value = 0x%08X\n",
                    (unsigned int)numeric_val);

            sdk_display((const UCHAR *)msg_string);

            /* 
             * The User confirmation is done automatically here.
             * But application developer can display this numeric value to the
             * user and read his response (accept/reject)
             */
            sdk_display((const UCHAR *)"Replying to UI User Conf Request ... ");
            user_reply = 0x01;
            retval = BT_sm_user_conf_request_reply(bd_addr, user_reply);
            if (retval != API_SUCCESS) {
                sprintf((char *)msg_string,
                        (const char *)"FAILED ! Reason = 0x%04X\n", retval);

                sdk_display((const UCHAR *)msg_string);
            } else {
                sdk_display((const UCHAR *)"OK\n");
            }
        }
        break;

    case SM_USER_PASSKEY_REQUEST_NTF:
        sprintf((char *)msg_string,
                "Received UI User Passkey Request from SM\n"
                "BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n", bd_addr[0],
                bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

        sdk_display((const UCHAR *)msg_string);
        /* 
         * Application developer has to provide HMI interface for entering the
         * pass key which is notified on the peer device using
         * SM_USER_PASSKEY_NTF event on its side. The pass key has to be replied
         * using BT_sm_user_passkey_request_reply API.
         * [NOTE: Local device has to send the Passkey entry started event,
         *        Passkey digit entered event, Passkey digit erased event,
         *        Passkey cleared event or Passkey entry completed event
         *        when it recieves respective events from HMI using the API
         *        BT_hci_send_keypress_notification.
         */

        sdk_display((const UCHAR *)
                    "Please Reply to User Passkey Request using HMI.\n");
        break;

    case SM_USER_PASSKEY_NTF:
        sprintf((char *)msg_string,
                "Received UI User Passkey Notification from SM\n"
                "BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n", bd_addr[0],
                bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

        sdk_display((const UCHAR *)msg_string);

        /* Get Numeric Value */
        numeric_val = event_data[3];
        numeric_val <<= 8;
        numeric_val |= event_data[2];
        numeric_val <<= 8;
        numeric_val |= event_data[1];
        numeric_val <<= 8;
        numeric_val |= event_data[0];

        /* 
         * Application developer has to display this passkey, this passkey value
         * has to be supplied by the remote device upon notifying with
         * SM_USER_PASSKEY_REQUEST_NTF event on its side.
         */
        sprintf((char *)msg_string, "Numeric Value = %u (0x%08X)\n",
                (unsigned int)numeric_val, (unsigned int)numeric_val);

        sdk_display((const UCHAR *)msg_string);

        break;
#endif /* #ifdef BT_SSP */

    default:
        sprintf((char *)msg_string, "*** Unknown/Undefined Event Type 0x%02X\n",
                event_type);

        sdk_display((const UCHAR *)msg_string);
        break;
    }

    return retval;
}


/**
 * \fn      appl_dump_bytes
 * \brief   Function to dump a buffer byte stream onto the SDK display
 * \param   buffer      Pointer to the buffer to be displayed
 * \param   length      Length of the buffer
 * \return  void
 */
void appl_dump_bytes(UCHAR * buffer, UINT16 length)
{
    char hex_stream[49];
    char char_stream[17];
    UINT32 i;
    UINT16 offset, count;
    UCHAR c;
    sdk_display((const UCHAR *)"\n");
    sdk_display((const UCHAR *)
                "-------------------------------------------------------------------\n");
    count = 0;
    offset = 0;
    for (i = 0; i < length; i++) {
        c = buffer[i];
        sdk_display(hex_stream + offset, "%02X ", c);
        if ((c >= 0x20) && (c <= 0x7E)) {
            char_stream[count] = c;
        } else {
            char_stream[count] = '.';
        }

        count++;
        offset += 3;
        if (16 == count) {
            char_stream[count] = '\0';
            count = 0;
            offset = 0;
            sdk_display("%s %s\n", hex_stream, char_stream);
            memset(hex_stream, 0, 49);
            memset(char_stream, 0, 17);
        }
    }

    if (offset != 0) {
        char_stream[count] = '\0';
        /* Maintain the alignment */
        sdk_display("%-48s %s\n", hex_stream, char_stream);
    }

    sdk_display((const UCHAR *)
                "-------------------------------------------------------------------\n");
    sdk_display((const UCHAR *)"\n");
}



/**
 * \fn      sdk_hci_event_indication_callback
 * \brief   Callback function to decode and handle HCI event indications
 * \param   event_type Type of event indication
 * \param   event_data Pointer to buffer containing event byte stream
 * \param   event_datalen Length of the event buffer
 * \return  API_RESULT      API_SUCCESS/API_FAILURE
 * \see     sdk_pl.c
 */
API_RESULT sdk_hci_event_indication_callback(UCHAR event_type,
                                             UCHAR * event_data,
                                             UCHAR event_datalen)
{

    /* Switch on the Event Code */
    switch (event_type) {
    case HCI_CONNECTION_COMPLETE_EVENT:
        hci_connection_complete_event_handler(event_data);
        break;

    case HCI_DISCONNECTION_COMPLETE_EVENT:
        hci_disconnection_complete_event_handler(event_data);
        break;

    case HCI_COMMAND_COMPLETE_EVENT:
        hci_command_complete_event_handler(event_data);
        break;

    case HCI_COMMAND_STATUS_EVENT:
        hci_command_status_event_handler(event_data);
        break;

    case HCI_ROLE_CHANGE_EVENT:
        hci_role_change_event_handler(event_data);
        break;

    case HCI_MODE_CHANGE_EVENT:
        hci_mode_change_event_handler(event_data);
        break;

    case HCI_LINK_KEY_NOTIFICATION_EVENT:
        hci_link_key_notification_event_handler(event_data);
        break;

    case HCI_INQUIRY_COMPLETE_EVENT:
        hci_inquiry_complete_event_handler(event_data);
        break;

    case HCI_INQUIRY_RESULT_EVENT:
        hci_inquiry_result_event_handler(event_data);
        break;

    case HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT:
        hci_remote_name_request_complete_event_handler(event_data,
                                                       event_datalen);
        break;

    case HCI_VENDOR_SPECIFIC_DEBUG_EVENT:
        hci_vendor_specific_debug_event_handler(event_data, event_datalen);
        break;

    default:
        sdk_display("Unknown Event Code 0x%02X Received.\n", *event_data);
        break;
    }

    return API_SUCCESS;
}

/**
 * \fn      hci_link_key_notification_event_handler
 * \brief   HCI link key notification event handler
 * \param   event_data
 * \return  void
 */
void hci_link_key_notification_event_handler(UCHAR * event_data)
{
    UCHAR value_1;

    sdk_display((const UCHAR *)
                "Received HCI_LINK_KEY_NOTIFICATION_EVENT.\n");
    /* BD_ADDR */
    sdk_display((const UCHAR *)"\tBD_ADDR: ");
    appl_hci_print_bd_addr(event_data);
    event_data += 6;
    /* Link Key */
    sdk_display((const UCHAR *)"\tLink Key: ");
    appl_dump_bytes(event_data, 16);
    event_data += 16;
    /* Key Type */
    hci_unpack_1_byte_param(&value_1, event_data);
    sdk_display("\tKey Type: 0x%02X", value_1);
    switch (value_1) {
    case COMBINATION_KEY:
        sdk_display((const UCHAR *)" -> Combination Key\n");
        break;
    case LOCAL_UNIT_KEY:
        sdk_display((const UCHAR *)" -> Local Unit Key\n");
        break;
    case REMOTE_UNIT_KEY:
        sdk_display((const UCHAR *)" -> Remote Unit Key\n");
        break;
    default:
        sdk_display((const UCHAR *)" -> ???\n");
        break;
    }
    event_data += 1;
}

/**
 * \fn      hci_role_change_event_handler
 * \brief   HCI role change event handler
 * \param   event_data
 * \return  void
 */
void hci_role_change_event_handler(UCHAR * event_data)
{
    UCHAR value_1;
    UCHAR status;

    sdk_display((const UCHAR *)
                "Received HCI_ROLE_CHANGE_EVENT.\n");
    /* Status */
    hci_unpack_1_byte_param(&status, event_data);
    sdk_display("\tStatus: 0x%02X\n", status);
    event_data += 1;

    if (0x00 == status) {
        /* BD_ADDR of Remote Device */
        sdk_display((const UCHAR *)"\tBD_ADDR: ");
        appl_hci_print_bd_addr(event_data);
        event_data += 6;
        /* New Role */
        hci_unpack_1_byte_param(&value_1, event_data);
        sdk_display("\tNew Role: 0x%02X", value_1);
        switch (value_1) {
        case MASTER_ROLE:
            sdk_display((const UCHAR *)" -> Master\n");
            break;
        case SLAVE_ROLE:
            sdk_display((const UCHAR *)" -> Slave\n");
            break;
        default:
            sdk_display((const UCHAR *)" -> ???\n");
            break;
        }
        event_data += 1;
    }
}
