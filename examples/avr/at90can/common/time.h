/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2022 Andreas Merkle
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
@file   time.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides the current time.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __TIME_H__
#define __TIME_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "system.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

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

/** Timer structure with all clock typical values. */
typedef struct
{
    uint16_t    ms; /**< Milliseconds */
    uint8_t     s;  /**< Seconds */
    uint8_t     m;  /**< Minutes */
    uint8_t     h;  /**< Hours */

} time_Time;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern void time_init(void);

/**
 * Set time.
 *
 * @param[in]  time    Timer to set
 */
extern void time_set(const time_Time* time);

/**
 * Get time values.
 *
 * @param[out]  time    Timer structure, which to fill.
 */
extern void time_get(time_Time* time);

/**
 * Get time values.
 *
 * Use this function only in ISR routines, because the internal timer structure
 * is not protected against concurrency.
 *
 * @param[out]  time    Timer structure, which to fill.
 */
extern void time_isrGet(time_Time* time);

/**
 * Get day overflow counter.
 * Can be called in an ISR too.
 *
 * @return Day overflow counter
 */
extern uint8_t time_getDay(void);

/**
 * Calculate the difference of two timestamps.
 *
 * @param[in]   ts1     Timestamp 1
 * @param[in]   ts2     Timestamp 2
 * @param[out]  diff    Difference
 */
extern void time_calculateDiff(const time_Time *ts1, const time_Time *ts2, time_Time *diff);

/**
 * This function updates the system time.
 *
 * @param[in]   periode Period in ticks of calling this function.
 */
extern void time_process(uint16_t periode);

#ifdef __cplusplus
}
#endif

#endif  /* __TIME_H__ */
