/**
 * Copyright (C) 2010 Texas Instruments, Inc.
 *
 * \file    hal_i2c.h 
 * \brief   hal_i2c.c header file
 * \version July 2010
 */

#ifndef HAL_I2C_H
#define HAL_I2C_H


/*******************************************************************************
    Defines Section
*******************************************************************************/

/********************************* Generic ************************************/
// Port, pins and registers resources for the I2C Interface 
// SCL = P10.2, SDA = P10.1
#define PORT_I2C_DIR  P10DIR
#define PORT_I2C_SEL  P10SEL
#define PORT_I2C_OUT  P10OUT
#define PIN_SCL       BIT2     
#define PIN_SDA       BIT1     
// Register and flag definitions
#define SLAVE_ADDR    UCB3I2CSA
#define CTL_REG_0     UCB3CTL0
#define CTL_REG_1     UCB3CTL1 
#define BAUD_REG_L    UCB3BR0
#define BAUD_REG_H    UCB3BR1
#define RX_BUFFER     UCB3RXBUF
#define TX_BUFFER     UCB3TXBUF
#define IRQ_REG       UCB3IFG
#define TX_IFG        UCTXIFG
#define RX_IFG        UCRXIFG
/******************************* End of Generic *******************************/

/******************************* Accelerometer ********************************/
// Port, pin and registers resources for the Accelerometer Interrupt 
// INT_ACCL = P1.2
#define PORT_INTA_DIR P1DIR
#define PORT_INTA_SEL P1SEL
#define PORT_INTA_IN  P1IN
#define PORT_INTA_IE  P1IE
#define PORT_INTA_IES P1IES
#define PORT_INTA_IFG P1IFG
#define PIN_INT_A     BIT2
/*************************** End of Accelerometer *****************************/

/******************************** Temperature *********************************/
// Port, pin and registers resources for the Temperature Sensor Interrupt 
// INT_TEMP = P1.7
#define PORT_INTT_DIR P1DIR
#define PORT_INTT_SEL P1SEL
#define PORT_INTT_IN  P1IN
#define PORT_INTT_IE  P1IE
#define PORT_INTT_IES P1IES
#define PORT_INTT_IFG P1IFG
#define PIN_INT_T     BIT7
/***************************** End of Temperature *****************************/


/*******************************************************************************
    Global Variables Section
*******************************************************************************/

/******************************* Accelerometer ********************************/
// CMA3000 registers
#define WHO_AM_I      0x00    // Identification register (R)
#define REVID         0x01    // ASIC revision ID, fixed in metal (R)
#define CTRL          0x02    // Configuration (por, operation modes) (RW)
#define STATUS        0x03    // Status (por, EEPROM parity) (R)
#define RSTR          0x04    // Reset Register (RW)
#define INT_STATUS    0x05    // Interrupt status register (R)
#define DOUTX         0x06    // X channel output data register (R)
#define DOUTY         0x07    // Y channel output data register (R)
#define DOUTZ         0x08    // Z channel output data register (R)
#define MDTHR         0x09    // Motion detection threshold value register (RW)
#define MDFFTMR       0x0A    // Free fall and motion detection time register (RW)
#define FFTHR         0x0B    // Free fall threshold value register (RW)
#define ACC_ADDR      0x1C    // Accelerometer I2C address (R)

// Control Register options (CTRL 0x02)
#define G_RANGE_2     0x80    // 2g range, 8g range otherwise
#define INT_LEVEL_LOW 0x40    // INT pin is active high, active low otherwise
#define MDET_NO_EXIT  0x20    // Remain in motion detection mode, goes to measurement mode otherwise
#define I2C_DIS       0x10    // I2C disabled, I2C enabled otherwise
#define MODE_PD       0x00    // Power Down - default mode
#define MODE_100      0x02    // Measurement mode 100 Hz ODR
#define MODE_400      0x04    // Measurement mode 400 Hz ODR
#define MODE_40       0x06    // Measurement mode 40 Hz ODR
#define MODE_MD_10    0x08    // Motion detection mode 10 Hz ODR
#define MODE_FF_100   0x0A    // Free fall detection mode 100 Hz ODR
#define MODE_FF_400   0x0C    // Free fall detection mode 400 Hz ODR
#define INT_DIS       0x01    // Interrupts disabled, interrupts enabled otherwise
/*************************** End of Accelerometer *****************************/

/******************************** Temperature *********************************/
#define TEMP_ADDR     0x48    // Temperature sensor I2C address (R)

// TMP106 registers
#define TEMP_DATA     0x00    // Temperature register (R)
#define CONFIG_TEMP   0x01    // Configuration register (RW)
#define T_LOW         0x02    // High Temp Threshold register (RW)
#define T_HIGH        0x03    // Low Temp Threshold register (RW)

// Configuration Register options (CONFIG_TEMP 0x01)
#define SHUTD_MODE    0x01    // Shutdown mode, continuous conversion state otherwise
#define THERM_MODE    0x02    // Interrupt mode, comparator mode otherwise
#define ALERT_POL     0x04    // Alert pin is active high, active low otherwise
#define FAULT_C2      0x08    // Fault Queue 2 consecutive faults
#define FAULT_C4      0x10    // Fault Queue 4 consecutive faults
#define FAULT_C6      0x18    // Fault Queue 6 consecutive faults
#define RES_10BITS    0x20    // Converter resolution 10 Bits (0.25 Celsius)
#define RES_11BITS    0x40    // Converter resolution 11 Bits (0.125 Celsius)
#define RES_12BITS    0x60    // Converter resolution 12 Bits (0.0625 Celsius)
#define ONE_SHOT      0x80    // Single temperature conversion in Shutdown mode
/***************************** End of Temperature *****************************/


/*******************************************************************************
    Prototypes Section
*******************************************************************************/

/********************************* Generic ************************************/
void halI2CInit(unsigned char clock);
void halI2CSetSlaveAddress(unsigned char address);
void halI2CWriteRegister(unsigned char address, unsigned char data); 
unsigned char halI2CRead1ByteRegister (unsigned char address);
unsigned int halI2CRead2ByteRegister (unsigned char address);
void halI2CShutdown(void);
/******************************* End of Generic *******************************/

/******************************* Accelerometer ********************************/
void halAccStart(void);
void halAccRead(char *coord);
void halAccStop(void);
/*************************** End of Accelerometer *****************************/

/******************************** Temperature *********************************/
void halTempStart(void);
int halTempRead(void);
void halTempStop(void);
/***************************** End of Temperature *****************************/

#endif