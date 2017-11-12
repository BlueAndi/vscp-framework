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
 * Copyright (c) 2014 - 2017, Andreas Merkle
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
#include "fsl_debug_console.h"

#include "vscp_core.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Halt program */
#define HALT()  do{ for(;;) { __asm("NOP"); } }while(0)

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

    /* ********** Run level 1 - interrupts disabled ********** */

    /* Enter run level 1 */
    if (MAIN_RET_OK != main_initRunLevel1())
    {
        /* Error */
        HALT();
    }

    /* ********** Run level 2 - interrupts enabled ********** */

#if 0
    /* Enable interrupts */
    sei();
#endif

    /* Enter run level 2 */
    if (MAIN_RET_OK != main_initRunLevel2())
    {
        /* Error */
        HALT();
    }

    /* ********** Run level 3 - main loop starts ********** */

#if 0
    /* Show VSCP protocol and framework version */
    main_print("VSCP framework ");
    main_print(VSCP_CORE_FRAMEWORK_VERSION);
    main_print("\n");
    main_print("VSCP ");
    main_print(VSCP_CORE_VERSION_STR);
    main_print("\n\n");
#endif

    /* Main loop */
    for(;;)
    {
        /* Process VSCP framework */
        vscp_core_process();

#if 0

        /* Initialize the VSCP segment, because user pressed the segment
         * initialization button?
         *
         * If the user keeps the button pressed and it looks like the user never
         * release it, the initialization will anyway take place only once.
         */
        if ((TRUE == main_isInitButtonPressed) &&
            (FALSE == lastSegInitButtonState))
        {
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

            /* Process the time */
            time_process(MAIN_SWTIMER_250MS_PERIOD);
        }

        /* Some stuff shall only be done in case that VSCP is in ACTIVE state. */
        if (TRUE == vscp_core_isActive())
        {
            /* Implement your code here ... */


        }

#endif

    }

    return 0 ;
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

    /* Initialize the VSCP framework */
    if (VSCP_CORE_RET_OK != vscp_core_init())
    {
        status = MAIN_RET_ERROR;
    }

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

    /* Implement your code here ... */

    return status;
}
