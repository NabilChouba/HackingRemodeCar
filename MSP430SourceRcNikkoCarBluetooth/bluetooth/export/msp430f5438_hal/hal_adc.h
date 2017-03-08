
/**
 * @file  hal_adc.h
 * Copyright 2008 Texas Instruments, Inc.
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H


#define TEMP_CHANNEL                ADC12INCH_10
#define VCC_CHANNEL                 ADC12INCH_11

#define CELSIUS 			        0xFF
#define FAHRENHEIT 			        0x00

#define CELSIUS_MUL_F5438	 		7040
#define CELSIUS_OFFSET_F5438 		2620
#define FAHRENHEIT_MUL_F5438 		12672
#define FAHRENHEIT_OFFSET_F5438	 	3780

#define CELSIUS_MUL_F5438A	 		7040
#define CELSIUS_OFFSET_F5438A 		2620
#define FAHRENHEIT_MUL_F5438A 		12672
#define FAHRENHEIT_OFFSET_F5438A	3780

#ifndef EZ430_PLATFORM
#define ACC_PWR_PIN                 BIT0
#define ACC_PORT_DIR                P6DIR
#define ACC_PORT_OUT                P6OUT
#define ACC_PORT_SEL                P6SEL

#define ACC_X_PIN                   BIT1
#define ACC_Y_PIN                   BIT2
#define ACC_Z_PIN                   BIT3
#define ACC_X_CHANNEL               ADC12INCH_1
#define ACC_Y_CHANNEL               ADC12INCH_2
#define ACC_Z_CHANNEL               ADC12INCH_3

#define AUDIO_PORT_DIR              P6DIR
#define AUDIO_PORT_OUT              P6OUT
#define AUDIO_PORT_SEL              P6SEL

#define MIC_POWER_PIN               BIT4
#define MIC_INPUT_PIN               BIT5
#define MIC_INPUT_CHAN              ADC12INCH_5
#define AUDIO_OUT_PWR_PIN           BIT6

#define AUDIO_OUT_DIR               P4DIR
#define AUDIO_OUT_OUT               P4OUT
#define AUDIO_OUT_SEL               P4SEL

#define AUDIO_OUT_PIN               BIT4

#define ACC_X_THRESHOLD             200
#define ACC_Y_THRESHOLD             200
#define ACC_X_MAX                   1000
#define ACC_Y_MAX                   1000
#define ACC_Z_MAX                   1000

#define ACC_X_LOW_OFFSET            1950
#define ACC_X_HIGH_OFFSET           2150
#define ACC_Y_LOW_OFFSET            1950
#define ACC_Y_HIGH_OFFSET           2150
#define ACC_Z_LOW_OFFSET            1950
#define ACC_Z_HIGH_OFFSET           2150

enum { ADC_OFF_MODE, ADC_ACC_MODE, ADC_TEMP_MODE };

/*-------------Accelerometer Functions----------------------------------------*/
void halAccelerometerInit(void);
void halAccelerometerCalibrate(void);
void halAccelerometerSetCalibratedOffset(int x, int y);
void halAccelerometerGetCalibratedOffset(int *x, int *y);
void halAccelerometerRead(int *x, int *y);
void halAccelerometerReadWithOffset(int *x, int *y);
void halAccelerometerShutDown(void);

#endif  /* EZ430_PLATFORM */

/*-------------Temperature & VCC Functions------------------------------------*/
void halAdcInitTempVcc(void);
void halAdcShutDownTempVcc(void);
void halAdcSetTempConversionType(unsigned char conversion);
int halAdcGetTemp(void);
int halAdcGetVcc(void);
long int halAdcConvertTempVccFromADC(void);
void halAdcReadTempVcc(char *TemperatureStr, char *VccStr);

/*-------------Generic ADC12 Functions----------------------------------------*/
void halAdcStartRead(void);
void halAdcSetQuitFromISR(unsigned char quit);
void halBoardInitADCCalibration(void);
void haldisptemp(char *TemperatureStr, long int temp_val);

#endif
