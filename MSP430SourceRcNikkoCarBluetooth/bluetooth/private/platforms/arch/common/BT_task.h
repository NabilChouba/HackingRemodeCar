
/**
 * Copyright (C) 2000-2010. MindTree Ltd. All rights reserved.
 * \file 	BT_task.h
 * \brief 	This Header File contains the lists of tasks
 *			needed for the EtherMind Bluetooth stack.
 */

#ifndef _H_BT_TASK_
#define _H_BT_TASK_

/* Task Priorites. 3 is the highest priority and 1 being the lowest priority */
#define READ_TASK_PRIORITY    3
#define WRITE_TASK_PRIORITY   2
#define USER_TASK_PRIORITY    1

/* Stack size for each task */
#define READ_TASK_STACK_SIZE  350
#define WRITE_TASK_STACK_SIZE 300
#define USER_TASK_STACK_SIZE  275

/* Task name */
#define READ_TASK_NAME        "ReadTask"
#define WRITE_TASK_NAME       "WriteTask"
#define USER_TASK_NAME        "UserTask"

typedef struct {
    CHAR *name;
    UINT8 priority;
    UINT16 task_stack_size;
} BT_OS_THREAD_PROPERTY;

#endif /* _H_BT_TASK_ */
