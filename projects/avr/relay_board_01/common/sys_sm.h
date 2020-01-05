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
@brief  System state machine
@file   sys_sm.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains the system state machine.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __SYS_SM_H__
#define __SYS_SM_H__

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

/** This type defines the actions which are executed in the error or idle state. */
typedef enum
{
    SYS_SM_ACTION_NOTHING = 0,  /**< Nothing to do */
    SYS_SM_ACTION_KEEP,         /**< Keep current assigned action */
    SYS_SM_ACTION_REBOOT,       /**< Reboot system */
    SYS_SM_ACTION_HALT          /**< Halt system */

} SYS_SM_ACTION;

/** This type defines the system states. */
typedef enum
{
    SYS_SM_STATE_START_UP = 0,  /**< System starts up (this state can not be requested) */
    SYS_SM_STATE_ACTIVE,        /**< System is active (operating mode) */
    SYS_SM_STATE_SHUTDOWN,      /**< System shutdown */
    SYS_SM_STATE_ERROR,         /**< System is in error state */
    SYS_SM_STATE_IDLE,          /**< System is idle (this state can not be requested) */

} SYS_SM_STATE;

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
extern void sys_sm_init(void);

/**
 * This function requests a system state transition.
 *
 * @param[in] state Next system state
 * @param[in] action If state is reached, this action will be executed (application specific).
 */
extern void sys_sm_requestState(SYS_SM_STATE state, SYS_SM_ACTION action);

/**
 * This function returns the current system state.
 *
 * @param[in,out] action    Current assigned action
 *
 * @return Current system state
 */
extern SYS_SM_STATE sys_sm_getState(SYS_SM_ACTION * const action);

#ifdef __cplusplus
}
#endif

#endif  /* __SYS_SM_H__ */
