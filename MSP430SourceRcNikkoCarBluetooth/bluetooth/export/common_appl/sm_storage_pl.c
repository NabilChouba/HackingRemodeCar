
/**
 * Copyright (c) 2009-2010. MindTree Ltd.  All rights reserved.
 * \file    sm_storage_pl.c
 * \brief   This file contains the implementation for the storage related API's
 *          The example shows an implementation where the information is stored
 *          in the RAM and is maintained across BT on/off cycles without
 *          recycling the power.
 *          An empty region of the RAM is selected based on the map file output
 *          Similar implementation could be done for access to flash.
 */

/* Header File Inclusion */
#include "sm_storage_pl.h"
#define SM_STORAGE_START_ADDR 0x4C00    /* Initialize to free RAM location */
static volatile UCHAR *sm_storage_ptr;  /* location for storing SM info */

/* Function definitions */

 /**
 * \fn      sm_ps_open
 * \brief   Function to initialize the persistent storage for reading or writing
 *          The implemenation is platform specific.
 * \param   mode    mode of operation(read - PS_READ or write - PS_WRITE)
 * \return  void
 */

void sm_ps_open(SDK_SM_PS_OPEN_MODES mode)
{
    sm_storage_ptr = (unsigned char *)SM_STORAGE_START_ADDR;
}

 /**
 * \fn      sm_ps_close
 * \brief   Function to close the operations on persistent storage
 *          The implemenation is platform specific.
 * \param   void
 * \return  void
 */

void sm_ps_close(void)
{
}


 /**
 * \fn      sm_ps_write
 * \brief   Function to write to the persistent storage. The implemenation is
 *          platform specific.
 * \param   p       Address of buffer
 * \param   nb      Number of bytes to be written to persistent storage
 * \return  void
 */

void sm_ps_write(UCHAR * p, UINT16 nb)
{
    UINT16 st_index;

    __disable_interrupt();
    UART_DISABLE_BT_UART_RTS();

    /* Copy information to SM storage location */
    for (st_index = 0; st_index < nb; st_index++) {
        *sm_storage_ptr = *p;
        sm_storage_ptr++;
        p++;

    }
    __enable_interrupt();
}

 /**
 * \fn      sm_ps_read
 * \brief   Function to read from the persistent storage. The implemenation is
 *          platform specific.
 * \param   p       Address of buffer
 * \param   nb      Number of bytes to be read to persistent storage
 * \return  void
 */

void sm_ps_read(UCHAR * p, UINT16 nb)
{
    UINT16 st_index;

    /* Read information from SM storage */
    for (st_index = 0; st_index < nb; st_index++) {
        *(p + st_index) = *(sm_storage_ptr + st_index);
    }

    sm_storage_ptr += nb;

}
