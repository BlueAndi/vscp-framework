/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2021, Andreas Merkle
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
@brief  This module initializes the hardware.
@file   hw.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module initializes the hardware for the first run. Later other modules
can change these hardware settings.

*******************************************************************************/
/** @defgroup hw Hardware initialization
 * The hardware is initialized for the first run. Later other modules can
 * change these hardware settings.
 *
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __HW_H__
#define __HW_H__

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

/** Status LED port */
#define HW_PORT_LED_STATUS      PORTC

/** Status LED pin */
#define HW_PIN_LED_STATUS       PC5

/** Enable status LED */
#define HW_ENABLE_STATUS_LED()  BIT_SET(HW_PORT_LED_STATUS, HW_PIN_LED_STATUS)

/** Disable status LED */
#define HW_DISABLE_STATUS_LED() BIT_CLR(HW_PORT_LED_STATUS, HW_PIN_LED_STATUS)

/** Toggle status LED */
#define HW_TOGGLE_STATUS_LED()  do{ HW_PORT_LED_STATUS ^= _BV(HW_PIN_LED_STATUS); }while(0)

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
extern void hw_init(void);

/**
 * Get the state of the segment initialization button.
 * Note that the button state is not debounced!
 *
 * @return Button state
 * @return FALSE    Button is released
 * @return TRUE     Button is pressed
 */
extern BOOL hw_getSegInitButtonState(void);

#ifdef __cplusplus
}
#endif

#endif  /* __HW_H__ */

/** @} */
