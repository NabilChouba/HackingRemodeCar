
/**
 * Copyright (C) 2010 MindTree Ltd. All rights reserved.
 * \file    sdk_bluetooth_config.h
 * \brief   This header file contains configuration parameters common to all the
 *          packages
 */

#ifndef _H_SDK_BLUETOOTH_COMMON_CONFIG_
#define _H_SDK_BLUETOOTH_COMMON_CONFIG_

/* Max length of Remote device name */
#define SDK_REM_BT_DEV_NAME_MAX_LEN          17

/**
 * Default PIN Code, this pin code is used if authentication procedure takes
 * place with a remote BT device that doesn't support SSP (Secure Simple
 * Pairing), but if both devices support SSP then this parameter is ignored.
 */
#define SDK_CONFIG_PIN                      "0000"

/* ACL packet type to be used while initiating connection */
#define SDK_CONFIG_ACL_PKT_TYPE           (LMP_ACL_DM1|LMP_ACL_DH1|LMP_ACL_DM3|\
                                          LMP_ACL_DH3|LMP_ACL_DM5|LMP_ACL_DH5)

                                 /* Bluetooth Link Policy Settings */
#define SDK_CONFIG_LINK_POLICY_SETTINGS     0x0005

/* HCI Inquiry Parameters */
#define SDK_NUM_RESPONSES                   0x00

/**
 * HCI Inquiry Scan Parameter, additional inquiry scan access codes can be
 * included in the list by adding a comma and the access code.
 */
#define SDK_INQUIRY_SCAN_LAP                BT_GIAC, BT_LIAC

/* Maximum number of devices discovered during Inquiry */
#define SDK_INQ_MAX_DEVICES                 0x07

/* SDK Class of Device Field */
#define SDK_CONFIG_COD               BT_MSC_LIM_DISC_MODE|BT_MDC_TOY|BT_TMC_GAME

/* SDK Input Output Capability */
/* Note: The default value (No Input No Output) uses "Just Works" association
 * scenarios. */
#define SDK_IO_CAPABILITY                   SM_IO_CAPABILITY_NO_INPUT_NO_OUTPUT

/* 
 * Port address of peer SPP de-vice, to be used only if the ap-plication needs
 * to initiate con-nection with a fixed SPP de-vice on this fixed port.
 * It is not necessary that if SDK_CONFIG_SPP_PEER_BD_ADDR is defined
 * then this configuration parameter is also defined.
 */
#define SDK_CONFIG_SPP_PEER_SERVER_CHANNEL   0x01

/* SPP Service Authentication level */
#define SDK_CONFIG_SPP_AUTHENTICATION_LEVEL  SM_SERVICE_SECURITY_LEVEL_2
/* SPP Service Authorization Requirements */
#define SDK_CONFIG_SPP_AUTHORIZATION_REQ     SM_SERVICE_AUTHORIZATION_NONE
/* SPP Service Bonding Requirements */
#define SDK_CONFIG_SPP_BONDING_REQ           0x00


/* SDP query buffer length for SPP */
#define SDK_SPP_ATTRIB_DATA_LEN              32

/**
 *  Output Power level related configuration.
 *  supported output power range for SDK_MAX_OUTPUT_POWER_LEVEL is
 *  from 0dBm to 12dBm in steps of 0.5dBm, and supported output power
 *  range for SDK_MAX_LE_ANT_OUTPUT_POWER_LEVEL is from 0dBm to 10dBm
 *  in steps of 0.5dBm.
*/
#define SDK_MAX_OUTPUT_POWER_LEVEL          OP_POWER_6_0
#define SDK_MAX_LE_ANT_OUTPUT_POWER_LEVEL   OP_POWER_10_0


/**
 * UART Related Configuration Parameters
 */
/* Note : The default baud rate for all frequencies is 115200 bps. 921600 bps
 * is supported only with 25MHz CPU clock. */
#define BT_UART_CONFIG_BAUDRATE             SDK_BAUDRATE_115200

/**
 * MSP430 Related Configuration Parameters
 */
/* Define the system clock for MSP430 */
/* Supported CPU clocks: 8MHz, 12MHz, 18MHz, 25MHz */
/* Note 1: 25MHz is not supported on F5438 device */
/* Note 2: See baudrate section above for supported baudrate-CPU combinations */
#define SYSTEM_CLK                          SYSCLK_18MHZ

/* Define the inactivity period for MSP430 to enter LPM */
#define INACTIVITY_TIMEOUT                  30

/* Enable MSP430 LPM feature */
#define MSP430_LPM_ENABLE

/* Define the low power mode for MSP430 */
#define SDK_MSP430_LPM                      MSP430_LPM_3


/* Macros for the debug messages */
#ifdef DEBUG_TESTING
extern void testAction(void);
#define debugAction()                       testAction()
#define SDK_DEBUG_MODULE_INIT()             halUsbInit()
#ifdef __IAR_SYSTEMS_ICC__
#define SDK_DEBUG_PRINT_STRING(...)         halUsbSendString(__VA_ARGS__);   \
                                            halUsbSendChar('\n')
#define SDK_DEBUG_PRINT_CHAR(...)           halUsbSendChar(__VA_ARGS__)
#else /* __IAR_SYSTEMS_ICC__ */
#define SDK_DEBUG_PRINT_STRING              halUsbSendString
#define SDK_DEBUG_PRINT_CHAR                halUsbSendChar
#endif /* __IAR_SYSTEMS_ICC__ */
#else /* DEBUG_TESTING */
#define debugAction()
#define SDK_DEBUG_MODULE_INIT()
#ifdef __IAR_SYSTEMS_ICC__
#define SDK_DEBUG_PRINT_STRING(...)
#define SDK_DEBUG_PRINT_CHAR(...)
#else /* __IAR_SYSTEMS_ICC__ */
#define SDK_DEBUG_PRINT_STRING
#define SDK_DEBUG_PRINT_CHAR
#endif /* __IAR_SYSTEMS_ICC__ */
#endif /* DEBUG_TESTING */

#endif /* _H_SDK_BLUETOOTH_COMMON_CONFIG_ */
