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
@brief  Watchdog
@file   watchdog.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see watchdog.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "watchdog.h"
#include <avr/wdt.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Watchdog observation period */
#define WATCHDOG_PERIOD WDTO_30MS

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/******************************************************************************\
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern void watchdog_init(void)
{
    /* Nothing to do */

    return;
}

/**
 * This function enables or disables the watchdog timer.
 *
 * @param[in] enableIt  Enable (TRUE) or disable (FALSE) watchdog timer
 */
extern void watchdog_enable(BOOL enableIt)
{
    if (FALSE == enableIt)
    {
        wdt_disable();
    }
    else
    {
        wdt_enable(WATCHDOG_PERIOD);
    }

    return;
}

/**
 * This function process the watchdog.
 */
extern void watchdog_process(void)
{
    /* Trigger watchdog */
    wdt_reset();

    return;
}

/**
 * This function returns the reset source.
 *
 * @return Reset source
 */
extern WATCHDOG_RESET_SRC watchdog_getResetSource(void)
{
    WATCHDOG_RESET_SRC  src = WATCHDOG_RESET_SRC_EMPTY;

    /* Reset source stored in general purpose i/o register in the init3 phase. */
    if (TRUE == IS_BIT_SET(GPIOR0, PORF))
    {
        src = WATCHDOG_RESET_SRC_POWER_ON;
    }
    else if (TRUE == IS_BIT_SET(GPIOR0, EXTRF))
    {
        src = WATCHDOG_RESET_SRC_EXTERNAL;
    }
    else if (TRUE == IS_BIT_SET(GPIOR0, BORF))
    {
        src = WATCHDOG_RESET_SRC_BROWN_OUT;
    }
    else if (TRUE == IS_BIT_SET(GPIOR0, WDRF))
    {
        src = WATCHDOG_RESET_SRC_WD;
    }

    return src;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
