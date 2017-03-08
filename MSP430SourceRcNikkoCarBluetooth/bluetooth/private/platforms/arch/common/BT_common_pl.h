
/**
 *  @file BT_common_pl.h
 *
 *  This file contains the Function Declaration, and Constant Definitions
 *  for the EtherMind Bluetooth Stack in Windows User Mode.
 */

/* 
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_COMMON_PL_
#define _H_BT_COMMON_PL_

/* ------------------------------------------- Header File Inclusion */


/* ------------------------------------------- Common PL Debug */
#define PL_ERR                                  BT_debug_error

#ifdef PL_DEBUG

#define PL_TRC                                  BT_debug_trace
#define PL_INF                                  BT_debug_info

#else /* PL_DEBUG */

#define PL_TRC                                  BT_debug_null
#define PL_INF                                  BT_debug_null

#endif /* PL_DEBUG */


/* ------------------------------------------- Global Definitions/Macros */
/* Set Thread's Data Access Permission - Not used */
#define BT_THREAD_SET_DATA_ACCESS_PERMISSIONS()



/* ------------------------------------------- Data Structures */
typedef struct {
    CHAR byte[16];
} INT128;


/* ------------------------------------------- Function Declarations */
/* EtherMind-Init: Platform Lower & Upper Handler */
void ethermind_init_lower_pl(void);
void ethermind_init_upper_pl(void);

/* Bluetooth-ON: Platform Lower & Upper Handler */
void bluetooth_on_lower_pl(void);
void bluetooth_on_upper_pl(void);

/* Bluetooth-OFF: Platform Lower & Upper Handler */
void bluetooth_off_lower_pl(void);
void bluetooth_off_upper_pl(void);

/* To set Serial (UART) parameters in HCI-UART module */
/* int set_serial_params (HANDLE fd, int baud); */


#endif /*_H_BT_COMMON_PL*/
