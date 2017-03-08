
/**
 * Copyright (C) 2010 MindTree Ltd.  All rights reserved.
 * \file    sdk_pin_config.h
 * \brief   This header file contains pin definitions for the MSP430 setup
 */

#ifndef _H_SDK_PIN_CONFIG_
#define _H_SDK_PIN_CONFIG_

/* 
 * UART parameters, the PIOs which are mapped to Rx, Tx, RTS, CTS,
 * ACLK (32.768kHz slow clock), nShutdown and "flight mode" related
 * Pin Assignments for CC2560
 */
/* Bluetooth UART port definitions */
/* Select UART Tx and Rx port selction register */
#define BT_UART_PORT_SEL                P9SEL
/* select UART Tx and Rx port direction register */
#define BT_UART_PORT_DIR                P9DIR
/* select UART Tx and Rx port OUTregister */
#define BT_UART_PORT_OUT                P9OUT
/* UART tx pin */
#define BT_UART_TX_PIN                  BIT4
/* UART rx pin */
#define BT_UART_RX_PIN                  BIT5
/* UART RTS port direction selection register */
#define BT_UART_RTS_PORT_DIR            P1DIR
/* UART CTS port direction selection register */
#define BT_UART_CTS_PORT_DIR            P1DIR

#ifdef EZ430_PLATFORM
#define BT_UART_CTS_PIN                 BIT4    /* UART CTS pin */
#define BT_UART_RTS_PIN                 BIT3    /* UART RTS pin */
/* Interrupt vector value generated for CTS pin interrupt */
#define BT_UART_CTS_REG_PXIV            P1IV_P1IFG4
#else /* MSP-EXPF5438 platform */
#define BT_UART_CTS_PIN                 BIT3    /* UART CTS pin */
#define BT_UART_RTS_PIN                 BIT4    /* UART RTS pin */
/* Interrupt vector value generated for CTS pin interrupt */
#define BT_UART_CTS_REG_PXIV            P1IV_P1IFG3
#endif /* EZ430_PLATFORM */
#define BT_CTS_PIN_VECTOR               PORT1_VECTOR

/* UART CTS PORT IES */
#define BT_UART_CTS_PORT_IES            P1IES
/* UART CTS PROT IFG */
#define BT_UART_CTS_PORT_IFG            P1IFG
/* UART VTS PORT IE */
#define BT_UART_CTS_PORT_IE	P1IE
/* UART CTS PORT SELECT */
#define BT_UART_CTS_PORT_SEL            P1SEL
/* UART RTS PORT SELECT */
#define BT_UART_RTS_PORT_SEL            P1SEL
/* UART RTS port output register */
#define BT_UART_RTS_PORT_OUT            P1OUT
/* UART CTS port output register */
#define BT_UART_CTS_PORT_OUT            P1OUT
/* UART CTS port input register */
#define BT_UART_CTS_PORT_IN             P1IN
/* UART CTS Port Resistor Enable */
#define BT_UART_CTS_PORT_REN            P1REN
/* UART control register 0 */
#define BT_UART_REG_UCAXCTL0            UCA2CTL0
/* UART control register 1 */
#define BT_UART_REG_UCAXCTL1            UCA2CTL1
/* UART baudrate selecation register 0 */
#define BT_UART_REG_UCAXBR0             UCA2BR0
/* UART baudrate selecation register 1 */
#define BT_UART_REG_UCAXBR1             UCA2BR1
/* UART Interrupt vector register */
#define BT_UART_REG_UCAXIV              UCA2IV
/* UART output status register */
#define BT_UART_REG_UCAXSTAT            UCA2STAT
/* UART Tx buffer register */
#define BT_UART_REG_UCAXTXBUF           UCA2TXBUF
/* UART Rx buffer register */
#define BT_UART_REG_UCAXRXBUF           UCA2RXBUF
/* UART modulation control register */
#define BT_UART_REG_UCAXMCTL            UCA2MCTL
/* UART interrupt enable register */
#define BT_UART_REG_UCAXIE              UCA2IE
/* UART interrupt flag register */
#define BT_UART_REG_UCAXIFG             UCA2IFG
#define BT_UART_VECTOR                  USCI_A2_VECTOR
#define USB_UART_VECTOR                 USCI_A1_VECTOR



/**
 * BT controller Related Configuration Parameters
 */
#ifdef EZ430_PLATFORM
#define BT_RF_CLK_PORT_SEL              P2SEL   /* Clock Port Select */
#define BT_RF_CLK_PORT_PIN              BIT6    /* Clock Port Pin */
#define BT_RF_CLK_PORT_DIR              P2DIR   /* Clock Port Direction */
#define BT_RF_CLK_PORT_OUT              P2OUT   /* Clock Port Out */

#define BT_RF_NSHUTDOWN_PORT_SEL        P2SEL   /* nShutdown Port Select */
#define BT_RF_NSHUTDOWN_PORT_DIR        P2DIR   /* nShutdown Port Direction */
#define BT_RF_NSHUTDOWN_PORT_PIN        BIT7    /* nShutdown Port Pin */
#define BT_RF_NSHUTDOWN_PORT_OUT        P2OUT   /* nShutdown Port Out */
#else /* MSP-EXPF5438 platform */
#define BT_RF_CLK_PORT_SEL              P11SEL  /* Clock Port Select */
#define BT_RF_CLK_PORT_PIN              BIT0    /* Clock Port Pin */
#define BT_RF_CLK_PORT_DIR              P11DIR  /* Clock Port Direction */
#define BT_RF_CLK_PORT_OUT              P11OUT  /* Clock Port Out */

#define BT_RF_NSHUTDOWN_PORT_SEL        P8SEL   /* nShutdown Port Select */
#define BT_RF_NSHUTDOWN_PORT_DIR        P8DIR   /* nShutdown Port Direction */
#define BT_RF_NSHUTDOWN_PORT_PIN        BIT2    /* nShutdown Port Pin */
#define BT_RF_NSHUTDOWN_PORT_OUT        P8OUT   /* nShutdown Port Out */
#endif /* EZ430_PLATFORM */

#endif /* _H_SDK_PIN_CONFIG_ */
