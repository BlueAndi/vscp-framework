/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2020, Andreas Merkle
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
@brief  Time
@file   time.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see time.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "time.h"
#include <stddef.h>
#include <util/atomic.h>

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

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Current system time */
static volatile time_Time   time_systemTime;

/** Day counter (overflows!) */
static volatile uint8_t     time_day;

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
extern void time_init(void)
{
    time_systemTime.h   = 0;
    time_systemTime.m   = 0;
    time_systemTime.s   = 0;
    time_systemTime.ms  = 0;
    time_day            = 0;

    return;
}

/**
 * Set time.
 *
 * @param[in]  time    Timer to set
 */
extern void time_set(const time_Time* time)
{
    if (NULL != time)
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            time_systemTime = *time;

            /* Increase day counter to signal that the time changed. */
            time_day++;
        }
    }

    return;
}

/**
 * Get time values.
 *
 * @param[out]  time    Timer structure, which to fill.
 */
extern void time_get(time_Time* time)
{
    if (NULL != time)
    {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            *time = time_systemTime;
        }
    }

    return;
}

/**
 * Get time values.
 *
 * Use this function only in ISR routines, because the internal timer structure
 * is not protected against concurrency.
 *
 * @param[out]  time    Timer structure, which to fill.
 */
extern void time_isrGet(time_Time* time)
{
    if (NULL != time)
    {
        *time = time_systemTime;
    }

    return;
}

/**
 * Get day overflow counter.
 * Can be called in an ISR too.
 *
 * @return Day overflow counter
 */
extern uint8_t time_getDay(void)
{
    /* Note, because the day counter is a uint8_t and therefore a atomic
     * instruction, no interrupt blocking is necessary.
     */
    return time_day;
}

/**
 * Calculate the difference of two timestamps.
 *
 * @param[in]   ts1     Timestamp 1
 * @param[in]   ts2     Timestamp 2
 * @param[out]  diff    Difference
 */
extern void time_calculateDiff(const time_Time *ts1, const time_Time *ts2, time_Time *diff)
{
    const time_Time *old        = ts2;
    const time_Time *now        = ts1;
    uint8_t         overflow    = 0;

    /* Determine the older timestamp */
    if (ts1->h < ts2->h)
    {
        old = ts1;
        now = ts2;
    }
    else if (ts1->m > ts2->m)
    {
        old = ts1;
        now = ts2;
    }
    else if (ts1->s > ts2->s)
    {
        old = ts1;
        now = ts2;
    }
    else if (ts1->ms > ts2->ms)
    {
        old = ts1;
        now = ts2;
    }

    if (now->ms < old->ms)
    {
        diff->ms = (uint16_t)1000 - old->ms + now->ms;
        overflow = 1;
    }
    else
    {
        diff->ms = now->ms - old->ms;
        overflow = 0;
    }

    if (now->s < (old->s + overflow))
    {
        diff->s = 60 - old->s - overflow + now->s;
        overflow = 1;
    }
    else
    {
        diff->s = now->s - old->s - overflow;
        overflow = 0;
    }

    if (now->m < (old->m + overflow))
    {
        diff->m = 60 - old->m - overflow + now->m;
        overflow = 1;
    }
    else
    {
        diff->m = now->m - old->m - overflow;
        overflow = 0;
    }

    if (now->h < (old->h + overflow))
    {
        diff->h = 60 - old->h - overflow + now->h;
    }
    else
    {
        diff->h = now->h - old->h - overflow;
    }

    return;
}

/**
 * This function updates the system time.
 *
 * @param[in]   period  Period in ms of calling this function.
 */
extern void time_process(uint16_t period)
{
    time_systemTime.ms += period;

    if ((uint16_t)1000 <= time_systemTime.ms)
    {
        time_systemTime.ms = 0;

        time_systemTime.s++;
        if (60 <= time_systemTime.s)
        {
            time_systemTime.s = 0;

            time_systemTime.m++;
            if (60 <= time_systemTime.m)
            {
                time_systemTime.m = 0;

                time_systemTime.h++;
                if (24 <= time_systemTime.h)
                {
                    time_systemTime.h = 0;
                    time_day++;
                }
            }
        }
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
