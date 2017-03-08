
/**
 *  @file BT_debug_api.h
 *
 *  This Header File contains the APIs and the ADTs exported by the
 *  EtherMind Debug Library for Windows (User-mode).
 */

/* 
 *  Copyright (C) 2000-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_DEBUG_API_
#define _H_BT_DEBUG_API_


/* ----------------------------------------------- Header File Inclusion */


/* ----------------------------------------------- Timer Debug Macros */
#ifdef WIN32
#define BT_debug_error \
        fflush (bt_debug_fd); \
        fprintf (bt_debug_fd, "\n[** ERR **]:[%s]:[%d]: ", __FILE__, __LINE__); \
        fprintf

#define BT_debug_trace \
        fflush (bt_debug_fd); \
        fprintf (bt_debug_fd, "\n[-- TRC --]:[%s]:[%d]: ", __FILE__, __LINE__); \
        fprintf

#define BT_debug_info \
        fflush (bt_debug_fd); \
        fprintf (bt_debug_fd, "\n[== INF ==]:[%s]:[%d]: ", __FILE__, __LINE__); \
        fprintf

#define BT_debug_dump \
        fflush (bt_debug_fd); \
        fprintf (bt_debug_fd, "[++ HEX ++]: "); \
        fprintf

#define BT_debug_direct \
        fflush (bt_debug_fd); \
        fprintf (bt_debug_fd, "[-- TRC --]: "); \
        fprintf
#else
#ifdef __IAR_SYSTEMS_ICC__
#define BT_debug_error(...)
#define BT_debug_trace(...)
#define BT_debug_info(...)
#define BT_debug_dump(...)
#define BT_debug_direct(...)
#define BT_debug_null(...)
#else
#define BT_debug_error
#define BT_debug_trace
#define BT_debug_info
#define BT_debug_dump
#define BT_debug_direct
#define BT_debug_null
#endif /* __IAR_SYSTEMS_ICC__ */
#endif /* WIN32 */

/* ----------------------------------------------- Global Definitions */

/* ----------------------------------------------- Structures/Data Types */


/* ----------------------------------------------- Function Declarations */

void BT_Init_Debug_Library(void);

/* Debug Library ON & OFF Routines */
void debug_bt_init(void);
void debug_bt_shutdown(void);

INT32 BT_debug_dump_bytes(UCHAR * buffer, UINT32 length);
INT32 BT_debug_dump_decimal(UCHAR * buffer, UINT32 length);


#ifdef __cplusplus
extern "C" {
#endif

    void BT_debug_settings(UINT16 flag);

#ifdef __cplusplus
};
#endif


#ifdef HAVE_ENCRYPTED_DEBUG
void BT_debug_encrypt(FILE * fp, const char *format, ...);
#endif /* HAVE_ENCRYPTED_DEBUG */

#ifdef WINDOWS_SDK
void BT_debug_display(FILE * fp, const char *format, ...);
#endif

#endif /* _H_BT_DEBUG_API_ */
