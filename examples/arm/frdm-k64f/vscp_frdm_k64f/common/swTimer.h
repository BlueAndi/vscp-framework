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
@brief  Software timer
@file   swTimer.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides software timer, which are triggered by a hardware timer.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __SWTIMER_H__
#define __SWTIMER_H__

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

/** Number of available software timer */
#define SWTIMER_NUM 2

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

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
extern void swTimer_init(void);

/**
 * This function start a timer with the given value.
 *
 * @param[in]   id      Timer id
 * @param[in]   value   Start value
 * @param[in]   manual  Restart timer manual or automatically
 */
extern void swTimer_start(uint8_t id, uint8_t value, BOOL manual);

/**
 * This function stops a timer.
 *
 * @param[in]   id  Timer id
 */
extern void swTimer_stop(uint8_t id);

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
extern BOOL swTimer_getStatus(uint8_t id);

/**
 * This function process all software timer and is typically called in a
 * hardware timer interrupt service routine.
 */
extern void swTimer_process(void);

#ifdef __cplusplus
}
#endif

#endif  /* __SWTIMER_H__ */
