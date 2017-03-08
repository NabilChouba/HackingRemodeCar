
/**
 * Copyright (C) 2010 MindTree Ltd. All rights reserved.
 * \file    arch_defines.h
 * \brief   This header file is used to abstract the MSP430 device variant
 */

#ifndef _H_ARCH_DEFINES_
#define _H_ARCH_DEFINES_

/**
 * The default device is chosen as MSP430BT5190. If the build is required for
 * a different variant of the MSP430 family, include the appropriate header
 * file instead of msp430bt5190.h
 */
#include <msp430bt5190.h>

#endif /* _H_ARCH_DEFINES_ */
