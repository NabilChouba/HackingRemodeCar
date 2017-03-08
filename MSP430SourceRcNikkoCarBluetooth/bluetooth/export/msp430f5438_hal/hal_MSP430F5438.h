
/**
 * Copyright (C) 2010 Texas Instruments, Inc.
 *
 * \file    hal_MSP430F5438.h
 * \brief   This is the master header file and also the only necessary
 *          file to be included in order to use MSP430F548 HAL.
 */

#ifndef HAL_MSP430F5438_H
#define HAL_MSP430F5438_H

#include "arch_defines.h"

#include "hal_buttons.h"
#include "hal_adc.h"
#include "hal_board.h"
#include "hal_usb.h"
#include "hal_tlv.h"

#ifdef EZ430_PLATFORM
#include "hal_i2c.h"
#else /* MSP-EXP430 platform only */
#include "hal_lcd.h"
#endif /* EZ430_PLATFORM */

#endif /* HAL_MSP430F5438_H */
