
/**
 * Copyright (C) 2010 Texas Instruments, Inc.
 *
 * \file    hal_tlv.h
 * \brief   hal tlv header file
 * \version July 2010
 */

#ifndef HAL_TLV_H
#define HAL_TLV_H

#include "arch_defines.h"

#define DEVICE_ID_0		(0x1A04)
#define DEVICE_ID_1		(0x1A05)

struct s_TLV_Die_Record {
    unsigned char die_record[10];
};

struct s_TLV_ADC_Cal_Data {
    unsigned int adc_gain_factor;
    unsigned int adc_offset;
    unsigned int adc_ref15_factor;
    unsigned int adc_ref15_30_temp;
    unsigned int adc_ref15_85_temp;
    unsigned int adc_ref25_factor;
    unsigned int adc_ref25_30_temp;
    unsigned int adc_ref25_85_temp;
};

struct s_TLV_REF_Cal_Data {
    unsigned int ref_ref15;
    unsigned int ref_ref20;
    unsigned int adc_ref25;
};

typedef enum {
    F5438 = 0,
    F5438A
} device_t;

/************************************************************
* function declaration
************************************************************/
void Get_TLV_info(unsigned char tag, unsigned char *length,
                  unsigned int **data_address);
device_t Get_Device_Type(void);


#endif /* HAL_TLV_H */
