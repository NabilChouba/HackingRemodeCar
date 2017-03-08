
/**
 *  @file measurement.h
 *
 *  This file contains macros/datatype specific to CPU utilization calculation
 */

/* 
 *  Copyright (C) 2009-2010. MindTree Ltd.
 *  All rights reserved.
 */

#ifndef _H_MEASUREMENT_
#define _H_MEASUREMENT_

typedef struct cpuUtilMeasurement {
    volatile UINT32 taskStart;
    volatile UINT32 taskEnd;
    volatile UINT32 taskDuration;
    UINT8 taskStartFlag;
} cpuUtilMeasurement_t;

/* max Timer B value is set to the max it can count */
#define MAX_TIMERB_VALUE 32768

/* macro to set spp data transfer status */
#define SDK_IS_SENDING_DATA (1 == send_spp_data)

/* macro to change spp data transfer status to send */
#define SDK_STARTED_SENDING (send_spp_data = 1)

#define START_TASK_CALCULATION()\
        portENTER_CRITICAL();                                   \
        if (0 == dataStats.taskStartFlag)                       \
        {                                                       \
            dataStats.taskStart = resetTimerBCounter();         \
            dataStats.taskEnd = 0;                              \
            dataStats.taskStartFlag = 1;                        \
            P8OUT |= BIT5;                                      \
        }                                                       \
        portEXIT_CRITICAL();                                    \


#define END_CRITICAL_TASK_CALCULATION()\
        portENTER_CRITICAL();                                      \
          END_TASK_CALCULATION();                                  \
        portEXIT_CRITICAL();

#define END_TASK_CALCULATION()                                     \
        if (dataStats.taskStartFlag)                               \
        {                                                          \
           UINT32 temp_duration, temp_taskEnd, temp_taskStart;     \
           dataStats.taskEnd = readTimerBCounter();                \
           temp_taskEnd = dataStats.taskEnd;                       \
           temp_taskStart = dataStats.taskStart;                   \
           temp_duration = temp_taskEnd - temp_taskStart;          \
           dataStats.taskDuration = temp_duration;                 \
           totalTimerCount_g += dataStats.taskDuration;            \
           dataStats.taskStartFlag = 0;                            \
        }

void configTimerB(void);
void configRTC(void);
UINT16 resetTimerBCounter(void);
UINT32 readTimerBCounter(void);
void resetMeasurementParams(void);
void configMSP430Timers(void);
void LcdDisplay(void);
void LCD_disp_utilz(UINT32 util_count);

#endif /* _H_MEASUREMENT */
