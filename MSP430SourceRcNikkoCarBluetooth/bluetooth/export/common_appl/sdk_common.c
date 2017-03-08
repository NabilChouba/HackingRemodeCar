
/**
 * Copyright (C) 2009. MindTree Ltd. All rights reserved.
 *  @file     sdk_common.c
 *  @brief    This file contains common platform specific function definitions
 */

/* Header File Inclusion */

#include "sdk_pl.h"
#include "BT_task.h"
#include "appl_sdk.h"
#include "l2cap.h"
#include "vendor_specific_init.h"
#include "bt_sdk_error.h"

/* Extern variables */

#ifdef __IAR_SYSTEMS_ICC__      /* Toolchain Specific Code */
extern void __program_start();
#elif defined (__TI_COMPILER_VERSION__)
extern void __interrupt _c_int00();
#else
#warning "Toolchain Unknown"
_reset();
#endif /* Toolchain Specific Code */

/* Local name string used in application */
extern CHAR sdk_local_name[];
#if ((defined HCI_HAVE_INIT_COMMAND_MASK) && (defined BT_SSP))
extern UCHAR hci_event_mask[8];
#endif /* #if ((defined HCI_HAVE_INIT_COMMAND_MASK) && (defined BT_SSP)) */

/* This variable holds the value of configured BT UART baud rate */
extern UINT32 configured_bt_uart_baudrate;
extern UINT32 hci_vs_local_name_address;
extern UINT32 hci_vs_local_name_length_address;
extern UCHAR hci_local_bd_addr[BT_BD_ADDR_SIZE];
extern CHAR *hci_local_name;
extern UINT32 sdk_error_code;
extern UCHAR vs_local_name_write_state;
/* Change Local Name command parameter length */
extern UCHAR vs_change_local_name_param_len;
extern void hci_init_sequence_completed(void);

/* Variable to store hci local device name suffix length */
UCHAR hci_local_device_name_suffix_len = 0;

BT_OS_THREAD_PROPERTY bt_os_thread_property[] = {
    {(CHAR *) WRITE_TASK_NAME, WRITE_TASK_PRIORITY, WRITE_TASK_STACK_SIZE}
    ,
    {(CHAR *) READ_TASK_NAME, READ_TASK_PRIORITY, READ_TASK_STACK_SIZE}
};

CHAR *get_task_details(UCHAR index, volatile UINT16 * stack_size,
                       volatile UINT8 * priority);
/* spp connections status information */
SDK_SPP_CONNECTION_STATUS sdk_status[SPP_MAX_ENTITY];

/* Local Bluetooth Device Name */
CHAR sdk_local_name[SDK_REM_BT_DEV_NAME_MAX_LEN] = "NAME            \0";

/* Bluetooth Power On/Off Status Flag */
UCHAR sdk_bt_power = SDK_BT_OFF;

 /* BT Discovery On/Off Status */
UCHAR sdk_bt_visible = SDK_DISC_OFF;

/* User Context (HANDLE) used for SDP query to find out SPP record */
SDP_HANDLE appl_spp_sdp_handle;

UCHAR appl_spp_remote_server_ch;

UCHAR appl_spp_local_server_ch;

/* Based on the L2CAP Tx Buffer Flow this state will be set */
UCHAR appl_l2cap_tx_buf_state = L2CAP_TX_QUEUE_FLOW_ON;

UCHAR rem_dev_index = 0, rem_dev_num = 0;

UCHAR sys_clk_frequency = SYSTEM_CLK;

/**
 *  \fn     get_task_details
 *  \brief  This function is used to get the task details.
 *  \param  index             index of the bt_os_thread_property structure
 *  \param  task_stack_size   Stack Size of the task
 *  \param  task_priority     Priority of the task
 *  \return task_name         Task name
 */

CHAR *get_task_details(UCHAR index, volatile UINT16 * stack_size,
                       volatile UINT8 * priority)
{
    *stack_size = bt_os_thread_property[index].task_stack_size;
    *priority = bt_os_thread_property[index].priority;

    return bt_os_thread_property[index].name;
}

void common_init_bsp(void)
{
    /* disable Watch dog timer */
    WDTCTL = WDTPW + WDTHOLD;

    /* do the port initializations */
    halBoardInit();

    /* Initialization routine for XT1 */
    halBoardStartXT1();

    /* Set the inital operating frequency */
    halBoardSetSystemClock(SYSTEM_CLK);

    /* BT controller realted port configuration */
    sdk_bt_rf_port_config();

    /* Updating the UART baudrate value according to config file */
    configured_bt_uart_baudrate = BT_UART_CONFIG_BAUDRATE;

    /* Initialize the Debugging Module */
    SDK_DEBUG_MODULE_INIT();
}

void sdk_set_frequency(UCHAR freq)
{
    sys_clk_frequency = freq;
    halBoardSetSystemClock(sys_clk_frequency);
}

/**
 * \fn      sdk_bt_rf_port_config
 * \brief   This function is used to configure pins connecting MSP430 and the
 *          BT controller
 * \param   void
 * \return  void
 */
void sdk_bt_rf_port_config(void)
{
#ifndef EZ430_PLATFORM
    /* RF1 and RF2 Jumper pins configured for PAN1315 */
    P1DIR &= ~(BIT2 + BIT5 + BIT6);
    P8DIR &= ~(BIT1);
#endif /* EZ430_PLATFORM */

    /* set ACLK as clock for BT controller */
    BT_RF_CLK_PORT_SEL |= BT_RF_CLK_PORT_PIN;

    /* set BT_RESET as nreset for BT controller */
    BT_RF_NSHUTDOWN_PORT_DIR |= BT_RF_NSHUTDOWN_PORT_PIN;
}

/**
 * \fn      sdk_uart_error_handler
 * \brief   Function to indicate uart error
 * \param   void
 * \return  void
 **/
void sdk_uart_error_handler(void)
{
#ifdef __IAR_SYSTEMS_ICC__      /* Toolchain Specific Code */
    FP fp = (FP) __program_start;
#elif defined (__TI_COMPILER_VERSION__)
    FP fp = (FP) _c_int00;
#else
#warning "Toolchain Unknown"
    FP fp = (FP) _reset();
#endif /* Toolchain Specific Code */

    fp();
}

/**
 * \fn      appl_get_free_status_instance
 * \brief   Function to get free instance in sdk_status_array
 * \param   id [OUT] parameter containing index of unused connection instance
 * \return  API_RESULT      API_success/API_failure
 */
API_RESULT appl_get_free_status_instance(UCHAR * id)
{
    UCHAR index;

    for (index = 0; index < SPP_MAX_ENTITY; index++) {
        if (SDK_IS_DISCONNECTED(index)) {
            *id = index;
            return API_SUCCESS;
        }
    }
    sdk_display("No more free SPP Connection instances\n");
    return API_FAILURE;
}


/**
 * \fn      appl_get_spp_connection_progress
 * \brief   Function to get status of SPP connection
 * \param   id [OUT] parameter containing index of SPP connection
 * \return  API_RESULT      API_success/API_failure
 */
API_RESULT appl_get_spp_connection_progress(UCHAR * id)
{
    UCHAR index;

    for (index = 0; index < SPP_MAX_ENTITY; index++) {
        if (SDK_IS_SPP_CONNECTION(index)) {
            *id = index;
            return API_SUCCESS;
        }
    }
    return API_FAILURE;
}

/**
 *  \fn     sdk_handle_init_sequence:
 *  \brief  This function handles the application section of the Bluetooth
 *          initialization sequence of the the Host Controller in the
 *          NON BLOCKING mode of the stack.Any incorrect modification to this
 *          function can prevent the proper functioning of the bluetooth module.
 *
 *  \param  opcode [IN] The opcode for which the event is received from the Host
 *                     Controller.
 *  \param  status [IN] The status received for that opcode event
 *  \return None
 */
void sdk_handle_init_sequence(UINT16 opcode, UCHAR status)
{
    API_RESULT retval;
    UINT32 name_length;
    UINT32 iac[] = { SDK_INQUIRY_SCAN_LAP };

    if (0x00 != status) {
        if (HC_COMMAND_DISALLOWED == status) {
            sdk_error_code = SDK_COMMAND_DISALLOWED;
            sdk_error_handler();
        } else {
            sdk_error_code = SDK_WRONG_STATUS_IN_INIT_SEQ;
            sdk_error_handler();
        }
    } else {
        switch (opcode) {
            /* The first event to be handled in this function, should be the
             * last event generated by the init script download, else the init
             * sequence will be stalled. */
        case SDK_BT_RF_SET_SLEEP_MODE:
            /* Configure the maximum output power */
            sdk_set_output_power(SDK_MAX_OUTPUT_POWER_LEVEL,
                                 SDK_MAX_LE_ANT_OUTPUT_POWER_LEVEL);
            break;

        case SDK_BT_RF_ENABLE_CALIBRATION: /* Power calibration Completed */
#if ((defined HCI_HAVE_INIT_COMMAND_MASK) && (defined BT_SSP))
            /* Setting Event Mask for Bluetooth v2.1+EDR */
            BT_hci_set_event_mask(hci_event_mask);
#else
            /* Retrieving Local BD_ADDR */
            BT_hci_read_bd_addr();
#endif /* #if ((defined HCI_HAVE_INIT_COMMAND_MASK) && (defined BT_SSP)) */
            break;

#ifdef BT_SSP
        case HCI_SET_EVENT_MASK_OPCODE:
            /* If SSP is not masked, enable SSP */
            if (API_SUCCESS !=
                BT_hci_check_init_command_mask(HCI_INIT_COMMAND_MASK_SSP)) {
                /* Enabling Secure Simple Pairing */
                BT_hci_write_simple_pairing_mode(0x01);
            } else {            /* if SSP is masked, send the next command */

                /* Retrieving Local BD_ADDR */
                BT_hci_read_bd_addr();
            }
            break;

        case HCI_WRITE_SIMPLE_PAIRING_MODE_OPCODE:
            /* Retrieving Local BD_ADDR */
            BT_hci_read_bd_addr();

            break;
#endif /* BT_SSP */

        case HCI_READ_BD_ADDR_OPCODE:
            /* Retrieving Buffer Size Information */
            BT_hci_read_buffer_size();

            break;

        case HCI_READ_BUFFER_SIZE_OPCODE:
            if (hci_local_name != NULL) {
                /* Get the given device name length */
                name_length = (UINT16) BT_str_len(hci_local_name);
                sdk_set_config_local_name_suffix(&name_length);
#ifdef HCI_VS_LOCAL_NAME_SHORT_COMMAND
                /* Copy the local name string used in application */
                BT_str_copy(sdk_local_name, &hci_local_name[5]);
#else
                BT_str_copy(sdk_local_name, hci_local_name);
#endif

                BT_hci_change_local_name((UCHAR *) hci_local_name, name_length);
#ifndef HCI_VS_LOCAL_NAME_SHORT_COMMAND
                /* Free the local name variable */
                BT_free_mem(hci_local_name);
                hci_local_name = NULL;
#endif /* HCI_VS_LOCAL_NAME_SHORT_COMMAND */

            } else {
                /* Name is NULL, skip setting local name */
                /* Setting Maximum Page Timeout Value */
                BT_hci_write_page_timeout(SDK_MAX_PAGE_TIMEOUT_VALUE);
            }

            break;

#ifdef HCI_VS_LOCAL_NAME_SHORT_COMMAND
        case SDK_VS_WRITE_MEMORY_BLOCK:
            if (0x01 == vs_local_name_write_state) {
                /* Local name length written, now write local name to the
                 * memory */
                retval =
                    BT_hci_vendor_specific_command(SDK_VS_WRITE_MEMORY_BLOCK,
                                                   (UCHAR *) (hci_local_name),
                                                   vs_change_local_name_param_len);
                if (API_SUCCESS == retval) {
                    /* Local name write issued */
                    vs_local_name_write_state = 0x02;
                    /* Free the change local name parameter */
                    BT_free_mem(hci_local_name);
                    hci_local_name = NULL;
                    return;
                }
            }
            if (0x02 == vs_local_name_write_state) {
                /* Both local name length and local name written to memory */
                /* Reset the variable */
                vs_local_name_write_state = 0x00;
                BT_hci_write_page_timeout(SDK_MAX_PAGE_TIMEOUT_VALUE);
            }
            break;
#endif /* HCI_VS_LOCAL_NAME_SHORT_COMMAND */

        case HCI_CHANGE_LOCAL_NAME_OPCODE:
            /* Setting Maximum Page Timeout Value */
            BT_hci_write_page_timeout(SDK_MAX_PAGE_TIMEOUT_VALUE);

            break;

        case HCI_WRITE_PAGE_TIMEOUT_OPCODE:
#ifndef HCI_HOST_CONTROLLER_FLOW_ON
            /* Set the default link policy */
            BT_hci_write_default_link_policy_settings
                (SDK_CONFIG_LINK_POLICY_SETTINGS);
            break;
#else /* HCI_HOST_CONTROLLER_FLOW_ON */
            /* Setting Host Controller to Host Flow Control */
            BT_hci_set_host_controller_to_host_flow_control(0x01);

            break;

        case HCI_SET_HOST_CONTROLLER_TO_HOST_FLOW_CONTROL_OPCODE:
            /* Sending Host Buffer Size to Host Controller */
            BT_hci_host_buffer_size();

            break;

        case HCI_HOST_BUFFER_SIZE_OPCODE:
            /* Set the default link policy */
            BT_hci_write_default_link_policy_settings
                (SDK_CONFIG_LINK_POLICY_SETTINGS);
            break;
#endif /* HCI_HOST_CONTROLLER_FLOW_ON */
        case HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_OPCODE:
            /* Write Inquiry Scan mode (Interlaced scanning) */
            BT_hci_write_inquiry_scan_type(0x01);
            break;

        case HCI_WRITE_INQUIRY_SCAN_TYPE_OPCODE:
            /* Change write Page Scan mode to Interlaced scanning */
            BT_hci_write_page_scan_type(0x01);
            break;

        case HCI_WRITE_PAGE_SCAN_TYPE_OPCODE:
            BT_hci_write_current_iac_lap((sizeof(iac) / sizeof(UINT32)), iac);
            /* HCI Host Controller Initialization Complete */
            break;

        case HCI_WRITE_CURRENT_IAC_LAP_OPCODE:
            /* Set Class of Device */
            BT_hci_write_class_of_device(SDK_CONFIG_COD);
            break;

        case HCI_WRITE_CLASS_OF_DEVICE_OPCODE:
            /* Inform the application about bluetooth on */
            hci_init_sequence_completed();
            break;

        default:
            /* Wrong event during init sequence */
            sdk_error_code = SDK_UNEXPECTED_EVENT_IN_INIT_SEQ;
            sdk_error_handler();
            break;
        }
    }
}

/**
 * \fn      appl_get_status_instance_acl
 * \brief   Function to get spp connection instance based on acl connection
 *          handle.
 * \param   id [OUT] paramter containing instance id corresponding to the handle
 * \param   acl_connection_handle Connection handle for ACL connection
 * \return  API_RESULT      API_success/API_failure
 */
API_RESULT appl_get_status_instance_acl(UCHAR * id,
                                        UINT16 acl_connection_handle)
{
    UCHAR index;

    for (index = 0; index < SPP_MAX_ENTITY; index++) {
        if ((acl_connection_handle == sdk_status[index].acl_connection_handle)
            && (!SDK_IS_DISCONNECTED(index))) {
            *id = index;
            return API_SUCCESS;
        }
    }
    sdk_display
        ("No Such SPP Connection instance available for this acl connection handle %04X\n",
         acl_connection_handle);
    return API_FAILURE;
}

/**
 * \fn      appl_get_status_instance_spp
 * \brief   Function to get spp connection instance based on spp connection
 *          handle
 * \param   id [OUT] paramter containing instance id corresponding to the handle
 * \param   acl_connection_handle Connection handle for SPP connection
 * \return  API_RESULT   API_success/API_failure
 */
API_RESULT appl_get_status_instance_spp(UCHAR * id,
                                        UINT16 spp_connection_handle)
{
    UCHAR index;

    for (index = 0; index < SPP_MAX_ENTITY; index++) {
        if ((spp_connection_handle == sdk_status[index].spp_connection_handle)
            && (!SDK_IS_DISCONNECTED(index))) {
            *id = index;
            return API_SUCCESS;
        }
    }
    sdk_display
        ("No Such SPP Connection instance available for this SPP handle %04X\n",
         spp_connection_handle);
    return API_FAILURE;
}

/**
 * \fn      appl_get_status_instance_bd_addr
 * \brief   Function to get spp connection instance based on peer bd address
 * \param   id[OUT] paramter containing instance id corresponding to the address
 * \param   rem_bd_addr BT address of device to be found
 * \return  API_RESULT      API_success/API_failure
 */
API_RESULT appl_get_status_instance_bd_addr(UCHAR * id, UCHAR * rem_bd_addr)
{
    UCHAR index;

    for (index = 0; index < SPP_MAX_ENTITY; index++) {
        if (!memcmp(sdk_status[index].peer_bd_addr, rem_bd_addr, 6)
            && (!SDK_IS_DISCONNECTED(index))) {
            *id = index;
            return API_SUCCESS;
        }
    }
    if (SPP_MAX_ENTITY == index) {
        sdk_display("No Such SPP Connection instance available\n");
    }
    return API_FAILURE;
}


/**
 * \fn      sdk_shutdown_BL6450
 * \brief   Function to shutdown BL6450
 * \param   void
 * \return  void
 */
void sdk_shutdown_BL6450(void)
{
    /* Shutdown bluetooth controller */
    BT_RF_NSHUTDOWN_PIN_LOW();
}

/**
 * \fn      appl_bt_on_sdk_indication
 * \brief   Indicate bluetooth on is in progress
 * \param   void
 * \return  void
 **/
void appl_bt_on_sdk_indication(void)
{
    appl_bt_on_in_progress_indication();
}


/**
 * \fn      sdk_vs_get_local_name_addr
 * \brief   This function is used to read local name address location
 * \param   sdk_vs_local_name_addr - Buffer in which local name address location
 *                                   is populated
 * \return  void
 */
void sdk_vs_get_local_name_addr(UINT32 * sdk_vs_local_name_addr)
{
    if (NULL == sdk_vs_local_name_addr) {
        return;
    }

  /**
   * Populate the local name address with the defined paramater
   * "HCI_VS_LOCAL_NAME_ADDRESS"
   */
    (*sdk_vs_local_name_addr) = hci_vs_local_name_address;
}


/**
 * \fn      sdk_vs_get_local_name_len_addr
 * \brief   This function is used to read local name length address location
 * \param   sdk_vs_local_name_addr - Buffer in which local name length address
 *                                   location is populated
 * \return  void
 */
void sdk_vs_get_local_name_len_addr(UINT32 * sdk_vs_local_name_len_addr)
{
    if (NULL == sdk_vs_local_name_len_addr) {
        return;
    }

  /**
   * Populate the local name address with the defined paramater
   * "HCI_VS_LOCAL_NAME_ADDRESS"
   */
    (*sdk_vs_local_name_len_addr) = hci_vs_local_name_length_address;
}




/**
 * \fn      sdk_simple_pairing_debug_mode_handler
 * \brief   Function to enable simple pairing debug mode
 * \param   void
 * \return  void
 */
void sdk_simple_pairing_debug_mode_handler(void)
{
#ifdef BT_SSP_DEBUG
#ifdef BT_SSP_DEBUG_ENABLE
    /* 0x0 - Disable; 0x1 - Enable */
    if (SDK_IS_BT_POWERED_ON()) {
        BT_hci_write_simple_pairing_debug_mode(0x01);
    }
#endif /* BT_SSP_DEBUG_ENABLE */
#endif /* BT_SSP_DEBUG */
}

/**
 * \fn              sdk_set_config_local_name_suffix
 * \brief           Function to set the suffix to the config local name
 * \param[INOUT]    name_length_ptr Pointer to the name length field
 * \return          void
 */
void sdk_set_config_local_name_suffix(UINT32 * name_length_ptr)
{
    INT8 val;
    UCHAR local_name_len;

    *name_length_ptr = *name_length_ptr - hci_local_device_name_suffix_len;

    /**
     * hci_local_bd_addr[1] and hci_local_bd_addr[0] contains 2 bytes
     * of company assigned part of the bd addr, extract each nibble and convert
     * into ascii character
     */
    /* Extract the first nibble */
    val = (hci_local_bd_addr[1] & 0xF0) >> 4;
    if ((val >= 0) && (val <= 9)) {
        /* val is a numerical between (0 - 9), so add ASCII value of zero */
        val += 0x30;
    } else {
        /* val is a numerical between (A - F), so add ASCII value of 'A' */
        val += 0x37;
    }
    /* Add to the local name variable */
    hci_local_name[(*name_length_ptr)++] = val;


    /* Extract the second nibble */
    val = (hci_local_bd_addr[1] & 0x0F);
    if ((val >= 0) && (val <= 9)) {
        /* val is a numerical between (0 - 9), so add ASCII value of zero */
        val += 0x30;
    } else {
        /* val is a numerical between (A - F), so add ASCII value of 'A' */
        val += 0x37;
    }
    /* Add to the local name variable */
    hci_local_name[(*name_length_ptr)++] = val;

    /* Extract the Third nibble */
    val = (hci_local_bd_addr[0] & 0xF0) >> 4;
    if ((val >= 0) && (val <= 9)) {
        /* val is a numerical between (0 - 9), so add ASCII value of zero */
        val += 0x30;
    } else {
        /* val is a numerical between (A - F), so add ASCII value of 'A' */
        val += 0x37;
    }
    /* Add to the local name variable */
    hci_local_name[(*name_length_ptr)++] = val;

    /* Extract the Last nibble */
    val = (hci_local_bd_addr[0] & 0x0F);
    if ((val >= 0) && (val <= 9)) {
        /* val is a numerical between (0 - 9), so add ASCII value of zero */
        val += 0x30;
    } else {
        /* val is a numerical between (A - F), so add ASCII value of 'A' */
        val += 0x37;
    }
    /* Add to the local name variable */
    hci_local_name[(*name_length_ptr)++] = val;

    /* Complete the string */
    hci_local_name[(*name_length_ptr)] = '\0';

    local_name_len = strlen((char *)hci_local_name);

    sdk_check_max_len_of_hci_local_device_name(local_name_len);

}

/**
 * \fn              sdk_check_max_len_of_hci_local_device_name
 * \brief           Function to check the max hci length of hci local name and
 *                  if the length is greater than the BT_LOCAL_NAME_SIZE,
 *                  sdk_error_handler is invoked with appropriate sdk_error_code
 * \param[IN]       len length of the hci local device name
 * \return          void
 */
void sdk_check_max_len_of_hci_local_device_name(UCHAR len)
{
    if (len > BT_LOCAL_NAME_SIZE) {
        sdk_error_code = SDK_ERROR_IN_HCI_LOCAL_NAME_LEN;
        sdk_error_handler();
    }
}

/**
 * \fn              sdk_read_hci_local_device_name_from_config_file
 * \brief           Function to set the suffix to the config local name
 * \param[IN]       void
 * \return          local_device_name hci local device name containing the
 *                  device name read from the config file and also with the
 *                  suffix. Initially the suffix field added is dummy which
 *                  will be populated by sdk_set_config_local_name_suffix()
 */
CHAR *sdk_read_hci_local_device_name_from_config_file(void)
{
    /* Variable to store the local device name */
    static CHAR local_device_name[BT_LOCAL_NAME_SIZE] = SDK_CONFIG_DEVICE_NAME;

    /* Dummy suffix is added which will be later updated in
     * sdk_set_config_local_name_suffix () */
    CHAR *local_device_name_suffix = (CHAR *)"AAAA";

    /* Variable to store the local device name length */
    UCHAR local_device_name_len;

    /* Calculating the str length of the local device name */
    local_device_name_len = strlen((char *)local_device_name);

    sdk_check_max_len_of_hci_local_device_name(local_device_name_len);

    /* Calculating the str length of the local device name suffix */
    hci_local_device_name_suffix_len =
        strlen((char const *)local_device_name_suffix);

    strcpy((char *)&local_device_name[strlen((char *)SDK_CONFIG_DEVICE_NAME)],
           (char const *)local_device_name_suffix);

    /* Calculating the length of local device name after appending the suffix */
    local_device_name_len = strlen((char *)local_device_name);

    sdk_check_max_len_of_hci_local_device_name(local_device_name_len);

    return local_device_name;
}
