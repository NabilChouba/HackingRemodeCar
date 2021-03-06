
/**
 *  @file write_task.h
 *
 *  Module Header File for Write Task module.
 */

/* 
 *  Copyright (C) 2009-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_WRITE_TASK_
#define _H_WRITE_TASK_

/* ----------------------------------------------- Header File Inclusion */

/* ----------------------------------------------- Type Definition */
/* The Write Task Queues - in the order of decreasing priority */
#define WRITE_TASK_INTERNAL_QUEUE       0x00
#define WRITE_TASK_USER_API_QUEUE       0x01
#define WRITE_TASK_USER_DATA_QUEUE      0x02


/* ----------------------------------------------- Function Declarations */

/** Init routine */
API_RESULT write_task_init(void);

/** Activating routine */
API_RESULT write_task_bt_init(void);

/** Shutdown routine */
API_RESULT write_task_bt_shutdown(void);

/** Enqueue routine */
API_RESULT write_task_enqueue(UCHAR queue_type /* Queue Type */ ,
                              void (*bh_func) (void *, UINT16)  /* Bottom Half
                                                                 * Function
                                                                 * Pointer */ ,
                              void *data    /* Data to be passed to Bottom Half 
                                             */ ,
                              UINT16 datalen    /* Data length */
    );


#endif /* _H_WRITE_TASK_ */
