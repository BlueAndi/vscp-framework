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
@brief  Watchdog
@file   watchdog.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module handles the watchdog.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

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

/** This type defines possible reset sources. */
typedef enum
{
    WATCHDOG_RESET_SRC_EMPTY = 0,   /**< No reset source available */
    WATCHDOG_RESET_SRC_POWER_ON,    /**< Power-on reset */
    WATCHDOG_RESET_SRC_EXTERNAL,    /**< External reset */
    WATCHDOG_RESET_SRC_BROWN_OUT,   /**< Brown-out reset */
    WATCHDOG_RESET_SRC_WD           /**< Watchdog reset */

} WATCHDOG_RESET_SRC;

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
extern void watchdog_init(void);

/**
 * This function enables or disables the watchdog timer.
 *
 * @param[in] enableIt  Enable (TRUE) or disable (FALSE) watchdog timer
 */
extern void watchdog_enable(BOOL enableIt);

/**
 * This function process the watchdog.
 */
extern void watchdog_process(void);

#ifdef __cplusplus
}
#endif

#endif  /* __WATCHDOG_H__ */
