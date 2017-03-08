
/**
 *  @file BT_hci_api.h
 *
 *  This Header File contains the API Function declarations of the APIs
 *  exported by the MindTree Bluetooth Stack for the HCI Layer.
 */

 /* 
  *  Copyright (C) 2000-2010. MindTree Ltd.
  *  All rights reserved.
  */

#ifndef _H_BT_HCI_API_
#define _H_BT_HCI_API_

/* =================================================== Header File Inclusion */
#include "BT_common.h"


/* =================================================== Constants/Definitions */

#ifdef HCI_HAVE_INIT_COMMAND_MASK

/** Bitmap values for HCI BT-Init Command Mask */
#define HCI_INIT_COMMAND_MASK_RESET             0x00000001
#define HCI_INIT_COMMAND_MASK_SSP               0x00000002

/** Maximum & Default Bitmap values for HCI BT-Init Command Mask */
#define HCI_INIT_COMMAND_MASK_VALUE_DEFAULT     0x00000000
#define HCI_INIT_COMMAND_MASK_VALUE_MAX         0x00000002

#endif /* HCI_HAVE_INIT_COMMAND_MASK */

/* HCI Device Role - Master/Slave */
#define BT_DEVICE_ROLE_MASTER                   0x00
#define BT_DEVICE_ROLE_SLAVE                    0x01

/* HCI Link Types ACL/SCO/eSCO */
#define HCI_SCO_LINK                            0x00
#define HCI_ACL_LINK                            0x01
#ifdef BT_HCI_1_2
#define HCI_ESCO_LINK                           0x02
#endif /* BT_HCI_1_2 */

/* LMP Packet Types - ACL Connection */
#define LMP_ACL_DM1                             0x0008
#define LMP_ACL_DH1                             0x0010
#define LMP_ACL_DM3                             0x0400
#define LMP_ACL_DH3                             0x0800
#define LMP_ACL_DM5                             0x4000
#define LMP_ACL_DH5                             0x8000

#ifdef BT_2_0_EDR
#define LMP_ACL_2_DH1                           0x0002
#define LMP_ACL_3_DH1                           0x0004
#define LMP_ACL_2_DH3                           0x0100
#define LMP_ACL_3_DH3                           0x0200
#define LMP_ACL_2_DH5                           0x1000
#define LMP_ACL_3_DH5                           0x2000
#endif /* BT_2_0_EDR */

/* LMP Packet Types - SCO Connection */
#define LMP_SCO_HV1                             0x0020
#define LMP_SCO_HV2                             0x0040
#define LMP_SCO_HV3                             0x0080

/* LMP Packet Types - eSCO Connection */
#define LMP_ESCO_HV1                            0x0001
#define LMP_ESCO_HV2                            0x0002
#define LMP_ESCO_HV3                            0x0004
#define LMP_ESCO_EV3                            0x0008
#define LMP_ESCO_EV4                            0x0010
#define LMP_ESCO_EV5                            0x0020

#ifdef BT_2_0_EDR
#define LMP_ESCO_2_EV3                          0x0040
#define LMP_ESCO_3_EV3                          0x0080
#define LMP_ESCO_2_EV5                          0x0100
#define LMP_ESCO_3_EV5                          0x0200
#endif /* BT_2_0_EDR */

/* Synchronous Connections Parameters - Max Latency */
#define LMP_MAX_LATENCY_DONT_CARE               0xFFFF

/* Synchronous Connections Parameters - Retransmission Effort */
#define LMP_RETX_EFFORT_NONE                    0x00
#define LMP_RETX_EFFORT_POWER                   0x01
#define LMP_RETX_EFFORT_QUALITY                 0x02
#define LMP_RETX_EFFORT_DONT_CARE               0xFF

/* Synchronous Connections Parameters - Voice Settings */
/* Input Coding */
#define LMP_VOICE_INPUT_CODING_LINEAR           0x0000
#define LMP_VOICE_INPUT_CODING_U_LAW            0x0100
#define LMP_VOICE_INPUT_CODING_A_LAW            0x0200

/* Input Data Format */
#define LMP_VOICE_INPUT_FORMAT_1S_COMPLEMENT    0x0000
#define LMP_VOICE_INPUT_FORMAT_2S_COMPLEMENT    0x0040
#define LMP_VOICE_INPUT_FORMAT_SIGN_MAGNITUDE   0x0080
#define LMP_VOICE_INPUT_FORMAT_UNSIGNED         0x00C0

/* Input Sample Size */
#define LMP_VOICE_INPUT_SAMPLE_8_BIT            0x0000
#define LMP_VOICE_INPUT_SAMPLE_16_BIT           0x0020

/* Linear PCM Bit Position */
#define LMP_VOICE_LINEAR_BIT_POS_0              0x0000
#define LMP_VOICE_LINEAR_BIT_POS_1              0x0004
#define LMP_VOICE_LINEAR_BIT_POS_2              0x0008
#define LMP_VOICE_LINEAR_BIT_POS_3              0x000C
#define LMP_VOICE_LINEAR_BIT_POS_4              0x0010
#define LMP_VOICE_LINEAR_BIT_POS_5              0x0014
#define LMP_VOICE_LINEAR_BIT_POS_6              0x0018
#define LMP_VOICE_LINEAR_BIT_POS_7              0x001C

/* Air Coding Format */
#define LMP_VOICE_AIR_CODING_CVSD               0x0000
#define LMP_VOICE_AIR_CODING_U_LAW              0x0001
#define LMP_VOICE_AIR_CODING_A_LAW              0x0002
#define LMP_VOICE_AIR_CODING_TRANSPARENT        0x0003

/* 
 *  Default Voice Settings to be used for Synchronous Connections:
 *  Input Coding: A-Law
 *  Input Format: 2's Compliment
 *  Input Sample Size: 8-bit
 *  Linear PCM Bit Position: 3
 *  Air Coding Format: CVSD
 */
#define LMP_VOICE_SETTING_DEFAULT \
        (LMP_VOICE_INPUT_CODING_LINEAR|\
         LMP_VOICE_INPUT_FORMAT_2S_COMPLEMENT|\
         LMP_VOICE_INPUT_SAMPLE_16_BIT|\
         LMP_VOICE_LINEAR_BIT_POS_0|\
         LMP_VOICE_AIR_CODING_CVSD)

/* Default eSCO Channel Parameters */
/* Transmit & Receive Bandwidth - 8000 octets/sec */
#define LMP_ESCO_TX_BANDWIDTH_DEFAULT           8000
#define LMP_ESCO_RX_BANDWIDTH_DEFAULT           8000

/* Default eSCO Max Latency */
#define LMP_ESCO_MAX_LATENCY_DEFAULT            LMP_MAX_LATENCY_DONT_CARE

/* Default eSCO Retransmission Effort */
#define LMP_ESCO_RETX_EFFORT_DEFAULT            LMP_RETX_EFFORT_DONT_CARE

/* Default eSCO Packet Type */
#define LMP_ESCO_PACKET_TYPE_DEFAULT \
        (LMP_ESCO_HV1|LMP_ESCO_HV2|LMP_ESCO_HV3|\
         LMP_ESCO_EV3|LMP_ESCO_EV4|LMP_ESCO_EV5)

/* Type of PIN Codes */
#define HCI_VARIABLE_PIN_CODE                   0x00
#define HCI_FIXED_PIN_CODE                      0x01

/* Invalid HCI Connection Handle */
#define HCI_INVALID_CONNECTION_HANDLE           0xFFFF

/* Type of Link Keys */
#define HCI_LINK_KEY_COMBINATION                0x00
#define HCI_LINK_KEY_LOCAL_UNIT                 0x01
#define HCI_LINK_KEY_REMOTE_UNIT                0x02
#define HCI_LINK_KEY_DEBUG_COMBINATION          0x03
#define HCI_LINK_KEY_UNAUTHENTICATED            0x04
#define HCI_LINK_KEY_AUTHENTICATED              0x05
#define HCI_LINK_KEY_CHANGED_COMBINATION        0x06

#ifdef BT_EIR
#define HCI_EIR_MAX_DATA_LEN                    240
#endif /* BT_EIR */



/* +++ HCI Command OpCodes ++++++++++++++++++++++++++++++++++++++++ */
/* No Operation Opcode */
#define HCI_NO_OPERATION_OPCODE                                 0x0000

/* Link Control Commands - OGF : 0x01 */
#define HCI_INQUIRY_OPCODE                                      0x0401
#define HCI_INQUIRY_CANCEL_OPCODE                               0x0402
#define HCI_PERIODIC_INQUIRY_MODE_OPCODE                        0x0403
#define HCI_EXIT_PERIODIC_INQUIRY_MODE_OPCODE                   0x0404
#define HCI_CREATE_CONNECTION_OPCODE                            0x0405
#define HCI_DISCONNECT_OPCODE                                   0x0406
#define HCI_ADD_SCO_CONNECTION_OPCODE                           0x0407
#define HCI_ACCEPT_CONNECTION_REQUEST_OPCODE                    0x0409
#define HCI_REJECT_CONNECTION_REQUEST_OPCODE                    0x040A
#define HCI_LINK_KEY_REQUEST_REPLY_OPCODE                       0x040B
#define HCI_LINK_KEY_REQUEST_NEGATIVE_REPLY_OPCODE              0x040C
#define HCI_PIN_CODE_REQUEST_REPLY_OPCODE                       0x040D
#define HCI_PIN_CODE_REQUEST_NEGATIVE_REPLY_OPCODE              0x040E
#define HCI_CHANGE_CONNECTION_PACKET_TYPE_OPCODE                0x040F
#define HCI_AUTHENTICATION_REQUESTED_OPCODE                     0x0411
#define HCI_SET_CONNECTION_ENCRYPTION_OPCODE                    0x0413
#define HCI_CHANGE_CONNECTION_LINK_KEY_OPCODE                   0x0415
#define HCI_MASTER_LINK_KEY_OPCODE                              0x0417
#define HCI_REMOTE_NAME_REQUEST_OPCODE                          0x0419
#define HCI_READ_REMOTE_SUPPORTED_FEATURES_OPCODE               0x041B
#define HCI_READ_REMOTE_VERSION_INFORMATION_OPCODE              0x041D
#define HCI_READ_CLOCK_OFFSET_OPCODE                            0x041F

#ifdef BT_HCI_1_2
#define HCI_CREATE_CONNECTION_CANCEL_OPCODE                     0x0408
#define HCI_REMOTE_NAME_REQUEST_CANCEL_OPCODE                   0x041A
#define HCI_READ_REMOTE_EXTENDED_FEATURES_OPCODE                0x041C
#define HCI_SETUP_SYNCHRONOUS_CONNECTION_OPCODE                 0x0428
#define HCI_ACCEPT_SYNCHRONOUS_CONNECTION_REQUEST_OPCODE        0x0429
#define HCI_REJECT_SYNCHRONOUS_CONNECTION_REQUEST_OPCODE        0x042A
#endif /* BT_HCI_1_2 */

#ifdef BT_SSP
#define HCI_IO_CAP_REQUEST_REPLY_OPCODE                         0x042B
#define HCI_IO_CAP_REQUEST_NEGATIVE_REPLY_OPCODE                0x0434
#endif /* BT_SSP */

#ifdef BT_SSP_UC
#define HCI_USER_CONF_REQUEST_REPLY_OPCODE                      0x042C
#define HCI_USER_CONF_REQUEST_NEGATIVE_REPLY_OPCODE             0x042D
#endif /* BT_SSP_UC */

#ifdef BT_SSP_PE
#define HCI_USER_PASSKEY_REQUEST_REPLY_OPCODE                   0x042E
#define HCI_USER_PASSKEY_REQUEST_NEGATIVE_REPLY_OPCODE          0x042F
#endif /* BT_SSP_PE */

#ifdef BT_SSP_OOB
#define HCI_REMOTE_OOB_DATA_REQUEST_REPLY_OPCODE                0x0430
#define HCI_REMOTE_OOB_DATA_REQUEST_NEGATIVE_REPLY_OPCODE       0x0431
#endif /* BT_SSP_OOB */

/* Link Policy Commands - OGF : 0x02 */
#define HCI_HOLD_MODE_OPCODE                                    0x0801
#define HCI_SNIFF_MODE_OPCODE                                   0x0803
#define HCI_EXIT_SNIFF_MODE_OPCODE                              0x0804
#define HCI_PARK_MODE_OPCODE                                    0x0805
#define HCI_EXIT_PARK_MODE_OPCODE                               0x0806
#define HCI_QOS_SETUP_OPCODE                                    0x0807
#define HCI_ROLE_DISCOVERY_OPCODE                               0x0809
#define HCI_SWITCH_ROLE_OPCODE                                  0x080B
#define HCI_READ_LINK_POLICY_SETTINGS_OPCODE                    0x080C
#define HCI_WRITE_LINK_POLICY_SETTINGS_OPCODE                   0x080D

#ifdef BT_HCI_1_2
#define HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_OPCODE            0x080E
#define HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_OPCODE           0x080F
#define HCI_FLOW_SPECIFICATION_OPCODE                           0x0810
#endif /* BT_HCI_1_2 */

#ifdef BT_SSR
#define HCI_SNIFF_SUBRATING_OPCODE                              0x0811
#endif /* BT_SSR */

/* Host Controller & BaseBand Commands - OGF : 0x03 */
#define HCI_SET_EVENT_MASK_OPCODE                               0x0C01
#define HCI_RESET_OPCODE                                        0x0C03
#define HCI_SET_EVENT_FILTER_OPCODE                             0x0C05
#define HCI_FLUSH_OPCODE                                        0x0C08
#define HCI_READ_PIN_TYPE_OPCODE                                0x0C09
#define HCI_WRITE_PIN_TYPE_OPCODE                               0x0C0A
#define HCI_CREATE_NEW_UNIT_KEY_OPCODE                          0x0C0B
#define HCI_CHANGE_LOCAL_NAME_OPCODE                            0x0C13
#define HCI_READ_LOCAL_NAME_OPCODE                              0x0C14
#define HCI_READ_CONNECTION_ACCEPT_TIMEOUT_OPCODE               0x0C15
#define HCI_WRITE_CONNECTION_ACCEPT_TIMEOUT_OPCODE              0x0C16
#define HCI_READ_PAGE_TIMEOUT_OPCODE                            0x0C17
#define HCI_WRITE_PAGE_TIMEOUT_OPCODE                           0x0C18
#define HCI_READ_SCAN_ENABLE_OPCODE                             0x0C19
#define HCI_WRITE_SCAN_ENABLE_OPCODE                            0x0C1A
#define HCI_READ_PAGE_SCAN_ACTIVITY_OPCODE                      0x0C1B
#define HCI_WRITE_PAGE_SCAN_ACTIVITY_OPCODE                     0x0C1C
#define HCI_READ_INQUIRY_SCAN_ACTIVITY_OPCODE                   0x0C1D
#define HCI_WRITE_INQUIRY_SCAN_ACTIVITY_OPCODE                  0x0C1E
#define HCI_READ_AUTHENTICATION_ENABLE_OPCODE                   0x0C1F
#define HCI_WRITE_AUTHENTICATION_ENABLE_OPCODE                  0x0C20
#define HCI_READ_ENCRYPTION_MODE_OPCODE                         0x0C21
#define HCI_WRITE_ENCRYPTION_MODE_OPCODE                        0x0C22
#define HCI_READ_CLASS_OF_DEVICE_OPCODE                         0x0C23
#define HCI_WRITE_CLASS_OF_DEVICE_OPCODE                        0x0C24
#define HCI_READ_VOICE_SETTING_OPCODE                           0x0C25
#define HCI_WRITE_VOICE_SETTING_OPCODE                          0x0C26
#define HCI_READ_AUTOMATIC_FLUSH_TIMEOUT_OPCODE                 0x0C27
#define HCI_WRITE_AUTOMATIC_FLUSH_TIMEOUT_OPCODE                0x0C28
#define HCI_READ_NUM_BROADCAST_RETRANSMISSIONS_OPCODE           0x0C29
#define HCI_WRITE_NUM_BROADCAST_RETRANSMISSIONS_OPCODE          0x0C2A
#define HCI_READ_HOLD_MODE_ACTIVITY_OPCODE                      0x0C2B
#define HCI_WRITE_HOLD_MODE_ACTIVITY_OPCODE                     0x0C2C
#define HCI_READ_TRANSMIT_POWER_LEVEL_OPCODE                    0x0C2D
#define HCI_READ_SCO_FLOW_CONTROL_ENABLE_OPCODE                 0x0C2E
#define HCI_WRITE_SCO_FLOW_CONTROL_ENABLE_OPCODE                0x0C2F
#define HCI_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL_OPCODE     0x0C31
#define HCI_HOST_BUFFER_SIZE_OPCODE                             0x0C33
#define HCI_HOST_NUMBER_OF_COMPLETED_PACKETS_OPCODE             0x0C35
#define HCI_READ_LINK_SUPERVISION_TIMEOUT_OPCODE                0x0C36
#define HCI_WRITE_LINK_SUPERVISION_TIMEOUT_OPCODE               0x0C37
#define HCI_READ_NUMBER_OF_SUPPORTED_IAC_OPCODE                 0x0C38
#define HCI_READ_CURRENT_IAC_LAP_OPCODE                         0x0C39
#define HCI_WRITE_CURRENT_IAC_LAP_OPCODE                        0x0C3A
#define HCI_READ_PAGE_SCAN_PERIOD_MODE_OPCODE                   0x0C3B
#define HCI_WRITE_PAGE_SCAN_PERIOD_MODE_OPCODE                  0x0C3C
#define HCI_READ_PAGE_SCAN_MODE_OPCODE                          0x0C3D
#define HCI_WRITE_PAGE_SCAN_MODE_OPCODE                         0x0C3E

#ifdef BT_HCI_1_2
#define HCI_SET_AFH_HOST_CHANNEL_CLASSIFICATION_OPCODE          0x0C3F
#define HCI_READ_INQUIRY_SCAN_TYPE_OPCODE                       0x0C42
#define HCI_WRITE_INQUIRY_SCAN_TYPE_OPCODE                      0x0C43
#define HCI_READ_INQUIRY_MODE_OPCODE                            0x0C44
#define HCI_WRITE_INQUIRY_MODE_OPCODE                           0x0C45
#define HCI_READ_PAGE_SCAN_TYPE_OPCODE                          0x0C46
#define HCI_WRITE_PAGE_SCAN_TYPE_OPCODE                         0x0C47
#define HCI_READ_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE             0x0C48
#define HCI_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE            0x0C49
#endif /* BT_HCI_1_2 */

#ifdef BT_EIR
#define HCI_READ_EXTENDED_INQUIRY_RESPONSE_OPCODE               0x0C51
#define HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_OPCODE              0x0C52
#define HCI_READ_INQUIRY_RESP_TX_POWER_LEVEL_OPCODE             0x0C58
#define HCI_WRITE_INQUIRY_RESP_TX_POWER_LEVEL_OPCODE            0x0C59
#endif /* BT_EIR */

#ifdef BT_EPR
#define HCI_REFRESH_ENCRYPTION_KEY_OPCODE                       0x0C53
#endif /* BT_EPR */

#ifdef BT_EDR
#define HCI_READ_DEFAULT_ERR_DATA_REPORTING_OPCODE              0x0C5A
#define HCI_WRITE_DEFAULT_ERR_DATA_REPORTING_OPCODE             0x0C5B
#endif /* BT_EDR */

#ifdef BT_SSP
#define HCI_READ_SIMPLE_PAIRING_MODE_OPCODE                     0x0C55
#define HCI_WRITE_SIMPLE_PAIRING_MODE_OPCODE                    0x0C56
#endif /* BT_SSP */

#ifdef BT_SSP_PE
#define HCI_SEND_KEY_PRESS_NOTIFICATION_OPCODE                  0x0C60
#endif /* BT_SSP_PE */

#ifdef BT_SSP_OOB
#define HCI_READ_LOCAL_OOB_DATA_OPCODE                          0x0C57
#endif /* BT_SSP_OOB */

#ifdef BT_ENHANCED_FLUSH
#define HCI_ENHANCED_FLUSH_OPCODE                               0x0C5F
#endif /* BT_ENHANCED_FLUSH */

/* Informational Parameters - OGF : 0x04 */
#define HCI_READ_LOCAL_VERSION_INFORMATION_OPCODE               0x1001
#define HCI_READ_LOCAL_SUPPORTED_FEATURES_OPCODE                0x1003
#define HCI_READ_BUFFER_SIZE_OPCODE                             0x1005
#define HCI_READ_COUNTRY_CODE_OPCODE                            0x1007
#define HCI_READ_BD_ADDR_OPCODE                                 0x1009

#ifdef BT_HCI_1_2
#define HCI_READ_LOCAL_SUPPORTED_COMMANDS_OPCODE                0x1002
#define HCI_READ_LOCAL_EXTENDED_FEATURES_OPCODE                 0x1004
#endif /* BT_HCI_1_2 */

/* Status Parameters - OGF : 0x05 */
#define HCI_READ_FAILED_CONTACT_COUNTER_OPCODE                  0x1401
#define HCI_RESET_FAILED_CONTACT_COUNTER_OPCODE                 0x1402
#define HCI_GET_LINK_QUALITY_OPCODE                             0x1403
#define HCI_READ_RSSI_OPCODE                                    0x1405

#ifdef BT_HCI_1_2
#define HCI_READ_AFH_CHANNEL_MAP_OPCODE                         0x1406
#define HCI_READ_CLOCK_OPCODE                                   0x1407
#endif /* BT_HCI_1_2 */

/* Testing Commands - OGF : 0x06 */
#ifdef HCI_TESTING_COMMANDS
#define HCI_READ_LOOPBACK_MODE_OPCODE                           0x1801
#define HCI_WRITE_LOOPBACK_MODE_OPCODE                          0x1802
#define HCI_ENABLE_DEVICE_UNDER_TEST_MODE_OPCODE                0x1803
#endif /* HCI_TESTING_COMMANDS */

#ifdef BT_SSP_DEBUG
#define HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE_OPCODE              0x1804
#endif /* BT_SSP_DEBUG */

/* Vendor Specific Parameters - OGF : 0x3F */


/* +++ HCI Event Codes ++++++++++++++++++++++++++++++++++++++++++++ */
#define HCI_INQUIRY_COMPLETE_EVENT                              0x01
#define HCI_INQUIRY_RESULT_EVENT                                0x02
#define HCI_CONNECTION_COMPLETE_EVENT                           0x03
#define HCI_CONNECTION_REQUEST_EVENT                            0x04
#define HCI_DISCONNECTION_COMPLETE_EVENT                        0x05
#define HCI_AUTHENTICATION_COMPLETE_EVENT                       0x06
#define HCI_REMOTE_NAME_REQUEST_COMPLETE_EVENT                  0x07
#define HCI_ENCRYPTION_CHANGE_EVENT                             0x08
#define HCI_CHANGE_CONNECTION_LINK_KEY_COMPLETE_EVENT           0x09
#define HCI_MASTER_LINK_KEY_COMPLETE_EVENT                      0x0A
#define HCI_READ_REMOTE_SUPPORTED_FEATURES_COMPLETE_EVENT       0x0B
#define HCI_READ_REMOTE_VERSION_INFORMATION_COMPLETE_EVENT      0x0C
#define HCI_QOS_SETUP_COMPLETE_EVENT                            0x0D
#define HCI_COMMAND_COMPLETE_EVENT                              0x0E
#define HCI_COMMAND_STATUS_EVENT                                0x0F
#define HCI_HARDWARE_ERROR_EVENT                                0x10
#define HCI_FLUSH_OCCURRED_EVENT                                0x11
#define HCI_ROLE_CHANGE_EVENT                                   0x12
#define HCI_NUMBER_OF_COMPLETED_PACKETS_EVENT                   0x13
#define HCI_MODE_CHANGE_EVENT                                   0x14
#define HCI_RETURN_LINK_KEYS_EVENT                              0x15
#define HCI_PIN_CODE_REQUEST_EVENT                              0x16
#define HCI_LINK_KEY_REQUEST_EVENT                              0x17
#define HCI_LINK_KEY_NOTIFICATION_EVENT                         0x18
#define HCI_LOOPBACK_COMMAND_EVENT                              0x19
#define HCI_DATA_BUFFER_OVERFLOW_EVENT                          0x1A
#define HCI_MAX_SLOTS_CHANGE_EVENT                              0x1B
#define HCI_READ_CLOCK_OFFSET_COMPLETE_EVENT                    0x1C
#define HCI_CONNECTION_PACKET_TYPE_CHANGED_EVENT                0x1D
#define HCI_QOS_VIOLATION_EVENT                                 0x1E
#define HCI_PAGE_SCAN_MODE_CHANGE_EVENT                         0x1F
#define HCI_PAGE_SCAN_REPETITION_MODE_CHANGE_EVENT              0x20

#ifdef BT_HCI_1_2
#define HCI_FLOW_SPECIFICATION_COMPLETE_EVENT                   0x21
#define HCI_INQUIRY_RESULT_WITH_RSSI_EVENT                      0x22
#define HCI_REMOTE_EXTENDED_FEATURES_COMPLETE_EVENT             0x23
#define HCI_SYNCHRONOUS_CONNECTION_COMPLETE_EVENT               0x2C
#define HCI_SYNCHRONOUS_CONNECTION_CHANGED_EVENT                0x2D
#endif /* BT_HCI_1_2 */

#ifdef BT_2_1_EDR
#define HCI_SNIFF_SUBRATING_EVENT                               0x2E
#define HCI_EXTENDED_INQUIRY_RESULT_EVENT                       0x2F
#define HCI_ENCRYPTION_KEY_REFRESH_COMPLETE_EVENT               0x30
#define HCI_IO_CAPABILITY_REQUEST_EVENT                         0x31
#define HCI_IO_CAPABILITY_RESPONSE_EVENT                        0x32
#define HCI_USER_CONFIRMATION_REQUEST_EVENT                     0x33
#define HCI_USER_PASSKEY_REQUEST_EVENT                          0x34
#define HCI_REMOTE_OOB_DATA_REQUEST_EVENT                       0x35
#define HCI_SIMPLE_PAIRING_COMPLETE_EVENT                       0x36
#define HCI_LINK_SUPERVISION_TIMEOUT_CHANGED_EVENT              0x38
#define HCI_ENHANCED_FLUSH_COMPLETE_EVENT                       0x39
#define HCI_USER_PASSKEY_NOTIFICATION_EVENT                     0x3B
#define HCI_KEY_PRESS_NOTIFICATION_EVENT                        0x3C
#define HCI_REMOTE_HOST_SUPPORTED_FEATURES_NOTIFICATION_EVENT   0x3D
#endif /* BT_2_1_EDR */

/* HCI Vendor Specific Debug Event */
#define HCI_VENDOR_SPECIFIC_DEBUG_EVENT                         0xFF

/* 
 * Defines for Link status (current mode) returned by
 * BT_hci_get_connection_details()
 */
#define BT_HCI_ACTIVE_MODE           0x00
#define BT_HCI_HOLD_MODE             0x01
#define BT_HCI_SNIFF_MODE            0x02
#define BT_HCI_PARK_MODE             0x03


/* =================================================== Macro's */
#define hci_pack_1_byte_param(dest, src) \
    *((dest) + 0) = (UCHAR)(*((UCHAR *)(src)));

#define hci_pack_2_byte_param(dest, src) \
    *((dest) + 0) = (UCHAR)(*((UINT16 *)(src))); \
    *((dest) + 1) = (UCHAR)(*((UINT16 *)(src)) >> 8);

#define hci_pack_3_byte_param(dest, src) \
    *((dest) + 0) = (UCHAR)(*((UINT32 *)(src)));\
    *((dest) + 1) = (UCHAR)(*((UINT32 *)(src)) >> 8);\
    *((dest) + 2) = (UCHAR)(*((UINT32 *)(src)) >> 16);

#define hci_pack_4_byte_param(dest, src) \
    *((dest) + 0) = (UCHAR)(*((UINT32 *)(src)));\
    *((dest) + 1) = (UCHAR)(*((UINT32 *)(src)) >> 8);\
    *((dest) + 2) = (UCHAR)(*((UINT32 *)(src)) >> 16);\
    *((dest) + 3) = (UCHAR)(*((UINT32 *)(src)) >> 24);

#define hci_unpack_1_byte_param(dest, src) \
    *((UCHAR *)(dest)) = *(src);

#define hci_unpack_2_byte_param(dest, src) \
    *((UINT16 *)(dest))  = *((src) + 1); \
    *((UINT16 *)(dest))  = *((UINT16 *)(dest)) << 8; \
    *((UINT16 *)(dest)) |= *((src) + 0);

#define hci_unpack_3_byte_param(dest, src) \
    *((UINT32 *)(dest))  = *((src) + 2); \
    *((UINT32 *)(dest))  = (*((UINT32 *)(dest))) << 8;\
    *((UINT32 *)(dest)) |= *((src) + 1);\
    *((UINT32 *)(dest))  = (*((UINT32 *)(dest))) << 8;\
    *((UINT32 *)(dest)) |= *((src) + 0);

#define hci_unpack_4_byte_param(dest, src) \
    *((UINT32 *)(dest))  = *((src) + 3);\
    *((UINT32 *)(dest))  = (*((UINT32 *)(dest))) << 8;\
    *((UINT32 *)(dest)) |= *((src) + 2);\
    *((UINT32 *)(dest))  = (*((UINT32 *)(dest))) << 8;\
    *((UINT32 *)(dest)) |= *((src) + 1);\
    *((UINT32 *)(dest))  = (*((UINT32 *)(dest))) << 8;\
    *((UINT32 *)(dest)) |= *((src) + 0);


/* =================================================== Structures/Data Types */
/* HCI Link Key */
typedef struct {
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    UCHAR link_key[16];

} HCI_H_LINK_KEY;


/* HCI Command/API Opcode */
typedef struct {
    UCHAR ogf;
    UINT16 ocf;

} HCI_OPCODE;


/* HCI Inquiry Result Structure */
typedef struct {
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    UCHAR page_scan_repetition_mode;
    UCHAR page_scan_period_mode;
    UCHAR page_scan_mode;
    UINT32 class_of_device;
    UINT16 clock_offset;

#ifdef BT_HCI_1_2
    UCHAR rssi;
#endif                          /* BT_HCI_1_2 */

} HCI_INQUIRY_RESULT;


#ifdef BT_HCI_1_2
/* Synchronous Connection Input Parameters */
typedef struct {
    UINT32 tx_bandwidth;
    UINT32 rx_bandwidth;
    UINT16 max_latency;
    UINT16 voice_setting;
    UINT16 packet_type;
    UCHAR rtx_effort;

} HCI_SCO_IN_PARAMS;


/* Synchronous Connection Output Parameters */
typedef struct {
    UINT16 sco_handle;
    UCHAR link_type;
    UCHAR tx_interval;
    UCHAR rtx_window;
    UINT16 rx_length;
    UINT16 tx_length;
    UCHAR air_mode;

} HCI_SCO_OUT_PARAMS;
#endif /* BT_HCI_1_2 */


/* HCI Connection List */
typedef struct {
    /* ACL Connection Handle */
    UINT16 acl_handle;

    /* 
     *  SCO Handles for all SCO connections that exists with the
     *  remote Device identified by the ACL Connection Handle.
     */
    UINT16 sco_handle[HCI_MAX_SCO_CHANNELS];

    /* BD_ADDR of the Remote Device */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

    /* Current Mode of the ACL Link */
    UCHAR current_mode;

} HCI_CONNECTION_LIST;


/* HCI QoS */
typedef struct {
    /* Token Rate */
    UINT32 token_rate;

    /* Token Bucket Size */
    UINT32 token_bucket_size;

    /* Peak Bandwidth */
    UINT32 peak_bandwidth;

    /* Latency */
    UINT32 latency;

    /* Delay Variation */
    UINT32 delay_variation;

    /* Service Type */
    UCHAR service_type;

    /* Flow Direction */
    UCHAR flow_direction;

} HCI_QOS;


#ifdef BT_EIR
typedef struct {
    /* EIR Data */
    UCHAR *eir_data;

    /* EIR Data Length */
    UCHAR eir_data_len;

    /* EIR Data Type */
    UCHAR eir_data_type;

} HCI_EIR_DATA;
#endif /* BT_EIR */


/* ========================================================== API Functions */

#ifdef __cplusplus
extern "C" {
#endif

    /* Internal Functions (Not APIs)
     * ------------------------------------------- */
    API_RESULT hci_write_command_UCHAR(UINT16 opcode, UCHAR param);

    API_RESULT hci_write_command_UINT16(UINT16 opcode, UINT16 param);

    API_RESULT hci_write_command_UINT16_UINT16(UINT16 opcode, UINT16 param_1,
                                               UINT16 param_2);

    /* Common API Handler */
    API_RESULT hci_common_api_handler_no_params(UINT16 opcode);
    /* ------------------------------------------------------------------------- 
     */


    /* HCI Internal APIs
     * ------------------------------------------------------- */

      API_RESULT
        BT_hci_register_event_indication_callback(API_RESULT(*callback_ptr)
                                                    (UINT8 event_code,
                                                     UINT8 * event_data,
                                                     UINT8 event_datalen)
        );

      API_RESULT
        BT_hci_register_error_indication_callback(API_RESULT(*callback_ptr)
                                                    (UINT16 opcode,
                                                     UINT16 error_code)
        );

#ifdef HCI_SCO
    API_RESULT BT_hci_register_sco_data_handler(API_RESULT(*callback_ptr)
                                                  (UCHAR *, UINT16)
        );
#endif                          /* HCI_SCO */

    API_RESULT BT_hci_set_device_role(UCHAR * bd_addr, UCHAR role);

    API_RESULT BT_hci_get_local_bd_addr(UCHAR * bd_addr);

    API_RESULT BT_hci_get_acl_connection_handle(UCHAR * bd_addr,
                                                UINT16 * handle);

    API_RESULT BT_hci_get_sco_connection_handle(UCHAR * bd_addr,
                                                UINT16 * handle,
                                                UCHAR * num_handle);

    API_RESULT BT_hci_get_connection_details(HCI_CONNECTION_LIST *
                                             hci_conn_list, UCHAR num_allocated,
                                             UCHAR * num_present);

    API_RESULT BT_hci_decode_opcode(UINT16 opcode, HCI_OPCODE * hci_opcode);

    API_RESULT BT_hci_decode_inquiry_result(UCHAR * buffer,
                                            HCI_INQUIRY_RESULT * result);

#ifdef HCI_HAVE_INIT_COMMAND_MASK
    API_RESULT BT_hci_set_init_command_mask(UINT32 command_mask);

    API_RESULT BT_hci_check_init_command_mask(UINT32 command_mask);
#endif                          /* HCI_HAVE_INIT_COMMAND_MASK */

#ifdef BT_HCI_1_2
    API_RESULT BT_hci_decode_inquiry_result_rssi(UCHAR * buffer,
                                                 HCI_INQUIRY_RESULT * result);

    API_RESULT BT_hci_set_esco_channel_parameters(HCI_SCO_IN_PARAMS *
                                                  sco_params);
#endif                          /* BT_HCI_1_2 */

#ifdef HCI_SCO
#if 0
#define BT_hci_sco_write(p, pl) \
        hci_transport_write_data (HCI_SCO_DATA_PACKET, (p), (pl), 0x01)
#endif

    API_RESULT BT_hci_sco_write(UCHAR * packet, UINT16 packet_len);

#if defined BT_USB && defined WINDOWS
    API_RESULT BT_hci_usb_sco_set(UINT16 con_handle, UCHAR packet_size);
    API_RESULT BT_hci_usb_sco_reset(UINT16 con_handle);
#else                           /* BT_USB && WINDOWS */
#define BT_hci_usb_sco_set(c, s)
#define BT_hci_usb_sco_reset(c)
#endif                          /* BT_USB && WINDOWS */
#endif                          /* HCI_SCO */


    /* ------------------------------------------------------------------------- 
     */

    /* HCI Link Control APIs
     * --------------------------------------------------- */

#ifndef HCI_LITE
    API_RESULT BT_hci_inquiry(UINT32 lap, UCHAR inquiry_length,
                              UCHAR num_responses);

#define BT_hci_inquiry_cancel() \
        hci_common_api_handler_no_params (HCI_INQUIRY_CANCEL_OPCODE)

    API_RESULT BT_hci_periodic_inquiry_mode(UINT16 max_period_length,
                                            UINT16 min_period_length,
                                            UINT32 lap, UCHAR inquiry_length,
                                            UCHAR num_responses);

#define BT_hci_exit_periodic_inquiry_mode() \
        hci_common_api_handler_no_params (HCI_EXIT_PERIODIC_INQUIRY_MODE_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_create_connection(UCHAR * bd_addr, UINT16 packet_type,
                                        UCHAR page_scan_repetition_mode,
                                        UCHAR page_scan_mode,
                                        UINT16 clock_offset,
                                        UCHAR allow_role_switch);

    API_RESULT BT_hci_disconnect(UINT16 connection_handle, UCHAR reason);

    API_RESULT BT_hci_add_sco_connection(UCHAR * bd_addr, UINT16 packet_type);

#define BT_hci_change_connection_packet_type(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_CHANGE_CONNECTION_PACKET_TYPE_OPCODE, (p1), (p2))

    API_RESULT BT_hci_remote_name_request(UCHAR * bd_addr,
                                          UCHAR page_scan_repetition_mode,
                                          UCHAR page_scan_mode,
                                          UINT16 clock_offset);

    API_RESULT BT_hci_read_remote_supported_features(UINT16 connection_handle);

    API_RESULT BT_hci_read_remote_version_information(UINT16 connection_handle);

#define BT_hci_read_clock_offset(p1) \
        hci_write_command_UINT16 (HCI_READ_CLOCK_OFFSET_OPCODE, (p1))

    /* ------------------------------------------------------------------------- 
     */

    /* HCI Link Policy APIs
     * ---------------------------------------------------- */

    API_RESULT BT_hci_hold_mode(UINT16 connection_handle,
                                UINT16 hold_max_interval,
                                UINT16 hold_min_interval);

    API_RESULT BT_hci_sniff_mode(UINT16 connection_handle,
                                 UINT16 sniff_max_interval,
                                 UINT16 sniff_min_interval,
                                 UINT16 sniff_attempt, UINT16 sniff_timeout);

#define BT_hci_exit_sniff_mode(p1) \
        hci_write_command_UINT16 (HCI_EXIT_SNIFF_MODE_OPCODE, (p1))

    API_RESULT BT_hci_park_mode(UINT16 connection_handle,
                                UINT16 beacon_max_interval,
                                UINT16 beacon_min_interval);

    API_RESULT BT_hci_exit_park_mode(UINT16 connection_handle);

    API_RESULT BT_hci_qos_setup(UINT16 connection_handle,
                                HCI_QOS * qos_requested);

    API_RESULT BT_hci_role_discovery(UINT16 connection_handle);

    API_RESULT BT_hci_switch_role(UCHAR * bd_addr, UCHAR role);

    API_RESULT BT_hci_read_link_policy_settings(UINT16 connection_handle);

#define BT_hci_write_link_policy_settings(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_WRITE_LINK_POLICY_SETTINGS_OPCODE, (p1), (p2))

    /* ------------------------------------------------------------------------- 
     */

    /* HCI Host Controller & Baseband APIs
     * ------------------------------------- */

    API_RESULT BT_hci_set_event_mask(UCHAR * event_mask);

#define BT_hci_reset() \
        hci_common_api_handler_no_params (HCI_RESET_OPCODE)

    API_RESULT BT_hci_set_event_filter(UCHAR filter_type,
                                       UCHAR filter_condition_type,
                                       UCHAR * condition,
                                       UINT16 condition_length);

    API_RESULT BT_hci_flush(UINT16 connection_handle);

#ifndef HCI_LITE
#define BT_hci_create_new_unit_key() \
        hci_common_api_handler_no_params (HCI_CREATE_NEW_UNIT_KEY_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_change_local_name(UCHAR * name, UINT16 name_length);

#ifndef HCI_LITE
#define BT_hci_read_local_name() \
        hci_common_api_handler_no_params (HCI_READ_LOCAL_NAME_OPCODE)
#endif                          /* HCI_LITE */

#ifndef HCI_LITE
#define BT_hci_read_connection_accept_timeout() \
        hci_common_api_handler_no_params \
        (HCI_READ_CONNECTION_ACCEPT_TIMEOUT_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_connection_accept_timeout(UINT16 timeout);

#ifndef HCI_LITE
#define BT_hci_read_page_timeout() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_TIMEOUT_OPCODE)
#endif                          /* HCI_LITE */

#define BT_hci_write_page_timeout(p1) \
        hci_write_command_UINT16 (HCI_WRITE_PAGE_TIMEOUT_OPCODE, (p1))

#ifndef HCI_LITE
#define BT_hci_read_scan_enable() \
        hci_common_api_handler_no_params (HCI_READ_SCAN_ENABLE_OPCODE)
#endif                          /* HCI_LITE */

#define BT_hci_write_scan_enable(p1) \
        hci_write_command_UCHAR (HCI_WRITE_SCAN_ENABLE_OPCODE, (p1))

#ifndef HCI_LITE
#define BT_hci_read_page_scan_activity() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_SCAN_ACTIVITY_OPCODE)
#endif                          /* HCI_LITE */

#define BT_hci_write_page_scan_activity(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_WRITE_PAGE_SCAN_ACTIVITY_OPCODE, (p1), (p2))

#ifndef HCI_LITE
#define BT_hci_read_inquiry_scan_activity() \
        hci_common_api_handler_no_params \
        (HCI_READ_INQUIRY_SCAN_ACTIVITY_OPCODE)
#endif                          /* HCI_LITE */

#define BT_hci_write_inquiry_scan_activity(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_WRITE_INQUIRY_SCAN_ACTIVITY_OPCODE, (p1), (p2))

#ifndef HCI_LITE
#define BT_hci_read_authentication_enable() \
        hci_common_api_handler_no_params (HCI_READ_AUTHENTICATION_ENABLE_OPCODE)
#endif                          /* HCI_LITE */

#define BT_hci_write_authentication_enable(p1) \
        hci_write_command_UCHAR (HCI_WRITE_AUTHENTICATION_ENABLE_OPCODE, (p1))

#ifndef HCI_LITE
#define BT_hci_read_encryption_mode() \
        hci_common_api_handler_no_params (HCI_READ_ENCRYPTION_MODE_OPCODE)
#endif                          /* HCI_LITE */

#define BT_hci_write_encryption_mode(p1) \
        hci_write_command_UCHAR (HCI_WRITE_ENCRYPTION_MODE_OPCODE, (p1))

#ifndef HCI_LITE
#define BT_hci_read_class_of_device() \
        hci_common_api_handler_no_params (HCI_READ_CLASS_OF_DEVICE_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_class_of_device(UINT32 class_of_device);

#ifndef HCI_LITE
#define BT_hci_read_voice_setting() \
        hci_common_api_handler_no_params (HCI_READ_VOICE_SETTING_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_voice_setting(UINT16 voice_setting);

    API_RESULT BT_hci_read_automatic_flush_timeout(UINT16 handle);

    API_RESULT BT_hci_write_automatic_flush_timeout(UINT16 handle,
                                                    UINT16 timeout);

#ifndef HCI_LITE
#define BT_hci_read_num_broadcast_retransmissions() \
        hci_common_api_handler_no_params \
        (HCI_READ_NUM_BROADCAST_RETRANSMISSIONS_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_num_broadcast_retransmissions(UCHAR
                                                          num_broadcast_rx);

#ifndef HCI_LITE
#define BT_hci_read_hold_mode_activity() \
        hci_common_api_handler_no_params (HCI_READ_HOLD_MODE_ACTIVITY_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_hold_mode_activity(UCHAR hold_mode_activity);

    API_RESULT BT_hci_read_transmit_power_level(UINT16 handle,
                                                UCHAR power_type);

#ifndef HCI_LITE
#define BT_hci_read_sco_flow_control_enable() \
        hci_common_api_handler_no_params \
        (HCI_READ_SCO_FLOW_CONTROL_ENABLE_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_sco_flow_control_enable(UCHAR flow_enable);

    API_RESULT BT_hci_set_host_controller_to_host_flow_control(UCHAR
                                                               flow_enable);

#ifdef HCI_HOST_CONTROLLER_FLOW_ON
    API_RESULT BT_hci_host_buffer_size(void);
#endif                          /* HCI_HOST_CONTROLLER_FLOW_ON */

    API_RESULT BT_hci_read_link_supervision_timeout(UINT16 handle);

#define BT_hci_write_link_supervision_timeout(p1, p2) \
        hci_write_command_UINT16_UINT16 \
        (HCI_WRITE_LINK_SUPERVISION_TIMEOUT_OPCODE, (p1), (p2))

#ifndef HCI_LITE
#define BT_hci_read_number_of_supported_iac() \
        hci_common_api_handler_no_params \
        (HCI_READ_NUMBER_OF_SUPPORTED_IAC_OPCODE)

#define BT_hci_read_current_iac_lap() \
        hci_common_api_handler_no_params (HCI_READ_CURRENT_IAC_LAP_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_current_iac_lap(UCHAR num_iac, UINT32 * iac_lap);

#ifndef HCI_LITE
#define BT_hci_read_page_scan_period_mode() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_SCAN_PERIOD_MODE_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_page_scan_period_mode(UCHAR mode);

#ifndef HCI_LITE
#define BT_hci_read_page_scan_mode() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_SCAN_MODE_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_page_scan_mode(UCHAR mode);

    /* ------------------------------------------------------------------------- 
     */

    /* HCI Host Controller Informational APIs
     * ---------------------------------- */

#define BT_hci_read_local_version_information() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_VERSION_INFORMATION_OPCODE)

#define BT_hci_read_local_supported_features() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_SUPPORTED_FEATURES_OPCODE)

#define BT_hci_read_bd_addr() \
        hci_common_api_handler_no_params (HCI_READ_BD_ADDR_OPCODE)

#define BT_hci_read_buffer_size() \
        hci_common_api_handler_no_params (HCI_READ_BUFFER_SIZE_OPCODE)

    /* ------------------------------------------------------------------------- 
     */

    /* HCI Status Parameters APIs
     * ---------------------------------------------- */

    API_RESULT BT_hci_read_rssi(UINT16 connection_handle);

    /* ------------------------------------------------------------------------- 
     */

    /* HCI Testing Commands APIs
     * ----------------------------------------------- */

#ifdef HCI_TESTING_COMMANDS

#define BT_hci_read_loopback_mode() \
        hci_common_api_handler_no_params (HCI_READ_LOOPBACK_MODE_OPCODE)

    API_RESULT BT_hci_write_loopback_mode(UCHAR loopback_mode);

#define BT_hci_enable_device_under_test_mode() \
        hci_common_api_handler_no_params \
        (HCI_ENABLE_DEVICE_UNDER_TEST_MODE_OPCODE)

#endif                          /* HCI_TESTING_COMMANDS */

    /* ------------------------------------------------------------------------- 
     */

    /* New APIs from Bluetooth Spec 1.2
     * ---------------------------------------- */

#ifdef BT_HCI_1_2

    /* Name Changes in HCI Commands from Bluetooth Specification 1.2 */

#define BT_hci_write_local_name \
        BT_hci_change_local_name

#define BT_hci_read_synchronous_flow_control_enable \
        BT_hci_read_sco_flow_control_enable

#define BT_hci_park_state \
        BT_hci_park_mode

#define BT_hci_exit_park_state \
        BT_hci_exit_park_mode

#define BT_hci_write_synchronous_flow_control_enable \
        BT_hci_write_sco_flow_control_enable

#define BT_hci_set_controller_to_host_flow_control \
        BT_hci_set_host_controller_to_host_flow_control



    /* New HCI Commands from Bluetooth Specification 1.2 */

    API_RESULT BT_hci_create_connection_cancel(UCHAR * bd_addr);

    API_RESULT BT_hci_remote_name_request_cancel(UCHAR * bd_addr);

    API_RESULT BT_hci_read_remote_extended_features(UINT16 handle,
                                                    UCHAR page_number);

    API_RESULT BT_hci_setup_synchronous_connection(UINT16 handle,
                                                   HCI_SCO_IN_PARAMS *
                                                   sco_in_params);

    API_RESULT BT_hci_flow_specification(UINT16 handle,
                                         HCI_QOS * qos_requested);

#define BT_hci_read_default_link_policy_settings() \
        hci_common_api_handler_no_params \
        (HCI_READ_DEFAULT_LINK_POLICY_SETTINGS_OPCODE)

#define BT_hci_write_default_link_policy_settings(p1) \
        hci_write_command_UINT16 \
        (HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_OPCODE, (p1))

    API_RESULT BT_hci_set_afh_host_channel_classification(UCHAR *
                                                          channel_params);

#ifndef HCI_LITE
#define BT_hci_read_inquiry_scan_type() \
        hci_common_api_handler_no_params (HCI_READ_INQUIRY_SCAN_TYPE_OPCODE)
#endif                          /* HCI_LITE */

#define BT_hci_write_inquiry_scan_type(p1) \
        hci_write_command_UCHAR (HCI_WRITE_INQUIRY_SCAN_TYPE_OPCODE, (p1))

#ifndef HCI_LITE
#define BT_hci_read_inquiry_mode() \
        hci_common_api_handler_no_params (HCI_READ_INQUIRY_MODE_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_inquiry_mode(UCHAR inquiry_mode);

#ifndef HCI_LITE
#define BT_hci_read_page_scan_type() \
        hci_common_api_handler_no_params (HCI_READ_PAGE_SCAN_TYPE_OPCODE)
#endif                          /* HCI_LITE */

#define BT_hci_write_page_scan_type(p1) \
        hci_write_command_UCHAR (HCI_WRITE_PAGE_SCAN_TYPE_OPCODE, (p1))

#ifndef HCI_LITE
#define BT_hci_read_afh_channel_assessment_mode() \
        hci_common_api_handler_no_params \
        (HCI_READ_AFH_CHANNEL_ASSESSMENT_MODE_OPCODE)
#endif                          /* HCI_LITE */

    API_RESULT BT_hci_write_afh_channel_assessment_mode(UCHAR
                                                        afh_assessment_mode);

    API_RESULT BT_hci_read_afh_channel_map(UINT16 handle);

#define BT_hci_read_local_supported_commands() \
        hci_common_api_handler_no_params \
        (HCI_READ_LOCAL_SUPPORTED_COMMANDS_OPCODE)

    API_RESULT BT_hci_read_local_extended_features(UCHAR page_number);

    API_RESULT BT_hci_read_clock(UCHAR which_clock, UINT16 handle);

#endif                          /* BT_HCI_1_2 */

    /* ------------------------------------------------------------------------- 
     */

    /* New APIs from Bluetooth Spec 2.1+EDR
     * ------------------------------------ */

#ifdef BT_SSR
    API_RESULT BT_hci_sniff_subrating(IN UINT16 connection_handle,
                                      IN UINT16 maximum_latency,
                                      IN UINT16 minimum_remote_timeout,
                                      IN UINT16 minimum_local_timeout);
#endif                          /* BT_SSR */

#ifdef BT_EIR
#define BT_hci_read_extended_inquiry_response() \
        hci_common_api_handler_no_params \
        (HCI_READ_EXTENDED_INQUIRY_RESPONSE_OPCODE)

    API_RESULT BT_hci_write_extended_inquiry_response(IN UCHAR fec_required,
                                                      IN HCI_EIR_DATA *
                                                      eir_data_field,
                                                      IN UCHAR
                                                      total_eir_fields);

#define BT_hci_read_inquiry_response_transmit_power_level() \
        hci_common_api_handler_no_params \
        (HCI_READ_INQUIRY_RESPONSE_TRANSMIT_POWER_LEVEL_OPCODE)

    API_RESULT BT_hci_write_inquiry_response_transmit_power_level(IN CHAR
                                                                  tx_power);
#endif                          /* BT_EIR */

#ifdef BT_EPR
#define BT_hci_refresh_encryption_key(p1) \
        hci_write_command_UINT16 (HCI_REFRESH_ENCRYPTION_KEY_OPCODE, (p1))
#endif                          /* BT_EPR */

#ifdef BT_EDR
#define BT_hci_read_default_erroneous_data_reporting() \
        hci_common_api_handler_no_params \
        (HCI_READ_DEFAULT_ERRONEOUS_DATA_REPORTING_OPCODE)

    API_RESULT BT_hci_write_default_erroneous_data_reporting(UCHAR
                                                             erroneous_data_reporting);
#endif                          /* BT_EDR */

#ifdef BT_SSP
#define BT_hci_read_simple_pairing_mode() \
        hci_common_api_handler_no_params (HCI_READ_SIMPLE_PAIRING_MODE_OPCODE)

#define BT_hci_write_simple_pairing_mode(p1) \
        hci_write_command_UCHAR (HCI_WRITE_SIMPLE_PAIRING_MODE_OPCODE, (p1))
#endif                          /* BT_SSP */

#ifdef BT_SSP_PE
    API_RESULT BT_hci_send_keypress_notification(UCHAR * bd_addr,
                                                 UCHAR notification_type);
#endif                          /* BT_SSP_PE */

#ifdef BT_SSP_OOB
#define BT_hci_read_local_oob_data() \
        hci_common_api_handler_no_params (HCI_READ_LOCAL_OOB_DATA_OPCODE)
#endif                          /* BT_SSP_OOB */

#ifdef BT_ENHANCED_FLUSH
    API_RESULT BT_hci_enhanced_flush(IN UINT16 connection_handle,
                                     IN UCHAR packet_type);
#endif                          /* BT_ENHANCED_FLUSH */

#ifdef BT_SSP_DEBUG
#define BT_hci_write_simple_pairing_debug_mode(p1) \
        hci_write_command_UCHAR \
        (HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE_OPCODE, (p1))
#endif                          /* BT_SSP_DEBUG */

    /* ------------------------------------------------------------------------- 
     */

    /* HCI Vendor Specific Commands APIs
     * --------------------------------------- */

    API_RESULT BT_hci_vendor_specific_command(UINT16 ocf, UCHAR * params,
                                              UCHAR params_length);

    /* ------------------------------------------------------------------------- 
     */


    /* APIs for EtherMind Security Manager
     * ------------------------------------- */
    API_RESULT hci_sm_link_key_request_reply(UCHAR * bd_addr, UCHAR * link_key,
                                             UCHAR link_key_present);

    API_RESULT hci_sm_pin_code_request_reply(UCHAR * bd_addr, UCHAR * pin_code,
                                             UCHAR pin_length);

    API_RESULT hci_sm_set_pin_type_master_link_key(UINT16 opcode, UCHAR flag);

#define hci_sm_set_pin_type(pt) \
        hci_sm_set_pin_type_master_link_key (HCI_WRITE_PIN_TYPE_OPCODE, (pt))
#define hci_sm_master_link_key(kf) \
        hci_sm_set_pin_type_master_link_key (HCI_MASTER_LINK_KEY_OPCODE, (kf))

#ifdef BT_SSP_UC
    API_RESULT hci_sm_user_confirmation_request_reply(UCHAR * bd_addr,
                                                      UCHAR
                                                      confirmation_accept);
#endif                          /* BT_SSP_UC */

#ifdef BT_SSP_PE
    API_RESULT hci_sm_user_passkey_request_reply(UCHAR * bd_addr,
                                                 UCHAR * passkey);
#endif                          /* BT_SSP_PE */

    /* ------------------------------------------------------------------------- 
     */


#ifdef __cplusplus
};
#endif


#endif /* _H_BT_HCI_API_ */
