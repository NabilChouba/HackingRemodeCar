
/**
 *  \file BT_features.h
 *
 *  \brief EtherMind Compilation Switches Configuration File.
 *
 *  This file lists all the Compilation Flags available in various
 *  EtherMind Bluetooth Stack modules.
 */

/* 
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_FEATURES_
#define _H_BT_FEATURES_

/* ----------------------------------------------------------------------- */
/* ======= FreeRTOS =============================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  FREE_RTOS
 *
 *  This flag is used when the EtherMind stack is being compiled on FreeRTOS .
 *
 *  Dependency: None.
 */
#define FREE_RTOS


/* ----------------------------------------------------------------------- */
/* ==== Bluetooth Specification Flags ==================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  BT_1_2
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 1.2.
 *
 *  Currently, this flag only affects the L2CAP and HCI implementation.
 *  The appropriate source code must be chosen for compilation
 *  (the l2cap_1.2 & hci_1.2 source code directories).
 *
 *  The BT_1_2 flag is splitted into BT_HCI_1_2 and BT_L2CAP_1_2 so as to
 *  provide option for application to include Bluetooth v1.2 specific
 *  features for HCI & L2CAP individually.
 *
 *  If BT_1_2 is defined, then both BT_L2CAP_1_2 & HCI_1_2 gets defined
 *  automatically in BT_common.h
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 */
#define BT_L2CAP_1_2
#define BT_HCI_1_2

/* 
 *  BT_2_0_EDR
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification v2.0+EDR.
 *
 *  Currently, this flag only affects the HCI implementation, source code
 *  for which is available under hci_1.2 directory.
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 & BT_HCI_1_2 must be defined.
 */
#define BT_2_0_EDR

/* 
 *  BT_2_1_EDR
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth specification version 2.1+EDR.
 *
 *  Currently, this flag only affects the L2CAP, HCI and SM implementation.
 *  The appropriate source code must be chosen for compilation
 *  (the l2cap_1.2 & hci_1.2 source code directories).
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 *
 *  Both BT_1_2 & BT_2_0_EDR must be defined.
 */
#define BT_2_1_EDR

/* 
 *  BT_CSA_1
 *
 *  This flag is used when the EtherMind stack is being compiled for
 *  Bluetooth Core Specification Addendum 1 (CSA1).
 *
 *  Dependency
 *  ----------
 *  The BT_ARCH_1_2 must be defined, as Bluetooth v1.2 onwards implementation
 *  uses different stack architecture than earlier Bluetooth v1.1
 *  implementation.
 *
 *  The BT_1_2 and BT_2_0_EDR must be defined.
 *  The BT_2_1_EDR may be defined.
 */
/* #define BT_CSA_1 */


/* ----------------------------------------------------------------------- */
/* ==== Bluetooth v2.1+EDR Feature Flags ================================= */
/* ----------------------------------------------------------------------- */
#ifdef BT_2_1_EDR

/* 
 *  BT_EIR
 *
 *  This flag is used to enabling support for Enhanced Inquiry Response (EIR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_EIR

/* 
 *  BT_EPR
 *
 *  This flag is used to enabling support for Encryption Pause/Resume (EPR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_EPR

/* 
 *  BT_EDR
 *
 *  This flag is used to enabling support for Enhanced Data Reporting (EDR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_EDR

/* 
 *  BT_LSTO
 *
 *  This flag is used to enabling support for Link Supervision Timeout (LSTO)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_LSTO

/* 
 *  BT_2_1_QOS
 *  BT_PBF
 *
 *  This flag is used to enabling support for Packet Boundary Flag (PBF) and
 *  Enhanced Flush feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_2_1_QOS

#ifdef BT_2_1_QOS
#define BT_PBF
#define BT_ENHANCED_FLUSH
#endif /* BT_2_1_QOS */

/* 
 *  BT_SSR
 *
 *  This flag is used to enabling support for Sinff Subrating (SSR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_SSR

/* 
 *  BT_SSP
 *
 *  This flag is used to enabling support for Secure Simple Pairing (EIR)
 *  feature of Bluetooth Specification v2.1+EDR.
 *
 *  Dependency: BT_2_1_EDR must be defined.
 */
#define BT_SSP

/* 
 *  BT_SSP_DEBUG
 *
 *  This flag is used to enable support for Simple Pairing Debug Mode
 *  Command, which can be used to configure the Controller to use a predefined
 *  DH private key for SSP to enable debug equipments monitor the data over
 *  an encrypted connection.
 *
 *  Dependency: BT_SSP must be defined.
 */
#ifdef BT_SSP
#define BT_SSP_DEBUG
#endif /* BT_SSP */

#endif /* BT_2_1_EDR */


/* ----------------------------------------------------------------------- */
/* ==== Bluetooth v2.1+EDR Secure Simple Pairing Feature Flags =========== */
/* ----------------------------------------------------------------------- */
#ifdef BT_SSP

/* 
 *  BT_SSP_JW
 *
 *  This flag is used for enabling support for Just Works Association
 *  Model for Secure Simple Pairing feature of Bluetooth v2.1+EDR.
 *
 *  Dependency: BT_SSP must be defined.
 */
/* #define BT_SSP_JW */

/* 
 *  BT_SSP_NC
 *
 *  This flag is used for enabling support for Numeric Comparison Association
 *  Model for Secure Simple Pairing feature of Bluetooth v2.1+EDR.
 *
 *  Dependency: BT_SSP must be defined.
 */
#define BT_SSP_NC

/* 
 *  BT_SSP_UC
 *
 *  This flag is used for enabling support for User Confirmation Request or
 *  Response handling in HCI & SM.
 *
 *  Dependency: either BT_SSP_JW or BT_SSP_NC must be defined.
 */
#if defined BT_SSP_JW || defined BT_SSP_NC
#define BT_SSP_UC
#endif /* BT_SSP_JW || BT_SSP_NC */

#if defined BT_SSP_JW && defined BT_SSP_NC
#error "Please define either BT_SSP_JW or BT_SSP_NC, not both."
#endif /* BT_SSP_JW && BT_SSP_NC */

/* 
 *  BT_SSP_OOB
 *
 *  This flag is used for enabling support for Out of Band Association
 *  Model for Secure Simple Pairing feature of Bluetooth v2.1+EDR.
 *
 *  Dependency: BT_SSP must be defined.
 */
#define BT_SSP_OOB

/* 
 *  BT_SSP_PE
 *
 *  This flag is used for enabling support for Passkey Entry Association
 *  Model for Secure Simple Pairing feature of Bluetooth v2.1+EDR.
 *
 *  Dependency: BT_SSP must be defined.
 */
#define BT_SSP_PE

#endif /* BT_SSP */


/* ----------------------------------------------------------------------- */
/* ==== Bluetooth Seattle Release Feature Flags ========================== */
/* ----------------------------------------------------------------------- */
#ifdef BT_CSA_1

/* 
 *  BT_ENH_L2CAP
 *
 *  This flag enables the Enhanced L2CAP implementation.
 *
 *  Dependency: BT_CSA_1 must be defined.
 */
#define BT_ENH_L2CAP

#endif /* BT_SEATTLE */


/* ----------------------------------------------------------------------- */
/* ==== Stack Architecture Flags ========================================= */
/* ----------------------------------------------------------------------- */
/* 
 *  BT_ARCH_1_2
 *
 *  This flag is used when the EtherMind stack is being compiled for Bluetooth
 *  specification version 1.2 specific architectural changes.
 *
 *  Currently, this flag only affects the L2CAP and HCI implementation,
 *  and HCI-L2CAP internal interface.
 *
 *  If the source code of HCI and L2CAP from directories hci_1.2 and l2cap_1.2
 *  are used, then BT_ARCH_1_2 is mandatory and must be enabled.
 *
 *  If the source code of HCI and L2CAP from directories hci and l2cap are used,
 *  then this flag must not be enabled.
 *
 *  It should be noted that BT_ARCH_1_2 has nothing to do with Bluetooth
 *  Specification v1.2. It only enables architectural and interface change
 *  that is done in EtherMind HCI & L2CAP modules.
 *
 *  The hci_1.2 and l2cap_1.2 source codes are compliant to both Bluetooth
 *  specification version 1.1 and 1.2:
 *  - For Bluetooth v1.2 stack, both BT_ARCH_1_2 & BT_1_2 must be enabled.
 *  - For Bluetooth v1.1 stack, only BT_ARCH_1_2 must be enabled.
 *
 *  Dependency: None.
 */
#define BT_ARCH_1_2

/* 
 *  This flag is used to perform the EtherMind Read Task's job using the
 *  EtherMind Write Task, and hence only one single task is created and used.
 *  For details on EtherMind Tasks and their usage/function, refer to
 *  EtherMind Stack Architecture Document, and EtherMind Programmer's Guide.
 *
 *  Dependency: None.
 */
/* #define BT_TRANSPORT_SINGLE_TASK */


/* ----------------------------------------------------------------------- */
/* ==== HCI Transport Flags ============================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  BT_UART, BT_USB, BT_BCSP, BT_SOCKET, BT_SW_TXP
 *
 *  These flags are used to choose the HCI Transport module for communication
 *  with the Bluetooth hardware, in a hosted Stack.
 *
 *  For some platforms, these flags are defined in Makefiles & Workspaces.
 *  In such cases, these must not be enabled here.
 *
 *  Dependency: None.
 */
#define BT_UART
/* #define BT_USB */
/* #define BT_BCSP */
/* #define BT_SOCKET */
/* #define BT_SW_TXP */

/* 
 *  BT_CUSTOM_SERIAL_SETTINGS
 *
 *  This flag enables application to make use of hci_uart_set_serial_settings()
 *  API (defined in BT_api.h), to configure the COM/Serial Port (and, Baud
 *  Rate) on which the Bluetooth hardware is available or connected.
 *
 *  Dependency: The BT_UART must be defined.
 */
#define BT_CUSTOM_SERIAL_SETTINGS

/* 
 *  HCI_UART_COLLECT_AND_WR_COMPLETE_PKT
 *
 *  This flag enables application to instruct HCI-UART module to recombine
 *  fragments of a HCI Command or Data packet before the same is send over
 *  to the underlying Bluetooth Controller.
 *
 *  Dependency: The BT_UART must be defined.
 */
#ifdef BT_UART
#define HCI_UART_COLLECT_AND_WR_COMPLETE_PKT
#endif /* BT_UART */

#define WT_HCI_COMMAND_DO_NOT_FREE

/* 
 *  HT_DUMP_RX_BUF_ON_SYNC_LOSS
 *
 *  This flag enables dumping the entire transport receive buffer
 *  to help in analyzing when synchronization loss is detected.
 *
 *  Dependency: None.
 */
#define HT_DUMP_RX_BUF_ON_SYNC_LOSS

/* ----------------------------------------------------------------------- */
/* ==== Stack Feature Flags ============================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  BT_DEBUG_ASSERT
 *
 *  This flag enables EtherMind BT_assert() macro, for evaluating truth
 *  value of an expression. For details, refer to BT_assert.h for the
 *  platform.
 *
 *  The BT_assert() macro is used for debugging during development, and
 *  can be excluded in builds for the final product.
 *
 *  Dependency: None.
 */
/* #define BT_DEBUG_ASSERT */

/* 
 *  BT_STATUS
 *
 *  This flag enables EtherMind Status Flag APIs for reporting various
 *  Status, as described in BT_status_api.h. The EtherMind Status APIs
 *  are designed and to be used for debugging purposes during development
 *  and can be excluded in builds for final products.
 *
 *  Dependency: None.
 */
/* #define BT_STATUS */

/* 
 *  BT_DISABLE_MUTEX
 *
 *  This flag disables the use of mutex and conditional variables from all
 *  stack modules (protocols and profiles).
 *  This should be defined in EtherMind builds where synchronization
 *  primitives are not required (such as, single task build of EtherMind).
 *
 *  Dependency: None.
 */
/* #define BT_DISABLE_MUTEX */

/* 
 *  BT_HAVE_STATIC_DECL
 *
 *  This flag enables provision for declaring functions and/or globals in
 *  a file as 'static'.
 */
#define BT_HAVE_STATIC_DECL

/* 
 *  BT_HAVE_CONST_DECL
 *
 *  This flag enables provision for declaring globals in a file as 'const'.
 */
#define BT_HAVE_CONST_DECL

/* 
 * BT_LOG_FIXED_DATA_SIZE
 *
 * This flag enables logging of global data sizes for each module.
 * The logging is only once during the initialization of respective modules.
 */
/* #define BT_LOG_FIXED_DATA_SIZE */

/* 
 * BT_DYNAMIC_MEM_STATISTICS
 *
 * This flag enables dynamic memory statistics collection.
 * The memory statistics will be logged on Bluetooth Off.
 */
/* #define BT_DYNAMIC_MEM_STATISTICS */

/* #define WT_STATS */


/* ----------------------------------------------------------------------- */
/* ==== Module Inclusion Flags for EtherMind Protocols =================== */
/* ----------------------------------------------------------------------- */
/* 
 *  SDP, RFCOMM, TCS, BNEP, AVDTP, AVCTP, OBEX, MCAP
 *
 *  These flags enable initialization and shutdown of various Stack protocol
 *  modules, above L2CAP, and should be defined if any one of them is included
 *  in the build.
 *
 *  Dependency: None.
 */
#define SDP
#define RFCOMM

/* 
 * #define TCS #define BNEP #define AVDTP #define AVCTP #define OBEX #define
 * MCAP */

/* ----------------------------------------------------------------------- */
/* ==== Module Inclusion Flags for EtherMind Profiles ==================== */
/* ----------------------------------------------------------------------- */
/* 
 *  Module inclusion flags for various EtherMind Profiles, and should be
 *  defined according to the modules included in the build.
 *
 *  Dependency: None.
 */
#define SPP
#if 0
#define OPP
#define FTP
#define HSP_UNIT
#define HSP_AG
#define HFP_UNIT
#define HFP_AG
#define DUNP_DT
#define DUNP_GW
#define BIP_INITIATOR
#define BIP_RESPONDER
#define BPP_SENDER
#define BPP_PRINTER
/* #define PAN */
#define CTP_TL
#define CTP_GW
#define ICP
/* #define HID_HOST */
/* #define HID_DEVICE */
/* #define A2DP */
#define A2DP_SOURCE
#define A2DP_SINK
/* #define AVRCP_TG */
/* #define AVRCP_CT */
/* #define HCRP */
/* #define DID_CLIENT */
/* #define DID_SERVER */
/* #define SAP_SERVER */
/* #define SAP_CLIENT */
#define PBAP_PCE
#define PBAP_PSE
#define HDP
#endif /* 0 */

/* ----------------------------------------------------------------------- */
/* ==== HCI Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  HCI_VENDOR_SPECIFIC_COMMANDS
 *
 *  This flag enables HCI module to provide applications the ability to send
 *  vendor-specific HCI commands.
 *
 *  Specifically, the BT_hci_vendor_specific_command() API is available only
 *  if this flag is defined.
 *
 *  Dependency: None.
 */
#define HCI_VENDOR_SPECIFIC_COMMANDS

/* 
 *  HCI_NO_ADD_SCO_CONNECTION
 *
 *  This flag disables support for older HCI Add SCO Connection API & Command
 *  handling in HCI. The newer HCI Setup Synchronous Connection API & Command
 *  should be used instead.
 *
 *  Dependency: None.
 */
/* #define HCI_NO_ADD_SCO_CONNECTION */

/* 
 *  HCI_HOST_CONTROLLER_FLOW_ON
 *
 *  This flag enables HCI module to switch on HCI Host Controller to Host Flow
 *  Control feature offered by the Bluetooth device.
 *
 *  If switched on, this feature will enable HCI to send HCI Host Number of
 *  Completed Packets Command to the Bluetooth Hardware on reception of every
 *  ACL packet.
 *
 *  Dependency: None.
 *
 *  NOTE: This feature is not fully supported.
 */
/* #define HCI_HOST_CONTROLLER_FLOW_ON */

/* 
 *  HCI_TESTING_COMMANDS
 *
 *  This flag enables HCI Testing Commands APIs:
 *  - BT_hci_read_loopback_mode()
 *  - BT_hci_write_loopback_mode()
 *  - BT_hci_enable_device_under_test_mode()
 *
 *  Dependency: None.
 */
#define HCI_TESTING_COMMANDS

/* 
 *  HCI_SCO
 *
 *  This flag enables HCI SCO data packet handling.
 *
 *  Dependency: None.
 */
/* #define HCI_SCO */

/* 
 *  HCI_HAVE_INIT_COMMAND_MASK
 *
 *  This flag enables BT_hci_set_init_command_mask() API.
 *
 *  Using the BT_hci_set_init_command_mask() API, application
 *  can control the HCI commands sent during BT_bluetooth_on()
 *
 *  Dependency: None.
 */
#define HCI_HAVE_INIT_COMMAND_MASK

/* 
 *  HCI_VS_LOCAL_NAME_SHORT_COMMAND
 *
 *  If this flag is enabled then HCI Change Local name and HCI Read Local
 *  name commands are mapped to write and read of the memory location
 *  respectively as specified by the vendor.
 *
 *  Dependency: None.
 */
#define HCI_VS_LOCAL_NAME_SHORT_COMMAND

/* ----------------------------------------------------------------------- */
/* ==== SM Module Specific Flags ========================================= */
/* ----------------------------------------------------------------------- */
/* 
 *  SM_HAVE_MODE_2
 *
 *  This flag is needed if Security Manager (SM) module is required to provide
 *  Bluetooth Security Mode 2 operations.
 *
 *  Dependency: None.
 */
#define SM_HAVE_MODE_2

/* 
 *  SM_STORAGE
 *
 *  This flag enables Security Manager (SM) module to store configuration data
 *  (such as, the Security Mode, default PIN Code, default Authorization Flag
 *  etc.) in persistent storage device, if one is available and configured.
 *  The SM attempts to store the configuration data at the time when SM is
 *  shutdown from the context of BT_bluetooth_off().
 *
 *  On next initialization, during BT_bluetooth_on(), SM reads the stored
 *  configuration data from the persistent storage media.
 *
 *  Dependency: None.
 */
#define SM_STORAGE

/* 
 *  If SSP is defined, then SM_HAVE_MODE_2 must be enabled.
 */
#if defined BT_SSP && !defined SM_HAVE_MODE_2
#error "SSP needs SM_HAVE_MODE_2 to be enabled!"
#endif /* defined BT_SSP && !defined SM_HAVE_MODE_2 */

/* 
 *  SM_IO_CAP_DYNAMIC
 *
 *  This flag enables APIs that allow dynamic configuration of device's
 *  Input/Output Capability, for the purpose of Secure Simple Pairing.
 *  For most platform/devices, this configuration should be commented
 *  as IO Capability is not expected to change over time.
 *
 *  Dependency: BT_SSP must be defined.
 */
#ifdef BT_SSP
#define SM_IO_CAP_DYNAMIC
#endif /* BT_SSP */


/* ----------------------------------------------------------------------- */
/* ==== L2CAP Module Specific Flags ====================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  L2CAP_HAVE_GROUP_SUPPORT
 *
 *  This flag enables the L2CAP Group, and Connectionless (Broadcast) data
 *  transmission and reception in L2CAP module.
 *
 *  Dependency: None.
 */
/* #define L2CAP_HAVE_GROUP_SUPPORT */

/* 
 *  L2CAP_HAVE_PING_INFO_SUPPORT
 *
 *  This flag enables support for sending the "L2CAP_EchoREQ" and
 *  "L2CAP_GetInfoREQ" commands its peer in a remote Bluetooth device.
 *  Specifically, this flag enables the l2ca_ping_req() and l2ca_getinfo_req()
 *  APIs, and the corresponding callback registration and handling.
 *  Refer the EtherMind Stack API Document, Part II, for details.
 *
 *  Dependency: None.
 */
/* #define L2CAP_HAVE_PING_INFO_SUPPORT */

/* 
 *  L2CAP_DATA_TX_NO_WRITE_TASK
 *
 *  If this flag is defined then L2CAP avoids using Write Task for
 *  transmission of its PDUs on explicitly created Write Task's context.
 *  The data transmission is performed as and when the data are queued
 *  in its transmission queue.
 *
 *  On the other hand, if this flag is not defined, L2CAP queues a Bottom Half
 *  function (BH) to Write Task, to trigger the data transmission, as and when
 *  data are queued in its transmission queue. Actual transmission of data is
 *  done when the Write Task calls the queued BH.
 *
 *  [Refer EtherMind Programmer's Guide, for relevant details on the Write
 *  Task and the concept of Bottom Half function]
 *
 *  Dependency: This flag is available only for L2CAP Bluetooth specification
 *  v1.2 implementation. Hence, the BT_ARCH_1_2 must be defined.
 */
/* #define L2CAP_DATA_TX_NO_WRITE_TASK */

/* 
 *  L2CAP_TX_QUEUE_FLOW
 *
 *  This flag enables L2CAP provides additional APIs and processing to
 *  application so that it is possible to know and control buffer occupancy
 *  of the L2CAP Transmission Queue.
 *
 *  Dependency: This flag is available only for L2CAP Bluetooth specification
 *  v1.2 implementation. Hence, the BT_ARCH_1_2 must be defined.
 */
#define L2CAP_TX_QUEUE_FLOW


/* ----------------------------------------------------------------------- */
/* ==== SDP Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  SDP_SERVER
 *
 *  This flag enables the SDP "Server" functionalities. If this flag is
 *  defined, then upper layer profiles, and applications, can register their
 *  "Service Record", and activate/deactivate the same whenever required.
 *  Also, other remote SDP "Client" Bluetooth devices will be able to search
 *  and find the services (profiles) available on the local device,
 *  and their attributes and parameters.
 *
 *  Dependency: The SDP Database module specific files must be included
 *  in the build.
 */
#define SDP_SERVER

/* 
 *  SDP_CLIENT
 *
 *  This flag enables the SDP "Client" functionalities. If this flag is
 *  defined, then upper layer profiles, and applications, will be able to
 *  query for services, and their attributes and parameters, on other remote
 *  Bluetooth devices having SDP "Server" functionalities.
 *
 *  All the SDP APIs, which is described in EtherMind Stack API, Part II,
 *  is available only if this flag is defined.
 *
 *  Dependency: None.
 */
#define SDP_CLIENT

/* 
 *  SDP_HAVE_HANDLE_WITH_CB
 *
 *  If defined, this flag enables an extra parameter in SDP Callback to
 *  application to send the SDP_HANDLE parameter. This helps in identifying
 *  the remote Bluetooth device (using BD_ADDR) when multiple SDP requests
 *  are initiated to multiple Bluetooth devices at the same time.
 *
 *  Dependency: None
 */
/* #define SDP_HAVE_HANDLE_WITH_CB */

/* 
 *  SDP_CLIENT_HAVE_PARTIAL_SSA_SUPPORT
 *
 *  This flag is used to control the behaviour of SDP Client on receiving
 *  Service Search Attribute (SSA) Response.
 *
 *  If this flag is defined, SDP Client will pass the response received
 *  from the peer (even if it is a partial response) to the application.
 *
 *  After receiving the partial response, application can choose to
 *  continue with the same SSA request by invoking the SDP SSA API again
 *  or can call some other SDP APIs like Close or Service Search
 *  or Attribute Search.
 *
 *  If this flag is not defined, SDP Client will take care of the partial
 *  response received from the peer and try to pack the entire response
 *  received through multiple SSA responses into the buffer provided by
 *  the application through SDP SSA request API.
 *
 *  Note: By default this flag should be disabled.
 *
 *  Dependency: SDP_CLIENT must be defined.
 */
/* #define SDP_CLIENT_HAVE_PARTIAL_SSA_SUPPORT */

#ifdef SDP_SERVER
/* 
 *  SDP_SERVER_HAVE_CB_IND_SUPPORT
 *
 *  If application wants to get notified when a peer device tries to
 *  perform SDP query with the local device and when the peer initiated
 *  SDP operation is complete, this flag shall be enabled and application
 *  shall register a callback with SDP Server using the SDP Server API
 *  "BT_sdp_server_register_ind_cb()".
 *
 *  If this flag is defined, and the application has registed callback
 *  using the above mentioned API, SDP Server will indicate following
 *  events to application:
 *  - SDP_SERVER_CONNECT_IND   :
 *    When a peer device establishes L2CAP connection
 *    with local SDP Server
 *
 *  - SDP_SERVER_DISCONNECT_IND:
 *    When the already existing L2CAP connection
 *    with local SDP server is dropped
 *
 *  Note: With both the above event indications, the SDP Server Callback
 *  will also provide the 'event_data' parameter which will contain
 *  the bluetooth device address of the peer device and the 'event_datalen'
 *  parameter which will be the length of the bluetooth device address
 *  of the peer device (i.e. 6 octet).
 *
 *  Dependency: SDP_SERVER must be defined.
 */
#define SDP_SERVER_HAVE_CB_IND_SUPPORT
#endif /* SDP_SERVER */


/* ----------------------------------------------------------------------- */
/* ==== RFCOMM Module Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  RFCOMM_ENABLE_SUPPORT_APIS
 *
 *  This compilation flag enables additional RFCOMM APIs for opening and
 *  closing an RFCOMM Session and DLC, as defined in rfcomm_support.h.
 *  These APIs are not defined in BT_rfcomm_api.h. Hence, these APIs are not
 *  directly available for applications to use. Currently, these APIs are only
 *  used in various RFCOMM test cases (such as, UPFs).
 *
 *  Dependency: None.
 */
/* #define RFCOMM_ENABLE_SUPPORT_APIS */

/* 
 *  RFCOMM_CREDIT_TX_USE_BH
 *
 *  This flag enables implementation of RFCOMM Credit Transmission related
 *  API function to make use of Write Task (and, Bottom Halves) for carrying
 *  out core work of sending and updating Credit information.
 *
 *  Dependency: None.
 */
#define RFCOMM_CREDIT_TX_USE_BH

/* TBD: Move to correct place */
#define BT_WT_NO_STATIC_BUFFER

/* 
 *  RFCOMM_ENABLE_FC_FROM_APP
 *
 *  This flag enables control for RFCOMM Credit Based Flow Control from
 *  application.
 *
 *  Dependency: None.
 */
#define RFCOMM_ENABLE_FC_FROM_APP

/* 
 *  RFCOMM_DATA_TX_NO_WRITE_TASK
 *
 *  If this flag is defined then RFCOMM avoids using Write Task for
 *  transmission of its PDUs on explicitly created Write Task's context.
 *  The data transmission is performed as and when the data are queued
 *  in its transmission queue.
 *
 *  On the other hand, if this flag is not defined, RFCOMM queues a Bottom Half
 *  function (BH) to Write Task, to trigger the data transmission, as and when
 *  data are queued in its transmission queue. Actual transmission of data is
 *  done when the Write Task calls the queued BH.
 *
 *  [Refer EtherMind Programmer's Guide, for relevant details on the Write
 *  Task and the concept of Bottom Half function]
 *
 */
/* #define RFCOMM_DATA_TX_NO_WRITE_TASK */

/* 
 *  RFCOMM_HAVE_WRITE_QUEUE
 *
 *  This flag enables RFCOMM to handle multiple Writes from application layer
 *  without waiting for the confirmation of previous RFCOMM Writes initiated
 *  by the same application.
 *
 */
/* #define RFCOMM_HAVE_WRITE_QUEUE */

/* 
 *  RFCOMM_USE_L2CAP_ERTM
 *
 *  This flag enables RFCOMM to use L2CAP's Enhanced Retransmission Mode,
 *  instead of default Basic Mode.
 *
 *  Dependency: BT_ENH_L2CAP must be defined.
 */
#ifdef BT_ENH_L2CAP
/* #define RFCOMM_USE_L2CAP_ERTM */
#endif /* BT_ENH_L2CAP */


/* ----------------------------------------------------------------------- */
/* ==== TCS Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  TCS_CTP
 *
 *  This flag enables support for CTP in TCS.
 *
 *  Dependency: Either CTP_TL or CTP_GW must be defined.
 */
#if defined CTP_TL || defined CTP_GW
#define TCS_CTP
#endif /* CTP_TL || CTP_GW */

/* 
 *  TCS_ICP
 *
 *  This flag enables support for ICP in TCS.
 *
 *  Dependency: ICP must be defined.
 */
#ifdef ICP
#define TCS_ICP
#endif /* ICP */


/* ----------------------------------------------------------------------- */
/* ==== BNEP Module Specific Flags ======================================= */
/* ----------------------------------------------------------------------- */
/* 
 *  BNEP_WRITE_NO_WRITE_TASK
 *
 *  This flag disables use of Write Task while writing data over BNEP.
 *
 *  Dependency: None.
 */
#define BNEP_WRITE_NO_WRITE_TASK


/* ---------------------------------------------------------------------- */
/* ==== AVDTP Content Protection Flag =================================== */
/* ---------------------------------------------------------------------- */
/* 
 *  AVDTP_HAVE_CONTENT_PROTECTION
 *
 *  This flag is used to enable the content protection feature.
 *
 *  Dependency: None.
 */
/* #define AVDTP_HAVE_CONTENT_PROTECTION */

/* 
 *  AVDTP_HAVE_MULTIPLEXING
 *
 *  This flag is used to enable the multiplexing feature.
 *
 *  Dependency: None.
 */
#define AVDTP_HAVE_MULTIPLEXING


/* ----------------------------------------------------------------------- */
/* ==== OBEX Module Specific Flags ======================================= */
/* ----------------------------------------------------------------------- */
/* 
 *  OBEX_SERVER
 *
 *  This flag enables the OBEX "Server" functionalities.
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 */
#define OBEX_SERVER

/* 
 *  OBEX_CLIENT
 *
 *  This flag enables the OBEX "Client" functionalities.
 *
 *  Dependency: The 'OBEX' flag must be enabled.
 */
#define OBEX_CLIENT


/* ----------------------------------------------------------------------- */
/* ==== MCAP Module Specific Flags ======================================= */
/* ----------------------------------------------------------------------- */
/* 
 *  MCAP_STANDARD_COMMANDS
 *
 *  This flag enables support for Standard commands in MCAP.
 *
 *  Dependency: The 'MCAP' flag must be enabled.
 */
#define MCAP_STANDARD_COMMANDS

/* 
 *  MCAP_CLOCK_SYNC
 *
 *  This flag enables support for Clock Synchronization in MCAP.
 *
 *  Dependency: The 'MCAP' flag must be enabled.
 */
#define MCAP_CLOCK_SYNC

/* 
 *  MCAP_STORAGE
 *
 *  This flag enables MCAP to store configuration data for established
 *  MCAP Data Links (MDLs), so that the configuration information could
 *  be re-used during reconnection.
 *
 *  Dependency: The 'MCAP' flag must be enabled.
 */
#define MCAP_STORAGE


/* ----------------------------------------------------------------------- */
/* ==== HFP Unit Module Specific Flags =================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  HFP_UNIT_1_5
 *
 *  This flag enables HFP-1.5 features and API functions for HFP Unit module.
 *
 *  Dependency: None.
 */
#define HFP_UNIT_1_5


/* ----------------------------------------------------------------------- */
/* ==== HFP AG Module Specific Flags ===================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  HFP_1_5_RSP
 *
 *  This flag enables handling of HFP-1.5 commands in HFP-AG application.
 *
 *  Dependency: None.
 */
#define HFP_1_5_RSP


/* ----------------------------------------------------------------------- */
/* ==== AVRCP Module Specific Flags ====================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  AVRCP_1_3
 *  This flag enable the AVRCP 1.3 specific features in the
 *  EtherMind AVRCP module.
 */
#define AVRCP_1_3


/* ----------------------------------------------------------------------- */
/* ==== OPP Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  OPP_EXTENDED_CALLBACK
 *
 *  If defined, this flag enables extra OBEX Header parameters in OPP Callback
 *  to the application. Application can then parse the received OBEX headers
 *  from the peer and extract information, which is not readily available
 *  from OPP implementation.
 *
 *  Dependency: None
 */
#define OPP_EXTENDED_CALLBACK

/* ----------------------------------------------------------------------- */
/* ==== FTP Module Specific Flags ======================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  FTP_EXTENDED_CALLBACK
 *
 *  If defined, this flag enables extra OBEX Header parameters in FTP Callback
 *  to the application. Application can then parse the received OBEX headers
 *  from the peer and extract information, which is not readily available
 *  from FTP implementation.
 *
 *  Dependency: None
 */
#define FTP_EXTENDED_CALLBACK


/* ----------------------------------------------------------------------- */
/* ==== EtherMind LITE Stack Specific Flags ============================== */
/* ----------------------------------------------------------------------- */
/* 
 *  This flag is used to choose the LITE version of EtherMind Stack.
 *  At present, this flag affects HCI, L2CAP, SDP and RFCOMM Modules.
 *  This flag helps in removing unnecessary APIs and related fuctions
 *  for specific application (such as, Mono Headset Build).
 *
 *  Dependency: None.
 */
/* #define BT_LITE */

/* If BT_LITE is defined, choose features from below */
#ifdef BT_LITE

/* Common LITE Feature Flags */
#define BT_NO_BLUETOOTH_OFF
#define TIMER_NO_RESTART_TIMER

/* HCI LITE Feature Flags */
#define HCI_LITE
#define HCI_NO_SYNC_LOST_IND

/* SM LITE Feature Flags */
#define SM_LITE

/* L2CAP LITE Feature Flags */
#define L2CAP_NO_UNREGISTER_PSM

/* SDP LITE Feature Flags */
#define SDP_NO_SS_REQ
#define SDP_NO_SA_REQ
#define SDP_NO_GET_RECORD_HANDLE
#define SDP_NO_GET_PTR_2_NEXT_ATTR_LIST
#define SDP_NO_GET_SERVICE_RECORD_HANDLE
#define SDP_NO_GET_SERVICE_CLASS_ID_LIST
#define SDP_NO_GET_SERVICE_RECORD_STATE
#define SDP_NO_GET_SERVICE_ID
#define SDP_NO_GET_BROWSE_GROUP_LIST
#define SDP_NO_GET_LANG_BASE_ATTR_ID_LIST
#define SDP_NO_GET_SERVICE_INFO_TIME_TO_LIVE
#define SDP_NO_GET_SERVICE_AVAILABILITY
#define SDP_NO_GET_BT_PROFILE_DESC_LIST
#define SDP_NO_GET_DOCUMENTATION_URL
#define SDP_NO_GET_CLIENT_EXECUTABLE_URL
#define SDP_NO_GET_ICON_URL
#define SDP_NO_GET_SERVICE_NAME
#define SDP_NO_GET_SERVICE_DESCRIPTION
#define SDP_NO_GET_PROVIDER_NAME
#define SDP_NO_GET_VERSION_NUMBER_LIST
#define SDP_NO_GET_SERVICE_DATABASE_STATE
#define SDP_NO_GET_GROUP_ID
#define SDP_NO_GET_ADDITIONAL_CHANNEL
#define SDP_NO_GET_PSM
#define SDP_NO_GET_ADDITIONAL_PSM

#define SDP_NO_GET_UINT32_ATTRIBUTE_VALUE
#define SDP_NO_GET_UUID_ATTRIBUTE_VALUE
#define SDP_NO_GET_STRING_ATTRIBUTE_VALUE
#define SDP_NO_GET_INT_ATTRIBUTE_VALUE

/* SDP Databse LITE Feature Flags */
#define DB_NO_INACTIVATE_RECORD

/* RFCOMM LITE Feature Flags */
#define RFCOMM_NO_QUERY_STATE
#define RFCOMM_NO_SEND_PN
#define RFCOMM_NO_SEND_RPN
#define RFCOMM_NO_SEND_MSC
#define RFCOMM_NO_SEND_RLS
#define RFCOMM_NO_SEND_TEST
#define RFCOMM_NO_SEND_FC
#define RFCOMM_NO_GET_LOCAL_RPN
#define RFCOMM_NO_SET_LOCAL_RPN
#define RFCOMM_NO_GET_LOCAL_PN
#define RFCOMM_NO_GET_LOCAL_MSC

/* HFP Unit LITE Feature Flags */
#define HFP_UNIT_NO_STOP

/* HSP Unit LITE Feature Flags */
#define HSP_UNIT_NO_STOP

#endif /* BT_LITE */


/* ----------------------------------------------------------------------- */
/* ==== Debug Specification Flags ======================================== */
/* ----------------------------------------------------------------------- */
/* 
 *  Debug definitions for all the layers (protocol and profiles) should be
 *  defined in this section.
 */

/* 
 * By default, the Error Logs of all the layers are enabled.
 * To disable error logging of a module, define <module>_NO_DEBUG flag.
 * Example: Define HCI_NO_DEBUG to disable error logging of HCI layer.
 *
 * By default, the Trace, Information, Data and other Logs
 * of all the layers are disabled.
 * To enable debug logging of a module, define <module>_DEBUG flag.
 * Example: Define HCI_DEBUG to enable debug logging (Trace and Information)
 * of HCI layer.
 */

/* Protocol Modules */
/* #define AVCTP_NO_DEBUG */
/* #define AVCTP_DEBUG */

/* #define AVDTP_NO_DEBUG */
/* #define AVDTP_DEBUG */
/* #define AVDTP_DATA_DEBUG */

/* #define BCSP_NO_DEBUG */
/* #define BCSP_DEBUG */
/* #define BCSP_LINK_DEBUG */
/* #define BCSP_DATAGRAM_DEBUG */
/* #define BCSP_SEQUENCE_DEBUG */
/* #define BCSP_MUX_DEBUG */
/* #define BCSP_INTEGRITY_DEBUG */
/* #define BCSP_SLIP_DEBUG */

/* #define BNEP_NO_DEBUG */
/* #define BNEP_DEBUG */
/* #define BNEP_DATA_DEBUG */

/* #define DB_NO_DEBUG */
/* #define DB_DEBUG */

/* #define HCI_NO_DEBUG */
/* #define HCI_DEBUG */
/* #define HCI_DDEBUG */

/* #define HT_NO_DEBUG */
/* #define HT_DEBUG */
/* #define HT_INTR_DEBUG */

/* #define MCAP_NO_DEBUG */
/* #define MCAP_DEBUG */

/* #define OBEX_NO_DEBUG */
/* #define OBEX_DEBUG */

/* #define L2CAP_NO_DEBUG */
/* #define L2CAP_DEBUG */
/* #define L2CAP_LP_DEBUG */

/* #define RFCOMM_NO_DEBUG */
/* #define RFCOMM_DEBUG */

/* #define SDP_NO_DEBUG */
/* #define SDP_DEBUG */

/* #define SEC_NO_DEBUG */
/* #define SEC_DEBUG */

/* #define TCS_NO_DEBUG */
/* #define TCS_DEBUG */

/* #define WT_NO_DEBUG */
/* #define WT_DEBUG */

/* Profile Modules */
/* #define A2DP_NO_DEBUG */
/* #define A2DP_DEBUG */
/* #define A2DP_DATA_DEBUG */

/* #define AVRCP_NO_DEBUG */
/* #define AVRCP_DEBUG */

/* #define BIP_NO_DEBUG */
/* #define BIP_DEBUG */

/* #define BPP_NO_DEBUG */
/* #define BPP_DEBUG */

/* #define CTP_GW_NO_DEBUG */
/* #define CTP_GW_DEBUG */
/* #define CTP_GW_DATA_DEBUG */

/* #define CTP_TL_NO_DEBUG */
/* #define CTP_TL_DEBUG */
/* #define CTP_TL_DATA_DEBUG */

/* #define DUNP_GW_NO_DEBUG */
/* #define DUNP_GW_DEBUG */
/* #define DUNP_GW_DATA_DEBUG */

/* #define DUNP_DT_NO_DEBUG */
/* #define DUNP_DT_DEBUG */
/* #define DUNP_DT_DATA_DEBUG */

/* #define FTP_NO_DEBUG */
/* #define FTP_DEBUG */

/* #define HCRP_NO_DEBUG */
/* #define HCRP_DEBUG */

/* #define HFP_AG_NO_DEBUG */
/* #define HFP_AG_DEBUG */
/* #define HFP_AG_DATA_DEBUG */

/* #define HFP_UNIT_NO_DEBUG */
/* #define HFP_UNIT_DEBUG */

/* #define HID_HOST_NO_DEBUG */
/* #define HID_HOST_DEBUG */

/* #define HID_DEV_NO_DEBUG */
/* #define HID_DEV_DEBUG */

/* #define HSP_AG_NO_DEBUG */
/* #define HSP_AG_DEBUG */
/* #define HSP_AG_DATA_DEBUG */

/* #define HSP_UNIT_NO_DEBUG */
/* #define HSP_UNIT_DEBUG */

/* #define ICP_NO_DEBUG */
/* #define ICP_DEBUG */
/* #define ICP_DATA_DEBUG */

/* #define OPP_NO_DEBUG */
/* #define OPP_DEBUG */

/* #define PAN_NO_DEBUG */
/* #define PAN_DEBUG */
/* #define PAN_DATA_DEBUG */

/* #define PBAP_NO_DEBUG */
/* #define PBAP_DEBUG */

/* #define SYNCP_NO_DEBUG */
/* #define SYNCP_DEBUG */
/* #define SYNCP_DATA_DEBUG */

/* #define UDIP_NO_DEBUG */
/* #define UDIP_DEBUG */
/* #define UDIP_DATA_DEBUG */

#endif /* _H_BT_FEATURES_ */
