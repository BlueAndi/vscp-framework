/* The MIT License (MIT)
 * 
 * Copyright (c) 2014 - 2015, Andreas Merkle
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
@brief  Software timer
@file   swTimer.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see swTimer.h

@section svn Subversion
$Author: amerkle $
$Rev: 420 $
$Date: 2014-12-29 21:49:55 +0100 (Mo, 29 Dez 2014) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "swTimer.h"
#include <stddef.h>
#include <string.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines a single timer context and its parameter. */
typedef struct  
{
    uint8_t             startValue; /**< Start value */
    BOOL                isManual;   /**< Reset timer manual or automatically */

    volatile uint8_t    value;  /**< Current value, which is counted down. */

} swTimer_Context;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** All timer contexts */
static swTimer_Context  swTimer_timers[SWTIMER_NUM];

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
extern void swTimer_init(void)
{
    memset(swTimer_timers, 0, sizeof(swTimer_timers));

    return;
}

/**
 * This function start a timer with the given value.
 *
 * @param[in]   id      Timer id
 * @param[in]   value   Start value
 * @param[in]   manual  Restart timer manual or automatically
 */
extern void swTimer_start(uint8_t id, uint8_t value, BOOL manual)
{
    swTimer_Context*    timer   = NULL;

    if (SWTIMER_NUM <= id)
    {
        return;
    }

    timer = &swTimer_timers[id];

    timer->startValue   = value;
    timer->isManual     = manual;
    timer->value        = timer->startValue;

    return;
}

/**
 * This function stops a timer.
 *
 * @param[in]   id  Timer id
 */
extern void swTimer_stop(uint8_t id)
{
    swTimer_Context*    timer   = NULL;

    if (SWTIMER_NUM <= id)
    {
        return;
    }

    timer = &swTimer_timers[id];

    timer->value = 0;

    return;
}

/**
 * This function returns the timer status.
 * If the timer stopped and is configured for automatically restart,
 * the timer will be restarted.
 *
 * @param[in]   id  Timer id
 * @return Status
 * @retval FALSE    Timer is running
 * @retval TRUE     Timer stopped
 */
extern BOOL swTimer_getStatus(uint8_t id)
{
    swTimer_Context*    timer   = NULL;
    BOOL                status  = FALSE;

    if (SWTIMER_NUM <= id)
    {
        return TRUE;
    }

    timer = &swTimer_timers[id];

    if (0 == timer->value)
    {
        status = TRUE;

        /* Restart timer? */
        if (FALSE == timer->isManual)
        {
            timer->value = timer->startValue;
        }
    }

    return status;
}

/**
 * This function process all software timer and is typically called in a
 * hardware timer interrupt service routine.
 */
extern void swTimer_process(void)
{
    uint8_t index   = 0;

    for(index = 0; index < SWTIMER_NUM; ++index)
    {
        swTimer_Context*    timer   = &swTimer_timers[index];

        if (0 < timer->value)
        {
            --timer->value;
        }
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
