
/**
 *  @file BT_queue.h
 *
 *  This file contains macros for circular queue implementation
 */

/* 
 *  Copyright (C) 2009-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_QUEUE_
#define _H_BT_QUEUE_

#include "BT_common.h"

/* To Get Free Space in Q Circular Buffer */
#define Q_BUFFER_GET_SPACE(size, rd, wr, cnt) \
        (cnt) = ((rd) - ((wr) + 1)) & (size - 1);

/* 
 *  To Get Free (Linear) Space in Q Circular Buffer
 *  (so that 'memcpy' could be called to copy incoming data)
 */
#define Q_BUFFER_GET_SPACE_LINEAR(size, rd, wr, cnt) \
        { \
            UINT16 cnt_end; \
            cnt_end = (size - 1 - (wr)); \
            (cnt) = (cnt_end + (rd)) & (size - 1); \
            if ((cnt) > cnt_end) (cnt) = cnt_end + 1; \
        }

/* To Get Amount of Data queued in Q Circular Buffer */
#define Q_BUFFER_GET_COUNT(size, rd, wr, cnt) \
        (cnt) = ((wr) - (rd)) & (size - 1);

/* 
 *  To Get Amount of Data (Linear) queued in Q Circular Buffer
 *  (so that 'memcpy' could be called to copy incoming data)
 */
#define Q_BUFFER_GET_COUNT_LINEAR(size, rd, wr, cnt) \
        { \
            UINT16 cnt_end; \
            cnt_end = (size - (rd)); \
            (cnt) = ((wr) + cnt_end) & (size - 1); \
            if ((cnt) >= cnt_end) (cnt) = cnt_end; \
        }

#define Q_BUFFER_UPDATE_RDWR_PTR(size, ptr, cnt) \
        (ptr) = ((ptr) + (cnt)) & (size - 1);

#endif /* H_BT_QUEUE_ */
