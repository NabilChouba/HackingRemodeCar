
/**
 * Copyright (C) 2000 MindTree Ltd. All rights reserved.
 * \file    hci_uart.h
 * \brief   Contains the definitions used by the HCI UART Transport Layer
 */

#ifndef _H_HCI_UART_
#define _H_HCI_UART_

/** --------------------------------- Header File Inclusion */


/* ---------------------------------- macro defintitions */

/* Define the UART Transport transmit and receive buffer sizes */
#define UART_TX_BUFFER_SIZE                 128
#define UART_RX_BUFFER_SIZE                 260
#define MAX_DATA_RX_PACKETS                 10
#define MAX_PKT_HDR_LEN                     5

#define HCI_EVENT_PACKET_HEADER_LEN	        3
#define HCI_ACL_DATA_PACKET_HEADER_LEN      5

#define BT_HEADER_FIRST_BYTE                0
#define BT_HEADER                           1
#define BT_PAYLOAD                          2

typedef struct {
    UINT16 start_idx;
    UINT16 length;
    UCHAR pkt_type;
} DATA_RX_QUEUE;

/* ----------------------------------------------- typedefs */
/* MSP430 UART parameter structure */
#ifdef __IAR_SYSTEMS_ICC__
typedef struct {
    volatile UINT8 *const uart_port_sel;
    volatile UINT8 *const uart_port_dir;
    volatile UINT8 *const uart_port_out;
    const UINT16 uart_tx_pin;
    const UINT16 uart_rx_pin;
    volatile UINT8 *const uart_rts_port_dir;
    volatile UINT8 *const uart_cts_port_dir;
    const UINT16 uart_rts_pin;
    const UINT16 uart_cts_pin;
    volatile UINT8 *const uart_rts_port_out;
    volatile UINT8 *const uart_cts_port_out;
    unsigned char const volatile __data16 *uart_cts_port_in;
    volatile UINT8 *const uart_cts_port_ren;
    volatile UINT8 *const uart_cts_port_sel;
    volatile UINT8 *const uart_rts_port_sel;
    volatile UINT8 *const uart_cts_ies;
    volatile UINT8 *const uart_cts_ifg;
    volatile UINT8 *const uart_cts_ie;
    volatile UINT8 *const uart_reg_ucaxctl0;
    volatile UINT8 *const uart_reg_ucaxctl1;
    volatile UINT8 *const uart_reg_ucaxbr0;
    volatile UINT8 *const uart_reg_ucaxbr1;
    unsigned short volatile __data16 *const uart_reg_ucaxiv;
    volatile UINT8 *const uart_reg_ucaxstat;
    volatile UINT8 *const uart_reg_ucaxtxbuf;
    unsigned char const volatile __data16 *uart_reg_ucaxrxbuf;
    volatile UINT8 *const uart_reg_ucaxmctl;
    volatile UINT8 *const uart_reg_ucaxie;
    volatile UINT8 *const uart_reg_ucaxifg;
} UART_CONFIG_PARAMS;
#else /* ccsv4 */
typedef struct {
    volatile UINT8 *const uart_port_sel;
    volatile UINT8 *const uart_port_dir;
    volatile UINT8 *const uart_port_out;
    const UINT16 uart_tx_pin;
    const UINT16 uart_rx_pin;
    volatile UINT8 *const uart_rts_port_dir;
    volatile UINT8 *const uart_cts_port_dir;
    const UINT16 uart_rts_pin;
    const UINT16 uart_cts_pin;
    volatile UINT8 *const uart_rts_port_out;
    volatile UINT8 *const uart_cts_port_out;
    volatile UINT8 *const uart_cts_port_in;
    volatile UINT8 *const uart_cts_port_ren;
    volatile UINT8 *const uart_cts_port_sel;
    volatile UINT8 *const uart_rts_port_sel;
    volatile UINT8 *const uart_cts_ies;
    volatile UINT8 *const uart_cts_ifg;
    volatile UINT8 *const uart_cts_ie;
    volatile UINT8 *const uart_reg_ucaxctl0;
    volatile UINT8 *const uart_reg_ucaxctl1;
    volatile UINT8 *const uart_reg_ucaxbr0;
    volatile UINT8 *const uart_reg_ucaxbr1;
    volatile unsigned int *const uart_reg_ucaxiv;
    volatile UINT8 *const uart_reg_ucaxstat;
    volatile UINT8 *const uart_reg_ucaxtxbuf;
    volatile UINT8 *const uart_reg_ucaxrxbuf;
    volatile UINT8 *const uart_reg_ucaxmctl;
    volatile UINT8 *const uart_reg_ucaxie;
    volatile UINT8 *const uart_reg_ucaxifg;
} UART_CONFIG_PARAMS;

#endif /* __IAR_SYSTEMS_ICC__ */

extern const UART_CONFIG_PARAMS bt_uart_config;

#define UART_TRANSMIT(byte) \
{ \
    while (bt_uart_config.uart_cts_pin & *(bt_uart_config.uart_cts_port_in)); \
    *(bt_uart_config.uart_reg_ucaxtxbuf) = (byte); \
}

#define UART_DISABLE_BT_UART_RTS() \
{ \
	 *(bt_uart_config.uart_rts_port_out) |= (bt_uart_config.uart_rts_pin);\
}


/* Enable RTS only if UART RX interrupt is not pending */
#define UART_ENABLE_BT_UART_RTS() \
{ \
    if ((!(*(bt_uart_config.uart_reg_ucaxifg) & UCRXIFG)) && (!(ADC12IFG)))  {\
	 *(bt_uart_config.uart_rts_port_out) &= ~(bt_uart_config.uart_rts_pin);\
    }\
}

#define UART_ENABLE_BT_UART_TX() \
{ \
	 *(bt_uart_config.uart_reg_ucaxie) |= UCTXIE;\
}

#define UART_DISABLE_BT_UART_TX() \
{ \
	 *(bt_uart_config.uart_reg_ucaxie) &= ~UCTXIE;\
}

#define UART_ENABLE_BT_UART_RX() \
{ \
	 *(bt_uart_config.uart_reg_ucaxie) |= UCRXIE;\
}

#define UART_DISABLE_BT_UART_RX() \
{ \
	 *(bt_uart_config.uart_reg_ucaxie) &= ~UCRXIE;\
}

#define DISABLE_UART() \
{ \
     *(bt_uart_config.uart_reg_ucaxctl1) |= UCSWRST; \
}

#define ENABLE_UART()\
{ \
    *(bt_uart_config.uart_reg_ucaxctl1) &= ~UCSWRST; \
    UART_ENABLE_BT_UART_RX(); \
    UART_ENABLE_BT_UART_TX(); \
}
/* ---------------------------------- Global Definitions */
#if defined BT_UART && !defined BT_BCSP
#define hci_transport_write_data    hci_uart_send_data
#endif /* BT_UART && !BT_BCSP */

#ifdef BT_BCSP
#define hci_transport_write_data    hci_bcsp_send_data
#endif /* BT_BCSP */


/* ---------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/** HCI UART Initialization & Shutdown */
    void hci_uart_init(void);
    void hci_uart_bt_init(void);
    void hci_uart_bt_shutdown(void);

    /* This function is used to initialize UART */
    void uart_setup(void);
    /* This function is used to shutdown HCI-UART as part of Bluetooth OFF. */
    void hci_uart_bt_shutdown(void);

    /* This function writes data to tx buffer based on the availability space
     * in the tx buffer. */
    void hci_uart_write_data(UCHAR * buf, UINT16 length);

    void sdk_set_controller_uart_baudrate(UINT32 baudarate);

#ifdef __cplusplus
};
#endif

#endif /* _H_HCI_UART_ */
