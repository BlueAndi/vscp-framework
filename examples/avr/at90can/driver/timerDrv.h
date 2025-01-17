/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2025 Andreas Merkle
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
@brief  Hardware timer driver
@file   timerDrv.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides an 8-bit hardware timer.

*******************************************************************************/
/** @defgroup timerDrv Hardware timer driver
 * A 8-bit hardware timer is provided with general functionality.
 *
 * Used periphery:
 * - 8-bit timer 0
 *
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __TIMERDRV_H__
#define __TIMERDRV_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "system.h"

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

/** Timer tick in ms */
#define TIMERDRV_TICK_IN_MS 1

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Calculates the number ticks for the given time in ms. */
#define TIMERDRV_MS(__var)  ((__var) / TIMERDRV_TICK_IN_MS)

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines a timer tick count callback. */
typedef void    (*timerDrv_Callback)(void);

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
extern void timerDrv_init(void);

/**
 * This function set the callback function which will be called every tick
 * in the timer ISR.
 *
 * @param[in]   cb  Callback
 */
extern void timerDrv_setCallback(timerDrv_Callback cb);

/**
 * This function returns the tick counter value.
 *
 * @return  Tick counter value
 */
extern uint8_t   timerDrv_getTickCount(void);

/**
 * This function sleeps the given time in ticks. Important: The calling task
 * will be blocked until sleep time is over.
 *
 * @param[in]   sleepTime   Time to sleep in ticks
 */
extern void timerDrv_sleep(uint8_t sleepTime);

#ifdef __cplusplus
}
#endif

#endif  /* __TIMERDRV_H__ */

/** @} */
