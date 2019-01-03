/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2019, Andreas Merkle
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
@brief  Relay driver
@file   vscp_ps.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module is the driver for the relays. It provides an interface to enable
or disable a relay.

Because to switch a relay a higher current is necessary, than for holding a
relay in its on state, the relay are powered with a pulse width modulation.

Used periphery:
* 8-bit timer 1 for PWM output

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __RELAY_H__
#define __RELAY_H__

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

/** Number of relays to control */
#define RELAY_NUM   8

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
extern void relay_init(void);

/**
 * Enable/Disable a relay.
 *
 * @param[in]   index       Index of the relay
 * @param[in]   enableIt    Enable/Disable it
 */
 void relay_enable(uint8_t index, BOOL enableIt);

 /**
 * This function returns the relay enable/disable status.
 *
 * @return Relay is enabled (TRUE) or not (FALSE)
 */
extern BOOL relay_isEnabled(uint8_t index);

/**
 * Activate or deactivate a relay.
 *
 * @param[in]   index       Index of the relay
 * @param[in]   activate    Activate or deactivate it
 */
extern void relay_activate(uint8_t index, BOOL activate);

/**
 * Is relay activated or not.
 *
 * @return Relay status
 * @retval FALSE    Relay is deactivated
 * @retval TRUE     Relay is activated
 */
extern BOOL relay_isActivated(uint8_t index);

/**
 * Set PWM switching value, so that the relay goes from off to on.
 *
 * @param[in]   value   Switching PWM value
 */
extern void relay_setSwitchingPwm(uint16_t value);

/**
 * Set PWM holding value, so that the relay stays on.
 *
 * @param[in]   value   Holding PWM value
 */
extern void relay_setHoldingPwm(uint16_t value);

#ifdef __cplusplus
}
#endif

#endif  /* __RELAY_H__ */
