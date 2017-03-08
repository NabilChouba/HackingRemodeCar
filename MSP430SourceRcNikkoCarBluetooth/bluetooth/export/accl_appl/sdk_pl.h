
/**
 * Copyright (C) 2009. MindTree Ltd. All rights reserved.
 *
 * \file    sdk_pl.h
 * \brief   This file contains function/datatype specific to this platform
 */

#ifndef _H_SDK_PL_
#define _H_SDK_PL_

/* Header File Inclusion */
#include "appl_menu_pl.h"
#include "sdk_bluetooth_config.h"
#include "msp430_uart.h"
#include "hci_uart.h"
#include "BT_hci_api.h"
#include "task.h"
#include "sdk_common.h"

/* Macros */
/* Button interrupt vector (P2Iv) values for different switches */
#ifdef EZ430_PLATFORM
#define SWITCH_S1         0x06
#define SWITCH_S2         0x08
#else /* MSP-EXPF5438 platform */
#define SWITCH_S1         0x0E
#define SWITCH_S2         0x10
#endif /* EZ430_PLATFORM */

#define POWER_ON_RESET    0x11
#define SWITCH_INTERRUPT (SWITCH_S1 | SWITCH_S2)

/* define the timeout for LCD contrast adjusting */
#define SET_CONTRAST_TIMEOUT 60

#define sdk_start_scheduler() vTaskStartScheduler()

#define UPDATE_USER_BUFFER(data) \
      { \
        circular_user_buffer[circular_user_buf_wt] = data;\
        circular_user_buf_wt++;\
        if (circular_user_buf_wt == MAX_USER_BUF) {\
                circular_user_buf_wt = 0;\
        }\
        bytes_to_be_processed_in_user_buf++;\
      }

/* ----------------------------------------------- Functions */
#ifdef __cplusplus
extern "C" {
#endif

    void sdk_init_bsp(void);

#ifndef SDK_SKIP_UI_INIT
    void sdk_init_ui(void);
#endif                          /* SDK_SKIP_UI_INIT */

    /* Function to read data from accelerometer */
    void sdk_spp_data_read(UCHAR * buffer, UINT16 buf_len);

    void sdk_bt_rf_port_config(void);

    /* Function to set the local device BD Address */
    void sdk_set_bd_addr(void);

    /* Function to display main menu */
    void appl_sdk_display_status(UCHAR staus_update_option);

    void sensor_init(void);

    /* function clear the error message */
    void sdk_update_reconnect_error_status(void);

    void uart_enable_cts_interrupt(void);

    void uart_disable_cts_interrupt(void);

    void hci_uart_write_data(UCHAR *, UINT16);

    void init_user_task(void);

    void *user_task_routine(void);

    /* Configuring MSP430 Timers */
    void sdk_config_timer(void);

    /* This function does the uart initilizations of MSP430 */
    void uart_setup(void);

    void uart_set_baudrate(UINT32 baudrate);

    /* Function to restore peripheral staus on exiting LPM */
    void restore_peripheral_status(void);

    void sdk_error_handler(void);

    /* Function to is used to turn ON bluetooth */
    API_RESULT sdk_bluetooth_on(void);

    /* Function to is used to turn OFF bluetooth */
    API_RESULT sdk_bluetooth_off(void);

    /* This function acts as a call back function for
     * sdk_set_controller_uart_baudrate function */
    void sdk_update_baudrate_handler(void);

#ifdef __cplusplus
};
#endif


#endif /* _H_SDK_PL_ */
