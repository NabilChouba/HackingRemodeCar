
/**
 *  @file sm_internal.h
 *
 *  Internal header file of Security Manager module. This file contains
 *  the data structures, type definitions, and function declarations that
 *  the Security Manager implementation uses internally.
 *
 */

/*
 *  Copyright (C) 2000-2008. MindTree Ltd.
 *  All rights reserved.
 *
 */

#ifndef _H_SM_INTERNAL_
#define _H_SM_INTERNAL_

/* ----------------------------------------- Header File Inclusion */
/* EtherMind Common Header Files */
#include "BT_common.h"
#include "BT_debug.h"

/* Security Manager API & Module Header Files */
#include "BT_sm_api.h"
#include "sm.h"

/* For interaction with other EtherMind modules - HCI, L2CAP & RFCOMM */
#include "hci_h.h"
#include "l2cap.h"

#ifdef RFCOMM
    #include "rfcomm.h"
#endif /* RFCOMM */

/* For HCI events and error codes */
#include "BT_hci_api.h"

/* For the Write Task & Bottom Halves */
#include "write_task.h"
#include "BT_bottom_half.h"


/* ----------------------------------------- Security Manager Debugging */
#ifndef SEC_NO_DEBUG
#define SM_ERR                                  BT_debug_error
#else  /* SEC_NO_DEBUG */
#define SM_ERR                                  BT_debug_null
#endif /* SEC_NO_DEBUG */

#ifdef SEC_DEBUG

#define SM_TRC                                  BT_debug_trace
#define SM_INF                                  BT_debug_info

#else  /* SEC_DEBUG */

#define SM_TRC                                  BT_debug_null
#define SM_INF                                  BT_debug_null

#endif /* SEC_DEBUG */


/* ----------------------------------------- Global Definitions */
/** Boolean Truth Values */
#define SM_TRUE                                 0x1
#define SM_FALSE                                0x0

/** Pairable & Bondable Mode Mask */
#define SM_PAIRABLE_MASK                        0x01
#define SM_BONDABLE_MASK                        0x02

/** Bit Positions of SM Device Attributes in Device Database */
#define SM_DEV_TRUSTED_BIT                      0
#define SM_DEV_AUTHENTICATE_BIT                 1
#define SM_DEV_ENCRYPT_BIT                      3
#define SM_DEV_LK_AVLBL_BIT                     5
#define SM_DEV_INCOMING_BIT                     6
#define SM_DEV_ACL_INITIATED_BIT                7

/** Bit Masks for Setting & Resetting Device Attributes in Device Database */
#define SM_DEV_TRUSTED_MASK                     0xFE
#define SM_DEV_AUTHENTICATE_MASK                0xF9
#define SM_DEV_ENCRYPT_MASK                     0xE7
#define SM_DEV_LK_AVLBL_MASK                    0xDF
#define SM_DEV_INCOMING_MASK                    0xBF
#define SM_DEV_ACL_INITIATED_MASK               0x7F

/** State of a Device Database Entry */
#define SM_DEVICE_INVALID                       0x00
#define SM_DEVICE_USED                          0x01
#define SM_DEVICE_CAN_BE_FREED                  0x02

/** Bit Positions for SM Service Attributes in Service Database */
#define SM_SRV_VALID_BIT                        0
#define SM_SRV_AUTHENTICATE_BIT                 1
#define SM_SRV_AUTHORIZE_BIT                    3
#define SM_SRV_ENCRYPT_BIT                      5
#define SM_SRV_DEFAULT_SEC_BIT                  6
#define SM_SRV_NO_BONDING_BIT                   7

/** Bit Masks for Setting & Resetting Service Attributes in Service Database */
#define SM_SRV_VALID_MASK                       0xFFFFFFFE
#define SM_SRV_AUTHENTICATE_MASK                0xFFFFFFF9
#define SM_SRV_AUTHORIZE_MASK                   0xFFFFFFE7
#define SM_SRV_ENCRYPT_MASK                     0xFFFFFFDF
#define SM_SRV_DEFAULT_SEC_MASK                 0xFFFFFFBF
#define SM_SRV_NO_BONDING_MASK                  0xFFFFFF7F

/** States of SM Security Access Contexts */
#define SM_CONTEXT_INACTIVE                     0x00
#define SM_CONTEXT_ACTIVE                       0x01
#define SM_CONTEXT_W4_AUTHORIZATION             0x02
#define SM_CONTEXT_W4_AUTHENTICATION            0x03
#define SM_CONTEXT_W4_ENCRYPTION                0x04
#define SM_CONTEXT_ANY                          0xFF

/** Entry Points for Security Access State Machine */
#define SM_SAC_AUTHENTICATION                   0x01
#define SM_SAC_AUTHORIZATION                    0x02
#define SM_SAC_ENCRYPTION                       0x03

/** Device Authentication States */
#define SM_NOT_AUTHENTICATED                    0x00
#define SM_AUTHENTICATED                        0x01
#define SM_AUTHENTICATION_IN_PROGRESS           0x02

/** Device Encryption States */
#define SM_NOT_ENCRYPTED                        0x00
#define SM_ENCRYPTED                            0x01
#define SM_ENCRYPTION_IN_PROGRESS               0x03

/* Default Service Authentication & Encryption Requirements */
#ifdef BT_SSP
#define SM_DEFAULT_SERVICE_SECURITY_LEVEL       SM_SERVICE_SECURITY_LEVEL_2
#else  /* BT_SSP */
#define SM_DEFAULT_SERVICE_SECURITY_LEVEL       0x01
#endif /* BT_SSP */
#define SM_DEFAULT_SERVICE_ENCRYPTION           0x01


/* ----------------------------------------- Macros */
/** To Lock SM Global Mutex */
#define sm_lock()                  BT_MUTEX_LOCK(sm_mutex, SM)

/** To Unlock SM Global Mutex */
#define sm_unlock()                BT_MUTEX_UNLOCK(sm_mutex, SM)

/** To Lock SM Global Mutex */
#define sm_lock_void()             BT_MUTEX_LOCK_VOID(sm_mutex, SM)

/** To Unlock SM Global Mutex */
#define sm_unlock_void()           BT_MUTEX_UNLOCK_VOID(sm_mutex, SM)


/* Set/Reset Pairable/Bondable mode ============== */
/** Test if Pairable mode is set */
#define SM_IS_PAIRABLE() \
        ((sm_pairable_mode & SM_PAIRABLE_MASK) != 0)

/** Test if Bondable mode is set */
#define SM_IS_BONDABLE() \
        ((sm_pairable_mode & SM_BONDABLE_MASK) != 0)


/* Set/Reset Device Attributes =================== */
/** Set/Reset 'Trusted' Flag */
#define SM_SET_DEVICE_TRUSTED(i, trust_flag) \
        sm_devices[(i)].device_attr = \
        (UCHAR)((sm_devices[(i)].device_attr & SM_DEV_TRUSTED_MASK) \
        | ((trust_flag) << SM_DEV_TRUSTED_BIT));

/** Set/Reset 'Authentication' Flag */
#define SM_SET_DEVICE_AUTHENTICATION(i, auth_flag) \
        sm_devices[(i)].device_attr = \
        (UCHAR)((sm_devices[(i)].device_attr & SM_DEV_AUTHENTICATE_MASK) \
        | ((auth_flag) << SM_DEV_AUTHENTICATE_BIT));

/** Set/Reset 'Encryption' Flag */
#define SM_SET_DEVICE_ENCRYPTION(i, enc_flag) \
        sm_devices[(i)].device_attr = \
        (UCHAR)((sm_devices[(i)].device_attr & SM_DEV_ENCRYPT_MASK) \
        | ((enc_flag) << SM_DEV_ENCRYPT_BIT));

/** Set/Reset 'Link Key Availability' Flag */
#define SM_SET_DEVICE_LINK_KEY_AVAILABLE(i, val) \
        sm_devices[(i)].device_attr = \
        (UCHAR)((sm_devices[(i)].device_attr & SM_DEV_LK_AVLBL_MASK) \
        | ((val) << SM_DEV_LK_AVLBL_BIT));

/** Set/Reset 'Incoming ACL' Flag */
#define SM_SET_DEVICE_INCOMING(i, incoming_flag) \
        sm_devices[(i)].device_attr = \
        (UCHAR)((sm_devices[(i)].device_attr & SM_DEV_INCOMING_MASK) \
        | ((incoming_flag) << SM_DEV_INCOMING_BIT));

/** Set/Reset 'ACL Initiated' Flag */
#define SM_SET_DEVICE_ACL_INITIATED(i, flag) \
        sm_devices[(i)].device_attr = \
        (UCHAR)((sm_devices[(i)].device_attr & SM_DEV_ACL_INITIATED_MASK) \
        | ((flag) << SM_DEV_ACL_INITIATED_BIT));


/** Test if the device is 'Trusted' */
#define SM_IS_DEVICE_TRUSTED(i) \
        ((sm_devices[(i)].device_attr & \
         (0x01 << SM_DEV_TRUSTED_BIT)) > 0)

/** Test if ACL connection to the Device is encrypted */
#define SM_IS_DEVICE_ENCRYPTED(i) \
        ((sm_devices[(i)].device_attr & \
         (0x03 << SM_DEV_ENCRYPT_BIT)) == 0x08)

/** Test if Encryption is Pending */
#define SM_IS_DEVICE_ENCRYPTION_PENDING(i) \
        ((sm_devices[(i)].device_attr & \
         (0x03 << SM_DEV_ENCRYPT_BIT)) == 0x18)

/** Test if ACL connection to the Device is authenticated */
#define SM_IS_DEVICE_AUTHENTICATED(i) \
        ((sm_devices[(i)].device_attr & \
         (0x03 << SM_DEV_AUTHENTICATE_BIT)) == 0x02)

/** Test if Authentication is Pending */
#define SM_IS_DEVICE_AUTHENTICATION_PENDING(i) \
        ((sm_devices[(i)].device_attr & \
         (0x03 << SM_DEV_AUTHENTICATE_BIT)) == 0x04)

/** Test if Link Key is available for the device */
#define SM_IS_DEVICE_LINK_KEY_AVAILABLE(i) \
        ((sm_devices[(i)].device_attr & \
         (0x01 << SM_DEV_LK_AVLBL_BIT)) > 0)

/** Test if ACL connection to a device is locally or remotely originated */
#define SM_IS_DEVICE_INCOMING(i) \
        ((sm_devices[(i)].device_attr & \
         (0x01 << SM_DEV_INCOMING_BIT)) > 0)

/** Test if ACL connection to a device is initiated */
#define SM_IS_DEVICE_ACL_INITIATED(i) \
        ((sm_devices[(i)].device_attr & \
         (0x01 << SM_DEV_ACL_INITIATED_BIT)) > 0)


/* Set/Reset Service Attributes =================== */
/** Set Service 'Valid' Flag */
#define SM_SET_SERVICE_VALID(i, value) \
        sm_services[(i)].security_attr = \
        (sm_services[(i)].security_attr & SM_SRV_VALID_MASK) \
        | ((value) << SM_SRV_VALID_BIT);

/** Set Service 'Authenticate' Flag */
#define SM_SET_SERVICE_AUTHENTICATION(i, value) \
        sm_services[(i)].security_attr =  \
        (sm_services[(i)].security_attr & SM_SRV_AUTHENTICATE_MASK) \
        | ((value) << SM_SRV_AUTHENTICATE_BIT);

/** Set Service 'Authorize' Flag */
#define SM_SET_SERVICE_AUTHORIZATION(i, value) \
        sm_services[(i)].security_attr = \
        (sm_services[(i)].security_attr & SM_SRV_AUTHORIZE_MASK) \
        | ((value) << SM_SRV_AUTHORIZE_BIT);

/** Set Service 'Encrypt' Flag */
#define SM_SET_SERVICE_ENCRYPTION(i, value) \
        sm_services[(i)].security_attr = \
        (sm_services[(i)].security_attr & SM_SRV_ENCRYPT_MASK) \
        | ((value) << SM_SRV_ENCRYPT_BIT);

/** Set Service 'Default Security' Flag */
#define SM_SET_SERVICE_DEFAULT_SEC(i, value) \
        sm_services[(i)].security_attr = \
        (sm_services[(i)].security_attr & SM_SRV_DEFAULT_SEC_MASK) \
        | ((value) << SM_SRV_DEFAULT_SEC_BIT);

/** Set Service 'No Bonding' Flag */
#define SM_SET_SERVICE_NO_BONDING(i, value) \
        sm_services[(i)].security_attr = \
        (sm_services[(i)].security_attr & SM_SRV_NO_BONDING_MASK) \
        | ((value) << SM_SRV_NO_BONDING_BIT);

/** Get Service 'Authenticate' Flag */
#define SM_GET_SERVICE_AUTHENTICATION(i) \
        ((UCHAR)((sm_services[(i)].security_attr >> \
        SM_SRV_AUTHENTICATE_BIT) & 0x00000003))

/** Get Service 'Authorize' Flag */
#define SM_GET_SERVICE_AUTHORIZATION(i) \
        ((UCHAR)((sm_services[(i)].security_attr >> \
        SM_SRV_AUTHORIZE_BIT) & 0x00000003))

/** Get Service 'Encrypt' Flag */
#define SM_GET_SERVICE_ENCRYPTION(i) \
        ((UCHAR)((sm_services[(i)].security_attr >> \
        SM_SRV_ENCRYPT_BIT) & 0x00000001))

/** Get Service 'Default Security' Flag */
#define SM_GET_SERVICE_DEFAULT_SEC(i) \
        ((UCHAR)((sm_services[(i)].security_attr >> \
        SM_SRV_DEFAULT_SEC_BIT) & 0x00000001))

/** Get Service 'No Bonding' Flag */
#define SM_GET_SERVICE_NO_BONDING(i) \
        ((UCHAR)((sm_services[(i)].security_attr >> \
        SM_SRV_NO_BONDING_BIT) & 0x00000001))

/** Test if Service is Valid */
#define SM_IS_SERVICE_VALID(i) \
        ((sm_services[(i)].security_attr & \
         (0x01 << SM_SRV_VALID_BIT)) > 0)

/** Test if Service requires Authentication */
#define SM_IS_SERVICE_AUTHENTICATED(i) \
        ((sm_services[(i)].security_attr & \
         (0x03 << SM_SRV_AUTHENTICATE_BIT)) > 0)

/** Test if Service requires Authorization */
#define SM_IS_SERVICE_AUTHORIZED(i) \
        ((sm_services[(i)].security_attr & \
         (0x03 << SM_SRV_AUTHORIZE_BIT)) > 0)

/** Test if Service requires Encryption */
#define SM_IS_SERVICE_ENCRYPTED(i) \
        ((sm_services[(i)].security_attr & \
         (0x01 << SM_SRV_ENCRYPT_BIT)) > 0)

/** Test if Service has set for Default Security */
#define SM_IS_SERVICE_DEFAULT_SEC(i) \
        ((sm_services[(i)].security_attr & \
         (0x01 << SM_SRV_DEFAULT_SEC_BIT)) > 0)

/** Test if Service has set for No Bondin */
#define SM_IS_SERVICE_NO_BONDING(i) \
        ((sm_services[(i)].security_attr & \
         (0x01 << SM_SRV_NO_BONDING_BIT)) > 0)


/* ----------------------------------------- Structures/ Data Types */
/** The Security Manager Device Database */
typedef struct
{
    /** Platform specific Device Attributes */
    SM_DEVICE_ATTR_PL device_attr_pl;

    /** ACL Connection Handle for the Device */
    UINT16 acl_handle;

    /** Newly Allocated / Existing / Unallocated ? */
    UCHAR valid;

    /**
     *  Device Attributes of this Device Database entry:
     *  Bit   0: Trusted or Untrusted.
     *  Bit 1-2: whether Authentication is performed for this device
     *           00 - Not Authenticated
     *           01 - Authenticated
     *           10 - Authentication in progress
     *           11 - Unused
     *  Bit 3-4: whether Encryption is performed for this device
     *           00 - Not Encrypted
     *           01 - Pt-to-Pt Encrypted
     *           10 - Pt-to-Pt & Broadcast Encrypted (Unused)
     *           11 - Encryption in progress
     *  Bit   5: whether Link Key is available or not
     *  Bit   6: whether ACL connection is locally or remotely originated
     *  Bit   7: whether ACL connection initiated and completion is awaited
     */
    UCHAR device_attr;

    /** Length of Device's Name */
    UCHAR name_length;

    /** Length of Device's PIN Code */
    UCHAR pin_length;

    /** Type of Link Key */
    UCHAR link_key_type;

#ifdef BT_SSP
    /** Last received IO Capability of Remote Device */
    SM_IO_CAPS remote_io_cap;
#endif /* BT_SSP */

    /** Link Key for the Device */
    UCHAR link_key [BT_LINK_KEY_SIZE];

    /** Name of the Device */
    UCHAR name [SM_DEVICE_NAME_SIZE];

    /** PIN Code for the Device */
    UCHAR pin_code [BT_PIN_CODE_SIZE];

    /** BD_ADDR of the Device */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

} SM_DEVICE_ENTITY;


#ifdef SM_HAVE_MODE_2

/** The Security Manager Service Database */
typedef struct
{
    /** Profile's SM Callback for Authorization Request */
    SM_UI_NOTIFY_CB service_sm_cb;

    /**
     *  Security Attributes of this Service Database entry:
     *  Bit   0: whether this Sevice Database entry is allocated.
     *           0 - Free
     *           1 - Allocated
     *  Bit 1-2: whether Authentication is required for this service
     *           (security requirements as per Bluetooth v2.1+EDR).
     *           00 - Level 0
     *           01 - Level 1
     *           10 - Level 2
     *           11 - Level 3
     *  Bit 3-4: whether Authorization is required for this service.
     *           00 - No Authorization
     *           01 - Authorization required (Security Mode = 2)
     *           10 - Authorization required (Security Mode = 1 or 3)
     *  Bit   5: whether Encryption is required for this service.
     *           0 - No Encryption
     *           1 - Encryption required
     *  Bit   6: whether allocated for default security requirement.
     *           0 - not for default security
     *           1 - allocated to represent default security
     *  Bit   7: whether 'No Bonding' to be set in IO Capability.
     *           0 - service requires 'General Bonding' in IO Capability
     *           1 - service requires 'No Bonding' in IO Capability
     */
    UINT32 security_attr;

    /** L2CAP PSM Value */
    UINT16 psm;

    /** RFCOMM Server Channel (only if PSM is RFCOMM) */
    UCHAR  server_ch;

    /** Service ID */
    UCHAR  service_id;

} SM_SERVICE_ENTITY;


/** SM Security Access Context */
typedef struct
{
    /** Protocol specific information */
    UINT32 protocol_data;

    /** L2CAP PSM */
    UINT16 psm;

    /** State of the Security Context */
    UCHAR  state;

    /** RFCOMM Server Channel */
    UCHAR  server_ch;

    /** BD_ADDR of the Remote Device */
    UCHAR  bd_addr [BT_BD_ADDR_SIZE];

} SM_CONTEXT_ENTITY;


/** Bottom Half - SM Authorization Reply Parameters */
typedef struct
{
    /** Whether to Accept/Reject the UI Notification Request */
    UCHAR accept_flag;

    /** Event Type - Authorization Request in Mode 2 or 1/3 */
    UCHAR event_type;

    /** BD_ADDR of the Remote Device */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

} SM_UI_BH_PARAMS;

#endif /* SM_HAVE_MODE_2 */


/* ----------------------------------------- Function Declarations */
/** Operations on Security Manager Device Database */
UINT32 sm_create_device_entity
       (
           IN  UCHAR *    bd_addr
       );

UINT32 sm_search_and_add_device_entity
       (
           IN  UCHAR *    bd_addr
       );

UINT32 sm_search_device_entity
       (
           IN  UCHAR *    bd_addr,
           IN  UINT16     acl_handle
       );

void sm_delete_device_entity
     (
         IN  UINT32    indx
     );

void sm_set_device_valid_flag
     (
         IN  UINT32    indx
     );


#ifdef SM_HAVE_MODE_2

/** Operations on Security Manager Service Database */
UINT32 sm_search_service_entity
       (
           IN  UINT16    psm,
           IN  UCHAR     server_ch
       );

#define sm_delete_service_entity(s) \
        (sm_services[(s)].security_attr = SM_FALSE)


/** Operations on Security Manager Access Contexts */
#define sm_delete_security_context(ci) \
        SM_INF(bt_debug_fd, \
        "[SM] Deleting SM Security Access Context ...\n"); \
        SM_INF(bt_debug_fd, \
        "[SM] BD = %02X:%02X:%02X:%02X:%02X:%02X, PSM 0x%04X, Ch 0x%02X\n", \
        sm_contexts[(ci)].bd_addr[0], sm_contexts[(ci)].bd_addr[1], \
        sm_contexts[(ci)].bd_addr[2], sm_contexts[(ci)].bd_addr[3], \
        sm_contexts[(ci)].bd_addr[4], sm_contexts[(ci)].bd_addr[5], \
        sm_contexts[(ci)].psm, sm_contexts[(ci)].server_ch); \
        sm_contexts[(ci)].state = SM_CONTEXT_INACTIVE;


/** SM Security Access Context State Machine */
API_RESULT sm_state_machine
           (
               IN  UCHAR    event,
               IN  UINT32   ci,
               IN  UCHAR    status
           );

API_RESULT sm_context_state_machine
           (
                IN  UCHAR    entry_point,
                IN  UINT32    di,
                IN  UINT32    si,
                IN  UINT32    ci
           );

#ifndef BT_SSP
void sm_authorization_complete_mode_1_3
     (
         IN  UINT32   ci,
         IN  UCHAR    status
     );
#endif /* BT_SSP */
#endif /* SM_HAVE_MODE_2 */

#ifdef BT_SSP
/** To assert if Link Key for an authenticated device is good enough */
UCHAR sm_is_link_key_sufficient
      (
          IN  UINT32    di,
          IN  UINT32    si
      );
#endif /* BT_SSP */


#endif /* _H_SM_INTERNAL_ */


