
/**
 * Copyright (c) 2009 MindTree Ltd.  All rights reserved.
 * \file    sdk_pl.c
 * \brief   This file contains platform specific function definitions
 */

/* Header File Inclusion */
#include "sdk_pl.h"
#include "BT_queue.h"
#include "task.h"
#include "appl_sdk.h"
#include "appl_bt_rf.h"
#include "msp430_uart.h"
#include "vendor_specific_init.h"
#include "hal_MSP430F5438.h"
#include "bt_sdk_error.h"

/* external Variables */
/* This variable holds the value of configured BT UART baud rate */
extern UINT32 configured_bt_uart_baudrate;
extern UCHAR sdk_bt_power;      /* Bluetooth Power On/Off status */


#ifdef __IAR_SYSTEMS_ICC__      /* Toolchain Specific Code */
extern void __program_start();
#elif defined(__TI_COMPILER_VERSION__)
extern void __interrupt _c_int00();
#else
#warning "Toolchain unknown"
_reset();
#endif /* Toolchain Specific Code */


/* Global Variables */
/* Flag to check if USB is connected */
UCHAR sdk_usb_detected = FALSE;

/* variable to store LED status before entering LPM */
volatile UCHAR LED_STATUS = 0x00;

/* Functions Declarations */
extern void init_buttons(void);

/* Functions */

/**
 *  \fn       sdk_init_bsp
 *  \brief    Initialize the BSP
 *  \param    void
 *  \return   void
 */
void sdk_init_bsp(void)
{
    /* Configuring MSP430 Timers */
    sdk_config_timer();

    /* intialize hardware units to provide required data(accelerometer and
     * thermometer(optional) */
    sensor_init();

    /* Set the flag if USB is connected */
    if (USB_PORT_IN & USB_PIN_RXD) {
        sdk_usb_detected = TRUE;
        /* Initialize the UART port which is used as USB serial port */
        halUsbInit();
    }

    /* intialize the buttons of MSP430 */
    init_buttons();

}

/**
 * \fn      sdk_spp_data_read
 * \brief   This function is used to read accelerometer data
 * \param   buffer      Data buffer
 * \param   buf_len     Length of buffer
 * \return  void
 */
void sdk_spp_data_read(UCHAR * buffer, UINT16 buf_len)
{
    INT16 accl_x, accl_y;

#ifdef EZ430_PLATFORM
    char accl_coord[3];
    /* Define threshold to distinguish between Positive and Negative
     * Acceleration */
    const char ACCL_POS_NEG_TH = 128;
    /* Define offset for accelerometer value for scaling purposes */
    const int ACCL_SCALE_OFFSET = 2048;

    volatile int raw_accl_x, raw_accl_y;

    halAccRead(accl_coord);
    raw_accl_x = *(accl_coord + 0);
    raw_accl_y = *(accl_coord + 1);

    /**
     * Adjust raw accelerometer values to scale expected by keyboard events
     * generator app
     * Left/Up ~= 0x3F (63d) --> 0x0A76 (2678d)
     * Center ~= 0x00 (0d) --> 0x0800 (2848d)
     * Right/Down ~= 0xC0 (-64d) --> 0x0580 (1408d)
     */
    if (raw_accl_y < ACCL_POS_NEG_TH) { /* Positive Acc */
        accl_y = raw_accl_y * 10;
    } else {                    /* Negative Acc */
        accl_y = (0xFF - raw_accl_y) * 10;
        accl_y = 0xFFFF - accl_y;
    }

    if (raw_accl_x < ACCL_POS_NEG_TH) { /* Positive Acc */
        accl_x = raw_accl_x * 10;
    } else {                    /* Negative Acc */
        accl_x = (0xFF - raw_accl_x) * 10;
        accl_x = 0xFFFF - accl_x;
    }

    accl_x += ACCL_SCALE_OFFSET;
    accl_y += ACCL_SCALE_OFFSET;



#else /* MSP-EXP430F5438 Platform */
    /* start the ADC to read the accelerometer output */
    halAdcStartRead();
    /* read the digital accelerometer x- direction and y - direction output */
    halAccelerometerRead((int *)&accl_x, (int *)&accl_y);
#endif

    /* Pack the x and y co-ordinate values in to buffer */
    buffer[0] = accl_x;
    buffer[1] = (accl_x >> 8);
    buffer[2] = accl_y;
    buffer[3] = (accl_y >> 8);
}

/**
 * \fn      sdk_update_reconnect_error_status
 * \brief   This function is used to clear the error message
 * \param   void
 * \return  void
 */
void sdk_update_reconnect_error_status(void)
{
}

/**
 * \fn      sensor_init
 * \brief   initializes the accelerometer sensor
 * \param   void
 * \return  void
 */
void sensor_init(void)
{
#ifdef EZ430_PLATFORM
    halI2CInit(SYSTEM_CLK);
    halAccStart();
#else /* MSP-EXP430F5438 Platform */
    halAccelerometerInit();
#endif /* EZ430_PLATFORM */
}


/**
 * \fn      restore_peripheral_status
 * \brief   Restore peripheral status on exiting LPM
 * \param   void
 * \return  void
 **/
void restore_peripheral_status(void)
{
    UCHAR temp = 0x00;
    temp = LED_STATUS;
    /* Restore the LED status */
    LED_PORT_OUT |= temp;
    /* Initilize the Sensors */
    sensor_init();
    /* ResumeTimer1_A3 */
    TA1CTL |= MC_1;


    if (TRUE == sdk_usb_detected) {
        /* Initialize the UART port which is used as USB serial port */
        halUsbInit();
    }
}


/**
 * \fn      sdk_error_handler
 * \brief   Function to indicate error condition
 * \param   void
 * \return  none
 **/
void sdk_error_handler()
{
    __disable_interrupt();
    BT_RF_NSHUTDOWN_PIN_LOW();
    LED_OFF();
    while (1) {
        TOGGLE_LED();
        __delay_cycles(6250000);
    }
}

/**
 * \fn      sdk_bluetooth_on
 * \brief   Function to is used to turn ON bluetooth
 * \param   void
 * \return  API_SUCCESS/API_FAILURE
 */
API_RESULT sdk_bluetooth_on(void)
{
    API_RESULT retval = API_FAILURE;

    CHAR *local_device_name;

    local_device_name = sdk_read_hci_local_device_name_from_config_file();


    /* Turn ON the bluetooth controller */
    BT_RF_NSHUTDOWN_PIN_HIGH();
    configured_bt_uart_baudrate = BT_UART_CONFIG_BAUDRATE;
    retval =
        BT_bluetooth_on(sdk_hci_event_indication_callback,
                        sdk_bluetooth_on_complete, (CHAR *) local_device_name);
    return retval;
}


/**
 * \fn      sdk_bluetooth_off
 * \brief   Function to is used to turn OFF bluetooth
 * \param   void
 * \return  API_SUCCESS/API_FAILURE
 */
API_RESULT sdk_bluetooth_off(void)
{
    API_RESULT retval = API_FAILURE;
    sdk_bt_power = SDK_BT_OFF;
    retval = BT_bluetooth_off();
    if (API_SUCCESS == retval) {
        /* Shutdown bluetooth controller */
        BT_RF_NSHUTDOWN_PIN_LOW();
        configured_bt_uart_baudrate = BT_UART_CONFIG_BAUDRATE;
    }
    return retval;
}

/**
 * \fn      sdk_update_baudrate_handler
 * \brief   This function acts as a call back function for
 *          sdk_set_controller_uart_baudrate function. User can add the post
 *          baud rate change processing here.
 * \param   void
 * \return  void
 */
void sdk_update_baudrate_handler(void)
{

}
