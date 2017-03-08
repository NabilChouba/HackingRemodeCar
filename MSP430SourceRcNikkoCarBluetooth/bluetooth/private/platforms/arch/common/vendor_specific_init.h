
/**
 * Copyright (c) 2009 MindTree Ltd.  All rights reserved.
 * \file    vendor_specific_init.h
 * \brief   This file contains definitions related to vendor specific init script
 */



#ifndef _H_VENDOR_SPECIFIC_INIT_
#define _H_VENDOR_SPECIFIC_INIT_

/* Header File Inclusion */
#include "BT_common.h"

/* Global Definitions */

/* Macros */
#define V_INIT_FALSE                                        0
#define V_INIT_IN_PROGRESS                                  1
#define V_INIT_COMPLETE                                     2


/* defintions for BT controller vendor specific intialization state machine */
#define V_SET_INIT_SCRIPT_UART_BAUDRATE                     3
#define V_SET_INIT_SCRIPT_UART_BAUDRATE_COMPLETE            4

/* BT Controller vendor specific command opcode to update the uart baudrate */
#define SDK_BT_RF_UPDATE_UART_BAUDRATE_OPCODE               0xFF36
#define SDK_BT_RF_UPDATE_UART_BAUDRATE_COMMAND_LEN          0x04

/* Vendor SPecific command to configure BT Controller ACL Data buffers */
#define SDK_BT_RF_CONFIGURE_ACL_DATA_BUFFERS_OPCODE         0xFD7C
#define SDK_BT_RF_CONFIGURE_ACL_DATA_BUFFERS_COMMAND_LEN    0x6

/* BT controller vendor specific command to configure sleep mode */
#define SDK_BT_RF_SET_SLEEP_MODE                            0xFD0C
#define SDK_BT_RF_CONFIG_SLEEP_MODE                         0xFD2B

/* BT controller vendor specific command opcode to set the controller BD
 * Address */
#define SDK_BT_RF_SET_BD_ADDR_OPCODE                        0xFC06
#define SDK_BT_RF_SET_BD_ADDR_COMMAND_LEN                   0x06

/* Set MAX Power Opcodes */
#define SDK_BT_RF_SET_POWER_VECTOR                          0xFD82
#define SDK_BT_RF_SET_CLASS_2_SINGLE_POWER                  0xFD87
/* Enable Calibration Opcodes */
#define SDK_BT_RF_SET_CALIBRATION                           0xFD76
#define SDK_BT_RF_ENABLE_CALIBRATION                        0xFD80

/* To read the version of the controller */
#define HCI_VS_GET_SYSTEM_STATUS_OPCODE                     0xFE1F

/* Read/Write Memory block command opcodes */
#define SDK_VS_READ_MEMORY_BLOCK                            0xFF04
#define SDK_VS_WRITE_MEMORY_BLOCK                           0xFF05

/* REROUTING OPCODES */
#define HCI_VS_CONFIGURE_ARM_IO_OPCODE                      0xFF1A
#define HCI_VS_WRITE_ARM_IO_PORT_OPCODE                     0xFF1B
#define HCI_VS_READ_MODIFY_WRITE_HARDWARE_REGISTER_OPCODE   0xFD09

/* REROUTING OPCODE LENGTHS */
#define HCI_VS_CONFIGURE_ARM_IO_LEN                         0x02
#define HCI_VS_WRITE_ARM_IO_PORT_LEN                        0x02
#define HCI_VS_READ_MODIFY_WRITE_HARDWARE_REGISTER_LEN      0x08
/* power table macro's */
#define MAX_POWER_LEVELS                                    OP_POWER_12_0 + 1
#define POWER_PARAM_SIZE                                    19
#define TOTAL_POWER_PARAM_SIZE                              POWER_PARAM_SIZE + 1
#define SINGLE_POWER_PARAM_SIZE                             3
#define MODULATION_INDEX                                    0
/* Internal Functions */

/* API Declarations */

#ifdef VS_INIT_DEBUG

#define VS_INIT_TRC BT_debug_trace
#define VS_INIT_INF BT_debug_info
#define VS_INIT_ERR BT_debug_error

#else

#define VS_INIT_TRC BT_debug_null
#define VS_INIT_INF BT_debug_null
#define VS_INIT_ERR BT_debug_null

#endif /* VS_INIT_DEBUG */

/* Enumeration for output power level */
typedef enum {
    OP_POWER_0_0,
    OP_POWER_0_5,
    OP_POWER_1_0,
    OP_POWER_1_5,
    OP_POWER_2_0,
    OP_POWER_2_5,
    OP_POWER_3_0,
    OP_POWER_3_5,
    OP_POWER_4_0,
    OP_POWER_4_5,
    OP_POWER_5_0,
    OP_POWER_5_5,
    OP_POWER_6_0,
    OP_POWER_6_5,
    OP_POWER_7_0,
    OP_POWER_7_5,
    OP_POWER_8_0,
    OP_POWER_8_5,
    OP_POWER_9_0,
    OP_POWER_9_5,
    OP_POWER_10_0,
    OP_POWER_10_5,
    OP_POWER_11_0,
    OP_POWER_11_5,
    OP_POWER_12_0
} OUTPUT_POWER_LEVEL;

/* Enumeration for output power level params */
typedef enum {
    PARAM_INDEX_0,
    PARAM_INDEX_1,
    PARAM_INDEX_2,
    PARAM_INDEX_3,
    PARAM_INDEX_4,
    PARAM_INDEX_5,
    PARAM_INDEX_6,
    PARAM_INDEX_7,
    PARAM_INDEX_8,
    PARAM_INDEX_9,
    PARAM_INDEX_10,
    PARAM_INDEX_11,
    PARAM_INDEX_12,
    PARAM_INDEX_13,
    PARAM_INDEX_14,
    PARAM_INDEX_15
} POWER_PARAM_INDEX;

typedef enum {
    GFSK_SEND,
    GFSK_DONE,
    EDR2_DONE,
    EDR3_DONE
} SET_OUTPUT_POWER_LEVEL_STATUS;

typedef enum {
    PRE_INIT_RF_ENABLE,
    PERIODIC_INIT_RF_ENABLE,
    RF_ENABLE_DONE
} ENABLE_CALIBRATION_STATUS;

typedef enum {
    APPINIT_NOT_STARTED,
    APPINIT_IN_PROGRESS,
    APPINIT_DONE
} APP_INIT_STATUS;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   vendor specific initialization function used from application
 * @param   void
 * @return  void
 * @see     vendor_specific_init.c
 */
    void app_vendor_specific_init(void);
    void set_power_level_params(OUTPUT_POWER_LEVEL power_level, UCHAR * param);
    void send_single_power_table(OUTPUT_POWER_LEVEL output_power);

    /* To enable max-power setting by user application */
    API_RESULT sdk_set_output_power(OUTPUT_POWER_LEVEL bt_power,
                                    OUTPUT_POWER_LEVEL le_ant_power);

    /* This function is used to read local name address location */
    void sdk_vs_get_local_name_addr(UINT32 * sdk_vs_local_name_addr);

    /* This function is used to read local name length address location */
    void sdk_vs_get_local_name_len_addr(UINT32 * sdk_vs_local_name_len_addr);

    /* Function to set the hci_vs_local_name_address and
     * hci_vs_local_name_length_address */
    /* void set_hci_vs_local_name_address_params(unsigned short int version); */

#ifdef __cplusplus
};
#endif

#endif /* _H_VENDOR_SPECIFIC_INIT_ */
