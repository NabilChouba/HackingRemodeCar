
/**
 * Copyright (C) 2009 Texas Instruments, Inc.
 *
 * \file    hal_tlv.c
 * \brief   This file contains functions to get tag length value
 */

#include "hal_tlv.h"

/* Function Defintion Passing parameters: tag - The specific/particular tag
 * information (length and data address location) required by the user *length
 * - Address of the variable (byte long) where the number of bytes allocated
 * for the particular 'tag' is to be stored *data_address - Address of the
 * variable (word long) which is used to access the value info of the the
 * particular 'tag' (in the TLV structure) NOTE: If 'length' and 'data_address' 
 * * returned = 0, it implies that there was no tag match. This condition can
 * be checked for in the main function that calls this Get_TLV_info(..)
 * function and appropriate actions can be taken */

void Get_TLV_info(unsigned char tag, unsigned char *length,
                  unsigned int **data_address)
{
    /* TLV Structure Start Address */
    volatile char *TLV_address = (char *)TLV_START;

    while ((*TLV_address != tag) && (*TLV_address != TLV_TAGEND)
           && (TLV_address < (char *)TLV_END)) {
        /* add (Current TAG address + LENGTH) + 2 */
        TLV_address += *(TLV_address + 1) + 2;
    }
    if (*TLV_address == tag) {  /* Check if Tag match happened.. */
        /* Return length = Address + 1 */
        *length = *(TLV_address + 1);
        /* Return address of first data/value info = Address + 2 */
        *data_address = (unsigned int *)(TLV_address + 2);
    } else {
        /* If there was no tag match and the end of TLV structure was reached */
        *length = 0;            /* Return 0 for TAG not found */
        data_address = 0;       /* Return 0 for TAG not found */
    }
}

/* Function Defintion Returns device_type - either F5438 or F5438A, depending
 * on device */
device_t Get_Device_Type(void)
{
    device_t device_type;
    /* Check for device type - either F5438 or F5438A */
    if (*((char *)DEVICE_ID_0) == 0x54 && *((char *)DEVICE_ID_1) == 0x38)
        device_type = F5438;
    else if (*((char *)DEVICE_ID_0) == 0x05 && *((char *)DEVICE_ID_1) == 0x80)
        device_type = F5438A;

    return device_type;
}
