/*
 * Copyright (c) 2016, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2018, Andreas Merkle
 * http://www.blue-andi.de
 * vscp@blue-andi.de
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */


/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
@brief  Main entry point
@file   main.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains the main entry point.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdio.h>

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"

#include "fsl_common.h"         /* Interrupt handling */
#include "fsl_debug_console.h"  /* Debug console */
#include "fsl_pit.h"            /* Periodic Interrupt Timer */

#include "system.h"             /* System specific defines, types and constants. */
#include "swTimer.h"            /* Software timer */

#include "vscp_core.h"
#include "vscp_timer.h"
#include "vscp_portable.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** 10 ms software timer id */
#define MAIN_SWTIMER_10MS_ID        0

/** 10 ms software timer period */
#define MAIN_SWTIMER_10MS_PERIOD    10

/** 250 ms software timer id */
#define MAIN_SWTIMER_250MS_ID       1

/** 250 ms software timer period */
#define MAIN_SWTIMER_250MS_PERIOD   250

/** Segment initialization button debouncing time in ms. */
#define MAIN_SEG_INIT_DEBOUNCE_TIME 100

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK()  CLOCK_GetFreq(kCLOCK_BusClk)

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the supported function return values. */
typedef enum
{
    MAIN_RET_OK = 0,    /**< Successful */
    MAIN_RET_ERROR,     /**< Failed */
    MAIN_RET_ENULL      /**< Unexpected NULL pointer */

} MAIN_RET;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static MAIN_RET main_initRunLevel1(void);
static MAIN_RET main_initRunLevel2(void);
static void main_initPIT(void);
static void main_enableInterrupts(void);
static BOOL main_getInitButtonState(void);
static void main_processStatusLamp(void);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Segment init button state */
static volatile BOOL	main_isInitButtonPressed	= FALSE;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * Main entry point.
 *
 * @return Not used
 */
int main(void)
{
    BOOL    lastSegInitButtonState  = FALSE;    /* Used for raising edge detection */
    BOOL	vscpIsActive			= FALSE;	/* VSCP is in active state or not */

    /* ********** Run level 1 - interrupts disabled ********** */

    /* Enter run level 1 */
    if (MAIN_RET_OK != main_initRunLevel1())
    {
        /* Error */
        HALT();
    }

    /* ********** Run level 2 - interrupts enabled ********** */

    /* Enable interrupts */
    main_enableInterrupts();

    /* Enter run level 2 */
    if (MAIN_RET_OK != main_initRunLevel2())
    {
        /* Error */
        HALT();
    }

    /* ********** Run level 3 - main loop starts ********** */

    /* Show VSCP protocol and framework version */
    PRINTF("VSCP framework %s\r\n", VSCP_CORE_FRAMEWORK_VERSION);
    PRINTF("VSCP %s\r\n", VSCP_CORE_VERSION_STR);
    PRINTF("\r\n");

    /* Main loop */
    for(;;)
    {
        /* Process VSCP framework */
        vscp_core_process();

        /* Initialize the VSCP segment, because user pressed the segment
         * initialization button?
         *
         * If the user keeps the button pressed and it looks like the user never
         * release it, the initialization will anyway take place only once.
         */
        if ((TRUE == main_isInitButtonPressed) &&
            (FALSE == lastSegInitButtonState))
        {
        	LOG_CONSOLE("Start node segment initialization.");

            vscp_core_startNodeSegmentInit();
        }

        lastSegInitButtonState = main_isInitButtonPressed;

        /* 250 ms period */
        if (TRUE == swTimer_getStatus(MAIN_SWTIMER_250MS_ID))
        {
            /* Process VSCP timers */
            vscp_timer_process(MAIN_SWTIMER_250MS_PERIOD);

            /* Process VSCP lamp blinking */
            main_processStatusLamp();
        }

        /* Some stuff shall only be done in case that VSCP is in ACTIVE state. */
        if (TRUE == vscp_core_isActive())
        {
        	if (FALSE == vscpIsActive)
        	{
        		LOG_CONSOLE("VSCP entered active state.");
				vscpIsActive = TRUE;
        	}

            /* Implement your code here ... */


        }
        else if (TRUE == vscpIsActive)
        {
        	LOG_CONSOLE("VSCP left active state.");
        	vscpIsActive = FALSE;
        }

    }

    return 0 ;
}

/**
 * This function is called by the periodic interrupt timer channel 0.
 */
extern void PIT0_IRQHandler(void)
{
	static volatile uint8_t	segmentInitButtonDebounceCnt	= 0;

    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);

    /* Is the segment initialization button released? */

    if (FALSE != main_getInitButtonState())
    {
        segmentInitButtonDebounceCnt = 0;
        main_isInitButtonPressed = FALSE;
    }
    /* Segment initialization button is pressed.
     * Perform debouncing!
     */
    else if (MAIN_SEG_INIT_DEBOUNCE_TIME <= segmentInitButtonDebounceCnt)
    {
        main_isInitButtonPressed = TRUE;
    }
    else
    {
        ++segmentInitButtonDebounceCnt;
    }

    /* Process all software timer */
    swTimer_process();

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes run level 1.
 * Precondition: Interrupts are disabled
 *
 * @return Status
 */
static MAIN_RET main_initRunLevel1(void)
{
    MAIN_RET    status  = MAIN_RET_OK;

    /* Initialize the board hardware */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    /* Initialize FSL debug console. */
    BOARD_InitDebugConsole();

    /* Initialize LEDs */
    LED_RED_INIT(LOGIC_LED_OFF);
    LED_GREEN_INIT(LOGIC_LED_OFF);
    LED_BLUE_INIT(LOGIC_LED_OFF);

    /* Initialize periodic interrupt timer */
    main_initPIT();

    /* Initialize software timers */
    swTimer_init();

    /* Initialize the VSCP framework */
    if (VSCP_CORE_RET_OK != vscp_core_init())
    {
        status = MAIN_RET_ERROR;
    }

    /* Configure all software timer */
    swTimer_start(MAIN_SWTIMER_10MS_ID, MAIN_SWTIMER_10MS_PERIOD, FALSE);
    swTimer_start(MAIN_SWTIMER_250MS_ID, MAIN_SWTIMER_250MS_PERIOD, FALSE);

    return status;
}

/**
 * This function initializes run level 2.
 * Precondition: Interrupts are enabled
 *
 * @return Status
 */
static MAIN_RET main_initRunLevel2(void)
{
    MAIN_RET    status  = MAIN_RET_OK;

    /* Start periodic interrupt timer, channel 0 */
    PIT_StartTimer(PIT, kPIT_Chnl_0);

    return status;
}

/**
 * This function initializes the periodic interrupt timer and
 * setup the channel for a 1 ms tick.
 */
static void main_initPIT(void)
{
    pit_config_t    pitConfig   = { 0 };    /* Structure of initialize PIT */

    /* Get default configuration */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000U, PIT_SOURCE_CLOCK()));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    return;
}

static void main_enableInterrupts(void)
{
    /* Enable pit timer channel 0 interrupt at the NVIC */
    EnableIRQ(PIT0_IRQn);

    return;
}

/**
 * This function returns the current init button state.
 * Note, this state is not debounced!
 *
 * @return State of button
 * @retval FALSE	Button released
 * @retval TRUE		Button pressed
 */
static BOOL main_getInitButtonState(void)
{
	BOOL	result = FALSE;

	if (0 == (BOARD_SW2_GPIO->PDIR & (1 << BOARD_SW2_GPIO_PIN)))
	{
		result = TRUE;
	}

	return result;
}

/**
 * This function process the status lamp.
 */
static void main_processStatusLamp(void)
{
    VSCP_LAMP_STATE state   = vscp_portable_getLampState();
    static uint8_t  slowCnt = 0;

    switch(state)
    {
    case VSCP_LAMP_STATE_OFF:
    	LED_RED_OFF();
        break;

    case VSCP_LAMP_STATE_ON:
    	LED_RED_ON();
        break;

    case VSCP_LAMP_STATE_BLINK_SLOW:
        if (0 == (slowCnt % 4))
        {
        	LED_RED_TOGGLE();
        }
        break;

    case VSCP_LAMP_STATE_BLINK_FAST:
    	LED_RED_TOGGLE();
        break;

    default:
        break;
    }

    /* Reset counter for slow blinking */
    if (VSCP_LAMP_STATE_BLINK_SLOW != state)
    {
        slowCnt = 0;
    }
    else
    {
        ++slowCnt;
    }

    return;
}
