
/**
 * Copyright (C) 2010 Texas Instruments, Inc.
 * \file    hal_usb.h
 * \brief   Contains pin definitions related to USB functionality
 */
#ifndef HAL_USB_H
#define HAL_USB_H

#ifdef EZ430_PLATFORM
#define USB_PORT_IN         P10IN
#define USB_PORT_OUT        P10OUT
#define USB_PORT_SEL        P10SEL
#define USB_PORT_DIR        P10DIR
#define USB_PORT_REN        P10REN
#define USB_PIN_TXD         BIT4
#define USB_PIN_RXD         BIT5
#else /* MSP-EXPF5438 platform */
#define USB_PORT_IN         P5IN
#define USB_PORT_OUT        P5OUT
#define USB_PORT_SEL        P5SEL
#define USB_PORT_DIR        P5DIR
#define USB_PORT_REN        P5REN
#define USB_PIN_TXD         BIT6
#define USB_PIN_RXD         BIT7
#endif /* EZ430_PLATFORM */

void halUsbInit(void);
void halusb_set_baudrate(unsigned long int baudrate);
void halUsbShutDown(void);
void halUsbSendChar(const unsigned char character);
void halUsbSendString(const unsigned char string[]);

#endif /* HAL_USB_H */
