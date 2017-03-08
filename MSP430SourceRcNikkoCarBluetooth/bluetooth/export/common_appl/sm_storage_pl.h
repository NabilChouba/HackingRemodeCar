
/**
 * Copyright (c) 2009-2010. MindTree Ltd.  All rights reserved.
 * \file    sm_storage_pl.h
 * \brief   This file contains declarations for the storage related API's
 */

#ifndef _H_SM_STORAGE_PL_
#define _H_SM_STORAGE_PL_

/* Header File Inclusion */
#include "FreeRTOSConfig.h"
#include "portmacro.h"

/* Enumerations */
/* Enumeration for sm ps open mode */
typedef enum {
    PS_WRITE = 0x00,
    PS_READ = 0x01
} SDK_SM_PS_OPEN_MODES;


/* Functions declarations */

/* Function to initialize the perstance storage for reading or writing */
extern void sm_ps_open(SDK_SM_PS_OPEN_MODES mode);

/* Function to close the operations on perstance storage */
extern void sm_ps_close(void);

/* Function to write to the perstance storage. The implemenation is platform
 * specific */
extern void sm_ps_write(UCHAR * p, UINT16 nb);

/* Function to read from the perstance storage. The implemenation is platform
 * specific */
extern void sm_ps_read(UCHAR * p, UINT16 nb);

#endif /* _H_SM_STORAGE_PL_ */
