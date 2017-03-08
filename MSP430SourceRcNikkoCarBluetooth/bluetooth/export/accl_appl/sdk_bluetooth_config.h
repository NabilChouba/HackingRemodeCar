
/**
 * Copyright (C) 2009 MindTree Ltd. All rights reserved.
 * \file    sdk_bluetooth_config.h
 * \brief   This header file contains configuration parameters defined by the
 *          application developer
 */

#ifndef _H_SDK_BLUETOOTH_CONFIG_
#define _H_SDK_BLUETOOTH_CONFIG_

/* ----------------------------------------------- Header File Inclusion */
#include "sdk_bluetooth_common_config.h"
#include "sdk_pin_config.h"

/* ----------------------------------------------- Common Config Parameters */

/* Local device name Note : upper limit for the lenght of Bluetooth device name
 * is 24 characters */
#define SDK_CONFIG_DEVICE_NAME                  "BlueMSP-Demo"

/* Name of the Remote BT Device to which SPP connection has to be established */
#define SDK_REM_DEV_NAME_PREFIX                 "BlueMSP-Demo"

/* Bluetooth ACL Link Supervision Timeout */
#define SDK_CONFIG_LINK_SUPERVISION_TIMEOUT     0x1900

/* Bluetooth Link Policy Settings */
#define SDK_CONFIG_LINK_POLICY_SETTINGS         0x0005

/* HCI Inquiry Parameters */
#define SDK_INQUIRY_LAP                         BT_LIAC
#define SDK_INQUIRY_LEN                         0x07

/* Flag to enable sniff mode */
#define SDK_ENABLE_SNIFF_MODE

/* Minimum sniff interval to be used (180 ms) */
#define SDK_CONFIG_SNIFF_MIN_INTERVAL           0x0120
/* Maximum sniff interval to be used (180 ms) */
#define SDK_CONFIG_SNIFF_MAX_INTERVAL           0x0120
/* Sniff attempt */
#define SDK_CONFIG_SNIFF_ATTEMPT                8
/* Sniff timeout value */
#define SDK_CONFIG_SNIFF_TIMEOUT                1

/* Flag to enable insertion of application data into a basic
 * Header+Payload+Checksum packet format */
#define PACKETISE_USB_DATA


#endif /* _H_SDK_BLUETOOTH_CONFIG_ */
