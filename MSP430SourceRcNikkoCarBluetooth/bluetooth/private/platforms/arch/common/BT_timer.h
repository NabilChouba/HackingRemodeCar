
/**
 *  @file BT_timer.h
 *
 *  This Header File contains the APIs and the ADTs exported by the
 *  EtherMind Timer Library for Windows (User-mode).
 */

/*
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_TIMER_
#define _H_BT_TIMER_


/* --------------------------------------------------- Header File Inclusion */

/* --------------------------------------------------- Global Definitions */
/*
 *  Timer Handles must be initialized to this value
 */
#define BT_TIMER_HANDLE_INIT_VAL    NULL


/* ----------------------------------------------- Structures/Data Types */

/* Timer Entity */
typedef struct timer_entity_struct
{
    /* The Timer Handle */
    struct timer_entity_struct *handle;

    /* Callback to call when Timer expires */
    void (* callback) (void *, UINT16);

    UCHAR  *allocated_data;

    /* Next Element in the Timer Q */
    struct timer_entity_struct *next;

    UCHAR  static_data[BT_STATIC_DATA_SIZE];

    /* Time at which it is supposed to expire */
    UINT32 expire_at;

    /* Timeout Value asked by the User */
    UINT16 timeout;

    UINT16 data_length;

    /* Is this Entity Allocated ? */
    UCHAR in_use;

} TIMER_ENTITY;

typedef TIMER_ENTITY *  BT_timer_handle;



/* --------------------------------------------------- Function Declarations */

void timer_init ( void );
void timer_bt_init ( void );
void timer_bt_shutdown ( void );

API_RESULT BT_start_timer
           (
               BT_timer_handle *handle,
               UINT16 timeout,
               void (* callback) (void *, UINT16),
               void *args, UINT16 size_args
           );

API_RESULT BT_restart_timer
           (
               BT_timer_handle handle,
               UINT16 new_timeout
           );

API_RESULT BT_stop_timer ( BT_timer_handle handle );

API_RESULT BT_is_active_timer ( BT_timer_handle handle );

/* Debug Routine - Internal Use Only */
API_RESULT BT_list_timer ( void );

#endif /* _H_BT_TIMER_ */

