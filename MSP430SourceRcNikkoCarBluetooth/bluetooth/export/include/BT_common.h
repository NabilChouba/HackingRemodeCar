
/**
 *  @file BT_common.h
 *
 *  This EtherMind Header File containing common declarations for the
 *  EtherMind Bluetooth Stack & Profile modules.
 */

/* 
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_COMMON_
#define _H_BT_COMMON_

/* -------------------------------------------- Header File Inclusion */
#ifdef BT_1_2
#define BT_HCI_1_2
#define BT_L2CAP_1_2
#endif /* BT_1_2 */

/* For Memory (leak, corruption) Testing */
#ifdef MEMWATCH
#include "memwatch.h"
#endif /* MEMWATCH */

/* The EtherMind OS Abstraction */
#include "BT_os.h"

/* The EtherMind Tunable Constant */
#include "BT_limits.h"

/* The Bluetooth Assigned Numbers */
#include "BT_assigned_numbers.h"

/* The EtherMind Error Codes */
#include "BT_error.h"

/* For BT_assert() macro */
#include "BT_assert.h"

/* For Status Flag APIs */
#include "BT_status.h"

/* The EtherMind Timer Library */
#include "BT_timer.h"

/* The EtherMind Write Task */
#include "write_task.h"

/* The EtherMind SDP Static Database */
#include "db_gen.h"

/* EtherMind Platform Specific Initialization & Shutdown Handlers */
#include "BT_common_pl.h"

/* -------------------------------------------- Global Definitions */

#ifdef FALSE
#undef FALSE
#endif /* FALSE */

/* Definition for True/False */
#define FALSE                                   0

#ifdef TRUE
#undef TRUE
#endif /* TRUE */

#define TRUE                                    1

/* Size of Bluetooth Device Address (BD_ADDR) in number of Octets */
#define BT_BD_ADDR_SIZE                         6

/* L2CAP PSM Values */
#define SDP_PSM                                 0x0001
#define RFCOMM_PSM                              0x0003
#define TCS_BIN_PSM                             0x0005
#define BNEP_PSM                                0x000F
#define HID_CONTROL_PSM                         0x0011
#define HID_INTERRUPT_PSM                       0x0013
#define AVCTP_PSM                               0x0017
#define AVDTP_PSM                               0x0019

/* Type Definitions for 16, 32 & 128-bit UUIDs */
#define UUID_16                                 1
#define UUID_32                                 2
#define UUID_128                                4


/* -------------------------------------------- Macros */
/* Macro to Copy a BD_ADDR from a Source to Destination */
#define BT_COPY_BD_ADDR(dest, src) \
    *((dest) + 0) = *((src) + 0);  \
    *((dest) + 1) = *((src) + 1);  \
    *((dest) + 2) = *((src) + 2);  \
    *((dest) + 3) = *((src) + 3);  \
    *((dest) + 4) = *((src) + 4);  \
    *((dest) + 5) = *((src) + 5);


#ifndef BT_DISABLE_MUTEX

/* Macro to define a Mutex Variable */
#define BT_DEFINE_MUTEX(mutex) BT_thread_mutex_type mutex;

/* Macro to define a Mutex Variable with a type qualifier */
#define BT_DEFINE_MUTEX_TYPE(type, mutex) type BT_thread_mutex_type mutex;

/* Macro to define a Conditional Variable */
#define BT_DEFINE_COND(cond) BT_thread_cond_type cond;

/* Macro to define a Conditional Variable with a type qualifier */
#define BT_DEFINE_COND_TYPE(type, cond) type BT_thread_cond_type cond;

/* 
 *  Macro to Initialize Mutex.
 *  To be used in void function as it returns no error.
 */
#define BT_MUTEX_INIT_VOID(mutex, MODULE)                                \
    if (BT_thread_mutex_init(&(mutex), NULL) < 0)                        \
    {                                                                    \
        BT_debug_error(bt_debug_fd,                                      \
        "FAILED to Initialize Mutex in " #MODULE ".\n");                 \
        return;                                                          \
    }

/* 
 *  Macro to Initialize Mutex.
 *  It returns an error if mutex initialization fails.
 */
#define BT_MUTEX_INIT(mutex, MODULE)                                     \
    if (BT_thread_mutex_init(&(mutex), NULL) < 0)                        \
    {                                                                    \
        BT_debug_error(bt_debug_fd,                                      \
        "FAILED to Initialize Mutex in " #MODULE ".\n");                 \
        return MODULE##_MUTEX_INIT_FAILED;                               \
    }

/* 
 *  Macro to Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define BT_COND_INIT_VOID(cond, MODULE)                                  \
    if (BT_thread_cond_init(&(cond), NULL) < 0)                          \
    {                                                                    \
        BT_debug_error(bt_debug_fd,                                      \
        "FAILED to Initialize Conditional Variable in " #MODULE ".\n");  \
        return;                                                          \
    }

/* 
 *  Macro to Initialize Conditional Variable.
 *  It returns an error if conditional variable initialization fails.
 */
#define BT_COND_INIT(cond, MODULE)                                       \
    if (BT_thread_cond_init(&(cond), NULL) < 0)                          \
    {                                                                    \
        BT_debug_error(bt_debug_fd,                                      \
        "FAILED to Initialize Conditional Variable in " #MODULE ".\n");  \
        return MODULE##_COND_INIT_FAILED;                                \
    }

/* 
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. It returns an error if mutex lock fails.
 */
#define BT_MUTEX_LOCK(mutex, MODULE)                                 \
    if (BT_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        BT_debug_error(bt_debug_fd,                                  \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        return MODULE##_MUTEX_LOCK_FAILED;                           \
    }

/* 
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. To be used in void function as it
 *  returns no error.
 */
#define BT_MUTEX_LOCK_VOID(mutex, MODULE)                            \
    if (BT_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        BT_debug_error(bt_debug_fd,                                  \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        return;                                                      \
    }

/* 
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function.
 *  It returns the error 'value' if mutex lock failes.
 */
#define BT_MUTEX_LOCK_RETURN_ON_FAILURE(mutex, MODULE, value)        \
    if (BT_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        BT_debug_error(bt_debug_fd,                                  \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
        return (value);                                              \
    }

/* 
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. On failure, only an Error is logged.
 *  It can be used from both void and non-void functions.
 */
#define BT_MUTEX_LOCK_DONOT_RETURN_ON_FAILURE(mutex, MODULE)         \
    if (BT_thread_mutex_lock(&(mutex)) < 0)                          \
    {                                                                \
        BT_debug_error(bt_debug_fd,                                  \
        "FAILED to Lock Mutex in " #MODULE ".\n");                   \
    }

/* 
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. It returns an error if mutex unlock fails.
 */
#define BT_MUTEX_UNLOCK(mutex, MODULE)                               \
    if (BT_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        BT_debug_error(bt_debug_fd,                                  \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        return MODULE##_MUTEX_UNLOCK_FAILED;                         \
    }

/* 
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. To be used in void functions as it returns
 *  no error.
 */
#define BT_MUTEX_UNLOCK_VOID(mutex, MODULE)                          \
    if (BT_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        BT_debug_error(bt_debug_fd,                                  \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        return;                                                      \
    }

/* 
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into.
 *  It returns the error 'value' if mutex unlock failes.
 */
#define BT_MUTEX_UNLOCK_RETURN_ON_FAILURE(mutex, MODULE, value)      \
    if (BT_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        BT_debug_error(bt_debug_fd,                                  \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
        return (value);                                              \
    }

/* 
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. On failure, only Error is logged.
 *  It can be used from both void and non-void functions.
 */
#define BT_MUTEX_UNLOCK_DONOT_RETURN_ON_FAILURE(mutex, MODULE)       \
    if (BT_thread_mutex_unlock(&(mutex)) < 0)                        \
    {                                                                \
        BT_debug_error(bt_debug_fd,                                  \
        "FAILED to Unlock Mutex in " #MODULE ".\n");                 \
    }

#else /* BT_DISABLE_MUTEX */

/* Macro to define a Mutex Variable */
#define BT_DEFINE_MUTEX(mutex)

/* Macro to define a Mutex Variable with a type qualifier */
#define BT_DEFINE_MUTEX_TYPE(type, mutex)

/* Macro to define a Conditional Variable */
#define BT_DEFINE_COND(cond)

/* Macro to define a Conditional Variable with a type qualifier */
#define BT_DEFINE_COND_TYPE(type, cond)

/* 
 *  Macro to Initialize Mutex.
 *  To be used in void function as it returns no error.
 */
#define BT_MUTEX_INIT_VOID(mutex, MODULE)

/* 
 *  Macro to Initialize Mutex.
 *  It returns an error if mutex initialization fails.
 */
#define BT_MUTEX_INIT(mutex, MODULE)

/* 
 *  Macro to Initialize Conditional Variable.
 *  To be used in void function as it returns no error.
 */
#define BT_COND_INIT_VOID(cond, MODULE)

/* 
 *  Macro to Initialize Conditional Variable.
 *  It returns an error if conditional variable initialization fails.
 */
#define BT_COND_INIT(cond, MODULE)

/* 
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. It returns an error if mutex lock fails.
 */
#define BT_MUTEX_LOCK(mutex, MODULE)

/* 
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. To be used in void function as it
 *  returns no error.
 */
#define BT_MUTEX_LOCK_VOID(mutex, MODULE)

/* 
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function.
 *  It returns the error 'value' if mutex lock failes.
 */
#define BT_MUTEX_LOCK_RETURN_ON_FAILURE(mutex, MODULE, value)

/* 
 *  Locks the Module Specific Mutex which prevents any global variable being
 *  overwritten by any function. On failure, only an Error is logged.
 *  It can be used from both void and non-void functions.
 */
#define BT_MUTEX_LOCK_DONOT_RETURN_ON_FAILURE(mutex, MODULE)

/* 
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. It returns an error if mutex unlock fails.
 */
#define BT_MUTEX_UNLOCK(mutex, MODULE)

/* 
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. To be used in void functions as it returns
 *  no error.
 */
#define BT_MUTEX_UNLOCK_VOID(mutex, MODULE)

/* 
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into.
 *  It returns the error 'value' if mutex unlock failes.
 */
#define BT_MUTEX_UNLOCK_RETURN_ON_FAILURE(mutex, MODULE, value)

/* 
 *  Unlocks the Module Specific Mutex which realeses the global variables
 *  to be written into. On failure, only Error is logged.
 *  It can be used from both void and non-void functions.
 */
#define BT_MUTEX_UNLOCK_DONOT_RETURN_ON_FAILURE(mutex, MODULE)

#endif /* BT_DISABLE_MUTEX */


/* -------------------------------------------- Structures/Data Types */

/* 128-Bit UUID */
typedef struct {
    UCHAR byte[16];

} UINT128_ID;


/* The UUID Data Type */
typedef struct {
    UCHAR uuid_type;

    union {
        UINT16 uuid_16;
        UINT32 uuid_32;
        UINT128_ID uuid_128;
    } uuid_union;

} S_UUID;


/* -------------------------------------------- Function/API Declarations */
#ifdef __cplusplus
extern "C" {
#endif

    /* EtherMind Initialization */
    void BT_ethermind_init(void
        );

    /* Bluetooth ON */
    API_RESULT
        BT_bluetooth_on(API_RESULT(*appl_hci_event_ind_cb)
                        (UCHAR, UCHAR *, UCHAR),
                        API_RESULT(*appl_bluetooth_on_complete_cb) (void),
                        CHAR * local_name);

    /* Bluetooth OFF */
    API_RESULT BT_bluetooth_off(void
        );

    /* Function to trigger indication of Bluetooth ON Complete */
    API_RESULT BT_common_bluetooth_on_complete(void
        );

#ifdef __cplusplus
};
#endif


#endif /* _H_BT_COMMON_ */
