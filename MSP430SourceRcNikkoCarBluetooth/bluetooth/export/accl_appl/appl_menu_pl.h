
/**
 * Copyright (C) 2009-2010. MindTree Ltd.  All rights reserved.
 * \file    appl_menu_pl.h
 * \brief This file contains function/datatype specific to menu handling in MSP430
 */


#ifndef _H_APPL_MENU_PL_
#define _H_APPL_MENU_PL_

/* ----------------------------------------------- Header File Inclusion */
#include "hal_MSP430F5438.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"

/* ----------------------------------------------- Macros */
#define BT_DISCOVERABLE_STATE           0x21

/* Define the MAIN_MENU elements */
#define OP_INVALID                      0xFF
#define OP_PEER_CONNECT                 0x01
#define OP_PEER_DATASEND                0x02

#endif /* _H_APPL_MENU_PL_ */
