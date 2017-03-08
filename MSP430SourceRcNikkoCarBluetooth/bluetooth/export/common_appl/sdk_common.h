
/**
 * Copyright (C) 2010 MindTree Ltd.  All rights reserved.
 * \file    sdk_common.h
 * \brief   This file contains function/datatypes common for all the packages
 */

#ifndef _H_SDK_COMMON_
#define _H_SDK_COMMON_

#include "hal_MSP430F5438.h"

#define SDK_SW_VERSION                  "5.16  \0"

#define appl_hci_print_bd_addr(bd)      appl_dump_bytes((bd), BT_BD_ADDR_SIZE);

/* Connection States */
#define SDK_IN_ACL_CONNECTION           0x01
#define SDK_ACL_CONNECTED               0x02
#define SDK_IN_SDP_QUERY                0x03
#define SDK_IN_SPP_CONNECTION           0x04
#define SDK_SPP_CONNECTED               0x05
#define SDK_IN_SPP_DISCONNECTION        0x06
#define SDK_IN_ACL_DISCONNECTION        0x07
#define SDK_SPP_DISCONNECTED            0x08
#define SDK_DISCONNECTED                0x09
#define SDK_MAX_PAGE_TIMEOUT_VALUE      0x3200

/* Macro to check spp connection status */
#define SDK_IS_SPP_CONNECTED(index)   \
    (SDK_SPP_CONNECTED == sdk_status[(index)].connect_switch)

/* Macro to check acl connection status */
#define SDK_IS_DISCONNECTED(index)   \
    (SDK_DISCONNECTED == sdk_status[(index)].connect_switch)

/* Macro to check spp connection status */
#define SDK_IS_SPP_DISCONNECTED(index)   \
    (SDK_SPP_DISCONNECTED == sdk_status[(index)].connect_switch)

/* Macro to check spp connection status */
#define SDK_IS_SPP_IN_DISCONNECTION(index)   \
    (SDK_IN_SPP_DISCONNECTION == sdk_status[(index)].connect_switch)

/* Macro to check spp connection status */
#define SDK_IS_IN_ACL_CONNECTION(index)   \
    (SDK_IN_ACL_CONNECTION == sdk_status[(index)].connect_switch)

/* Macro to check spp link status */
#define SDK_IS_IN_ACTIVE_MODE(index)   \
    (SDK_ACTIVE == sdk_status[(index)].link_state)

#define SDK_IS_SPP_CONNECTION(index)    \
    (SDK_IN_SPP_CONNECTION == sdk_status[(index)].connect_switch)

#ifdef SDK_ENABLE_SNIFF_MODE
/* Macro to check spp link status */
#define SDK_IS_IN_SNIFF_MODE(index)   \
    (SDK_IN_SNIFF == sdk_status[(index)].link_state)
#else
#define SDK_IS_IN_SNIFF_MODE(index) FALSE
#endif /* SDK_ENABLE_SNIFF_MODE */

/* Macro to check data to be sent flag state */
#define SDK_IS_IN_DATA_SEND_STATE(index) \
    (TRUE == sdk_status[(index)].appl_spp_data_to_be_sent)

/* Change SPP Connection Status */
#define SDK_SPP_CHANGE_STATE(index, state)   \
    (sdk_status[(index)].connect_switch = (state))
/* Change SPP Data transfer state */
#define SDK_SPP_CHANGE_TX_STATE(index, state)   \
    sdk_status[(index)].sdk_data_sending = (state);
/* Change SPP Link state */
#define SDK_SPP_CHANGE_LINK_STATE(index, state)   \
    sdk_status[(index)].link_state = (state);
/* Change data to be sent flag state */
#define SDK_SPP_CHANGE_DATA_STATE(index, state) \
    sdk_status[(index)].appl_spp_data_to_be_sent = (state);

/* Macros to enter and exit flight mode */
#define SDK_ENTER_FLIGHT_MODE        BT_RF_NSHUTDOWN_PIN_LOW
#define SDK_EXIT_FLIGHT_MODE         BT_RF_NSHUTDOWN_PIN_HIGH

#define TEST_PIN_PORT_SEL            P8SEL
#define TEST_PIN                     BIT5
#define TEST_PIN_PORT_DIR            P8DIR
#define TEST_PIN_PORT_OUT            P8OUT

#define TEST_PIN_CONFIGURE_OUTPUT()  TEST_PIN_PORT_DIR |= TEST_PIN
#define TEST_PIN_TOGGLE()            TEST_PIN_PORT_OUT ^= TEST_PIN
#define TEST_PIN_HIGH()              TEST_PIN_PORT_OUT |= TEST_PIN
#define TEST_PIN_LOW()               TEST_PIN_PORT_OUT &= ~TEST_PIN


#define TEST_PIN1_PORT_SEL           P5SEL
#define TEST_PIN1                    BIT5
#define TEST_PIN1_PORT_DIR           P5DIR
#define TEST_PIN1_PORT_OUT           P5OUT

#define TEST_PIN1_CONFIGURE_OUTPUT() TEST_PIN1_PORT_DIR |= TEST_PIN1
#define TEST_PIN1_TOGGLE()           TEST_PIN1_PORT_OUT ^= TEST_PIN1
#define TEST_PIN1_HIGH()             TEST_PIN1_PORT_OUT |= TEST_PIN1
#define TEST_PIN1_LOW()              TEST_PIN1_PORT_OUT &= ~TEST_PIN1

#define TEST_PIN2_PORT_SEL           P4SEL
#define TEST_PIN2                    BIT7
#define TEST_PIN2_PORT_DIR           P4DIR
#define TEST_PIN2_PORT_OUT           P4OUT

#define TEST_PIN2_CONFIGURE_OUTPUT() TEST_PIN2_PORT_DIR |= TEST_PIN2
#define TEST_PIN2_TOGGLE()           TEST_PIN2_PORT_OUT ^= TEST_PIN2
#define TEST_PIN2_HIGH()             TEST_PIN2_PORT_OUT |= TEST_PIN2
#define TEST_PIN2_LOW()              TEST_PIN2_PORT_OUT &= ~TEST_PIN2


#define TEST_PIN3_PORT_SEL           P4SEL
#define TEST_PIN3                    BIT5
#define TEST_PIN3_PORT_DIR           P4DIR
#define TEST_PIN3_PORT_OUT           P4OUT

#define TEST_PIN3_CONFIGURE_OUTPUT() TEST_PIN3_PORT_DIR |= TEST_PIN3
#define TEST_PIN3_TOGGLE()           TEST_PIN3_PORT_OUT ^= TEST_PIN3
#define TEST_PIN3_HIGH()             TEST_PIN3_PORT_OUT |= TEST_PIN3
#define TEST_PIN3_LOW()              TEST_PIN3_PORT_OUT &= ~TEST_PIN3

/* Bluetooth On status */
#define SDK_BT_OFF                    0x01
#define SDK_BT_ON_IN_PROGRESS         0x02
#define SDK_BT_ON                     0x03

/* Bluetooth Visibility status */
#define SDK_DISC_OFF                  0x01
#define SDK_DISC_ON                   0x02

/* Bluetooth SPP Tx On/Off status */
#define SDK_SPP_TX_OFF                0x00
#define SDK_SPP_TX_ON                 0x01

#define SDK_ACTIVE                    0x01
#define SDK_IN_SNIFF                  0x02
#define SDK_OFF                       0x03
#define SDK_IS_BT_POWERED_ON()        (SDK_BT_ON == sdk_bt_power)
#define SDK_IS_BT_IN_PROGRESS()       (SDK_BT_ON_IN_PROGRESS == sdk_bt_power)
#define SDK_IS_BT_POWERED_OFF()       (SDK_BT_OFF == sdk_bt_power)
#define SDK_IS_BT_DISCOVERABLE()      (SDK_DISC_ON == sdk_bt_visible)
#define SDK_IS_BT_NON_DISCOVERABLE()  (SDK_DISC_OFF == sdk_bt_visible)
#define SDK_IS_SPP_TX_STARTED(index)  (SDK_SPP_TX_ON == sdk_status[(index)].sdk_data_sending)

/* Macro to enter LPM and it also enables GIE */
#define ENTER_MSP430_LPM()  __bis_SR_register(SDK_MSP430_LPM + GIE)
/* Macro to exit LPM */
#define EXIT_MSP430_LPM()   __bic_SR_register_on_exit(SDK_MSP430_LPM)

/* 
 * Defines for MSP430 Low power modes
 */
#define MSP430_LPM_0 LPM0_bits
#define MSP430_LPM_1 LPM1_bits
#define MSP430_LPM_2 LPM2_bits
#define MSP430_LPM_3 LPM3_bits
#define MSP430_LPM_4 LPM4_bits


/* LPM related states */
#define ENTER_LPM_OFF                          0x00
#define ENTER_LPM_ON                           0x01

/* MSP430 LED related macros */
#define TOGGLE_LED1()                          LED_PORT_OUT ^= LED_1
#define TOGGLE_LED2()                          LED_PORT_OUT ^= LED_2
#define TOGGLE_LED()                           LED_PORT_OUT ^=(LED_1 | LED_2)

#define LED1_ON()                              LED_PORT_OUT |= LED_1;
#define LED2_ON()                              LED_PORT_OUT |= LED_2;
#define LED1_OFF()                             LED_PORT_OUT &= ~LED_1;
#define LED2_OFF()                             LED_PORT_OUT &= ~LED_2;
#define LED_ON()                               LED_PORT_OUT |= (LED_1 | LED_2);
#define LED_OFF()                              LED_PORT_OUT &= ~(LED_1 | LED_2);


#define appl_bluetooth_on_indication()         LED1_ON()
#define appl_bluetooth_off_indication()        LED_OFF()
#define appl_bt_on_in_progress_indication()    TOGGLE_LED1()
#define appl_spp_data_indication()             TOGGLE_LED2()
#define appl_spp_disconnect_indication()       LED2_OFF();
#define appl_spp_connected_indication()        LED2_ON()

/* Macros */

#ifdef __IAR_SYSTEMS_ICC__      /* Toolchain Specific Code */
#define sdk_display(...)
#elif defined (__TI_COMPILER_VERSION__)
#define sdk_display
#else
#warning "Toolchain Unknown"
#define sdk_display
#endif /* Toolchain Specific Code */


/* Typedefs */
typedef void (*FP) (void);

/* Macro to set the CC2560 nSHUTDOWN pin high */
#define BT_RF_NSHUTDOWN_PIN_HIGH()  \
BT_RF_NSHUTDOWN_PORT_OUT |= BT_RF_NSHUTDOWN_PORT_PIN;

/* Macro to set the CC2560 nSHUTDOWN pin low */
#define BT_RF_NSHUTDOWN_PIN_LOW()  \
BT_RF_NSHUTDOWN_PORT_OUT &= ~BT_RF_NSHUTDOWN_PORT_PIN;

#ifdef __cplusplus
extern "C" {
#endif

    /* This function is used to configure pins connecting MSP430 and BT
     * controller */
    void common_init_bsp(void);

    /* Function to set and store the new sys_clk frequency */
    void sdk_set_frequency(UCHAR freq);

    void sdk_bt_rf_port_config(void);

    /* Function to indicate uart error */
    void sdk_uart_error_handler(void);

    /* Function to get free instance in sdk_status array */
    API_RESULT appl_get_free_status_instance(UCHAR * id);

    /* Function to get spp connection instance based on acl connection handle */
    API_RESULT appl_get_status_instance_acl(UCHAR * id,
                                            UINT16 acl_connection_handle);

    /* Function to get spp connection instance based on SPP connection handle */
    API_RESULT appl_get_status_instance_spp(UCHAR * id,
                                            UINT16 spp_connection_handle);

    /* Function to get spp connection instance based on peer bd adddress */
    API_RESULT appl_get_status_instance_bd_addr(UCHAR * id,
                                                UCHAR * rem_bd_addr);

    /* Function to shutdown BL6450 */
    void sdk_shutdown_BL6450(void);

    /* Function to handle simple pairing debug mode */
    void sdk_simple_pairing_debug_mode_handler(void);

    /* Function to check the spp connection status */
    API_RESULT appl_get_spp_connection_progress(UCHAR * id);

    /* Function to set the suffix for the config local name */
    void sdk_set_config_local_name_suffix(UINT32 * name_length_ptr);

    /* Function to read the local device name from the config file */
    CHAR *sdk_read_hci_local_device_name_from_config_file(void);

    /* Function to check the maximum length of the hci local device name */
    void sdk_check_max_len_of_hci_local_device_name(UCHAR len);

#ifdef __cplusplus
};
#endif


#endif /* _H_SDK_COMMON_ */
