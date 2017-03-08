
/**
 * Copyright (C) 2009. MindTree Ltd. All rights reserved.
 * \file    msp430_uart.c
 * \brief   Contains functions related to MSP430 BLuetooth UART
 */

/* Header File Inclusion */
#include "sdk_pl.h"
#include "hci_uart.h"
#include "BT_queue.h"
#include "appl_bt_rf.h"
#include "msp430_uart.h"
#include "vendor_specific_init.h"
#include "bt_sdk_error.h"


#define HCI_ACL_DATA_PACKET                         0x02
#define HCI_EVENT_PACKET                            0x04

/* Macro definitions */
#define UPDATE_RX_BUFFER(data) \
      { \
        uart_rx_buffer[uart_rx_wr] = data;\
        uart_rx_wr++;\
        if (UART_RX_BUFFER_SIZE == uart_rx_wr) {\
                uart_rx_wr = 0;\
        }\
      }

/* External Global Variables */
extern INT16 uart_rx_rd;
extern volatile UCHAR sem_flag;
/* Variable to store the packet header length */
extern UINT8 packet_header_len;

/* Buffer to store the header data */
extern UCHAR temp_header_buffer[MAX_PKT_HDR_LEN];

/* Index used in the temp_header_buffer */
extern UCHAR temp_header_buffer_idx;

/* Variable used to store current packet length */
extern volatile UINT16 current_pkt_len;

extern DATA_RX_QUEUE data_rx_queue;

/* External Global Variables */
extern volatile UINT16 bytes_available_in_tx_buffer;
extern volatile UINT16 bytes_to_be_processed;

extern INT16 uart_rx_wr;        /* Write pointer of uart rx buffer */
extern UINT8 uart_tx_wr;
extern volatile UINT16 bytes_expected;
extern xSemaphoreHandle xReadSemaphore, xWritePlSemaphore;
extern UCHAR uart_rx_buffer[];
extern UCHAR uart_tx_buffer[UART_TX_BUFFER_SIZE];
extern UINT8 uart_tx_rd;

#ifdef SDK_EHCILL_MODE
extern volatile UINT32 ehcill_rx_state;
extern volatile UINT32 ehcill_tx_state;
extern volatile UINT32 msp430_state;
#endif /* SDK_EHCILL_MODE */

extern volatile UCHAR packet_complete;
extern UCHAR expected_uart_data_type;

extern UINT32 sdk_error_code;
extern void sdk_uart_error_handler(void);

extern UCHAR read_task_buffer[UART_RX_BUFFER_SIZE];
/* The variable is used to hold the current BT UART baud rate value */
extern UINT32 current_bt_uart_baudrate;

/* Flag to indicate that MSP430 UART initialization is in progress */
extern UCHAR msp430_uart_init_flag;

/* This variable holds the value of configured BT UART baud rate */
extern UINT32 configured_bt_uart_baudrate;

extern UCHAR HCI_VS_Update_UART_HCI_Baudrate_command[];
extern const UCHAR
    HCI_VS_Read_Modify_Write_Hardware_Register_command_disable_events[];
extern UCHAR sys_clk_frequency;

/* Global variables */
volatile UINT32 inactivity_counter = 0;
volatile UCHAR lpm_mode = FALSE;
volatile UCHAR sdk_update_uart_baudrate_flag = FALSE;
UCHAR sdk_disable_events_cmd_byte_index = 0;
UCHAR sdk_uart_update_baudrate_cmd_byte_index = 0;

const UART_CONFIG_PARAMS bt_uart_config = {
    &BT_UART_PORT_SEL,
    &BT_UART_PORT_DIR,
    &BT_UART_PORT_OUT,
    BT_UART_TX_PIN,
    BT_UART_RX_PIN,
    &BT_UART_RTS_PORT_DIR,
    &BT_UART_CTS_PORT_DIR,
    BT_UART_RTS_PIN,
    BT_UART_CTS_PIN,
    &BT_UART_RTS_PORT_OUT,
    &BT_UART_CTS_PORT_OUT,
    &BT_UART_CTS_PORT_IN,
    &BT_UART_CTS_PORT_REN,
    &BT_UART_CTS_PORT_SEL,
    &BT_UART_RTS_PORT_SEL,
    &BT_UART_CTS_PORT_IES,
    &BT_UART_CTS_PORT_IFG,
    &BT_UART_CTS_PORT_IE,
    &BT_UART_REG_UCAXCTL0,
    &BT_UART_REG_UCAXCTL1,
    &BT_UART_REG_UCAXBR0,
    &BT_UART_REG_UCAXBR1,
    &BT_UART_REG_UCAXIV,
    &BT_UART_REG_UCAXSTAT,
    &BT_UART_REG_UCAXTXBUF,
    &BT_UART_REG_UCAXRXBUF,
    &BT_UART_REG_UCAXMCTL,
    &BT_UART_REG_UCAXIE,
    &BT_UART_REG_UCAXIFG,

};


/**
 * \fn      BT_UART_ISR
 * \brief   This function is an ISR for BT uart
 * \param   void
 * \return  void
 */
#ifdef __TI_COMPILER_VERSION__
#pragma CODE_SECTION(BT_UART_ISR, ".text:_isr");
#endif /* __TI_COMPILER_VERSION__ */

#pragma vector=BT_UART_VECTOR
__interrupt void BT_UART_ISR(void)
{
    UINT16 int_vect;
    signed portBASE_TYPE xHigherPriorityTaskWoken;
    volatile UCHAR uart_rx_val;
    volatile UCHAR uart_err_val;
    volatile UCHAR ehcill_data_flag = 0;
    UINT16 temp_ptr, i;

    UART_DISABLE_BT_UART_RTS();


    inactivity_counter = 0;
    xHigherPriorityTaskWoken = pdFALSE;

    int_vect = *(bt_uart_config.uart_reg_ucaxiv);
    /* RX interrupt handler */
    if (int_vect & 0x02) {
        UCHAR rx_octet;
        ehcill_data_flag = 0;
        if (FALSE == sdk_update_uart_baudrate_flag) {
            if (((*(bt_uart_config.uart_reg_ucaxstat)) & UCRXERR)) {
                /* Handle UART error */
                uart_err_val = *(bt_uart_config.uart_reg_ucaxstat);
                /* Read the UART RX Buffer value to clear the status register */
                uart_rx_val = *(bt_uart_config.uart_reg_ucaxrxbuf);
                if (uart_err_val & UCFE) {
                    /* Framing Error */
                    sdk_error_code = SDK_UART_FRAMING_ERROR;
                } else if (uart_err_val & UCPE) {
                    /* Parity Error */
                    sdk_error_code = SDK_UART_PARITY_ERROR;
                } else if (uart_err_val & UCOE) {
                    /* OverFlow Error */
                    sdk_error_code = SDK_UART_OVERFLOW_ERROR;
                } else {
                    /* Uart Error */
                    sdk_error_code = SDK_UART_ERROR;
                }

                sdk_uart_error_handler();
            } else {
                rx_octet = *(bt_uart_config.uart_reg_ucaxrxbuf);
#ifdef SDK_EHCILL_MODE
                /* Check if the data is the first byte of the packet;
                 * bytes_expected will be set to 1 and the
                 * expected_uart_data_type will be set to BT_HEADER_FIRST_BYTE */
                if ((1 == bytes_expected)
                    && (BT_HEADER_FIRST_BYTE == expected_uart_data_type)) {
                    /* Validating if the received octet is ehcill data : 0x30
                     * to 0x33 */
                    if ((rx_octet >= SDK_BT_RF_SLEEP_IND)
                        && (rx_octet <= SDK_BT_RF_WAKE_UP_ACK)) {
                        /* Setting the flag for ehcill data */
                        ehcill_data_flag = 1;
                        ehcill_rx_state = rx_octet;
                        /* Calling ehcill rx_handler for handling received
                         * ehcill byte */
                        ehcill_rx_handler();
                    }
                }
#endif /* SDK_EHCILL_MODE */
                /* Checking if the data received is not ehcill data */
                if (0 == ehcill_data_flag) {
                    /* Updating the uart rx buffer; this updates the uart_rx_wr
                     * pointer */
                    UPDATE_RX_BUFFER(rx_octet);
                    /* Decrementing bytes_expected on receiving the data */
                    bytes_expected--;
                    bytes_to_be_processed++;

                    /* On reception of expected data, decoding of the received
                     * bytes is done here */
                    if (0 == bytes_expected) {
                        while (bytes_to_be_processed) {
                            bytes_to_be_processed--;
                            switch (expected_uart_data_type) {
                            case BT_HEADER_FIRST_BYTE:
                                data_rx_queue.start_idx = uart_rx_rd;
                                /* Copying the header details from the uart tx
                                 * buffer */
                                temp_header_buffer[temp_header_buffer_idx] =
                                    uart_rx_buffer[uart_rx_rd];

                                temp_header_buffer_idx++;
                                /* Incrementing the current packet length and
                                 * updating the uart_rx_rd pointer */
                                current_pkt_len++;
                                uart_rx_rd++;
                                if (UART_RX_BUFFER_SIZE == uart_rx_rd) {
                                    uart_rx_rd = 0;
                                }
                                /* Setting the bytes_expected based on the
                                 * Packet header length */
                                switch (temp_header_buffer[0]) {
                                case HCI_EVENT_PACKET:
                                    bytes_expected =
                                        HCI_EVENT_PACKET_HEADER_LEN - 1;
                                    break;
                                case HCI_ACL_DATA_PACKET:
                                    bytes_expected =
                                        HCI_ACL_DATA_PACKET_HEADER_LEN - 1;
                                    break;
                                default:
                                    sdk_error_code =
                                        SDK_ERROR_IN_HEADER_FIRST_BYTE;
                                    sdk_error_handler();
                                    break;

                                }
                                /* Setting the expected_uart_data_type to
                                 * BT_HEADER, on reception of first byte of
                                 * header */
                                expected_uart_data_type = BT_HEADER;
                                break;
                            case BT_HEADER:
                                /* Copying the header details from the uart tx
                                 * buffer */
                                temp_header_buffer[temp_header_buffer_idx] =
                                    uart_rx_buffer[uart_rx_rd];

                                /* Incrementing the current packet lenght and
                                 * updating the uart_rx_rd pointer */
                                temp_header_buffer_idx++;
                                current_pkt_len++;
                                uart_rx_rd++;
                                if (UART_RX_BUFFER_SIZE == uart_rx_rd) {
                                    uart_rx_rd = 0;
                                }
                                switch (temp_header_buffer[0]) {
                                    /* Case to handler HCI Event Packet */
                                case HCI_EVENT_PACKET:
                                    /* Updating the bytes_expected based on the
                                     * packet header length */
                                    if (temp_header_buffer_idx >=
                                        HCI_EVENT_PACKET_HEADER_LEN) {
                                        data_rx_queue.length =
                                            temp_header_buffer[2] +
                                            HCI_EVENT_PACKET_HEADER_LEN;
                                        data_rx_queue.pkt_type =
                                            HCI_EVENT_PACKET;
                                        /* Updating the header length of HCI
                                         * Event */
                                        packet_header_len =
                                            HCI_EVENT_PACKET_HEADER_LEN;
                                        bytes_expected =
                                            data_rx_queue.length -
                                            HCI_EVENT_PACKET_HEADER_LEN;
                                        expected_uart_data_type = BT_PAYLOAD;
                                    }
                                    break;

                                    /* Case to handler ACL Data Packet */
                                case HCI_ACL_DATA_PACKET:
                                    /* If ACL packet, then the length of
                                     * payload will the 3rd and 4th byte of the
                                     * header */
                                    if (temp_header_buffer_idx >=
                                        HCI_ACL_DATA_PACKET_HEADER_LEN) {
                                        data_rx_queue.length =
                                            (temp_header_buffer[4] << 8) |
                                            (temp_header_buffer[3]);
                                        data_rx_queue.length +=
                                            HCI_ACL_DATA_PACKET_HEADER_LEN;
                                        data_rx_queue.pkt_type =
                                            HCI_ACL_DATA_PACKET;
                                        /* Updating the header length of ACL
                                         * Data packet */
                                        packet_header_len =
                                            HCI_ACL_DATA_PACKET_HEADER_LEN;
                                        bytes_expected =
                                            data_rx_queue.length -
                                            HCI_ACL_DATA_PACKET_HEADER_LEN;
                                        expected_uart_data_type = BT_PAYLOAD;
                                    }
                                    break;
                                default:
                                    /* Handling error case */
                                    sdk_error_code = SDK_ERROR_IN_HEADER;
                                    sdk_error_handler();
                                    break;
                                }
                                break;

                                /* Case to handle Payload */
                            case BT_PAYLOAD:
                                current_pkt_len++;

                                /* Updating the parameters, on receiving the
                                 * entire payload */
                                if (current_pkt_len >= data_rx_queue.length) {
                                    expected_uart_data_type =
                                        BT_HEADER_FIRST_BYTE;
                                    current_pkt_len = 0;
                                    temp_header_buffer_idx = 0;
                                    bytes_expected = 1;
                                    temp_ptr = data_rx_queue.start_idx;
                                    for (i = 0; i < data_rx_queue.length; i++) {
                                        /* Copying the entire packet from uart
                                         * rx buffer to the read_task_buffer */
                                        read_task_buffer[i] =
                                            uart_rx_buffer[temp_ptr];
                                        if (i >= packet_header_len) {
                                            uart_rx_rd++;
                                            if (UART_RX_BUFFER_SIZE ==
                                                uart_rx_rd) {
                                                uart_rx_rd = 0;
                                            }
                                        }
                                        temp_ptr++;
                                        if (UART_RX_BUFFER_SIZE == temp_ptr) {
                                            temp_ptr = 0;
                                        }
                                    }

                                    /* Checking if the sem flag is set to 1;
                                     * This flag is set to 1 in the read task,
                                     * after semaphore acquisition */
                                    if (1 == sem_flag) {
                                        /* Releasing the semaphore for read
                                         * task to continue further processing */
                                        if (pdPASS !=
                                            xSemaphoreGiveFromISR
                                            (xReadSemaphore,
                                             &xHigherPriorityTaskWoken)) {
                                            /* Error condition if the Sem
                                             * release returns failure */
                                            sdk_error_code =
                                                SDK_ERROR_IN_READ_SEM_GIVE;
                                            sdk_error_handler();
                                        } else {
                                            sem_flag = 0;
                                        }
                                    }
                                }
                                break;

                            default:
                                sdk_error_code = SDK_ERROR_IN_DATA_RECV;
                                sdk_error_handler();
                                break;

                            }
                        }

                    }
                }
            }
        }
    }

    /* TX INTERRUPT HANDLER */
    if (int_vect & 0x04) {
        if (FALSE == sdk_update_uart_baudrate_flag) {
            UINT16 count;
            volatile UCHAR buff_data;
            ehcill_data_flag = 0;
            /* If the transmission of data present in uart_tx_buffer is
             * completed and if it is end of packet, then call ehcill tx
             * handler to handle to ehcill state machine; If not, then
             * Semaphore is released for futher processing of the data to be
             * transmitted */
            if (0 == bytes_available_in_tx_buffer) {
#ifdef SDK_EHCILL_MODE
                if (TRUE == packet_complete) {
                    if ((BT_RF_NO_EHCILL_DATA != ehcill_rx_state)
                        || (BT_RF_NO_EHCILL_DATA != ehcill_tx_state)) {
                        ehcill_data_flag = 1;
                        ehcill_tx_handler();
                    }
                }
#endif /* SDK_EHCILL_MODE */
                if (0 == ehcill_data_flag) {
                    /* Releasing the semaphore after transmitting the data that
                     * is available in the tx buffer */
                    xSemaphoreGiveFromISR(xWritePlSemaphore,
                                          &xHigherPriorityTaskWoken);
                }
            } else {
#ifdef SDK_EHCILL_MODE
                /* Condition to handle wake up from Host */
                if (SDK_MSP430_SLEEP_STATE == msp430_state) {
                    ehcill_tx_handler();

                } else
#endif /* SDK_EHCILL_MODE */
                {

                    count = uart_tx_wr;
                    count -= uart_tx_rd;
                    count &= (UART_TX_BUFFER_SIZE - 1);
                    if (count > 0) {
                        /* Decrementing bytes_available_in_tx_buffer just
                         * before transmitting the byte */
                        bytes_available_in_tx_buffer--;
                        buff_data = uart_tx_buffer[uart_tx_rd];
                        /* Updating the uart_tx_rd pointer */
                        Q_BUFFER_UPDATE_RDWR_PTR(UART_TX_BUFFER_SIZE,
                                                 uart_tx_rd, 1);
                        UART_TRANSMIT(buff_data);

                    }
                }
            }
        } else {
            UART_ENABLE_BT_UART_RTS();
            if (sdk_disable_events_cmd_byte_index <
                (HCI_VS_READ_MODIFY_WRITE_HARDWARE_REGISTER_LEN + 4)) {
                UART_TRANSMIT
                    (HCI_VS_Read_Modify_Write_Hardware_Register_command_disable_events
                     [sdk_disable_events_cmd_byte_index]);
                sdk_disable_events_cmd_byte_index++;
            } else {
                if ((SDK_BT_RF_UPDATE_UART_BAUDRATE_COMMAND_LEN + 4) >
                    sdk_uart_update_baudrate_cmd_byte_index) {
                    UART_TRANSMIT(HCI_VS_Update_UART_HCI_Baudrate_command
                                  [sdk_uart_update_baudrate_cmd_byte_index]);
                    sdk_uart_update_baudrate_cmd_byte_index++;
                } else {
                    while (!
                           (bt_uart_config.uart_cts_pin &
                            *(bt_uart_config.uart_cts_port_in)));
                    while ((bt_uart_config.uart_cts_pin &
                            *(bt_uart_config.uart_cts_port_in)));
                    configured_bt_uart_baudrate = current_bt_uart_baudrate;
                    sdk_set_host_uart_baudrate(current_bt_uart_baudrate);
                    sdk_uart_update_baudrate_cmd_byte_index = 0;
                    sdk_disable_events_cmd_byte_index = 0;
                    sdk_update_uart_baudrate_flag = FALSE;

                }
            }
        }
    }
#ifdef MSP430_LPM_ENABLE
    if (TRUE == lpm_mode) {
        lpm_mode = FALSE;
        restore_peripheral_status();
        /* Exit LPM */
        EXIT_MSP430_LPM();
    }
#endif /* MSP430_LPM_ENABLE */

    if (xHigherPriorityTaskWoken) {
        portYIELD();
    }
}


/**
 * \fn      sdk_msp430_uart_init
 * \brief   Initializes UART ports and registers
 * \param   None
 * \return  void
 */
void sdk_msp430_uart_init(void)
{

    /* Set UCSWRST */
    *(bt_uart_config.uart_reg_ucaxctl1) |= UCSWRST;

    /* Initialize USCI registers */
    *(bt_uart_config.uart_reg_ucaxctl0) = UCMODE_0; /* set USCI block to UART */
    *(bt_uart_config.uart_reg_ucaxctl0) &= ~UC7BIT; /* select 8bit char mode */

    msp430_uart_init_flag = TRUE;
    sdk_set_host_uart_baudrate(current_bt_uart_baudrate);

    /* Configure ports */
    *(bt_uart_config.uart_port_sel) |=
        bt_uart_config.uart_tx_pin + bt_uart_config.uart_rx_pin;
    *(bt_uart_config.uart_port_dir) |= bt_uart_config.uart_tx_pin;
    *(bt_uart_config.uart_port_dir) &= ~bt_uart_config.uart_rx_pin;

    /* CTS AND RTS DIRECTION SETTINGS */
    *(bt_uart_config.uart_cts_port_dir) &= ~(bt_uart_config.uart_cts_pin);
    *(bt_uart_config.uart_cts_port_ren) |= (bt_uart_config.uart_cts_pin);
    *(bt_uart_config.uart_cts_port_out) |= (bt_uart_config.uart_cts_pin);

    /* Clearing IFG Flag */
    *(bt_uart_config.uart_cts_ifg) &= ~BT_UART_CTS_PIN;
    /* Clearing Interrupt flag */
    *(bt_uart_config.uart_cts_ie) &= ~BT_UART_CTS_PIN;

    *(bt_uart_config.uart_rts_port_dir) |= (bt_uart_config.uart_rts_pin);

    /* Initializing PxSEL for CTS\RTS pins */
    *(bt_uart_config.uart_cts_port_sel) &= ~(bt_uart_config.uart_cts_pin);
    *(bt_uart_config.uart_rts_port_sel) &= ~(bt_uart_config.uart_rts_pin);
    *(bt_uart_config.uart_reg_ucaxifg) &= ~(UCTXIFG + UCRXIFG);



    /* clear UCSWRST */
    *(bt_uart_config.uart_reg_ucaxctl1) &= ~UCSWRST;

    msp430_uart_init_flag = FALSE;

    /* ENABLE uart RX interrupt */
    UART_ENABLE_BT_UART_RX();
}

/**
 * \fn      sdk_msp430_bt_uart_shutdown
 * \brief   This function is used to shutdown the msp430 Bluetooth UART while
 *          entering LPM
 * \param   void
 * \return  void
 */
void sdk_msp430_bt_uart_shutdown(void)
{
    /* Disable UART interrupt */
    *(bt_uart_config.uart_reg_ucaxie) &= ~UCRXIE;
    /* Reset UART state */
    *(bt_uart_config.uart_reg_ucaxctl1) |= UCSWRST;
    *(bt_uart_config.uart_port_out) |=
        (bt_uart_config.uart_tx_pin + bt_uart_config.uart_rx_pin);
    /* Configure UART TX and Rx as pins as GPIO */
    *(bt_uart_config.uart_port_sel) &=
        ~(bt_uart_config.uart_tx_pin + bt_uart_config.uart_rx_pin);
    /* Configure TX and Rx GPIO pins as Output pins */
    *(bt_uart_config.uart_port_dir) |= bt_uart_config.uart_tx_pin;
    *(bt_uart_config.uart_port_dir) |= bt_uart_config.uart_rx_pin;
}





/**
 * \fn      sdk_set_host_uart_baudrate
 * \brief   Set the uart baudrate
 * \param   baudrate    The value of baudrate to be set
 * \return  void
 */
void sdk_set_host_uart_baudrate(UINT32 baudrate)
{

    if (FALSE == msp430_uart_init_flag) {
        /* Set UCSWRST */
        *(bt_uart_config.uart_reg_ucaxctl1) |= UCSWRST;
    }

    /* SMCLK is set as UART clock source */
    *(bt_uart_config.uart_reg_ucaxctl1) |= UCSSEL_2;

    switch (baudrate) {
    case SDK_BAUDRATE_115200:
        switch (sys_clk_frequency) {
        case SYSCLK_8MHZ:
            /* Using UCOS16 = 0 settings */
            *(bt_uart_config.uart_reg_ucaxbr1) = 0x00;
            *(bt_uart_config.uart_reg_ucaxbr0) = 0x45;
            *(bt_uart_config.uart_reg_ucaxmctl) = 0x08;
            break;
        case SYSCLK_12MHZ:
            /* Using UCOS16 = 0 settings */
            *(bt_uart_config.uart_reg_ucaxbr1) = 0x00;
            *(bt_uart_config.uart_reg_ucaxbr0) = 0x68;
            *(bt_uart_config.uart_reg_ucaxmctl) = 0x02;
            break;
        case SYSCLK_18MHZ:
            /* Using UCOS16 = 0 settings */
            *(bt_uart_config.uart_reg_ucaxbr1) = 0x00;
            *(bt_uart_config.uart_reg_ucaxbr0) = 0x9C;
            *(bt_uart_config.uart_reg_ucaxmctl) = 0x04;
            break;
        case SYSCLK_25MHZ:
            /* Using UCOS16 = 0 settings */
            *(bt_uart_config.uart_reg_ucaxbr1) = 0x00;
            *(bt_uart_config.uart_reg_ucaxbr0) = 0xD9;
            *(bt_uart_config.uart_reg_ucaxmctl) = 0x00;
            break;
        default:
            sdk_error_code = SDK_BAUDRATE_NOT_SUPPORTED;
            sdk_error_handler();
            break;
        }
        break;
    case SDK_BAUDRATE_921600:
        switch (sys_clk_frequency) {
        case SYSCLK_25MHZ:
            /* Using UCOS16 = 0 settings */
            *(bt_uart_config.uart_reg_ucaxbr1) = 0x00;
            *(bt_uart_config.uart_reg_ucaxbr0) = 0x1B;
            *(bt_uart_config.uart_reg_ucaxmctl) = 0x02;
            break;
        default:
            sdk_error_code = SDK_BAUDRATE_NOT_SUPPORTED;
            sdk_error_handler();
            break;
        }
        break;
    case SDK_BAUDRATE_230400:
        switch (sys_clk_frequency) {
        case SYSCLK_8MHZ:
            /* Using UCOS16 = 0 settings */
            *(bt_uart_config.uart_reg_ucaxbr1) = 0x00;
            *(bt_uart_config.uart_reg_ucaxbr0) = 0x22;
            *(bt_uart_config.uart_reg_ucaxmctl) = 0x0C;
            break;
        case SYSCLK_12MHZ:
            /* Using UCOS16 = 0 settings */
            *(bt_uart_config.uart_reg_ucaxbr1) = 0x00;
            *(bt_uart_config.uart_reg_ucaxbr0) = 0x34;
            *(bt_uart_config.uart_reg_ucaxmctl) = 0x00;
            break;
        case SYSCLK_18MHZ:
            /* Using UCOS16 = 0 settings */
            *(bt_uart_config.uart_reg_ucaxbr1) = 0x00;
            *(bt_uart_config.uart_reg_ucaxbr0) = 0x4E;
            *(bt_uart_config.uart_reg_ucaxmctl) = 0x02;
            break;
        case SYSCLK_25MHZ:
            /* Using UCOS16 = 0 settings */
            *(bt_uart_config.uart_reg_ucaxbr1) = 0x00;
            *(bt_uart_config.uart_reg_ucaxbr0) = 0x6C;
            *(bt_uart_config.uart_reg_ucaxmctl) = 0x08;
            break;
        default:
            break;
        }
        break;
    default:
        sdk_error_code = SDK_BAUDRATE_NOT_SUPPORTED;
        sdk_error_handler();
        break;
    }

    if (FALSE == msp430_uart_init_flag) {
        *(bt_uart_config.uart_reg_ucaxctl1) &= ~UCSWRST;
        /* ENABLE uart RX interrupt */
        UART_ENABLE_BT_UART_RX();
    }
}
