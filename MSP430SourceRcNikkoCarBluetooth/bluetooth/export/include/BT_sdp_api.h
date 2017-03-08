
/**
 * @file BT_sdp_api.h
 *  
 *  This File contains the declaration of the MindTree Bluetooth
 *  Service Discovery Protocol Application Program Interface functions.
 */

/* 
 *  Copyright (C) 2009-2010. MindTree Ltd.
 *  All rights reserved.
 */
 
#ifndef _H_BT_SDP_API_
#define _H_BT_SDP_API_

/* -------------------------------- Header File Inclusion */
#include "BT_common.h"


/* -------------------------------- Global Definitions */
/* ----------------- SDP API/PDU Identifiers */
/** PDU ID for SDP Error response */
#define SDP_ErrorResponse                  0x01

/** PDU ID for Service Search Request */
#define SDP_ServiceSearchRequest           0x02

/** PDU ID for Service Search Response */
#define SDP_ServiceSearchResponse          0x03

/** PDU ID for Service Attribute Request */
#define SDP_ServiceAttributeRequest        0x04

/** PDU ID for Service Attribute Response */
#define SDP_ServiceAttributeResponse       0x05

/** PDU ID for Service Search Attribute Request */
#define SDP_ServiceSearchAttributeRequest  0x06

/** PDU ID for Service Search Attribute Response */
#define SDP_ServiceSearchAttributeResponse 0x07

/* ----------------- Other APIs */
/** Identifer for SDP open operation */
#define SDP_Open                           0x08

/** Identifier for SDP Close operation */
#define SDP_Close                          0x09

/* ---------------- SDP Response Error code Values */
/** SDP error response code : Reserved */
#define SDP_INVALID_ERR                    0x0000

/** SDP error response code : Invalid version */
#define SDP_INVALID_VERSION                0x0001

/** SDP error response code : Invalid record handle */
#define SDP_INVALID_RECORD_HANDLE          0x0002

/** SDP error response code : Invalid request syntax */
#define SDP_INVALID_REQUEST_SYNTAX         0x0003

/** SDP error response : invalid  PDU size */
#define SDP_INVALID_PDU_SIZE               0x0004

/** SDP error response : Invalid continuation state */
#define SDP_INVALID_CONT_STATE             0x0005

/** SDP error response : Insufficient resources */
#define SDP_INSUFFICIENT_RESOURCE          0x0006

/** SDP Server Events */
#define SDP_SERVER_CONNECT_IND             0x11
#define SDP_SERVER_DISCONNECT_IND          0x12

/* ---------------- Parameter Types */



/* -------------------------------- Data Type/Structures/Typedefs */
/**
 *  Structure for SDP Profile Descriptor list.
 *  This contains two fields, UUID value followed by a Version number.
 *  Utility API for extracting profile descriptor list uses this structure.
 */
typedef struct profile_desc_list
{
     /** UUID */
     S_UUID uuid;

     /** Version number */
     UINT16 version;

} PROFILE_DESC_STRUCT;


/* Registering Callbacks to SDP */
#ifdef SDP_HAVE_HANDLE_WITH_CB
/* Forward declaration */
struct sdp_handle;

typedef void (* SDP_CB)
             (
                 struct sdp_handle * handle,
                 UCHAR               command,
                 UCHAR *             data,
                 UINT16              length,
                 UINT16              status
             );
#else  /* SDP_HAVE_HANDLE_WITH_CB */
/**
 * @brief SDP Response callback
 * @param [IN] type 
 *   This indicates the type of response.
 * @param [IN] response 
 *   This parameter represents the SDP response byte stream received from the 
 *   remote Bluetooth device in response to the request API called earlier by the application.
 * @param [IN] length 
 *   This parameter represents the size of SDP response byte stream.
 * @param [IN] status 
 *   This parameter indicates the result of the transaction.
 * @return API_RESULT:
 *   API_RESULT:  API_SUCCESS or one of the error codes defined in BT_error.h.
 * @see   BT_error.h
 */
typedef void (* SDP_CB)
             (
                 UCHAR      command,
                 UCHAR *    data,
                 UINT16     length,
                 UINT16     status
             );
#endif /* SDP_HAVE_HANDLE_WITH_CB */

/**
 *  SDP Handle.
 *
 *  For each SDP connection, SDP assigns a handle to be used while calling
 *  the SDP Request APIs later on.
 */
typedef struct sdp_handle
{
    /** Application callback */
    SDP_CB fn_ptr;

    /** Bluetooth Device Address */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

    /** Handle ID */
    UCHAR id;

} SDP_HANDLE;

/** Macros for Setting and Resetting the SDP handles */
#define SDP_SET_HANDLE(handle, bd, callback) \
        BT_COPY_BD_ADDR (handle.bd_addr, bd); \
        handle.fn_ptr = callback

#define SDP_RESET_HANDLE(handle) \
        memset (&handle, 0, sizeof (SDP_HANDLE))

/** Macros for UUID Conversion API */
#define BT_uuid_16_to_128       BT_uuid_32_to_128


/* -------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ---------------- Request APIs */
/**
 *  @brief This API initiates the SDP connection establishment procedure
 *  with a remote SDP Server. Application has to call this API
 *  before doing any SDP transaction requests with the server.
 *  Blocking mode Architecture blocks until the connection establishment or
 *  failure to proceed with the operation, whereas Non Blocking return
 *  immediately after initializing the operation and the registered
 *  callback (callback in the handle) will be called on
 *  completion (It may be successful or failure) of the operation. 
 *  @param handle (IN)
 *    SDP Handle for the connection. It also registers the callback specified in 
 *    the handle.
 *  @return API_RESULT
 *    API_SUCCESS, or, an Error Code from BT_error.h
 */
API_RESULT BT_sdp_open
           (
               IN   SDP_HANDLE *  /* handle */
           );

/**
 *  @brief This API initiates the termination of the SDP connection
 *  with the remote SDP server. It returns failure if there is
 *  any active SDP connection with that device. Blocking mode Architecture
 *  blocks until the completion of the operation (It may be failure or
 *  success), whereas Non Blocking return immediately after initializing
 *  the operation and the registered callback (callback in the handle) will
 *  be called on completion of the operation.
 *  @param handle (IN)
 *    SDP Handle for the connection.
 *  @return API_RESULT
 *    API_SUCCESS, or, an Error Code from BT_error.h
 */
API_RESULT BT_sdp_close
           (
               IN    SDP_HANDLE * /* handle */
           );


/* SDP Service/Attribute Searches */
API_RESULT BT_sdp_servicesearchrequest
           (
               IN    SDP_HANDLE * /* handle */,
               IN    S_UUID *     /* uuids  */,
               IN    UINT16       /* num_uuids */,
               OUT   UINT32 *     /* rec_handles */,
               INOUT UINT16 *     /* num_rec_handles */
           );

API_RESULT BT_sdp_serviceattributerequest
           (
               IN    SDP_HANDLE * /* handle */,
               IN    UINT32       /* rec_handle */,
               IN    UINT16 *     /* attribute_ids */,
               IN    UINT16       /* num_attribute_ids */,
               IN    UINT32 *     /* attribute_id_range */,
               IN    UINT16       /* num_attribute_id_range */,
               OUT   UCHAR *      /* attribute_data */,
               INOUT UINT16 *     /* len_attribute_data */
           );

/**
 *  @brief This API initiates Service Search Attribute transaction with the peer SDP
 *  server with which the L2CAP channel is already established.
 *  This transaction combines the capabilities of  SDP_ServiceSearch request
 *  and  SDP_ServiceAttribute request. It utilizes the "common API handler"
 *  to service the transaction.
 *  Blocking mode blocks on the user context until the API handler returns with
 *  the response for the Service Attribute or with the Error Code, whereas in
 *  Non-Blocking mode calls the registered callback function with the out
 *  parameters passed in this API, hence the caller must preserver the memory
 *  for these parameters until it gets the callback.
 *  @param handle (IN)
 *    SDP Handle for the connection.
 *  @param uuids (IN)
 *    Pointer to list of UUIDs. If the number of UUIDs exceeds the maximum value 
 *    (SDP_CLIENT_MAX_NUM_UUIDS), SDP returns failure. The service search pattern 
 *    for the request is framed using this UUID list.
 *  @param num_uuids (IN)
 *    Indicates the number of UUIDS in the list. If this value exceeds the 
 *    maximum permitted value (SDP_CLIENT_MAX_NUM_UUIDS) sdp truncates the 
 *    additional UUIDS from the list.
 *  @param attribue_ids (IN)
 *    List of attribute IDs for which the values to be retrieved from the service
 *    record. This value must be in ascendingd order.
 *  @param num_attribute_ids (IN)
 *    Number of attribute IDs in the list.
 *  @param attribute_id_range (IN)
 *    This is a list of attribute ID range for which the values to be retrieved.
 *    This also must be in the ascending order.
 *  @param num_attribute_id_range (IN)
 *    Number of attribute ID range in the attribute Id range list.
 *  @param attribute_data (OUT)
 *    This contains the list of attribute IDs and attribute values returned for
 *    this request.
 *  @param len_attribute_data (INOUT)
 *    As input this specifies the maximum number of bytes of attribute data to 
 *    be returned in the response to this request. Output indicates the length
 *    of attribute data sequence.
 *  @return API_RESULT
 *    API_SUCCESS, or, an Error Code from BT_error.h
 */
API_RESULT BT_sdp_servicesearchattributerequest
           (
               IN    SDP_HANDLE *   /* handle */,
               IN    S_UUID *       /* uuids */,
               IN    UINT16         /* num_uuids */,
               IN    UINT16 *       /* attribute_ids */,
               IN    UINT16         /* num_attribute_ids */,
               IN    UINT32 *       /* attribute_id_range */,
               IN    UINT16         /* num_attribute_id_range */,
               OUT   UCHAR *        /* attribute_data */,
               INOUT UINT16 *       /* len_attribute_data */
           );


/* -------------- UTILITY APIS */
API_RESULT BT_sdp_get_RecordHandle
           (
               IN    UCHAR *       /* buffer */,
               IN    UINT16        /* length */,
               IN    UINT32 *      /* rec_handles */,
               INOUT UINT16 *      /* num_handles */
           );

API_RESULT BT_sdp_get_ServiceRecordHandle
           (
               IN    UCHAR * attribute_data,
               OUT   UINT32 * handle
           );

API_RESULT BT_sdp_get_ServiceClassIDList
           (
               IN    UCHAR *  attribute_data,
               OUT   S_UUID * id_list,
               INOUT UINT16 * id_list_length
           );

API_RESULT BT_sdp_get_ServiceRecordState
           (
               IN    UCHAR *  attribute_data,
               OUT   UINT32 * record_state
           );

API_RESULT BT_sdp_get_LanguageBaseAttributeIDList
           (
               IN    UCHAR *  attribute_data,
               OUT   UINT16 * lang_base_id_list,
               INOUT UINT16 * lang_count
           );


API_RESULT BT_sdp_get_ServiceID
           (
               IN    UCHAR *  attribute_data,
               OUT   S_UUID * service_id
           );

API_RESULT BT_sdp_get_BrowseGroupList
           (
               IN    UCHAR *  attribute_data,
               OUT   S_UUID * browse_list,
               INOUT UINT16 * browse_list_length
           );

API_RESULT BT_sdp_get_ServiceInfoTimeToLive
           (
               IN    UCHAR *  attribute_data,
               OUT   UINT32 * ttl
           );

API_RESULT BT_sdp_get_ServiceAvailability
           (
               IN    UCHAR * attribute_data,
               OUT   UCHAR * service_availability
           );

API_RESULT BT_sdp_get_BluetoothProfileDescriptorList
           (
               IN    UCHAR *               attribute_data,
               OUT   PROFILE_DESC_STRUCT * prof_list,
               INOUT UINT16 *              profile_count
           );

API_RESULT BT_sdp_get_DocumentationURL
           (
               IN    UCHAR * attribute_data,
               OUT   UCHAR * url,
               IN    UINT16  length
           );

API_RESULT BT_sdp_get_ClientExecutableURL
           (
               IN    UCHAR * attribute_data,
               OUT   UCHAR * url,
               IN    UINT16  length
           );

API_RESULT BT_sdp_get_IconURL
           (
               IN    UCHAR * attribute_data,
               OUT   UCHAR * url,
               IN    UINT16  length
           );

/*
 * Language base Attribute ID should required to get
 * the human readable string of that language.
 * Ex: 0x0100 for English
 */

API_RESULT BT_sdp_get_ServiceName
           (
               IN    UCHAR * attribute_data,
               IN    UINT16  lang_base_attr_id,
               OUT   UCHAR * string,
               IN    UINT16  str_len
           );

API_RESULT BT_sdp_get_ServiceDescription
           (
               IN    UCHAR * attribute_data,
               IN    UINT16  lang_base_attr_id,
               OUT   UCHAR * string,
               IN    UINT16  str_len
           );

API_RESULT BT_sdp_get_ProviderName
           (
               IN    UCHAR * attribute_data,
               IN    UINT16  lang_base_attr_id,
               OUT   UCHAR * string,
               IN    UINT16  str_len
           );

API_RESULT BT_sdp_get_VersionNumberList
           (
               IN    UCHAR *  attribute_data,
               OUT   UINT16 * version_number
           );

API_RESULT BT_sdp_get_ServiceDatabaseState
           (
               IN    UCHAR *  attribute_data,
               OUT   UINT32 * database_state
           );

API_RESULT BT_sdp_get_GroupID
           (
               IN    UCHAR *  attribute_data,
               OUT   S_UUID * group_id,
               INOUT UINT16 * group_id_length
           );

/**
 *  @brief Extract the RFCOMM Server Channel value from the Protocol Descriptor
 *  list in the attribute data
 *  @param [IN] attribute_data
 *    Attribute data
 *  @param [IN] channel_number
 *    RFCOMM Server Channel
 *  @result
 *     API_RESULT : API_SUCCESS or Error code
 */
API_RESULT BT_sdp_get_channel_number
           (
               IN    UCHAR * attribute_data,
               OUT   UCHAR * channel_number
           );

API_RESULT BT_sdp_get_additional_channel
           (
               IN    UCHAR * attribute_data,
               OUT   UCHAR * channel_number
           );

API_RESULT BT_sdp_get_psm
           (
               IN    UCHAR *  attribute_data,
               OUT   UINT16 * psm
           );

API_RESULT BT_sdp_get_additional_psm
           (
               IN    UCHAR *  attribute_data,
               OUT   UINT16 * psm
           );

API_RESULT BT_sdp_get_DataSequence_attr_val
           (
               IN    UINT16   attrib_id,
               IN    UCHAR *  attribute_data,
               OUT   UCHAR *  attribute_value,
               INOUT UINT16 * attribute_len
           );

UCHAR * BT_sdp_get_Pointer_to_next_AttributeList
        (
            IN  UCHAR * attribute_data,
            IN  UCHAR   record_no
        );

#define BT_sdp_get_attr_value BT_sdp_get_DataSequence_attr_val

/* APIs to convert 2/4 bit UUID to 128 bit & 2 bit UUID to 4 bit */
void BT_uuid_32_to_128
     (
         IN   UINT32       source_uuid,
         OUT  UINT128_ID * dest_uuid
     );

void BT_uuid_16_to_32
     (
         IN  UINT16   source_uuid,
         OUT UINT32 * dest_uuid
     );



#ifdef SDP_TEST_MODE
API_RESULT BT_sdp_invalid_servicesearchattribute
           (
               SDP_HANDLE *    handle,
               UCHAR *         attribute_data,
               UINT16 *        len_attribute_data
           );

API_RESULT BT_sdp_invalid_serviceattribute
           (
               SDP_HANDLE *    handle,
               UINT32          record_handle,
               UCHAR *         attribute_data,
               UINT16 *        len_attribute_data
           );
#endif /* SDP_TEST_MODE */

/* ---------------- SDP Server Callback Registration API */
#ifdef SDP_SERVER_HAVE_CB_IND_SUPPORT
API_RESULT BT_sdp_server_register_ind_cb
           (
               API_RESULT (* callback_ptr)
                          (
                              UCHAR    event_type,
                              UCHAR *  event_data,
                              UCHAR    event_datalen
                          )
           );
#endif /* SDP_SERVER_HAVE_CB_IND_SUPPORT */

#ifdef __cplusplus
};
#endif  /* __cplusplus */

#endif  /* _H_BT_SDP_API_ */


