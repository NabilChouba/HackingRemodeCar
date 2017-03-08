
/**
 * Copyright (C) 2009 Texas Instruments, Inc.
 *
 * \file  hal_adc.c
 * \brief   This file contains functions to handle MSP430 EVM ADC functionality
 */


#include "arch_defines.h"
#include "hal_MSP430F5438.h"
#include "sdk_pl.h"
#include "hci_uart.h"

static int SavedADC12MEM0 = 0, SavedADC12MEM1 = 0, SavedADC12MEM2 = 0;
static int SavedADC12MEM3 = 0;

long int Vcc = 0, vcc_value = 0;

static float V30 = 0, V85 = 0;
float mref15 = 0, nref15 = 0;
static float mref25 = 0, nref25 = 0;
volatile struct s_TLV_ADC_Cal_Data *pADCCAL;

static unsigned char conversionType = FAHRENHEIT, adcMode = ADC_OFF_MODE;
static unsigned char exit_active_from_ADC12 = 0;

#ifndef EZ430_PLATFORM
int Acc_x = 0, Acc_y = 0, Acc_z = 0;
int Acc_x_offset = 0, Acc_y_offset = 0, Acc_z_offset = 0;

/**
 * @brief  Turns on and initializes ADC12, accelerometer in order to
 *         sample x, y, z-axis inputs.
 * @param  none
 * @return none
 */
void halAccelerometerInit(void)
{
    adcMode = ADC_ACC_MODE;
    ACC_PORT_SEL |= ACC_X_PIN + ACC_Y_PIN;  /* Enable A/D channel inputs */
    ACC_PORT_DIR &= ~(ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN);
    ACC_PORT_DIR |= ACC_PWR_PIN;    /* Enable ACC_POWER */
    ACC_PORT_OUT |= ACC_PWR_PIN;

    /* Sequence of channels, once, ACLK */
    ADC12CTL0 = ADC12ON + ADC12SHT02 + ADC12MSC;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1 + ADC12SSEL_0 + 0x2000;
    ADC12CTL2 = ADC12RES_2;
    ADC12MCTL2 = ACC_X_CHANNEL;
    ADC12MCTL3 = ACC_Y_CHANNEL + ADC12EOS;

    /* Allow the accelerometer to settle before sampling any data */
    /* This function is called for every accelerometer/thermometer data read
     * Due to the delay, the data throughput was affected drastically No valid
     * reason was also not found to add this delay So , commented the
     * __delay_cycles to increase the data throughput */
    /* __delay_cycles(200000); */


    UCSCTL8 |= MODOSCREQEN;
}

/**
 * @brief  Calibrates the offset values for x, y, and z axes.
 * @param  none
 * @return none
 */
void halAccelerometerCalibrate(void)
{
    unsigned char tempQuit;

    tempQuit = exit_active_from_ADC12;
    halAdcSetQuitFromISR(1);
    halAdcStartRead();

    __bis_SR_register(LPM3_bits + GIE);
    __no_operation();

    halAccelerometerReadWithOffset(&Acc_x_offset, &Acc_y_offset);
    halAdcSetQuitFromISR(tempQuit);
}

/**
 * @brief  Set function for the calibrated offsets for the x, y, and z axes.
 * @param  x Calibrated offset for the x-axis
 * @param  y Calibrated offset for the y-axis
 * @param  z Calibrated offset for the z-axis
 * @return none
 */
void halAccelerometerSetCalibratedOffset(int x, int y)
{
    Acc_x_offset = x;
    Acc_y_offset = y;
}


/**
 * @brief  Get function for the x, y, and z axes calibrated offsets
 * @param  x Pointer to the calibrated offset for the x-axis
 * @param  y Pointer to the calibrated offset for the y-axis
 * @param  z Pointer to the calibrated offset for the z-axis
 * @return none
 */
void halAccelerometerGetCalibratedOffset(int *x, int *y)
{
    *x = Acc_x_offset;
    *y = Acc_y_offset;
}


/**
 * @brief  Get function for the x, y, and z accelerometer samples,
 *         including the calibrated offsets.
 * @param  x Pointer to the accelerometer reading (x-axis)
 * @param  y Pointer to the accelerometer reading (y-axis)
 * @param  z Pointer to the accelerometer reading (z-axis)
 * @return none
 */
void halAccelerometerRead(int *x, int *y)
{
    Acc_x = SavedADC12MEM2;
    Acc_y = SavedADC12MEM3;

    *x = Acc_x - Acc_x_offset;
    *y = Acc_y - Acc_y_offset;
}


/**
 * @brief  Get function for the x, y, and z accelerometer samples,
 *         excluding the calibrated offsets.
 * @param  x Pointer to the accelerometer reading (x-axis)
 * @param  y Pointer to the accelerometer reading (y-axis)
 * @param  z Pointer to the accelerometer reading (z-axis)
 * @return none
 */
void halAccelerometerReadWithOffset(int *x, int *y)
{
    *x = SavedADC12MEM2;
    *y = SavedADC12MEM3;
}

/**
 * @brief  Disables the ADC12, accelerometer that sampled x, y, z-axis inputs.
 *
 * @param  none
 *
 * @return none
*/
void halAccelerometerShutDown(void)
{
    /* Turn off ADC Module */
    ADC12CTL0 &= ~(ADC12ON + ADC12ENC);
    ACC_PORT_OUT &= ~ACC_PWR_PIN;   /* Disable ACC_POWER */

    /* Disable A/D channel inputs */
    ACC_PORT_SEL &= ~(ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN);
    ACC_PORT_DIR |= (ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN + ACC_PWR_PIN);
    ACC_PORT_OUT &= ~(ACC_X_PIN + ACC_Y_PIN + ACC_Z_PIN + ACC_PWR_PIN);

    adcMode = ADC_OFF_MODE;
}

#endif /* EZ430_PLATFORM */

/*----------------------------------------------------------------------------*/

/**
 * @brief  Intializes the ADC12 to sample Temperature and Vcc.
 * @param  none
 * @return none
 */
void halAdcInitTempVcc(void)
{
    /* Sequence of channels, once, */
    adcMode = ADC_TEMP_MODE;
    UCSCTL8 |= MODOSCREQEN;
    ADC12CTL0 = ADC12ON + ADC12SHT0_15 + ADC12MSC + +ADC12REFON + ADC12REF2_5V;
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1 + ADC12SSEL_0;
    ADC12CTL2 = ADC12RES_2;

    ADC12MCTL0 = ADC12SREF_1 + TEMP_CHANNEL;
    ADC12MCTL1 = ADC12SREF_1 + VCC_CHANNEL + ADC12EOS;
}


/**
 * @brief  Turns off / disable the ADC12.
 * @param  none
 * @return none
 */
void halAdcShutDownTempVcc(void)
{
    ADC12CTL0 &= ~(ADC12ON + ADC12ENC + ADC12REFON);
    adcMode = ADC_OFF_MODE;
}

/**
 * @brief  Sets the conversion type to either Farenheit (F) or Celsius (C).
 * @param  conversion The #define constant CELSIUS or FAHRENHEIT.
 * @return none
 */
void halAdcSetTempConversionType(unsigned char conversion)
{
    conversionType = conversion;
}


/**
 * @brief  Get function for the current Vcc value.
 * @param  none
 * @return The current Vcc value.
 */
int halAdcGetVcc(void)
{
    return Vcc;
}

/**
 * @brief  Converts the Vcc and Temp readings from the ADC to BCD format.
 * @param  none
 * @return none
 */
long int halAdcConvertTempVccFromADC(void)
{
    float Temperature = 0;
    // Convert Vcc
    Vcc = SavedADC12MEM1;
    Vcc = Vcc * 50 + 2048;      // add .5*4096 so truncation rounds up
    Vcc = Vcc / 4096;
    vcc_value = Vcc;
    /* The temperature value is multiplied by 10 so that to be displyed in XX.X
     * format */
    Temperature = ((float)SavedADC12MEM0 * 5 * 10) / 8192;
    Temperature = (Temperature - (nref25 * 10)) / (mref25);

    if (conversionType == FAHRENHEIT) {
        Temperature = Temperature * 9 / 5 + (32 * 10);
    }
    return (long int)Temperature;
}

/**
 * @brief  Get function for the temperature and Vcc samples in "xxx^C/F" and
 *         "x.xV" format.
 * @param  TemperatureStr The string that holds the temperature reading
 * @param  Vcc            The string that holds the Vcc reading
 * @return none
 */
void haldisptemp(char *TemperatureStr, long int temp_val)
{
    unsigned char i, leadingZero = 0;
    long int Temperature;

    Temperature = temp_val;
    for (i = 0; i < 8; i++)
        TemperatureStr[i] = '\0';
    i = 0;
    /* Check for negative */
    if (Temperature < 0) {
        TemperatureStr[i++] = '-';
        Temperature = -Temperature;
    }
    TemperatureStr[i] = '0';
    if (Temperature >= 1000) {
        TemperatureStr[i] = '1';
        Temperature -= 1000;
        leadingZero = 1;
    }
    if (leadingZero == 1)
        i++;
    /* 100s digit */
    TemperatureStr[i] = '0';
    if (Temperature >= 100) {
        do {
            TemperatureStr[i]++;
            Temperature -= 100;
        }
        while (Temperature >= 100);
        leadingZero = 1;
    }
    if (leadingZero == 1)
        i++;
    /* 10s digit */
    TemperatureStr[i] = '0';
    if (Temperature >= 10) {
        do {
            TemperatureStr[i]++;
            Temperature -= 10;
        }
        while (Temperature >= 10);
    }
    TemperatureStr[++i] = '.';
    TemperatureStr[++i] = Temperature + '0';

    TemperatureStr[++i] = '^';
    if (conversionType == CELSIUS)
        TemperatureStr[++i] = 'C';
    else
        TemperatureStr[++i] = 'F';
}

/*----------------------------------------------------------------------------*/

/**
 * @brief  Starts the ADC conversion.
 * @param  none
 * @return none
 */
void halAdcStartRead(void)
{
    ADC12IFG &= ~(BIT2 + BIT1 + BIT0);  /* Clear any pending flags */

    if (adcMode == ADC_ACC_MODE) {
        ADC12CTL0 |= ADC12ENC | ADC12SC;
        ADC12IE |= BIT3;
    } else {
        ADC12CTL0 |= ADC12REFON;    /* Turn on ADC12 reference */

        /* Delay to stabilize ADC12 reference assuming the fastest MCLK of 18 */
        /* MHz.  */
        /* 35 us = 1 / 18 MHz * 630 */
        /* Commenting the delay for thermometer application */

    /*__delay_cycles(630); */

        ADC12IE |= BIT1;        /* Enable interrupt */
        ADC12CTL0 |= ADC12ENC | ADC12SC;
    }
}

/**
 * @brief  Sets the flag that causes an exit into active CPU mode from
 *         the ADC12 ISR.
 * @param  quit
 * - 1 - Exit active from ADC12 ISR
 * - 0 - Remain in LPMx on exit from ADC12ISR
 * @return none
 */
void halAdcSetQuitFromISR(unsigned char quit)
{
    exit_active_from_ADC12 = quit;
}

/**
 * @brief  Calibrate the ADC
 * @param  none
 * @return none
 */
void halBoardInitADCCalibration(void)
{
    unsigned char bADCCAL_bytes;

    /* Check for ADC TLV using the tag 0x10 for F5438 and 0x11 for F5438A */
    if (Get_Device_Type() == F5438)
        Get_TLV_info(0x10, &bADCCAL_bytes, (unsigned int **)&pADCCAL);
    else
        Get_TLV_info(0x11, &bADCCAL_bytes, (unsigned int **)&pADCCAL);

    /* *** Calculation of the temp slope & offset** */
    /* Reference 1,5V */
    V30 = ((float)pADCCAL->adc_ref15_30_temp * 1.5) / 4096;
    V85 = ((float)pADCCAL->adc_ref15_85_temp * 1.5) / 4096;
    mref15 = (V85 - V30) / (85 - 30);
    nref15 = V85 - mref15 * 85;

    /**
	 * For release F5438A revisions, comment out the if/else and leave the V30,
	 * V85 calculation below. The offset from the datasheet is +- 20 deg C, so a
     * wide variance in the temperature calculations for the F5438A is to be
     * expected if the .00225 slope and 680 mV offset are used.
	 */
    if (Get_Device_Type() == F5438) {
        // Reference 2,5V
        V30 = ((float)pADCCAL->adc_ref25_30_temp * 2.5) / 4096;
        V85 = ((float)pADCCAL->adc_ref25_85_temp * 2.5) / 4096;
        mref25 = (V85 - V30) / (85 - 30);
        nref25 = V85 - mref25 * 85;
    } else {
        mref25 = .00225;        /* .00225 slope per the datasheet */
        nref25 = .680;          /* 680 mV offset per the datasheet */
    }
}

/*----------------------------------------------------------------------------*/
#ifdef __TI_COMPILER_VERSION__
#pragma CODE_SECTION(ADC12_ISR,".text:_isr");
#endif /* __TI_COMPILER_VERSION__ */
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    UART_DISABLE_BT_UART_RTS();
    ADC12IFG = 0;               /* Clear the interrupt flags */
    ADC12CTL0 &= ~(ADC12ENC | ADC12SC | ADC12REFON);

    __bis_SR_register(GIE);

    SavedADC12MEM0 = ADC12MEM0; /* Store the sampled data */
    SavedADC12MEM1 = ADC12MEM1;
    SavedADC12MEM2 = ADC12MEM2; /* Store the sampled data */
    SavedADC12MEM3 = ADC12MEM3;
}
