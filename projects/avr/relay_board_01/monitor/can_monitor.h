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
@brief  CAN monitor
@file   can_monitor.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides a CAN monitor which checks for CAN bus-off.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __CAN_MONITOR_H__
#define __CAN_MONITOR_H__

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

typedef enum
{
    /** The error-active state is the usual state where the node can transmit
     * messages and active error frames (made of dominant bits) without any
     * restrictions.
     */
    CAN_MONITOR_STATE_ERR_ACTIVE = 0,

    /** In the error-passive state, messages and passive error frames (made of
     * recessive bits) may be transmitted.
     */
    CAN_MONITOR_STATE_ERR_PASSIVE,

    /** The bus-off state makes it temporarily impossible for the station to
     * participate in the bus communication. During this state, messages can
     * neither be received or transmitted.
     */
    CAN_MONITOR_STATE_BUS_OFF

} CAN_MONITOR_STATE;

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
extern void can_monitor_init(void);

/**
 * This function monitors the CAN bus state and in case of an
 * error passive or bus off, corresponding alarm bits are set.
 */
extern void can_monitor_process(void);

/**
 * This function returns the current CAN bus state.
 *
 * @return CAN bus state
 */
extern CAN_MONITOR_STATE can_monitor_getState(void);

#ifdef __cplusplus
}
#endif

#endif  /* __CAN_MONITOR_H__ */
