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
@brief  VSCP actions
@file   vscp_action.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_action.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_action.h"

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) || VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION )

#include "hw.h"
#include "vscp_evt_information.h"
#include "vscp_util.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Action 1: Enable the status LED */
#define VSCP_ACTION_001_ENABLE_STATUS_LED   1

/** Action 2: Disable the status LED */
#define VSCP_ACTION_002_DISABLE_STATUS_LED  2

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

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes the module.
 */
extern void vscp_action_init(void)
{
    /* Implement your code here ... */

    return;
}

/**
 * This function executes a action with the given parameter.
 *
 * @param[in]   action  Action id
 * @param[in]   par     Action parameter
 * @param[in]   msg     Received VSCP message which triggered the action
 */
extern void vscp_action_execute(uint8_t action, uint8_t par, vscp_RxMessage const * const msg)
{
    VSCP_UTIL_UNUSED(par);
    VSCP_UTIL_UNUSED(msg);

    switch(action)
    {
    /* Enable status LED, no parameter */
    case VSCP_ACTION_001_ENABLE_STATUS_LED:
        HW_ENABLE_STATUS_LED();
        vscp_evt_information_sendOn(0, 0xff, 0xff);
        break;

    /* Disable status LED, no parameter */
    case VSCP_ACTION_002_DISABLE_STATUS_LED:
        HW_DISABLE_STATUS_LED();
        vscp_evt_information_sendOff(0, 0xff, 0xff);
        break;

    default:
        break;
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) || VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION ) */
