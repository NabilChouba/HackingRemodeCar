
/**
 * Copyright (C) 2009  MindTree Ltd.  All rights reserved.
 *
 * \file    user_task.c
 * \brief   This File contains the functions related to user task handling.
 */

/* Header File Inclusion */
#include "sdk_pl.h"
#include "appl_sdk.h"
#include "task.h"
#include "BT_task.h"

/* Extern Fucntion Declaration */
extern void configTimer1_A3(void);

/* Extern variables */
extern volatile UINT32 inactivity_counter;
extern UCHAR sdk_bt_visible;
extern volatile UCHAR lpm_mode;
/* Flag to check if USB is connected */
extern UCHAR sdk_usb_detected;

/* Global variables */
/* Flag to indicate that SPP connection is in progress */
UCHAR sdk_connect_in_progress = FALSE;
UCHAR circular_user_buffer[MAX_USER_BUF];
INT16 circular_user_buf_wt = 0;
UCHAR inactivity_timeout = INACTIVITY_TIMEOUT;

static INT16 circular_user_buf_rd = 0;
static UCHAR debounce = 0;
static volatile UCHAR bytes_to_be_processed_in_user_buf = 0;

/* User Semaphore */
static xSemaphoreHandle xUserSemaphore;

/**
 * \fn      init_user_task
 * \brief   Create the user task
 * \param   void
 * \return  void
 */
void init_user_task(void)
{
    UCHAR ret_val;

    vSemaphoreCreateBinary(xUserSemaphore);

    ret_val =
        xTaskCreate((pdTASK_CODE) user_task_routine,
                    (const signed portCHAR *)USER_TASK_NAME,
                    USER_TASK_STACK_SIZE, (unsigned portLONG *)NULL,
                    (unsigned portBASE_TYPE)USER_TASK_PRIORITY,
                    (xTaskHandle *) NULL);
    if (ret_val != 0) {
        sdk_display((const UCHAR *)"Could not create user Task\n");
    }
}

/**
 *  \fn         user_task_routine
 *  \brief      Task to handle timer and menu related functionality
 *  \param      void
 *  \return     void
 */
void *user_task_routine(void)
{
    API_RESULT retval;

    UPDATE_USER_BUFFER(POWER_ON_RESET);
    bytes_to_be_processed_in_user_buf = 1;
    xSemaphoreGive(xUserSemaphore);

    while (1) {
        if (pdPASS == xSemaphoreTake(xUserSemaphore, 0xFFFF)) {
            while (bytes_to_be_processed_in_user_buf > 0) {
                bytes_to_be_processed_in_user_buf--;
                switch (circular_user_buffer[circular_user_buf_rd]) {
                case BT_DISCOVERABLE_STATE:
                    TOGGLE_LED2();
                    break;
                case SWITCH_S1:
                    /*
                     * Switch SW1 is mapped to SPP connection/disconnection
                     * request. SW1 is a toggle switch.
                     */
                    sdk_bluetooth_menu_handler(OP_PEER_CONNECT);
                    break;
                case SWITCH_S2:
                    /* Switch SW2 is mapped to contol datasend stop and pause
                     * functionality */
                    sdk_bluetooth_menu_handler(OP_PEER_DATASEND);
                    break;
                case POWER_ON_RESET:
                    /* Turn ON the bluetooth */
                    retval = sdk_bluetooth_on();
                    if (retval != API_SUCCESS) {
                        sdk_display((const UCHAR *)
                                    "Failed to turn on Bluetooth\n");
                    }
                    break;
                default:
                    sdk_error_handler();
                    break;
                }
                circular_user_buf_rd++;
                if (MAX_USER_BUF == circular_user_buf_rd) {
                    circular_user_buf_rd = 0;
                }
            }

        }
    }
}


/**
 * \fn      sdk_config_timer
 * \brief   Configure the timers used by the application
 * \param   void
 * \return  void
 */
void sdk_config_timer(void)
{
    /* Configure Timer1_A3 which is used for menu and Led indication related
     * functionalities */
    configTimer1_A3();
}

/**
 * \fn      TIMER1_A0_ISR
 * \brief   Interrupt routine for Timer1_A3.
 *          This handles the menu and LED indications related functionality
 * \param   void
 * \return  void
 */
#ifndef __IAR_SYSTEMS_ICC__
#pragma CODE_SECTION(TIMER1_A0_ISR, ".text:_isr");
#endif /* __IAR_SYSTEMS_ICC__ */
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    signed portBASE_TYPE xHigherPriorityTaskWoken;

    UART_DISABLE_BT_UART_RTS();
    xHigherPriorityTaskWoken = pdFALSE;

    TA1CCTL0 &= ~CCIFG;
    __bis_SR_register(GIE);

    /* If the debounce flag is set, clear the flag to allow processing of next
     * button interrupts */
    if (1 == debounce) {
        debounce = 0;
    }

    /* Halting the Timer1_A3 */
    TA1CTL &= ~MC_3;

    if (TRUE == sdk_connect_in_progress) {
        /* Setting the inactivity timeout accordingly */
        inactivity_timeout = (INACTIVITY_TIMEOUT << 2);
        /* Reinitializing theTimer register value to generate interuupt every
         * 0.25 sec. This is used to indicate the user that the device is
         * trying to inquire and connect to BlueMSP demo device */
        TA1CCR0 = 0x2000;
    } else {
        /* Setting the inactivity timeout accordingly */
        inactivity_timeout = INACTIVITY_TIMEOUT;
        /* Reinitializing the Timer register value to generate interrupt every
         * 1 sec */
        TA1CCR0 = 0x8000;
    }

    /* Resume Timer1_A3 */
    TA1CTL |= MC_1;

    if (inactivity_counter < (inactivity_timeout + 1)) {
        inactivity_counter++;
    }

    if (SDK_IS_BT_DISCOVERABLE()) {
        UPDATE_USER_BUFFER(BT_DISCOVERABLE_STATE);
    }
    /* If usb connection was not detected,during board initialisation, check
     * now */
    if (FALSE == sdk_usb_detected) {
        /* if the usb device is now connected, initialise the port */
        if (USB_PORT_IN & USB_PIN_RXD) {
            sdk_usb_detected = TRUE;
            halUsbInit();
        }
    }

    /* Unlocking User Task */
    if (bytes_to_be_processed_in_user_buf > 0) {
        /* Unblock the task by releasing the semaphore */
        xSemaphoreGiveFromISR(xUserSemaphore, &xHigherPriorityTaskWoken);
    }

    /* Force a context switch if xHigherPriorityTaskWoken was set to true */
    if (xHigherPriorityTaskWoken) {
        portYIELD();
    }
}


/**
 * \fn      PORT2_VECTOR_ISR
 * \brief   ISR routine for handling PORT2 interrupts. It handles switch
 *          debounce and switching modes for LPM
 * \return	void
 **/
#ifdef __TI_COMPILER_VERSION__
#pragma CODE_SECTION(PORT2_VECTOR_ISR, ".text:_isr");
#endif /* __TI_COMPILER_VERSION__ */
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_VECTOR_ISR(void)
{
    signed portBASE_TYPE xHigherPriorityTaskWoken;
    UINT8 int_vect;

    UART_DISABLE_BT_UART_RTS();

    xHigherPriorityTaskWoken = pdFALSE;

    int_vect = P2IV;
    __bis_SR_register(GIE);
    if (SWITCH_INTERRUPT & int_vect) {
        inactivity_counter = 0;

        if (0 == debounce) {
            /* to take care of switch debounce */
            debounce = 1;

            if (FALSE == lpm_mode) {
                UPDATE_USER_BUFFER(int_vect);
                /* Unlocking User Task */
                if (bytes_to_be_processed_in_user_buf > 0) {
                    /* Unblock the user task by releasing the semaphore */
                    xSemaphoreGiveFromISR(xUserSemaphore,
                                          &xHigherPriorityTaskWoken);
                }
            }
        }
    }


    /* If the processor is in LPM, move to active mode */
#ifdef MSP430_LPM_ENABLE
    if (TRUE == lpm_mode) {
        lpm_mode = FALSE;
        restore_peripheral_status();
        EXIT_MSP430_LPM();
    }
#endif /* MSP430_LPM_ENABLE */

    /* Force a context switch if xHigherPriorityTaskWoken was set to true */
    if (xHigherPriorityTaskWoken) {
        portYIELD();
    }
}

/**
 * \fn      init_buttons
 * \brief 	initializes the MSP430 board (S1 and S2) button related registers
 * \param   void
 * \return	void	
 **/
void init_buttons(void)
{
    debounce = 1;

    BUTTON_PORT_DIR &= ~(BUTTON_S1 + BUTTON_S2);
    BUTTON_PORT_REN |= (BUTTON_S1 + BUTTON_S2);
    BUTTON_PORT_OUT |= BUTTON_S1 + BUTTON_S2;
    BUTTON_PORT_SEL &= ~(BUTTON_S1 + BUTTON_S2);
    BUTTON_PORT_IES &= ~(BUTTON_S1 + BUTTON_S2);
    BUTTON_PORT_IFG &= ~(BUTTON_S1 + BUTTON_S2);
    BUTTON_PORT_IE |= (BUTTON_S1 + BUTTON_S2);
}
