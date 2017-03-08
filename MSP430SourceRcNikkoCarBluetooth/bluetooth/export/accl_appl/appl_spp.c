
/**
 * Copyright (c) 2009 MindTree Ltd.  All rights reserved.
 * \file    appl_spp.c
 * \brief   This file contains SPP specific function definitions
 */

/* Header File Inclusion */
#include "appl_spp.h"
#include "l2cap.h"
#include "appl_sdk.h"
#include "task.h"
#include "appl_bt_rf.h"

/* Extern variables */
/* spp connections status information */
extern SDK_SPP_CONNECTION_STATUS sdk_status[];
/* Flag to check if USB is connected */
extern UCHAR sdk_usb_detected;
/* Flag to check if the device is SPP Initiator */
extern UCHAR sdk_initiator;
/* Flag to indicate that SPP connection is in progress */
extern UCHAR sdk_connect_in_progress;
/* User Context (HANDLE) used for SDP query to find out SPP record */
extern SDP_HANDLE appl_spp_sdp_handle;
extern UCHAR appl_spp_remote_server_ch;
extern UCHAR appl_spp_local_server_ch;
/* Based on the L2CAP Tx Buffer Flow this state will be set */
extern UCHAR appl_l2cap_tx_buf_state;

#ifdef SDK_EHCILL_MODE
extern volatile UINT32 ehcill_data;
#endif /* #ifdef SDK_EHCILL_MODE */
extern volatile UCHAR lpm_mode;
extern volatile UINT32 inactivity_counter;
extern volatile UCHAR LED_STATUS;
extern UCHAR inactivity_timeout;

/* Flag to indicate sniff mode is requested after the SPP connection */
extern UCHAR sdk_sniff_mode_requested;

#ifdef PACKETISE_USB_DATA
/* Start of header */
#define SOH                 0x01
#define HEADER_SIZE         1
#define PAYLOAD_SIZE        4
#define CHECKSUM_SIZE       1
#define PACKET_SIZE         7
#else
#define HEADER_SIZE         0
#define PAYLOAD_SIZE        4
#define CHECKSUM_SIZE       0
#define PACKET_SIZE         5
#endif /* #ifdef PACKETISE_USB_DATA */
typedef enum {
    INDEX_0,
    INDEX_1,
    INDEX_2,
    INDEX_3,
    INDEX_4,
    INDEX_5,
    INDEX_6
} PACKET_INDEX;

/* Functions */

/**
 * \fn      appl_spp_sdp_query
 * \brief   Function to initiate SDP query after a successful ACL connection
 * \param   rem_bt_dev_index Index of the remote BT device
 * \return  void
 */
void appl_spp_sdp_query(UCHAR rem_bt_dev_index)
{
    API_RESULT retval;

    /*
     *  SDP Query Should be done only after the Previously Initiated
     *  ACL Connection Completes Successfully.
     *
     *  Start SDP Query to Find out if the Peer Device is running SPP
     *  Service and if so on which Server Channel the Remote SPP Server
     *  is accepting connection.
     *
     *  In this Non-Blocking Implemenation, the whole process of
     *  Performing SDP Query is boot-straped by first establishing
     *  an SDP Connection with the Peer.
     *
     *  At the time of Requesting SDP to Establish a Connection, the
     *  Application also Registers a Callback function with SDP, using
     *  that Callback function SDP Module will keep indicating
     *  Asynchronous SDP events to the Application.
     *
     *  The Application Callback for SDP is the Third argument of
     *  the SDP_SET_HANDLE() Macro.
     */

    /* Set the SDP Handle MAKE IT GLOBAL */
    SDP_SET_HANDLE(appl_spp_sdp_handle,
                   sdk_status[rem_bt_dev_index].peer_bd_addr,
                   appl_spp_sdp_callback);

    /* Set the Peer Server Channel with invalid value */
    appl_spp_remote_server_ch = 0xFF;

    /* Open SDP Connection */
    sdk_display("Initiating SDP Connection ... ");

    retval = BT_sdp_open(&appl_spp_sdp_handle);

    if (retval != API_SUCCESS) {
        sdk_display("** FAILED ** !! Reason Code = 0x%04X\n", retval);

        SDK_SPP_CHANGE_STATE(rem_bt_dev_index, SDK_ACL_CONNECTED);
        sdk_display("Initiating ACL disconnection\n");
        /* Disconnect ACL */
        retval =
            BT_hci_disconnect(sdk_status[rem_bt_dev_index].
                              acl_connection_handle, 0x13);
        if (retval != API_SUCCESS) {
            sdk_display("** FAILED ** !! Reason Code = 0x%04X\n", retval);
        }
    } else {
        sdk_display("OK.\n");
    }

}

/**
 * \fn      appl_spp_sdp_callback
 * \brief   callback function for sdp command
 * \param   command SDP command
 * \param   data Buffer with paramters
 * \param   length Length of data buffers
 * \param   status Status of the SDP command
 * \return  void
 */
void appl_spp_sdp_callback(UINT8 command, UINT8 * data, UINT16 length,
                           UINT16 status)
{
    S_UUID uuid[1];
    UINT16 num_uuids;
    UINT16 attrib_id[1];
    UINT16 num_attribute_ids;
    API_RESULT retval;

    /** Buffer to hold SPP SDP Attribute Data */
    UCHAR *appl_spp_attrib_data;
    UINT16 appl_spp_attrib_data_len;

    /* Handle failures */
    if (status != API_SUCCESS) {
        sdk_display("*** SDP Command 0x%02X: 0x%04X\n", command, status);

        if (SDP_ServiceSearchAttributeResponse == command) {
            /* Close SDP Connection */
            BT_sdp_close(&appl_spp_sdp_handle);
        }

        return;
    }

    switch (command) {
    case SDP_Open:
        /* Initialize UUID for SPP */
        uuid[0].uuid_type = UUID_16;
        uuid[0].uuid_union.uuid_16 = SERIALPORT_UUID;
        num_uuids = 0x01;

        /* Initiate Attribute ID for SPP */
        attrib_id[0] = PROTOCOL_DESC_LIST;
        num_attribute_ids = 0x01;

        /* Allocate Memory */
        appl_spp_attrib_data_len = SDK_SPP_ATTRIB_DATA_LEN;
        appl_spp_attrib_data = BT_alloc_mem(appl_spp_attrib_data_len);

        if (NULL == appl_spp_attrib_data) {
            sdk_display("SPP Open Failed, reason %04X\n", status);
        } else {

            /* Initiate SDP Service Search Attribute Request for SPP */
            sdk_display("Initiating SDP SSA Request for SPP ... ");

            retval =
                BT_sdp_servicesearchattributerequest(&appl_spp_sdp_handle, uuid,
                                                     num_uuids, attrib_id,
                                                     num_attribute_ids, NULL,
                                                     0x00, appl_spp_attrib_data,
                                                     &appl_spp_attrib_data_len);

            if (retval != API_SUCCESS) {
                sdk_display("** FAILED ** !! Reason Code = 0x%04X\n", retval);

                /* Free Memory */
                BT_free_mem(appl_spp_attrib_data);

                /* Close SDP Connection */
                BT_sdp_close(&appl_spp_sdp_handle);
            } else {
                sdk_display("OK.\n");
            }
        }
        break;

    case SDP_ServiceSearchAttributeResponse:
        /* Get/Decode RFCOMM Server Channel value */
        sdk_display("Received Response for SDP SSA Request for SPP.\n");
        sdk_display("Getting SPP Server Channel ... ");

        retval = BT_sdp_get_channel_number(data, &appl_spp_remote_server_ch);

        if (retval != API_SUCCESS) {
            sdk_display("*** FAILED to Get Server Channel Value: 0x%04X\n",
                        retval);
        } else {
            sdk_display("OK.\n");
            sdk_display("SPP Server Channel: 0x%02X\n",
                        appl_spp_remote_server_ch);

            /*
             * Mark flag to Initiate SPP Connection
             * after SDP disconnection.
             *
             * SPP Connection can be initiated from here too,
             * but it will done after SDP disconnection, for
             * better interoperability.
             *
             * Extracted Server Channel itself can be used as flag
             * to save memory
             */
        }

        /* Free Memory */
        BT_free_mem(data);

        /* Close SDP Connection */
        BT_sdp_close(&appl_spp_sdp_handle);

        break;

    case SDP_Close:            /* Fall Through */
        /* Check if the SPP Server Channel is extracted */
        if (0xFF != appl_spp_remote_server_ch) {
            /* Initiate SPP Connection */
            appl_spp_connect(appl_spp_sdp_handle.bd_addr,
                             appl_spp_remote_server_ch);
        }
        break;

    default:
        break;
    }

}

/**
 * \fn      appl_spp_connect
 * \brief   Function to initiate SPP connection with a peer
 * \param   bd_addr BD address of peer
 * \param   server_ch Server channel
 * \return  void
 */
void appl_spp_connect(UCHAR * bd_addr, UCHAR server_ch)
{
    API_RESULT retval;
    UCHAR rem_bt_dev_index;

    /* Get the remote device status instance */
    retval = appl_get_status_instance_bd_addr(&rem_bt_dev_index, bd_addr);

    if (retval != API_SUCCESS) {
        sdk_display("Unknown remote device\n");
    } else {

        sdk_display("Initiating SPP Connection ... ");
        retval = BT_spp_connect(bd_addr, server_ch);

        if (retval != API_SUCCESS) {
            sdk_display("** FAILED ** !! Reason Code = 0x%04X\n", retval);
            sdk_display("Initiating ACL disconnection\n");
            /* Disconnect ACL */
            retval =
                BT_hci_disconnect(sdk_status[rem_bt_dev_index].
                                  acl_connection_handle, 0x13);
            if (retval != API_SUCCESS) {
                sdk_display("** FAILED ** !! Reason Code = 0x%04X\n", retval);
            }
        } else {
            sdk_display("OK.\n");
            SDK_SPP_CHANGE_STATE(rem_bt_dev_index, SDK_IN_SPP_CONNECTION);
        }
    }
}

/**
 * \fn      appl_spp_disconnect
 * \brief   Function to initate a SPP disconnetion with a peer
 * \param   rem_bt_dev_index Index of peer device
 * \return  void
 */
void appl_spp_disconnect(UCHAR rem_bt_dev_index)
{
    API_RESULT retval;

    /* Check if there exist SPP Connection */
    /* Do SPP Close */
    sdk_display("Initiating SPP Disconnections ... ");
    retval =
        BT_spp_disconnect(sdk_status[rem_bt_dev_index].spp_connection_handle);

    if (retval != API_SUCCESS) {
        sdk_display("** FAILED ** !! Reason Code = 0x%04X\n", retval);
        sdk_display("Initiating ACL disconnection\n");
        /* Disconnect ACL */
        retval =
            BT_hci_disconnect(sdk_status[rem_bt_dev_index].
                              acl_connection_handle, 0x13);
        if (retval != API_SUCCESS) {
            sdk_display("** FAILED ** !! Reason Code = 0x%04X\n", retval);
        }
    } else {
        sdk_display("OK.\n");
        SDK_SPP_CHANGE_STATE(rem_bt_dev_index, SDK_IN_SPP_DISCONNECTION);
    }
}

#ifdef PACKETISE_USB_DATA

/**
*\brief  This function is used to calculate the checksum for the given buffer
*\param [in] buffer - Data for which checksum is caluclated
*\return checksum - returns the calculated checksum*
*/

UCHAR calculate_checksum(UCHAR * buffer)
{
    UCHAR checksum = 0;
    UCHAR index;
    for (index = 0; index < (HEADER_SIZE + PAYLOAD_SIZE); index++) {
        checksum += buffer[index];
    }
    return checksum;
}
#endif /* #ifdef PACKETISE_USB_DATA */



/**
 * \fn      appl_spp_notify_cb
 * \brief   Function to handle SPP indication
 * \param   handle Handle of SPP connection
 * \param   event_type Type of SPP event
 * \param   result Result
 * \param   data Data buffer to decode
 * \param   datalen Length of data buffer
 * \return  API_RESULT       API_SUCCESS/API_FAILURE
 */
API_RESULT appl_spp_notify_cb(IN SPP_HANDLE handle, IN SPP_EVENTS event_type,
                              IN API_RESULT result, IN void *data,
                              IN UINT16 datalen)
{
    UCHAR *l_data;
    API_RESULT retval;
    UINT8 index;
    UCHAR rem_bt_dev_index;

    /* String to store the SPP dat received */
    UCHAR packet[PACKET_SIZE];
    l_data = (UCHAR *) (data);


    sdk_display("\n" "SPP HANDLE : %u\n" "EVENT      : %d\n"
                "RESULT     : 0x%04X\n", (unsigned int)handle, event_type,
                result);
    if ((SPP_CONNECT_CNF == event_type) || (SPP_CONNECT_IND == event_type)) {
        /* Get the remote device status instance based on rem bd addr */
        appl_get_status_instance_bd_addr(&rem_bt_dev_index, l_data);
    } else {
        /* Get the remote device status instance based on spp connection handle
         */
        retval = appl_get_status_instance_spp(&rem_bt_dev_index, handle);
        if (retval != API_SUCCESS) {
            /* Unknown SPP connection instance */
            return retval;
        }
    }

    if (API_SUCCESS != result) {
        sdk_display("\nSPP Failure\n");

        if (SPP_CONNECT_CNF == event_type) {
            /* Try Reconnect SPP */
            appl_spp_sdp_query(rem_bt_dev_index);
        }
        return API_FAILURE;
    }

    switch (event_type) {
    case SPP_CONNECT_CNF:
        SDK_DEBUG_PRINT_STRING("SPP Connection successful\n");
        sdk_initiator = TRUE;
        appl_spp_connected_indication();
        sdk_display("SPP_CONNECT_CNF -> Connection Successful\n");
        sdk_display("Remote device BD_ADDR : %02X:%02X:%02X:%02X:%02X:%02X\n",
                    l_data[0], l_data[1], l_data[2], l_data[3], l_data[4],
                    l_data[5]);

        if (API_SUCCESS == result) {
            /* Save SPP Handle and Change State to SPP Connected */
            sdk_status[rem_bt_dev_index].spp_connection_handle = handle;
            SDK_SPP_CHANGE_STATE(rem_bt_dev_index, SDK_SPP_CONNECTED);
            /* Start sending the data */
            SDK_SPP_CHANGE_TX_STATE(rem_bt_dev_index, SDK_SPP_TX_ON);
            /* SPP Connection complete so reset the flag */
            sdk_connect_in_progress = FALSE;

#ifdef SDK_ENABLE_SNIFF_MODE
            if (!SDK_IS_IN_SNIFF_MODE(rem_bt_dev_index)) {
                sdk_sniff_mode_requested = TRUE;
                retval =
                    BT_hci_sniff_mode(sdk_status[rem_bt_dev_index].
                                      acl_connection_handle,
                                      SDK_CONFIG_SNIFF_MAX_INTERVAL,
                                      SDK_CONFIG_SNIFF_MIN_INTERVAL,
                                      SDK_CONFIG_SNIFF_ATTEMPT,
                                      SDK_CONFIG_SNIFF_TIMEOUT);
                if (API_SUCCESS == retval) {
                    sdk_display("Started sniff mode\n");
                    break;
                }
            }
#else
            appl_send_spp_data(rem_bt_dev_index);
#endif /* SDK_ENABLE_SNIFF_MODE */
        }
        break;

    case SPP_CONNECT_IND:
        sdk_initiator = FALSE;
        appl_spp_connected_indication();
        sdk_display("SPP_CONNECT_IND -> Connection Successful\n");
        sdk_display("Remote device BD_ADDR : %02X:%02X:%02X:%02X:%02X:%02X\n",
                    l_data[0], l_data[1], l_data[2], l_data[3], l_data[4],
                    l_data[5]);

        if (API_SUCCESS == result) {
            /* Save SPP Handle and Change State to SPP Connected */
            sdk_status[rem_bt_dev_index].spp_connection_handle = handle;
            SDK_SPP_CHANGE_STATE(rem_bt_dev_index, SDK_SPP_CONNECTED);
            /* Start sending the data */
            SDK_SPP_CHANGE_TX_STATE(rem_bt_dev_index, SDK_SPP_TX_ON);
        }

        appl_send_spp_data(rem_bt_dev_index);

        /* TBD: Reset state on failure */

        break;

    case SPP_DISCONNECT_CNF:
        sdk_display("SPP_DISCONNECT_CNF -> Disconnection Successful\n");
        sdk_display("Remote device BD_ADDR : %02X:%02X:%02X:%02X:%02X:%02X\n",
                    l_data[0], l_data[1], l_data[2], l_data[3], l_data[4],
                    l_data[5]);

        SDK_SPP_CHANGE_STATE(rem_bt_dev_index, SDK_SPP_DISCONNECTED);

        SDK_SPP_CHANGE_TX_STATE(rem_bt_dev_index, SDK_SPP_TX_OFF);
#ifdef SDK_ENABLE_SNIFF_MODE
        SDK_SPP_CHANGE_LINK_STATE(rem_bt_dev_index, SDK_OFF);
#endif /* SDK_ENABLE_SNIFF_MODE */
        /* Initiate acl disconnection */
        sdk_display("Initiating ACL disconnection\n");
        retval =
            BT_hci_disconnect(sdk_status[rem_bt_dev_index].
                              acl_connection_handle, 0x13);
        if (retval != API_SUCCESS) {
            sdk_display("** FAILED ** !! Reason Code = 0x%04X\n", retval);
        }
        break;

    case SPP_DISCONNECT_IND:
        sdk_display("SPP_DISCONNECT_IND -> Disconnection Successful\n");
        sdk_display("Remote device BD_ADDR : %02X:%02X:%02X:%02X:%02X:%02X\n",
                    l_data[0], l_data[1], l_data[2], l_data[3], l_data[4],
                    l_data[5]);

        SDK_SPP_CHANGE_STATE(rem_bt_dev_index, SDK_SPP_DISCONNECTED);
        /* To reflect the changes in the SPP cionnection to the user */
        SDK_SPP_CHANGE_TX_STATE(rem_bt_dev_index, SDK_SPP_TX_OFF);
#ifdef SDK_ENABLE_SNIFF_MODE
        SDK_SPP_CHANGE_LINK_STATE(rem_bt_dev_index, SDK_OFF);
#endif /* SDK_ENABLE_SNIFF_MODE */
        break;

    case SPP_STOP_CNF:
        sdk_display("SPP_STOP_CNF -> Stop Successful\n");
        break;

    case SPP_SEND_CNF:
        sdk_display("SPP_SEND_CNF -> Sent successfully\n");

        if (SDK_IS_SPP_CONNECTED(rem_bt_dev_index)
            && SDK_IS_SPP_TX_STARTED(rem_bt_dev_index)) {
            if (L2CAP_TX_QUEUE_FLOW_ON == appl_l2cap_tx_buf_state) {
                appl_send_spp_data(rem_bt_dev_index);
            } else {
                SDK_SPP_CHANGE_DATA_STATE(rem_bt_dev_index, TRUE);
            }
        }
        break;

    case SPP_RECVD_DATA_IND:
        sdk_display("SPP_RECVD_DATA_IND -> Data received successfully\n");
        sdk_display("\n----------------HEX DUMP------------------------\n");

        P7DIR |= BIT4;
        P7DIR |= BIT5;
        P7DIR |= BIT6;
        P7DIR |= BIT7;
        
        if (l_data[0]=='a') { // Down
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
            {P7OUT |= BIT7;P7OUT |= BIT7;}

            for (int i = 0; i < 0x0FFF; i++) 
              P7OUT &= ~ BIT7;
          }
        } 
        if (l_data[0]=='c') {//UP
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
            {P7OUT |= BIT5;P7OUT |= BIT5;}

            for (int i = 0; i < 0x0FFF; i++) 
              P7OUT &= ~ BIT5;
          }
        }
          
        if (l_data[0]=='d') {//Right
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x2FFF; i++) 
              P7OUT |= BIT4;

            for (int i = 0; i < 0x1FFF; i++) 
              P7OUT |= BIT4;
          }

        }          
        if (l_data[0]=='b') {//left
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
              P7OUT |= BIT6;

            for (int i = 0; i < 0x1FFF; i++) 
              P7OUT |= BIT6;
          }

        }

        if (l_data[0]=='0') { // Down 'a' DWLeft
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
            {P7OUT |= BIT7;P7OUT |= BIT6;}

            for (int i = 0; i < 0x0FFF; i++) 
            {P7OUT &= ~ BIT7;P7OUT |= BIT6;}
          }
        } 
        if (l_data[0]=='1') { // Down 'a' DWRight
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
            {P7OUT |= BIT7;P7OUT |= BIT4;}

            for (int i = 0; i < 0x0FFF; i++) 
              {P7OUT &= ~ BIT7;P7OUT |= BIT4;}
          }
        } 

        
        if (l_data[0]=='2') {//UP left
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
            {P7OUT |= BIT5;P7OUT |= BIT6;}

            for (int i = 0; i < 0x0FFF; i++) 
            {P7OUT &= ~ BIT5;P7OUT |= BIT6;}
          }
        }
          
        if (l_data[0]=='3') {//UP right
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
            {P7OUT |= BIT5;P7OUT |= BIT4;}

            for (int i = 0; i < 0x0FFF; i++) 
            {P7OUT &= ~ BIT5;P7OUT |= BIT4;}
          }
        }

        if (l_data[0]=='z') {//UP turbo
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
            {P7OUT |= BIT5;P7OUT |= BIT5;}

            for (int i = 0; i < 0x1FFF; i++) 
             {P7OUT |= BIT5;P7OUT |= BIT5;}
          }
        }

        if (l_data[0]=='x') {//UP left Turbo
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
            {P7OUT |= BIT5;P7OUT |= BIT6;}

            for (int i = 0; i < 0x1FFF; i++) 
              {P7OUT |= BIT5;P7OUT |= BIT6;}
          }
        }
          
        if (l_data[0]=='y') {//UP right Turbo
          for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 0x1FFF; i++) 
            {P7OUT |= BIT5;P7OUT |= BIT4;}

            for (int i = 0; i < 0x1FFF; i++) 
             {P7OUT |= BIT5;P7OUT |= BIT4;}
          }
        }
    
        P7OUT &= ~ BIT4;
        P7OUT &= ~ BIT5;
        P7OUT &= ~ BIT6;
        P7OUT &= ~ BIT7;
        
        for (index = 0; index < datalen; index++) {
            sdk_display("%02X ", l_data[index]);
        }
        sdk_display("\n------------------------------------------------\n");


        if (TRUE == sdk_usb_detected) {
#ifdef PACKETISE_USB_DATA
            packet[INDEX_0] = SOH;  /* SOH Character */
            memcpy(&packet[INDEX_1], l_data, PAYLOAD_SIZE);
            packet[INDEX_5] = calculate_checksum(packet);
            packet[INDEX_6] = '\0';
            halUsbSendString(packet);
#else
            memcpy(packet, l_data, PAYLOAD_SIZE);
            packet[INDEX_5] = '\0';
            halUsbSendString(packet);
#endif /* #ifdef PACKETISE_USB_DATA */

        }
        break;

    default:
        sdk_display("\nUnknown command type\n");
        break;
    }                           /* switch */

    return API_SUCCESS;
}

/**
 * \fn      appl_spp_write
 * \brief   Function to write data on a SPP connection
 * \param   rem_bt_dev_index Index of peer BT device
 * \param   data Pointer to data buffer
 * \param   data_len Length of the data buffer
 * \return  API_RESULT      API_SUCCESS/API_FAILURE
 */
API_RESULT appl_spp_write(UCHAR rem_bt_dev_index, UCHAR * data, UINT16 data_len)
{
    return BT_spp_send(sdk_status[rem_bt_dev_index].spp_connection_handle, data,
                       data_len);
}

/**
 * \fn      appl_spp_start
 * \brief   Function to init and start SPP from application
 * \param   void
 * \return  void
 */
void appl_spp_start(void)
{
    API_RESULT retval;
    UINT32 appl_spp_record_handle;
    SM_SERVICE appl_spp_sm_service;
    UCHAR appl_spp_sm_service_id;

    /* Get record handle */
    retval =
        BT_dbase_get_record_handle(DB_RECORD_SPP, 0, &appl_spp_record_handle);

    if (retval != API_SUCCESS) {
        sdk_display("*** FAILED to Get SPP Record Handle: 0x%04X\n", retval);
        return;
    }

    /* Get Server Channel from SPP Service Record */
    retval =
        BT_dbase_get_server_channel(appl_spp_record_handle, PROTOCOL_DESC_LIST,
                                    &appl_spp_local_server_ch);

    if (retval != API_SUCCESS) {
        sdk_display("*** FAILED to Get SPP Server Channel: 0x%04X\n", retval);
    } else {

        /**
         * Add RFCOMM Service (with SPP Server Channel) security needs to the
         * SM Service Database
         */
        appl_spp_sm_service.psm = RFCOMM_PSM;
        appl_spp_sm_service.server_ch = appl_spp_local_server_ch;
        /* Authentication level */
        appl_spp_sm_service.authenticate = SDK_CONFIG_SPP_AUTHENTICATION_LEVEL;
        /* Authorization Not Required */
        appl_spp_sm_service.authorize = SDK_CONFIG_SPP_AUTHORIZATION_REQ;
        /* Encryption is always ON under SSP */
        appl_spp_sm_service.encrypt = 0x1;
        /* Must Register UI Callback for SSP */
        appl_spp_sm_service.service_sm_cb = appl_sm_service_cb;
#ifdef BT_SSP
        /* No Bonding */
        appl_spp_sm_service.no_bonding = SDK_CONFIG_SPP_BONDING_REQ;
#endif /* BT_SSP */

        retval =
            BT_sm_add_service(&appl_spp_sm_service, &appl_spp_sm_service_id);
        if (retval != API_SUCCESS) {
            sdk_display
                ("*** FAILED to add RFCOMM Service security needs: 0x%04X\n",
                 retval);
        }

        /* Start SPP */
        retval = BT_spp_start(appl_spp_local_server_ch);

        if (API_SUCCESS != retval) {
            sdk_display("Failed to Start SPP, reason %04X\n", retval);
        }

        /* Activate SPP Service Record - TODO */
        retval = BT_dbase_activate_record(appl_spp_record_handle);

        if (retval != API_SUCCESS) {
            sdk_display("*** FAILED to Activate SPP Service Record: 0x%04X\n",
                        retval);
        } else {

            l2cap_register_tx_queue_flow_cb(appl_spp_l2cap_tx_queue_flow_cb);
        }
    }
}

/**
 * \fn      appl_spp_stop
 * \brief   Function to stop SPP from application
 * \param   void
 * \return  void
 */
void appl_spp_stop(void)
{
    BT_spp_stop();
}

/**
 * \fn      appl_spp_l2cap_tx_queue_flow_cb
 * \brief   Callback funtion for L2cap TX queue flow for SPP data
 * \param   void
 * \return  void
 */
API_RESULT appl_spp_l2cap_tx_queue_flow_cb(UCHAR state, UINT16 count)
{
    UCHAR index;

    /* Set the local tx state */
    appl_l2cap_tx_buf_state = state;

    /* Flow On - Check if data to be sent */
    if (L2CAP_TX_QUEUE_FLOW_ON == state) {
        for (index = 0; index < SPP_MAX_ENTITY; index++) {
            /* Check if data to be sent */
            if (SDK_IS_IN_DATA_SEND_STATE(index)) {
                if (SDK_IS_SPP_CONNECTED(index) && SDK_IS_SPP_TX_STARTED(index)) {
                    appl_send_spp_data(index);
                }

                SDK_SPP_CHANGE_DATA_STATE(index, FALSE);
            }
        }
    }
    /* Flow Off - Don't send data */
    else {
        /* Set data can't be sent */
    }

    return API_SUCCESS;
}

/**
 * \fn      appl_sm_service_cb
 * \brief   Callback function for registered RFCOMM service with SM.
 * \param   event_type Type of event
 * \param   bd_addr Address of peer device
 * \param   event_data Data buffer for events
 * \return  API_RESULT      API_SUCCESS/API_FAILURE
 */
API_RESULT appl_sm_service_cb(UCHAR event_type, UCHAR * bd_addr,
                              UCHAR * event_data)
{
    API_RESULT retval;
    UCHAR appl_sm_ui_authorization_request_reply;

    sdk_display("\n");
    sdk_display("Received SM Service UI Notification. Event Type 0x%02X\n",
                event_type);

    retval = API_SUCCESS;

    switch (event_type) {
    case SM_AUTHORIZATION_REQUEST_NTF:
        sdk_display("Received UI Authorization Request from SM\n");
        sdk_display("BD_ADDR = %02X:%02X:%02X:%02X:%02X:%02X\n", bd_addr[0],
                    bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

        /* Accept the authorization request */
        appl_sm_ui_authorization_request_reply = 0x01;

        sdk_display("Replying to UI Authorization Request ... ");

        retval =
            BT_sm_authorization_request_reply(bd_addr,
                                              appl_sm_ui_authorization_request_reply);

        if (retval != API_SUCCESS) {
            sdk_display("FAILED ! Reason = 0x%04X\n", retval);
            break;
        } else {
            sdk_display("OK\n");
        }

        break;
    default:
        sdk_display("*** Unknown/Undefined Event Type 0x%02X\n", event_type);
        break;
    }

    return retval;
}

/**
 * \fn      vApplicationIdleHook
 * \brief   Idle hook function for application
 * \param   void
 * \return  void
 */
void vApplicationIdleHook(void)
{

#ifdef MSP430_LPM_ENABLE
    UCHAR index = 0;
#endif /* MSP430_LPM_ENABLE */

#ifdef SDK_EHCILL_MODE
    __disable_interrupt();
    if (0x0FFFF == ehcill_data) {
        UART_DISABLE_BT_UART_RTS();
    } else {
        UART_ENABLE_BT_UART_RTS();
    }
    __enable_interrupt();
#else

     /**
      * If EHCILL is not defined RTS is enabled to handle exiting LPM from
      * peer device else CTS pulse is used to exit LPM
      */
    UART_ENABLE_BT_UART_RTS();
#endif /* SDK_EHCILL_MODE */


#ifdef MSP430_LPM_ENABLE
    if (inactivity_timeout == inactivity_counter) {
        __disable_interrupt();
        lpm_mode = TRUE;
        /* Save the LED pins status */
        LED_STATUS = LED_PORT_OUT & (LED_1 | LED_2);
        /* switch OFF the LEDs * */
        LED_PORT_OUT &= ~(LED_1 | LED_2);
        LED_PORT_DIR |= (LED_1 | LED_2);

#ifdef EZ430_PLATFORM
        halAccStop();
        halI2CShutdown();
#else /* MSP-EXP430F5438 Platform */
        halAccelerometerShutDown();
#endif
        /* Halting the Timer1_A3 */
        TA1CTL &= ~MC_3;
        for (index = 0; index < (SPP_MAX_ENTITY - 1); index++) {
            SDK_SPP_CHANGE_TX_STATE(index, SDK_SPP_TX_OFF);
        }
        /* Turn OFF UART1 which is used as USB serial port */
        halUsbShutDown();
        ENTER_MSP430_LPM();
    }
#endif /* MSP430_LPM_ENABLE */


}
