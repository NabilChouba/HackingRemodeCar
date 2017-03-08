
/**
 * Copyright (C) 2010 Texas Instruments, Inc.
 *
 * \file    hal_i2c.c
 * \brief   This file contains functions to handle i2c peripheral in the RF2560T
            It handles external accelerometer and temperature sensors
 * \version July 2010
 */

/*******************************************************************************
    Include Section
*******************************************************************************/

#include  "arch_defines.h"
#include "hal_MSP430F5438.h"
#include "hal_i2c.h"


/*******************************************************************************
    Functions Section
*******************************************************************************/

/********************************* Generic ************************************/

/**
 * \fn      halI2CInit
 * \brief   Setup I2C peripheral in the MSP430 device
 * \param   clock       the system clock frequency to set I2C clock
 * \return  void
 */
void halI2CInit(unsigned char clock)
{

    // Initialize I2C peripheral interface
    PORT_I2C_SEL |= PIN_SCL + PIN_SDA;  // Port 10, Pins 1 and 2

    CTL_REG_1 |= UCSWRST;       // Put I2C state machine in reset
    CTL_REG_1 |= UCSSEL_2;      // SMCLK as clock source
    CTL_REG_0 |= UCMST + UCSYNC + UCMODE_3; // I2C mode, master, synchronous
    BAUD_REG_H = 0;             // Set BR high byte divider to 0

    switch (clock) {            // Set BR low byte divider based on system
        // clock

    case (SYSCLK_1MHZ):
        BAUD_REG_L = 0x0A;      // (SMCLK)1MHz/10 = 100kHz
        break;

    case (SYSCLK_4MHZ):
        BAUD_REG_L = 0x28;      // (SMCLK)4MHz/40 = 100kHz
        break;

    case (SYSCLK_8MHZ):
        BAUD_REG_L = 0x50;      // (SMCLK)8MHz/80 = 100kHz
        break;

    case (SYSCLK_12MHZ):
        BAUD_REG_L = 0x78;      // (SMCLK)12MHz/120 = 100kHz
        break;

    case (SYSCLK_18MHZ):
        BAUD_REG_L = 0xB4;      // (SMCLK)18MHz/180 = 100kHz
        break;

    case (SYSCLK_20MHZ):
        BAUD_REG_L = 0xC8;      // (SMCLK)20MHz/200 = 100kHz
        break;

    case (SYSCLK_25MHZ):
        BAUD_REG_L = 0xFA;      // (SMCLK)25MHz/250 = 100kHz
        break;
    }

    CTL_REG_1 &= ~UCSWRST;      // Start I2C state machine
}

/**
 * \fn      halI2CSetSlaveAddress
 * \brief   Set slave address to be used in the MSP430 I2C peripheral
 * \param   address     I2C slave address
 * \return  void
 */
void halI2CSetSlaveAddress(unsigned char address)
{

    CTL_REG_1 |= UCSWRST;       // Put I2C state machine in reset to set SA
    SLAVE_ADDR = address;       // Set slave address in MSP430 I2C register
    CTL_REG_1 &= ~UCSWRST;      // Start I2C state machine
}

/**
 * \fn      halI2CWriteRegister
 * \brief   Write a 1-byte data value to a device through the I2C interface
 * \param   address     device register address
 * \param   data        value to be written in the selected device register
 *\return   void
 */
void halI2CWriteRegister(unsigned char address, unsigned char data)
{

    // Start I2C and send slave address
    CTL_REG_1 |= UCTR + UCTXSTT;    // I2C TX, START condition, send slave
    // address
    while (!(IRQ_REG & TX_IFG));    // Wait for slave address transmit to
    // complete

    // Select register address in slave device
    TX_BUFFER = address;        // Load TX buffer with register address
    while (!(IRQ_REG & TX_IFG));    // Wait for transmit to complete

    // Set data value to be written
    TX_BUFFER = data;           // Load TX buffer with data byte
    while (!(IRQ_REG & TX_IFG));    // Wait for transmit to complete

    // Terminate I2C TX communication
    CTL_REG_1 |= UCTXSTP;       // Set I2C STOP condition
    while (CTL_REG_1 & UCTXSTP);    // Wait for stop condition to complete
    IRQ_REG &= ~TX_IFG;         // Clear I2C TX int flag
}

/**
 * \fn      halI2CRead1ByteRegister
 * \brief   Read a 1-byte data value from a device through the I2C interface
 * \param   address     device register address
 * \return  result      data value from the selected device register
 */
unsigned char halI2CRead1ByteRegister(unsigned char address)
{

    unsigned char result;

    // Start I2C and send slave address
    CTL_REG_1 |= UCTR + UCTXSTT;    // I2C TX, START condition, send slave
    // address
    while (!(IRQ_REG & TX_IFG));    // Wait for slave address transmit to
    // complete

    // Select register address in slave device
    TX_BUFFER = address;        // Load TX buffer with register address
    while (!(IRQ_REG & TX_IFG));    // Wait for transmit to complete

    // Set I2C to receiver mode
    IRQ_REG &= ~TX_IFG;         // Clear I2C TX int flag
    CTL_REG_1 &= ~UCTR;         // Toggle to receiver mode

    // Restart I2C and send slave address
    CTL_REG_1 |= UCTXSTT;       // Set START condition, send slave address
    while (CTL_REG_1 & UCTXSTT);    // Wait for slave address transmit to
    // complete

    // Receive 1-byte data value
    CTL_REG_1 |= UCTXSTP;       // Set I2C STOP condition
    while (!(IRQ_REG & RX_IFG));    // Wait for receive buffer to fill
    result = RX_BUFFER;         // Fill buffer with received byte

    // Wait and terminate I2C RX communication
    while (CTL_REG_1 & UCTXSTP);    // Wait for stop condition to complete
    IRQ_REG &= ~RX_IFG;         // Clear USCI_B0 RX int flag

    // return read data
    return result;
}

/**
 * \fn      halI2CRead2ByteRegister
 * \brief   Read a 2-byte data value from a device through the I2C interface
 * \param   address, device register address
 * \return  result, data value from the selected device register
 */
unsigned int halI2CRead2ByteRegister(unsigned char address)
{

    unsigned char result[2];

    // Start I2C and send slave address
    CTL_REG_1 |= UCTR + UCTXSTT;    // I2C TX, START condition, send slave
    // address
    while (!(IRQ_REG & TX_IFG));    // Wait for slave address transmit to
    // complete

    // Select register address in slave device
    TX_BUFFER = address;        // Load TX buffer with register address
    while (!(IRQ_REG & TX_IFG));    // Wait for transmit to complete

    // Set I2C to receiver mode
    IRQ_REG &= ~TX_IFG;         // Clear I2C TX int flag
    CTL_REG_1 &= ~UCTR;         // Toggle to receiver mode

    // Restart I2C and send slave address
    CTL_REG_1 |= UCTXSTT;       // Set START condition, send slave address
    while (CTL_REG_1 & UCTXSTT);    // Wait for slave address transmit to
    // complete

    // Receive first byte data value
    while (!(IRQ_REG & RX_IFG));    // Wait for receive buffer to fill
    result[1] = RX_BUFFER;      // Fill buffer with received byte

    // Receive second byte data value
    CTL_REG_1 |= UCTXSTP;       // Set I2C STOP condition
    while (!(IRQ_REG & RX_IFG));    // Wait for receive buffer to fill
    // buffer[0] = RX_BUFFER; // Fill buffer with received byte
    result[0] = RX_BUFFER;      // Fill buffer with received byte

    // Wait and terminate I2C RX communication
    while (CTL_REG_1 & UCTXSTP);    // Wait for stop condition to complete
    IRQ_REG &= ~RX_IFG;         // Clear USCI_B0 RX int flag

    // Return read data
    return *(int *)result;
}

/**
 * \fn      halI2CShutdown
 * \brief   Shutdown I2C peripheral in the MSP430 device
 * \param   void
 * \return  void
 */
void halI2CShutdown(void)
{

    CTL_REG_1 |= UCSWRST;       // Put I2C state machine in reset

    // Close I2C peripheral interface
    PORT_I2C_OUT &= ~(PIN_SCL + PIN_SDA);   // I2C pins to low to avoid
    // floating
    PORT_I2C_DIR |= PIN_SCL + PIN_SDA;  // I2C pins to output
    PORT_I2C_SEL &= ~(PIN_SCL + PIN_SDA);   // Port pins to I/O functions
}

/******************************* End of Generic *******************************/

/******************************* Accelerometer ********************************/

/**
 * \fn      halAccStart
 * \brief   Initialize and configure accelerometer device
 * \param   void
 * \return  void
 */
void halAccStart(void)
{

    // Set interrupt pin
    PORT_INTA_SEL &= ~PIN_INT_A;    // Set INT pin to I/O
    PORT_INTA_DIR &= ~PIN_INT_A;    // Set INT pin to Input

    // Initialize accelerometer
    halI2CSetSlaveAddress(ACC_ADDR);    // Set I2C accelerometer's address
    halI2CRead1ByteRegister(REVID); // Read REVID register
    halI2CWriteRegister(CTRL, G_RANGE_2 | MODE_400);
    // 2g range, Measurement Mode 400Hz
    // and INT enabled
}

/**
 * \fn      halAccRead
 * \brief   Read XYZ coordinates from accelerometer device
 * \param   coord, 3-data array to save coordinate
 * \return  void
 */
void halAccRead(char *coord)
{

    halI2CSetSlaveAddress(ACC_ADDR);    // Set I2C accelerometer's address

    while (!(PORT_INTA_IN & PIN_INT_A));    // Data is valid?
    *(coord + 0) = halI2CRead1ByteRegister(DOUTX);  // Read DOUTX register

    while (!(PORT_INTA_IN & PIN_INT_A));    // Data is valid?
    *(coord + 1) = halI2CRead1ByteRegister(DOUTY);  // Read DOUTY register

    while (!(PORT_INTA_IN & PIN_INT_A));    // Data is valid?
    *(coord + 2) = halI2CRead1ByteRegister(DOUTZ);  // Read DOUTZ register
}

/**
 * \fn      halAccStop
 * \brief   Power down accelerometer device
 * \param   void
 * \return  void
 */
void halAccStop(void)
{

    // Send reset sequence to accelerometer
    halI2CSetSlaveAddress(ACC_ADDR);    // Set I2C accelerometer's address
    halI2CWriteRegister(RSTR, 0x02);
    halI2CWriteRegister(RSTR, 0x0A);
    halI2CWriteRegister(RSTR, 0x04);
}

/*************************** End of Accelerometer *****************************/

/******************************** Temperature *********************************/

/**
 * \fn       halTempStart
 * \brief    Initialize and configure temperature sensor
 * \param    void
 * \return   void
 */
void halTempStart(void)
{

    // Set alert pin
    PORT_INTT_SEL &= ~PIN_INT_T;    // Set ALERT pin to I/O
    PORT_INTT_DIR &= ~PIN_INT_T;    // Set ALERT pin to Input

    // Initialize temperature sensor
    halI2CSetSlaveAddress(TEMP_ADDR);   // Set I2C temp sensor's address
    halI2CWriteRegister(CONFIG_TEMP, ALERT_POL | RES_12BITS);
    // Continuous conversion, 12-Bit resolution
    // and ALERT active high
}

/**
 * \fn      halTempRead
 * \brief   Read temperature value from temperature sensor
 * \param   void
 * \return  temp    temperature reading
 */
int halTempRead(void)
{

    halI2CSetSlaveAddress(TEMP_ADDR);   // Set I2C temp sensor's address
    return halI2CRead2ByteRegister(TEMP_DATA);  // Read temperature register
    // High Byte ONLY
}

/**
 * \fn      halTempStop
 * \brief   Power down temperature sensor
 * \param   void
 * \return  void
 */
void halTempStop(void)
{

    // Send shutdown command to temperature sensor
    halI2CSetSlaveAddress(TEMP_ADDR);   // Set I2C temp sensor's address
    halI2CWriteRegister(CONFIG_TEMP, SHUTD_MODE);
}

/*************************** End of Temperature *****************************/
