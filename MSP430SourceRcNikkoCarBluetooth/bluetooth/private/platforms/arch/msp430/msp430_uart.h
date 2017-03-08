
/**
 * Copyright (c) 2009 MindTree Ltd. All rights reserved.
 * \file    msp430_uart.h
 * \brief   Contains the MSP430 UART related definitions
 */

#ifndef _H_MSP430_UART_
#define _H_MSP430_UART_

/**
 * UART Related Configuration Parameters
 */
/* Definitions to select the baudrate */
#define SDK_BAUDRATE_9600           9600    /* Not supported */
#define SDK_BAUDRATE_19200          19200   /* Not supported */
#define SDK_BAUDRATE_38400          38400   /* Not supported */
#define SDK_BAUDRATE_57600          57600   /* Not supported */
#define SDK_BAUDRATE_115200         115200  /* Supported at 8, 12, 18, 25 MHz */
#define SDK_BAUDRATE_230400         230400  /* Not supported */
#define SDK_BAUDRATE_460800         460800  /* Not supported */
#define SDK_BAUDRATE_921600         921600  /* Supported at 25 MHz */

#ifdef __cplusplus
extern "C" {
#endif

    /* This function is used to set the uart baudrate */
    void sdk_set_host_uart_baudrate(UINT32 baudrate);

    /* Initialize UART ports and registers */
    void sdk_msp430_uart_init(void);

    /* This function is used to shutdown the msp430 Bluetooth UART while
     * entering LPM */
    void sdk_msp430_bt_uart_shutdown(void);

#ifdef __cplusplus
};
#endif

#endif /* _H_MSP430_UART_ */
