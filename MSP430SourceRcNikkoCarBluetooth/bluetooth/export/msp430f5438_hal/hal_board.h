
/**
 * Copyright 2008 Texas Instruments, Inc.
 * \file    hal_board.h
 * \brief   Contains header definitions for the HAL functions
 */

#ifndef HAL_BOARD_H
#define HAL_BOARD_H

#define LED_PORT_DIR            P1DIR
#define LED_PORT_OUT            P1OUT
#define LED_1                   BIT0
#define LED_2                   BIT1

#define ACLK_PIN                BT_RF_CLK_PORT_PIN
#define CLK_PORT_DIR            BT_RF_CLK_PORT_DIR
#define CLK_PORT_OUT            BT_RF_CLK_PORT_OUT
#define CLK_PORT_SEL            BT_RF_CLK_PORT_SEL

#ifdef EZ430_PLATFORM

#define ALL_CLK_PINS            ACLK_PIN
#else /* MSP-EXPF5438 Platform */

#define MCLK_PIN                BIT1
#define SMCLK_PIN               BIT2

#define ALL_CLK_PINS            (ACLK_PIN | MCLK_PIN | SMCLK_PIN)
#endif /* EZ430_PLATFORM */

#define XT1_XTAL_DIR            P7DIR
#define XT1_XTAL_SEL            P7SEL
#define XT1_XTAL_OUT            P7OUT

#define SYSCLK_1MHZ             0
#define SYSCLK_4MHZ             1
#define SYSCLK_8MHZ             2
#define SYSCLK_12MHZ            3
#define SYSCLK_16MHZ            4
#define SYSCLK_18MHZ            5
#define SYSCLK_20MHZ            6
#define SYSCLK_25MHZ            7

#define DCO_MULT_1MHZ           30
#define DCO_MULT_4MHZ           122
#define DCO_MULT_8MHZ           244
#define DCO_MULT_12MHZ          366
#define DCO_MULT_16MHZ          488
#define DCO_MULT_18MHZ          549
#define DCO_MULT_20MHZ          610
#define DCO_MULT_25MHZ          763

#define DCORSEL_1MHZ            DCORSEL_2
#define DCORSEL_4MHZ            DCORSEL_4
#define DCORSEL_8MHZ            DCORSEL_4
#define DCORSEL_12MHZ           DCORSEL_5
#define DCORSEL_16MHZ           DCORSEL_5
#define DCORSEL_18MHZ           DCORSEL_5
#define DCORSEL_20MHZ           DCORSEL_6
#define DCORSEL_25MHZ           DCORSEL_7

#define VCORE_1MHZ              PMMCOREV_0
#define VCORE_4MHZ              PMMCOREV_0
#define VCORE_8MHZ              PMMCOREV_0
#define VCORE_12MHZ             PMMCOREV_1
#define VCORE_16MHZ             PMMCOREV_1
#define VCORE_18MHZ             PMMCOREV_2
#define VCORE_20MHZ             PMMCOREV_2
#define VCORE_25MHZ             PMMCOREV_3

#define VCORE_1_35V             PMMCOREV_0
#define VCORE_1_55V             PMMCOREV_1
#define VCORE_1_75V             PMMCOREV_2
#define VCORE_1_85V             PMMCOREV_3

/*----------------------------------------------------------------
 *                  Function Prototypes
 *----------------------------------------------------------------
 */
static void halBoardSetVCoreUp(unsigned char level);
static void halBoardSetVCoreDown(unsigned char level);
static void halBoardGetSystemClockSettings(unsigned char systemClockSpeed,
                                           unsigned char *setDcoRange,
                                           unsigned char *setVCore,
                                           unsigned int *setMultiplier);

void halBoardSetVCore(unsigned char level);
void halBoardDisableSVS(void);
void halBoardEnableSVS(void);
void halBoardStartXT1(void);
void halBoardSetSystemClock(unsigned char systemClockSpeed);
void halBoardOutputSystemClock(void);
void halBoardStopOutputSystemClock(void);
void halBoardInit(void);

#endif
