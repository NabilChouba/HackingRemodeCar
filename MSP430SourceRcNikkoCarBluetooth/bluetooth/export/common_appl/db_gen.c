
/**
 *  Copyright (C) 2000-2010. MindTree Ltd. All rights reserved.
 *  \file db_gen.c
 */

/* Header File Inclusion */
#include "db_gen.h"

#ifdef SDP_SERVER

/* --------------------------------------------- Global Variables */

/* Static SDP database for memory constrained implementations */
SDP_RECORD dbase[DB_MAX_RECORDS];

/* Total number of records in the database */
UINT16 num_records;

/* Total number of UUIDS stored in the global uuid array */
UINT16 num_128_ids;

/**
 *   Record 00  : SDP Record
 *   Record 01  : SPP
 */

/**
 *   Data Size Calculation (For ARM Processor):
 *
 *   Number of SDP records (n_rec) = 2
 *   Number of attributes (n_attr) = 9
 *   UUID List Length (uuid_list_len) = 5
 *   Number of UUIDS (n_uuid) = 5
 *   Total attribute value length (attr_len) = 68 Bytes
 *   Other Global variables (other): Two UINT16 type variable = 4 Bytes
 *
 *   Read Only (RO) data = uuid_list_len + (n_uuid * 16)
 *                       = 85 (0x55) Bytes
 *
 *   Read Write (RW) data = (n_attr * 8) + (attr_len)
 *                        = 140 (0x8C) Bytes
 *
 *   Zero Initialized (ZI) data = (n_rec * 12) + (other)
 *                              = 28 (0x1C) Bytes
 */

/* UUID indices array */
const UCHAR uuid_indices_arr[] = {

                                /** Service Record 0 */
    2,

                                /** Service Record 1 */
    0, 1, 3, 4
};

#ifdef BT_EIR
/* Service Class UUID indices array */
const UCHAR service_class_uuid_indices_arr[SERVICE_CLASS_UUID_INDICES_MAX] = {
    2, 4
};
#endif /* BT_EIR */

/* Attribute value array */
const UCHAR value_arr[] = {

    /** Service Record 0 */

    /* ServiceRecordHandle */
    0x0A, 0x00, 0x00, 0x00, 0x00,

    /* ServiceClassIDList */
    0x35, 0x03, 0x19, 0x10, 0x00,

    /* VersionNumberList */
    0x35, 0x03, 0x09, 0x01, 0x00,

    /** Service Record 1 */

    /* ServiceRecordHandle */
    0x0A, 0x00, 0x09, 0x00, 0x01,

    /* ServiceClassIDList */
    0x35, 0x03, 0x19, 0x11, 0x01,

    /* ProtocolDescriptorList */
    0x35, 0x0C, 0x35, 0x03, 0x19, 0x01, 0x00, 0x35, 0x05, 0x19,
    0x00, 0x03, 0x08, 0x01,

    /* BrowseGroupList */
    0x35, 0x03, 0x19, 0x10, 0x02,

    /* LanguageBaseAttributeIDList */
    0x35, 0x09, 0x09, 0x65, 0x6E, 0x09, 0x00, 0x6A, 0x09, 0x01,
    0x00,

    /* ServiceName */
    0x25, 0x0B, 0x42, 0x6C, 0x75, 0x65, 0x4D, 0x53, 0x50, 0x34,
    0x33, 0x30, 0x00
};

/* Attribute structure array */
const struct attr attr_arr[] = {

    /** Service Record 0 */

    /* ServiceRecordHandle */
    {0x0000, 0x0005, value_arr + 0x0000},

    /* ServiceClassIDList */
    {0x0001, 0x0005, value_arr + 0x0005},

    /* VersionNumberList */
    {0x0200, 0x0005, value_arr + 0x000A},

    /** Service Record 1 */
    /* ServiceRecordHandle */
    {0x0000, 0x0005, value_arr + 0x000F},

    /* ServiceClassIDList */
    {0x0001, 0x0005, value_arr + 0x0014},

    /* ProtocolDescriptorList */
    {0x0004, 0x000E, value_arr + 0x0019},

    /* BrowseGroupList */
    {0x0005, 0x0005, value_arr + 0x0027},

    /* LanguageBaseAttributeIDList */
    {0x0006, 0x000B, value_arr + 0x002C},

    /* ServiceName */
    {0x0100, 0x000D, value_arr + 0x0037}
};

/* Functions */

/**
 *  \fn BT_dbase_init
 *
 *  \brief Ethermind SDP dbase initializer
 *
 *  \par Description:
 *  This function initializes SDP dbase records statically.
 *  It is called as a part of EtherMind stack initialization.
 *
 *  \param None
 *
 *  \return None
 *
 */

void BT_dbase_init(void)
{
#ifdef BT_LOG_FIXED_DATA_SIZE
    {
        UINT32 total_size = 0;

        total_size += sizeof(dbase);
        total_size += sizeof(num_records);
        total_size += sizeof(num_128_ids);

#ifdef BT_EIR
        total_size += sizeof(service_class_uuid_indices_arr);
#endif /* BT_EIR */

        total_size += sizeof(value_arr);
        total_size += sizeof(attr_arr);

        BT_debug_trace(bt_debug_fd, "[DB GEN] [SIZE] 0x%08X (%d)\n", total_size,
                       total_size);
    }
#endif /* BT_LOG_FIXED_DATA_SIZE */

    /* 
     *    SDP Record
     *       Of
     *   SDP Server
     */
    dbase[0].rec_handle = 0x00000000;
    dbase[0].active = DB_RECORD_ACTIVE;
    dbase[0].num_uuids = 1;
    dbase[0].num_attrs = 3;
    dbase[0].attr_offset = 0;


    /* 
     *     SDP Record
     *        Of
     *  SPP Profile
     */
    dbase[1].rec_handle = 0x00090001;
    dbase[1].active = DB_RECORD_INACTIVE;
    dbase[1].num_uuids = 4;
    dbase[1].num_attrs = 6;
    dbase[1].attr_offset = 3;

    /* Number of SDP records */
    num_records = 2;

    /* UUID List */
    num_128_ids = 5;

}



/** List of all required UUIDs */
const UCHAR uuid_128_arr[DB_MAX_UUIDS][16] = {

    /**
     *  UUID Index 00 :
     *  ( 0x0003 ) -> RFCOMM
     */
    {0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x10, 0x00,
     0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB},

    /**
     *  UUID Index 01 :
     *  ( 0x0100 ) -> L2CAP
     */
    {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00,
     0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB},

    /**
     *  UUID Index 02 :
     *  ( 0x1000 ) -> ServiceDiscoveryServerServiceClassID
     */
    {0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00,
     0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB},

    /**
     *  UUID Index 03 :
     *  ( 0x1002 ) -> PublicBrowseGroup
     */
    {0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x10, 0x00,
     0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB},

    /**
     *  UUID Index 04 :
     *  ( 0x1101 ) -> SerialPort
     */
    {0x00, 0x00, 0x11, 0x01, 0x00, 0x00, 0x10, 0x00,
     0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB}
};



/* 
 *  Function Name:
 *    BT_dbase_get_record_handle
 *
 *  Description:
 *      This function returns the record handle in the database
 *      that is associated with a particular service type and its instance.
 *
 *  Input Parameters:
 *      UCHAR service_type      : Identifies the service type.
 *      UCHAR service_instance  : Identifies the instance of
 *                                a particular service type.
 *      UINT32 * rec_hdl        : Associated record handle returned by this API
 *
 *  Output Parameters:
 *       API_RESULT : API_SUCCESS in case of success, otherwise API_FAILURE
 */
API_RESULT BT_dbase_get_record_handle(IN UCHAR service_type,
                                      IN UCHAR service_instance,
                                      OUT UINT32 * rec_hdl)
{
    if (NULL == rec_hdl) {
        return API_FAILURE;
    }

    *rec_hdl = 0xFFFFFFFF;

    switch (service_type) {
    case DB_RECORD_SDP:
    {
        *rec_hdl = 0x00000000;
    }
        break;

    case DB_RECORD_SPP:
    {
        *rec_hdl = 0x00090001;
    }
        break;

    }

    return API_SUCCESS;
}

#endif /* SDP_SERVER */
