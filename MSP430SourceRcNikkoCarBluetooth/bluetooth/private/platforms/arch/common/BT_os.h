
/**
 * Copyright (C) 2000-2010. MindTree Ltd. All rights reserved.
 * \file    BT_os.h
 * \brief   This header file is part of EtherMind OS Abstraction module.
 *          In this file the platform specific data types are abstracted
 *          and mapped to data types used within the EtherMind Stack.
 */

#ifndef _H_BT_OS_
#define _H_BT_OS_

/* -------------------------------------------- Header File Inclusion */

#include <string.h>

#include "BT_features.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "semphr.h"
/* EtherMind Features */


/* -------------------------------------------- Global Definitions */

/* Definition for IN, OUT & INOUT Function/API Parameters */
#ifdef IN
#undef IN
#endif /* IN */

#define IN

#ifdef OUT
#undef OUT
#endif /* OUT */

#define OUT

#ifdef INOUT
#undef INOUT
#endif /* INOUT */

#define INOUT

/* -------------------------------------------- Structures/Data Types */

#define BT_thread_type OS_HANDLE

/* Datatype to represent File Handle */
/* 
 * typedef UINT32 BT_file_handle; typedef UINT32 BT_thread_attr_type; typedef
 * UINT32 BT_thread_cond_type; typedef UINT32 BT_thread_mutex_type; typedef
 * UINT32 BT_thread_cond_attr_type; typedef UINT32 BT_thread_mutex_attr_type;
 * typedef UINT32 HANDLE; */
#define BT_file_handle UINT32
#define BT_thread_attr_type UINT32
#define BT_thread_cond_type xSemaphoreHandle
#define BT_thread_mutex_type xSemaphoreHandle
#define BT_thread_cond_attr_type UINT32
#define BT_thread_mutex_attr_type UINT32

/* --------------------------------------------------- Macros */
/* Declaration of 'static' variable/function */
#ifdef BT_HAVE_STATIC_DECL
#define DECL_STATIC                     static
#else /* BT_HAVE_STATIC_DECL */
#define DECL_STATIC
#endif /* BT_HAVE_STATIC_DECL */

/* Declaration of 'const' variable */
#ifdef BT_HAVE_CONST_DECL
#define DECL_CONST                      const
#else /* BT_HAVE_CONST_DECL */
#define DECL_CONST
#endif /* BT_HAVE_CONST_DECL */

/* Abstractions for String library functions */
#define BT_str_len(s)                   strlen ((char *)(s))
#define BT_str_copy(d, s)               strcpy ((char *)(d), (char *)(s))
#define BT_str_cmp(s1, s2)              strcmp ((char *)(s1), (char *)(s2))
#define BT_str_cat(d, s)                strcat ((char *)(d), (char *)(s))


/* -------------------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif

    /* Task/Thread Creation Primitives */
    INT32 BT_thread_create(BT_thread_type * thread,
                           BT_thread_attr_type * thread_attr,
                           void *(*start_routine) (void *), void *thread_args);

    INT32 BT_thread_attr_init(BT_thread_attr_type * thread_attr);

    /* Task/Thread Synchronization Primitives */
    INT32 BT_thread_mutex_init(BT_thread_mutex_type * mutex,
                               BT_thread_mutex_attr_type * mutex_attr);

    INT32 BT_thread_mutex_lock(BT_thread_mutex_type * mutex);

    INT32 BT_thread_mutex_unlock(BT_thread_mutex_type * mutex);

    INT32 BT_thread_cond_init(BT_thread_cond_type * cond,
                              BT_thread_cond_attr_type * cond_attr);

    INT32 BT_thread_cond_wait(BT_thread_cond_type * cond,
                              BT_thread_mutex_type * cond_mutex);

    INT32 BT_thread_cond_signal(BT_thread_cond_type * cond);

    /* Memory Management Primitives */
    void BT_os_init(void);
    void *BT_alloc_mem(UINT32 size);
    void BT_free_mem(void *ptr);
    void *BT_mem_copy(void *dest, const void *src, UINT32 size);

    /* Task/Thread Delay Primitives */
    void BT_sleep(UINT32 tm);
    void BT_usleep(UINT32 tm);

#ifdef BT_DYNAMIC_MEM_STATISTICS
    void BT_os_mem_stat_init(void);

    void BT_os_dump_mem_stat(void);
#endif                          /* BT_DYNAMIC_MEM_STATISTICS */

#ifdef __cplusplus
};
#endif


#endif /* _H_BT_OS_ */
