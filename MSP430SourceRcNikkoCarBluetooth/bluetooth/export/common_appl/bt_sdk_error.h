/* 
 *  Copyright (C) 2009 MindTree Ltd.
 *  All rights reserved.
 */

/**
 *  @file bt_sdk_error.h
 *
 *  This header file contains error codes used by the library and code
 *  very closely related to bluetooth core functionality.
 */

#ifndef _H_BT_SDK_ERROR_
#define _H_BT_SDK_ERROR_

#define SDK_ERROR_CODE_VAL                                       0x00
/* Memory Allocation Failure during Pool Creation */
#define SDK_POOL_MEM_ALLOC_FAIL             SDK_ERROR_CODE_VAL + 0x01
/* Collision Case 2 in ehcill_tx_handler */
#define SDK_EHCILL_COLLISION_CASE_2_TX      SDK_ERROR_CODE_VAL + 0x02
/* Collision Case 2 in ehcill_rx_handler */
#define SDK_EHCILL_COLLISION_CASE_2_RX      SDK_ERROR_CODE_VAL + 0x03
/* Default Case in ehcill_tx_handler */
#define SDK_EHCILL_TX_DEFAULT_CASE          SDK_ERROR_CODE_VAL + 0x04
/* Default Case in ehcill_rx_handler" */
#define SDK_EHCILL_RX_DEFAULT_CASE          SDK_ERROR_CODE_VAL + 0x05
/* Error in User Task Routine - Default case */
#define SDK_EHCILL_USER_TASK_DEFAULT_CASE   SDK_ERROR_CODE_VAL + 0x06
/* Specified Baud rate not supported */
#define SDK_BAUDRATE_NOT_SUPPORTED          SDK_ERROR_CODE_VAL + 0x07
/* UART Framing Error */
#define SDK_UART_FRAMING_ERROR              SDK_ERROR_CODE_VAL + 0x08
/* UART Parity Error */
#define SDK_UART_PARITY_ERROR               SDK_ERROR_CODE_VAL + 0x09
/* UART OverFlow Error1 */
#define SDK_UART_OVERFLOW_ERROR             SDK_ERROR_CODE_VAL + 0x0A
/* UART Error */
#define SDK_UART_ERROR                      SDK_ERROR_CODE_VAL + 0x0B
/* Error in Sem Give of Read Sempahore */
#define SDK_ERROR_IN_READ_SEM_GIVE          SDK_ERROR_CODE_VAL + 0x0C
/* Error in Header First Byte */
#define SDK_ERROR_IN_HEADER_FIRST_BYTE      SDK_ERROR_CODE_VAL + 0x0D
/* Error in Header */
#define SDK_ERROR_IN_HEADER                 SDK_ERROR_CODE_VAL + 0x0E
/* Error in Data Reception */
#define SDK_ERROR_IN_DATA_RECV              SDK_ERROR_CODE_VAL + 0x0F
/* Error in hci_local_name_length */
#define SDK_ERROR_IN_HCI_LOCAL_NAME_LEN     SDK_ERROR_CODE_VAL + 0x10
/* Invalid output power Error */
#define SDK_SET_OUTPUT_POWER_ERROR          SDK_ERROR_CODE_VAL + 0x11
/* Invalid LE/ANT power Error */
#define SDK_LE_ANT_POWER_CHECK              SDK_ERROR_CODE_VAL + 0x12
/* Check compatibility between init script and silicon version */
#define SDK_INITSCRIPT_COMPAT_CHECK         SDK_ERROR_CODE_VAL + 0x13
/* Check the hardware manufacturer */
#define SDK_HW_MANUFACT_ERROR               SDK_ERROR_CODE_VAL + 0x14
/* Received a wrong event */
#define SDK_WRONG_EVENT_RECEIVED            SDK_ERROR_CODE_VAL + 0x15
/* Memory Allocation Failure */
#define SDK_BT_MEM_ALLOC_FAIL               SDK_ERROR_CODE_VAL + 0x16
/* Error in SemTake of WriteSem */
#define SDK_ERR_IN_WRITE_SEMTAKE            SDK_ERROR_CODE_VAL + 0x17
/* Received HCI hardware error event */
#define SDK_HARDWARE_ERROR_RECV             SDK_ERROR_CODE_VAL + 0x18
/* Received a failed status for an event during BT Init Sequence */
#define SDK_WRONG_STATUS_IN_INIT_SEQ        SDK_ERROR_CODE_VAL + 0x19
/* Received an unexpected event during BT Init Sequence */
#define SDK_UNEXPECTED_EVENT_IN_INIT_SEQ    SDK_ERROR_CODE_VAL + 0x1A
/* Wrong power level selected during init sequence */
#define SDK_WRONG_OUTPUT_POWER_LEVEL        SDK_ERROR_CODE_VAL + 0x1B
/* Received a command disallowed error during init sequence */
#define SDK_COMMAND_DISALLOWED              SDK_ERROR_CODE_VAL + 0x1C
/* Received generic application error */
#define SDK_APP_GENERIC_ERROR               SDK_ERROR_CODE_VAL + 0x1D
#endif /* _H_BT_SDK_ERROR_ */
