
/**
 *  @file BT_spp_api.h
 *
 *  This file contains the data structures, the type definitions
 *  and API functions that SPP exports to the application.
 */

/*
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_SPP_API_
#define _H_BT_SPP_API_

/* ----------------------------------------------- Header File Inclusion */
#include "BT_api.h"


/* ----------------------------------------------- Global Definitions */


/* ----------------------------------------------- Structures/Data Types */
/**
 *  SPP Callback Event Types
 */
typedef enum
{
    SPP_NO_EVENT,
    SPP_CONNECT_CNF,
    SPP_CONNECT_IND,
    SPP_DISCONNECT_CNF,
    SPP_DISCONNECT_IND,
    SPP_STOP_CNF,
    SPP_SEND_CNF,
    SPP_RECVD_DATA_IND
} SPP_EVENTS;

/**
 *  SPP Handle
 */
typedef UINT32 SPP_HANDLE;

/**
 * @brief Application should register a callback through the API BT_spp_init.       
 * @param [IN] spp_handle 
 *   Handle associates a SPP connection to a remote SPP device.
 * @param [IN] spp_event 
 *   Event indicated to application by Serial Port profile.
 * @param [IN] status 
 *   The status of the event.
 * @param [IN] data 
 *   Pointer to the array containing the parameters associated with the event.
 * @param [IN] data_length 
 *    Length of the parameters associated with the event.
 * @return API_RESULT:
 *   API_RESULT:  API_SUCCESS or one of the error codes defined in BT_error.h.
 * @see   BT_error.h
 */
typedef API_RESULT (* SPP_APPL_EVENT_NOTIFY_CB)
                   (
                       IN  SPP_HANDLE,    /* Handle */
                       IN  SPP_EVENTS,    /* Event Type */
                       IN  API_RESULT,    /* Event Result */
                       IN  void *,        /* Event Data */
                       IN  UINT16         /* Event Data Size */
                   );


/* ----------------------------------------------- Macros */


/* ----------------------------------------------- Function Declarations */
/**
 * @brief This API registers the application callback function with the SPP and 
 *        initializes the Serial Port Profile.
 * @param [IN] notify_cb
 *   Application callback function pointer.
 * @return API_RESULT:
 *   API_SUCCESS: Indicates Serial Port  profile initialized successfully
 *   SPP_ERR_NULL_PARAMETER: Indicates application is trying to register NULL as
 *   callback.
 *   SPP_ERR_MUTEX_INIT_FAILED: Indicates initialization failed due to failure
 *   to initialize mutex.
 * @see   BT_error.h
 */
API_RESULT BT_spp_init
           (
               IN  SPP_APPL_EVENT_NOTIFY_CB    notify_cb
           );

/**
 * @brief This API starts the SPP instance. SPP implementation starts listening
 *        on the RFCOMM Channel specified. It enables the application to use the 
 *        service of the Serial Port profile.
 * @param [IN] server_channel
 *   The RFCOMM server channel in which the SPP device will be listening for 
 *   peer initiated connection.
 * @return API_RESULT:
 *   API_SUCCESS: Indicates SPP started successfully
 *   SPP_ERR_INVALID_STATE: Cannot process user request. API called in wrong HS
 *   State.
 *   SPP_ERR_NO_FREE_ENTITY: Cannot Start. Free entity not available to allow
 *   incoming connections.
 * @see   BT_error.h
 */
API_RESULT BT_spp_start
           (
               IN  UCHAR    server_channel
           );

/**
 * @brief This API stops the SPP service function on the local device.
 *        Existing active SPP connection if any will be released.
 * @param none
 * @return API_RESULT:
 *   API_SUCCESS: Indicates SPP switched off
 *   SPP_ERR_STOP_PENDING: In the process of releasing active connections, wait
 *   until event is indicated in the callback.
 * @see   BT_error.h
 */
API_RESULT BT_spp_stop
           (
               void
           );

/**
 * @brief This API establishes service level connection between SPP Dev A and
 *        SPP Dev B
 * @param [IN] bd_addr
 *   Bluetooth Device address of the remote SPP Device with which connection
 *   needs to be established.
 * @param [IN] server_channel
 *   The server channel in which connection has to be initiated.
 * @return API_RESULT:
 *   API_SUCCESS: Indicates connection procedure initiated successfully
 *   SPP_ERR_INVALID_STATE: Cannot process user request. API called in wrong SPP
 *   State.
 *   SPP_ERR_MAX_CONNECT_LIMIT_REACHED: SPP already connected to maximum
 *   number of peers. Cannot initiate the SPP connection.
 * @see   BT_error.h
 */
API_RESULT BT_spp_connect
           (
               IN  UCHAR *    bd_addr,
               IN  UCHAR      server_channel
           );

/**
 * @brief This API disconnects existing service level connection with the peer
 *        SPP device.
 * @param [IN] handle 
 *   Handle identifies a SPP connection.
 * @return API_RESULT:
 *   API_SUCCESS: Indicates disconnection procedure initiated successfully
 *   SPP_ERR_INVALID_HANDLE: Invalid handle.
 *   SPP_ERR_INVALID_STATE: Cannot process user request. API called in wrong HS
 *   State.
 * @see   BT_error.h
 */
API_RESULT BT_spp_disconnect
           (
               IN  SPP_HANDLE    handle
           );

/**
 * @brief This API sends the data to remote SPP device.
 * @param [IN] spp_handle 
 *   Handle associates a SPP connection to a remote SPP device.
 * @param [IN] data 
 *   Data to be sent to peer over SPP channel.
 * @param [IN] data_len 
 *   Length of data to be sent to peer over SPP channel.
 * @return API_RESULT:
 *   API_SUCCESS: Indicates successful operation.
 *   SPP_ERR_INVALID_HANDLE: Invalid handle
 *   SPP_ERR_INVALID_STATE: Cannot process user request. API called in wrong SPP
 *   State.
 * @see   BT_error.h
 */
API_RESULT BT_spp_send
           (
               IN  SPP_HANDLE    spp_handle,
               IN  UCHAR *       data,
               IN  UINT16        data_len
           );


#endif /* _H_BT_SPP_API_ */

