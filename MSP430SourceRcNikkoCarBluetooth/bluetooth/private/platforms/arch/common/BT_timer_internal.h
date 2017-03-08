
/**
 *  @file BT_timer_internal.h
 *
 *  This Header File contains Internal Declarations of Structures,
 *  Functions and Global Definitions for Windows (User-mode).
 */

/*
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_TIMER_INTERNAL_
#define _H_BT_TIMER_INTERNAL_

/* ----------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_debug.h"
#include "BT_timer.h"
#include "write_task.h"

/* ----------------------------------------------- Timer Debug Macros */

#ifdef BT_TIMERL_DEBUG
#define BT_TIMERL_TRC           BT_debug_trace
#else  /* BT_TIMERL_DEBUG */
#define BT_TIMERL_TRC           BT_debug_null
#endif /* BT_TIMERL_DEBUG */

#ifdef BT_TIMERT_DEBUG
#define BT_TIMERT_TRC           BT_debug_trace
#else  /* BT_TIMERT_DEBUG */
#define BT_TIMERT_TRC           BT_debug_null
#endif /* BT_TIMERT_DEBUG */

#ifdef BT_TIMER_DEBUG

#define BT_TIMER_ERR            BT_debug_error
#define BT_TIMER_TRC            BT_debug_trace
#define BT_TIMER_INF            BT_debug_info

#else  /* BT_TIMER_DEBUG */

#define BT_TIMER_ERR            BT_debug_null
#define BT_TIMER_TRC            BT_debug_null
#define BT_TIMER_INF            BT_debug_null

#endif /* BT_TIMER_DEBUG */


/* ----------------------------------------------- Global Definitions */

/* Timer Malloc/Free Related --------- */
#define timer_malloc            BT_alloc_mem
#define timer_free              BT_free_mem

/* Lock/Unlock Timer Library */
#define timer_lock()            BT_thread_mutex_lock(&timer_mutex)
#define timer_unlock()          BT_thread_mutex_unlock(&timer_mutex)

/* Null Check for Timer Handles */
#define timer_null_check(p)     if ((p) == NULL) return -1;

/* Timer Task State Values */
#define TIMER_INIT              0x00
#define TIMER_WAITING           0x01
#define TIMER_RUNNING           0x02
#define TIMER_SHUTDOWN          0x03
#define TIMER_IMMEDIATE         0x04

/* One Timer Tick => 1 Second */
#define TIMER_TICK              1

/* ----------------------------------------------- Structures/Data Types */


/* ----------------------------------------------- Internal Functions */

void * timer_start_routine(void *);
void timer_service_queue (void);

API_RESULT timer_add_entity ( TIMER_ENTITY *timer );
API_RESULT timer_del_entity ( TIMER_ENTITY *timer, UCHAR free );
API_RESULT timer_search_entity ( TIMER_ENTITY *timer );
API_RESULT timer_init_entity ( TIMER_ENTITY *timer );

UINT32 timer_get_current_time ( void );

void timer_wait ( void );
void timer_signal ( void );
void timer_sleep (UINT16 seconds);

#endif /* _H_BT_TIMER_INTERNAL_ */

