/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2016, Andreas Merkle
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
@file   sys_sm.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see sys_sm.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "sys_sm.h"
#include <stddef.h>

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

/** Current system state */
static SYS_SM_STATE     sys_sm_state    = SYS_SM_STATE_START_UP;

/** Current assigned action */
static SYS_SM_ACTION    sys_sm_action   = SYS_SM_ACTION_NOTHING;

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
extern void sys_sm_init(void)
{
    /* Nothing to do */

    return;
}

/**
 * This function requests a system state transition.
 *
 * @param[in] state Next system state
 * @param[in] action If state is reached, this action will be executed (application specific).
 */
extern void sys_sm_requestState(SYS_SM_STATE state, SYS_SM_ACTION action)
{
    /* System startup can not be requested. If the user wish to reboot the system,
     * the user shall request the system shutdown with reboot action.
     */
    if (SYS_SM_STATE_START_UP != state)
    {
        sys_sm_state = state;

        if (SYS_SM_ACTION_KEEP != action)
        {
            sys_sm_action = action;
        }
    }

    return;
}

/**
 * This function returns the current system state.
 *
 * @param[in,out] action    Current assigned action
 *
 * @return Current system state
 */
extern SYS_SM_STATE sys_sm_getState(SYS_SM_ACTION * const action)
{
    if (NULL != action)
    {
        *action = sys_sm_action;
    }

    return sys_sm_state;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
