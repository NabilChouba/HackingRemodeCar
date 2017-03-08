
/**
 * Copyright (C) 2000-2010. MindTree Ltd. All rights reserved.
 * \file    BT_buffer.h
 * \brief   This Header File contains the lists of buffers
 *          needed for the EtherMind Bluetooth stack.
 */

#ifndef _H_BT_BUFFER_
#define _H_BT_BUFFER_

/* Memory Pool types used */
#define BT_POOL_8             0
#define BT_POOL_16            1
#define BT_POOL_32            2
#define BT_POOL_128           3
#define BT_POOL_260           4

#define BT_POOL_MAX           5     /* Max number of pools used */

/* Define sizes of the different memory blocks.
 * The actual number of each type of memory is defined in heap_bt.c
 */
#define BT_MEM_SIZE_8         8     /* Memory pool of size 8 bytes */
#define BT_MEM_SIZE_16        16    /* Memory pool of size 16 bytes */
#define BT_MEM_SIZE_32        32    /* Memory pool of size 32 bytes */
#define BT_MEM_SIZE_128       128   /* Memory pool of size 128 bytes */
#define BT_MEM_SIZE_260       260   /* Memory pool of size 256 bytes */

#endif /* _H_BT_BUFFER_ */
