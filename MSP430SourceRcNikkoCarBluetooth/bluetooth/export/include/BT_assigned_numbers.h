
/**
 *  @file BT_assigned_numbers.h
 *
 *  This header file describes various definitions from
 *  the Bluetooth Assigned Numbers Specification.
 */
 
 /* 
  *  Copyright (C) 2009-2010. MindTree Ltd.
  *  All rights reserved.
  */
  
#ifndef _H_BT_ASSIGNED_NUMBERS_
#define _H_BT_ASSIGNED_NUMBERS_

/* --------------------------------- Header File Inclusion */

/* --------------------------------- Global Definitions */

/* ===============================================  Inquiry Access Codes */
#define BT_GIAC                      0x9E8B33
#define BT_LIAC                      0x9E8B00

/* ===============================================  Class of Device (CoD) */
/*
 *  23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 * _________________________________________________________________________
 * |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 * |__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|
 *
 * <--------------------------------><-------------><----------------><---->
 *             |                            |             |             |
 *             v                            |             |             |
 *    MAJOR SERVICE CLASSES                 v             |             v
 *                                MAJOR DEVICE CLASS      |       FORMAT TYPE
 *                                                        v
 *                                                MINOR DEVICE CLASS
 *
 *  FORMAT TYPE:-
 *
 *   01 00
 *  _______
 *  |  |  |
 *  |__|__|
 *
 *   0  0 -> format #1
 */

/*
 * MAJOR SERVICE CLASSES:-
 *
 *    23 22 21 20 19 18 17 16 15 14 13
 *   __________________________________
 *   |  |  |  |  |  |  |  |  |  |  |  |
 *   |__|__|__|__|__|__|__|__|__|__|__|
 *     |  |  |  |  |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |  |  |  |  |_ Limited Discoveral Mode
 *     |  |  |  |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |  |  |  |__ (Reserved)
 *     |  |  |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |  |  |__ (Reserved)
 *     |  |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |  |___ Positioning (Location identification)
 *     |  |  |  |  |  |  |
 *     |  |  |  |  |  |  |____ Networking (LAN, Ad hoc, ...)
 *     |  |  |  |  |  |
 *     |  |  |  |  |  |_____ Rendering (Printing, Speaker, ...)
 *     |  |  |  |  |
 *     |  |  |  |  |______ Capturing (Scanner, Microphone, ...)
 *     |  |  |  |
 *     |  |  |  |_______ Object Transfer (v-Inbox, v-Folder, ...)
 *     |  |  |
 *     |  |  |________ Audio (Speaker, Microphone, Headset service, ...)
 *     |  |
 *     |  |________ Telephony (Cordless telephony, Modem, Headset service, ...)
 *     |
 *     |_________ Information (WEB-server, WAP-server, ...)
 *
 */

#define BT_MSC_LIM_DISC_MODE         0x00002000
#define BT_MSC_POSITIONING           0x00010000
#define BT_MSC_NETWORKING            0x00020000
#define BT_MSC_RENDERING             0x00040000
#define BT_MSC_CAPTURING             0x00080000
#define BT_MSC_OBJECT_TRANSFER       0x00100000
#define BT_MSC_AUDIO                 0x00200000
#define BT_MSC_TELEPHONY             0x00400000
#define BT_MSC_INFORMATION           0x00800000

/*
 *   MAJOR DEVICE CLASSES:-
 *
 *     12 11 10 09 08
 *    ________________
 *    |  |  |  |  |  |
 *    |__|__|__|__|__|
 *
 *      0  0  0  0  0 -> Miscellaneous
 *
 *      0  0  0  0  1 -> Computer (desktop, notebook, PDA, organizers, ...)
 *
 *      0  0  0  1  0 -> Phone (cellular, cordless, payphone, modem, ...)
 *
 *      0  0  0  1  1 -> LAN / Network Access point
 *
 *      0  0  1  0  0 -> Audio/Video (headset, speaker, stereo, video display,
 *                                    vcr, ...)
 *      0  0  1  0  1 -> Peripheral (mouse, joystick, keyboards, ...)
 *
 *      0  0  1  1  0 -> Imaging (printing, scanner, camera, display, ...)
 *
 *      0  0  1  1  1 -> Wearable
 *
 *      0  1  0  0  0 -> Toy
 *
 *      0  1  0  0  1 -> Health
 *
 *      1  1  1  1  1 -> Uncategorized, specific device code not specified
 *
 *      X  X  X  X  X -> All other values reserved
 *
 */

#define BT_MDC_MISCELLANEOUS                 0x00000000

#define BT_MDC_COMPUTER                      0x00000100

#define BT_MDC_PHONE                         0x00000200

#define BT_MDC_LAN_NETWORK_ACCESS_POINT      0x00000300
#define BT_MDC_LAN_ACCESS_POINT              0x00000300
#define BT_MDC_NETWORK_ACCESS_POINT          0x00000300

#define BT_MDC_AUDIO_VIDEO                   0x00000400
#define BT_MDC_AUDIO                         0x00000400
#define BT_MDC_VIDEO                         0x00000400

#define BT_MDC_PERIPHERAL                    0x00000500

#define BT_MDC_IMAGING                       0x00000600
#define BT_MDC_WEARABLE                      0x00000700
#define BT_MDC_TOY                           0x00000800
#define BT_MDC_HEALTH                        0x00000900

#define BT_MDC_UNCLASSIFIED                  0x00001F00

/*
 *  COMPUTER MAJOR CLASS:-
 *
 *     07 06 05 04 03 02
 *    ___________________
 *    |  |  |  |  |  |  |
 *    |__|__|__|__|__|__|
 *
 *      0  0  0  0  0  0 -> Uncategorized, code for device not assigned
 *
 *      0  0  0  0  0  1 -> Desktop workstation
 *
 *      0  0  0  0  1  0 -> Server-class computer
 *
 *      0  0  0  0  1  1 -> Laptop
 *
 *      0  0  0  1  0  0 -> Handheld PC/PDA (clam shell)
 *
 *      0  0  0  1  0  1 -> Palm sized PC/PDA
 *
 *      0  0  0  1  1  0 -> Wearable computer (Watch sized)
 *
 *      X  X  X  X  X  X -> All other values reserved
 */

#define BT_CMC_UNCLASSIFIED                0x00000000
#define BT_CMC_DESKTOP_WORKSTATION         0x00000004
#define BT_CMC_SERVER_CLASS_COMPUTER       0x00000008
#define BT_CMC_LAPTOP                      0x0000000C
#define BT_CMC_HANDHELD_PC_PDA             0x00000010
#define BT_CMC_PALM_SIZED_PC_PDA           0x00000014
#define BT_CMC_WEARABLE_COMPUTER           0x00000018

/*
 *   PHONE MAJOR CLASS:-
 *
 *    07 06 05 04 03 02
 *   ___________________
 *   |  |  |  |  |  |  |
 *   |__|__|__|__|__|__|
 *
 *     0  0  0  0  0  0 -> Uncategorized, code for device not assigned
 *
 *     0  0  0  0  0  1 -> Cellular
 *
 *     0  0  0  0  1  0 -> Cordless
 *
 *     0  0  0  0  1  1 -> Smart phone
 *
 *     0  0  0  1  0  0 -> Wired modem or voice gateway
 *
 *     0  0  0  1  0  1 -> Common ISDN Access
 *
 *     0  0  0  1  1  0 -> Sim Card Reader
 *
 *     X  X  X  X  X  X -> All other values reserved
 */

#define BT_PMC_UNCLASSIFIED                           0x00000000
#define BT_PMC_CELLULAR                               0x00000004
#define BT_PMC_CORDLESS                               0x00000008
#define BT_PMC_SMART_PHONE                            0x0000000C

#define BT_PMC_WIRED_MODEM_OR_VOICE_GATEWAY           0x00000010
#define BT_PMC_WIRED_MODEM                            0x00000010
#define BT_PMC_VOICE_GATEWAY                          0x00000010

#define BT_PMC_COMMON_ISDN_ACCESS                     0x00000014
#define BT_PMC_SIM_CARD_READER                        0x00000018



/*
 *  LAN ACCESS POINT MAJOR CLASS:-
 *
 *   07 06 05
 *  __________
 *  |  |  |  |
 *  |__|__|__|
 *
 *    0  0  0 -> Fully available
 *
 *    0  0  1 -> 1 - 17% utilized
 *
 *    0  1  0 -> 17 - 33% utilized
 *
 *    0  1  1 -> 33 - 50% utilized
 *
 *    1  0  0 -> 50 - 67% utilized
 *
 *    1  0  1 -> 67 - 83% utilized
 *
 *    1  1  0 -> 83 - 99% utilized
 *
 *    1  1  1 -> No service available
 *
 *
 *   04 03 02
 *  __________
 *  |  |  |  |
 *  |__|__|__|
 *
 *    0  0  0 -> Uncategorized (use this value if no other apply)
 *
 *    X  X  X -> other values reserved
 *
 */

#define BT_LMC_FULLY_AVAILABLE                0x00000000

#define BT_LMC_01_TO_17_PERCENT_UTILIZED      0x00000020
#define BT_LMC_17_TO_33_PERCENT_UTILIZED      0x00000040
#define BT_LMC_33_TO_50_PERCENT_UTILIZED      0x00000060
#define BT_LMC_50_TO_67_PERCENT_UTILIZED      0x00000080
#define BT_LMC_67_TO_73_PERCENT_UTILIZED      0x000000A0
#define BT_LMC_83_TO_99_PERCENT_UTILIZED      0x000000D0

#define BT_LMC_NO_SERVICE_AVAILABLE           0x000000E0


/*
 *  AUDIO/VIDEO MAJOR CLASS:-
 *
 *     07 06 05 04 03 02
 *    ___________________
 *    |  |  |  |  |  |  |
 *    |__|__|__|__|__|__|
 *
 *      0  0  0  0  0  0 -> Uncategorized, code not assigned
 *
 *      0  0  0  0  0  1 -> Device conforms to the Headset profile
 *
 *      0  0  0  0  1  0 -> Hands-free
 *
 *      0  0  0  0  1  1 -> (Reserved)
 *
 *      0  0  0  1  0  0 -> Microphone
 *
 *      0  0  0  1  0  1 -> Loudspeaker
 *
 *      0  0  0  1  1  0 -> Headphones
 *
 *      0  0  0  1  1  1 -> Portable Audio
 *
 *      0  0  1  0  0  0 -> Car audio
 *
 *      0  0  1  0  0  1 -> Set-top box
 *
 *      0  0  1  0  1  0 -> HiFi Audio Device
 *
 *      0  0  1  0  1  1 -> VCR
 *
 *      0  0  1  1  0  0 -> Video Camera
 *
 *      0  0  1  1  0  1 -> Camcorder
 *
 *      0  0  1  1  1  0 -> Video Monitor
 *
 *      0  0  1  1  1  1 -> Video Display and Loudspeaker
 *
 *      0  1  0  0  0  0 -> Video Conferencing
 *
 *      0  1  0  0  0  1 -> (Reserved)
 *
 *      0  1  0  0  1  0 -> Gaming/Toy
 *
 *      X  X  X  X  X  X -> All other values reserved
 *
 */

#define BT_AMC_UNCLASSIFIED                        0x00000000
#define BT_AMC_HEADSET_PROFILE                     0x00000004
#define BT_AMC_HANDS_FREE                          0x00000008
#define BT_AMC_MICROPHONE                          0x00000010
#define BT_AMC_LOUDSPEAKER                         0x00000014
#define BT_AMC_HEADPHONE                           0x00000018
#define BT_AMC_PORTABLE_AUDIO                      0x0000001C
#define BT_AMC_CAR_AUDIO                           0x00000020
#define BT_AMC_SET_TOP_BOX                         0x00000024
#define BT_AMC_HI_FI_AUDIO_DEVICE                  0x00000028
#define BT_AMC_VCR                                 0x0000002C
#define BT_AMC_VIDEO_CAMERA                        0x00000030
#define BT_AMC_CAMCORDER                           0x00000034
#define BT_AMC_VIDEO_MONITOR                       0x00000038
#define BT_AMC_VIDEO_DISPLAY_AND_LOUDSPEAKER       0x0000003C
#define BT_AMC_VIDEO_CONFERENCING                  0x00000040
#define BT_AMC_GAMING_TOY                          0x00000048


/*
 *  PERIPHERAL MAJOR CLASS:-
 *
 *    07 06
 *   _______
 *   |  |  |
 *   |__|__|
 *
 *     0  0 -> Not Keyboard / Not Pointing Device
 *
 *     0  1 -> Keyboard
 *
 *     1  0 -> Pointing device
 *
 *     1  1 -> Combo keyboard/pointing device
 *
 *
 *
 *    05 04 03 02
 *   _____________
 *   |  |  |  |  |
 *   |__|__|__|__|
 *
 *     0  0  0  0 -> Uncategorized device
 *
 *     0  0  0  1 -> Joystick
 *
 *     0  0  1  0 -> Gamepad
 *
 *     0  0  1  1 -> Remote control
 *
 *     0  1  0  0 -> Sensing device
 *
 *     0  1  0  1 -> Digitizer tablet
 *
 *     0  1  1  0 -> Card Reader (e.g. SIM Card Reader)
 *
 *     X  X  X  X -> All other values reserved
 *
 */

#define BT_PPMC_KEYBOARD                       0x00000040
#define BT_PPMC_POINTING_DEVICE                0x00000080
#define BT_PPMC_COMBO_KEYBOARD_POINTING_DEVICE 0x000000C0

#define BT_PPMC_JOYSTICK                       0x00000004
#define BT_PPMC_GAMEPAD                        0x00000008
#define BT_PPMC_REMOTE_CONTROL                 0x0000000C
#define BT_PPMC_SENSING_DEVICE                 0x00000010
#define BT_PPMC_DIGITIZER_TABLET               0x00000014

/*
 *  IMAGING MAJOR CLASS:-
 *
 *    07 06 05 04
 *   _____________
 *   |  |  |  |  |
 *   |__|__|__|__|
 *
 *     0  0  0  1 -> Display
 *
 *     0  0  1  0 -> Camera
 *
 *     0  1  0  0 -> Scanner
 *
 *     1  0  0  0 -> Printer
 *
 *     X  X  X  X -> All other values reserved
 *
 *
 *    03 02
 *   _______
 *   |  |  |
 *   |__|__|
 *
 *     0  0 -> Uncategorized, default
 *
 *     X  X -> All other values reserved
 *
 */

#define BT_IMC_DISPLAY               0x00000010
#define BT_IMC_CAMERA                0x00000020
#define BT_IMC_SCANNER               0x00000040
#define BT_IMC_PRINTER               0x00000080

/* 
 * TOY MAJOR CLASS:-
 *
 *     07 06 05 04 03 02
 *    ___________________
 *    |  |  |  |  |  |  |
 *    |__|__|__|__|__|__|
 *
 *      0  0  0  0  0  1 -> Robot
 *
 *      0  0  0  0  1  0 -> Vehicle
 *
 *      0  0  0  0  1  1 -> Doll / Action Figure
 *
 *      0  0  0  1  0  0 -> Controller
 *
 *      0  0  0  1  0  1 -> Game
 *
 *      X  X  X  X  X  X -> All other values reserved
 *
 */
#define BT_TMC_ROBOT                  0x00000004
#define BT_TMC_VEHICLE                0x00000008
#define BT_TMC_DOLL                   0x0000000C
#define BT_TMC_CONTROLLER             0x00000010
#define BT_TMC_GAME                   0x00000014

/* 
 * HEALTH MAJOR CLASS:-
 *
 *     07 06 05 04 03 02
 *    ___________________
 *    |  |  |  |  |  |  |
 *    |__|__|__|__|__|__|
 *
 *      0  0  0  0  0  0 -> Undefined
 *
 *      0  0  0  0  0  1 -> Blood Pressure Monitor
 *
 *      0  0  0  0  1  0 -> Thermometer
 *
 *      0  0  0  0  1  1 -> Weighing Scale
 *
 *      0  0  0  1  0  0 -> Glucose Meter
 *
 *      0  0  0  1  0  1 -> Pulse Oximeter
 *
 *      0  0  0  1  1  0 -> Heart/Pulse Rate Monitor
 *
 *      0  0  0  1  1  1 -> Medical Data Display
 *
 *      X  X  X  X  X  X -> All other values reserved
 *
 */
#define BT_HMC_UNDEFINED                           0x00000000
#define BT_HMC_BLOOD_PRESSURE_MONITOR              0x00000004
#define BT_HMC_THERMOMETER                         0x00000008
#define BT_HMC_WEIGHING_SCALE                      0x0000000C /* 0C */
#define BT_HMC_GLUCOSE_METER                       0x00000010 /* 10 */
#define BT_HMC_PULSE_OXIMETER                      0x00000014 /* 14 */
#define BT_HMC_HEART_PULSE_RATE_MONITOR            0x00000018 /* 18 */
#define BT_HMC_MEDICAL_DATA_DISPLAY                0x0000001C /* 1C */


/* ==========================================  EIR Data Type Definitions */
/* Flags */
#define HCI_EIR_DATA_TYPE_FLAGS                       0x01

/* Service Class UUIDs */
#define HCI_EIR_DATA_TYPE_INCOMPLETE_16_BIT_UUIDS     0x02
#define HCI_EIR_DATA_TYPE_COMPLETE_16_BIT_UUIDS       0x03
#define HCI_EIR_DATA_TYPE_INCOMPLETE_32_BIT_UUIDS     0x04
#define HCI_EIR_DATA_TYPE_COMPLETE_32_BIT_UUIDS       0x05
#define HCI_EIR_DATA_TYPE_INCOMPLETE_128_BIT_UUIDS    0x06
#define HCI_EIR_DATA_TYPE_COMPLETE_128_BIT_UUIDS      0x07

/* Local Name */
#define HCI_EIR_DATA_TYPE_SHORTENED_LOCAL_NAME        0x08
#define HCI_EIR_DATA_TYPE_COMPLETE_LOCAL_NAME         0x09

/* TX Power Level */
#define HCI_EIR_DATA_TYPE_TX_POWER_LEVEL              0x0A

/* Secure Simple Pairing Out of Band (OOB) */
#define HCI_EIR_DATA_TYPE_CLASS_OF_DEVICE             0x0D
#define HCI_EIR_DATA_TYPE_SIMPLE_PAIRING_HASH_C       0x0E
#define HCI_EIR_DATA_TYPE_SIMPLE_PAIRING_RANDOMIZER_R 0x0F

/* Device ID */
#define HCI_EIR_DATA_TYPE_DEVICE_ID                   0x10

/* Manufacturer Specific Data */
#define HCI_EIR_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA  0xFF

#endif /* _H_BT_ASSIGNED_NUMBERS_ */


