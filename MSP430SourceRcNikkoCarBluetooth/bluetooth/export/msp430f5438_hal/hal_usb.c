
/**
 * @file  hal_usb.c
 * Copyright 2008 Texas Instruments, Inc.
 */

#include  "arch_defines.h"
#include "hal_MSP430F5438.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "sdk_bluetooth_config.h"
#include "sdk_pl.h"
#include "bt_sdk_error.h"

#ifdef EZ430_PLATFORM
#define USB_CTL1            UCA3CTL1
#define USB_CTL0            UCA3CTL0
#define USB_BRW             UCA3BRW
#define USB_BR0             UCA3BR0
#define USB_BR1             UCA3BR1
#define USB_MCTL            UCA3MCTL
#define USB_IE              UCA3IE
#define USB_IFG             UCA3IFG
#define USB_TXBUF           UCA3TXBUF
#define USB_RXBUF           UCA3RXBUF
#else /* MSP-EXP430F5438 Platform */
#define USB_CTL1            UCA1CTL1
#define USB_CTL0            UCA1CTL0
#define USB_BRW             UCA1BRW
#define USB_BR0             UCA1BR0
#define USB_BR1             UCA1BR1
#define USB_MCTL            UCA1MCTL
#define USB_IE              UCA1IE
#define USB_IFG             UCA1IFG
#define USB_TXBUF           UCA1TXBUF
#define USB_RXBUF           UCA1RXBUF
#endif /* EZ430_PLATFORM */


/* Extern variables */
/* Current CPU frequency */
extern volatile UINT32 inactivity_counter;
extern volatile UCHAR lpm_mode;
extern UINT32 sdk_error_code;
extern UCHAR sys_clk_frequency;

/* Static variables */
char halUsbReceiveBuffer[255];
volatile unsigned char bufferSize = 0;
extern void restore_peripheral_status(void);

/* Function definitions */

/**
 * \fn      halUsbInit
 * \brief   Initializes the serial communications peripheral and GPIO ports
 *          to communicate with the TUSB3410.
 * \param   void
 * \return  void
 */
void halUsbInit(void)
{
    volatile unsigned char i;

    for (i = 0; i < 255; i++)
        halUsbReceiveBuffer[i] = '\0';

    bufferSize = 0;
    USB_PORT_SEL |= USB_PIN_RXD + USB_PIN_TXD;
    USB_PORT_DIR |= USB_PIN_TXD;
    USB_PORT_DIR &= ~USB_PIN_RXD;

    USB_CTL1 |= UCSWRST;        /* Reset State */
    USB_CTL0 = UCMODE_0;        /* UART mode */

    USB_CTL0 &= ~UC7BIT;        /* 8bit char */

    /* Set the baud rate for Serial port */
#ifdef EZ430_PLATFORM
    halusb_set_baudrate(SDK_BAUDRATE_9600);
#else
    halusb_set_baudrate(SDK_BAUDRATE_115200);
#endif

    USB_CTL1 &= ~UCSWRST;
    USB_IE |= UCRXIE;
}

/**
 * \fn      halUsbShutDown
 * \brief   Disables the serial communications peripheral and clears the GPIO
 *          settings used to communicate with the TUSB3410.
 * \param   void
 * \return  void
 */
void halUsbShutDown(void)
{
    USB_IE &= ~UCRXIE;
    USB_CTL1 = UCSWRST;         /* Reset State */
    USB_PORT_SEL &= ~(USB_PIN_RXD + USB_PIN_TXD);
    USB_PORT_DIR |= USB_PIN_TXD;
    USB_PORT_DIR |= USB_PIN_RXD;
    USB_PORT_OUT &= ~(USB_PIN_TXD + USB_PIN_RXD);
}

/**
 * \fn      halUsbSendChar
 * \brief   Sends a character over UART to the TUSB3410.
 * \param   character The character to be sent.
 * \return  void
 */
void halUsbSendChar(const unsigned char character)
{
    while (!(USB_IFG & UCTXIFG));
    USB_TXBUF = character;
}

/**
 * \fn      halUsbSendString
 * \brief   Sends a string of characters to the TUSB3410
 * \param   string[] The array of characters to be transmit to the TUSB3410.
 * \return  void
 */
void halUsbSendString(const unsigned char string[])
{
    unsigned char length;
    volatile unsigned char i;

    length = strlen((const char *)string);
    for (i = 0; i < length; i++) {
        halUsbSendChar(string[i]);
    }
}

/**
 * \fn      USB_UART_VECTOR
 * \brief   This is the USB interrupt handler.The byte received on the USB is
 *          copied to a receive buffer.
 * \param   void
 * \return  void
 */
#ifdef __TI_COMPILER_VERSION__
#pragma CODE_SECTION(USB_UART_ISR, ".text:_isr");
#endif /* __TI_COMPILER_VERSION__ */
#pragma vector=USB_UART_VECTOR
__interrupt void USB_UART_ISR(void)
{
    char temp_data;
    temp_data = USB_RXBUF;
    halUsbReceiveBuffer[bufferSize++] = temp_data;
    inactivity_counter = 0;
#ifdef MSP430_LPM_ENABLE
    if (TRUE == lpm_mode) {
        lpm_mode = FALSE;
        restore_peripheral_status();
        EXIT_MSP430_LPM();
    }
#endif /* MSP430_LPM_ENABLE */
#ifdef DEBUG_TESTING
    halUsbSendString("~\n");
#endif
}


/**
 * \fn      halusb_set_baudrate
 * \brief   This function is used to set baudrate register values for UART 1
 *          module, which is used as USB serial port. The baudrate is set as
 *          115200 bps
 * \param   void
 * \return  void
 */
void halusb_set_baudrate(unsigned long int baudrate)
{
    /* SMCLK is set as UART clock source */
    USB_CTL1 |= UCSSEL_2;
    switch (baudrate) {
    case SDK_BAUDRATE_9600:
        switch (sys_clk_frequency) {
            /* Initialize register values for UART baud rate = 9600bps */
            /* Set BR Low byte on system clock */
        case SYSCLK_8MHZ:
            USB_BRW = 0x0341;   /* 9600 Baud */
            USB_MCTL = 0x02;    /* 0x00; 0x02; */
            break;
        case SYSCLK_12MHZ:
            USB_BRW = 0x04E2;   /* 9600 Baud */
            USB_MCTL = 0x00;    /* 0x00; 0x00; */
            break;
        case SYSCLK_18MHZ:
            USB_BRW = 0x0753;   /* 9600 Baud */
            USB_MCTL = 0x00;    /* 0x00; 0x00; */
            break;
        case SYSCLK_25MHZ:
            USB_BRW = 0x0A2C;   /* 9600 Baud */
            USB_MCTL = 0x02;    /* 0x00; 0x02; */
            break;
        default:
            sdk_error_code = SDK_BAUDRATE_NOT_SUPPORTED;
            sdk_error_handler();
            break;
        }
        break;

    case SDK_BAUDRATE_115200:
        switch (sys_clk_frequency) {
            /* Initialize register values for UART baud rate = 115200bps */
        case SYSCLK_8MHZ:
            /* Using UCOS16 = 0 settings */
            USB_BR1 = 0x00;
            USB_BR0 = 0x45;
            USB_MCTL = 0x08;
            break;
        case SYSCLK_12MHZ:
            /* Using UCOS16 = 0 settings */
            USB_BR1 = 0x00;
            USB_BR0 = 0x68;
            USB_MCTL = 0x02;
            break;
        case SYSCLK_18MHZ:
            /* Using UCOS16 = 0 settings */
            USB_BR1 = 0x00;
            USB_BR0 = 0x9C;
            USB_MCTL = 0x04;
            break;
        case SYSCLK_25MHZ:
            /* Using UCOS16 = 0 settings */
            USB_BR1 = 0x00;
            USB_BR0 = 0xD9;
            USB_MCTL = 0x00;
            break;
        default:
            sdk_error_code = SDK_BAUDRATE_NOT_SUPPORTED;
            sdk_error_handler();
            break;
        }
        break;

    default:
        sdk_error_handler();
        break;
    }
}
