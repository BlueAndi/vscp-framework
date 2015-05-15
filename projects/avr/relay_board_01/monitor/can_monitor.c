/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2015, Andreas Merkle
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
@file   can_monitor.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see can_monitor.h

@section svn Subversion
$Author: amerkle $
$Rev: 401 $
$Date: 2014-12-27 23:42:44 +0100 (Sa, 27 Dez 2014) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "can_monitor.h"
#include "can.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** The MCP2515 is error-active if both error counters are below the error-passive
 * limit.
 */
#define CAN_MONITOR_ERR_PASSIVE_LIMIT   128

/** The MCP2515 goes bus-off if the transmit errors exceeds the bus-off limit. */
#define CAN_MONITOR_BUS_OFF_LIMIT       255

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

/******************************************************************************\
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern void can_monitor_init(void)
{
    /* Nothing to do */

    return;
}

/**
 * This function returns the current CAN state.
 *
 * @return CAN state
 */
extern CAN_MONITOR_STATE can_monitor_getState(void)
{
    can_error_register_t    canErrorReg = can_read_error_register();
    CAN_MONITOR_STATE       state       = CAN_MONITOR_STATE_ERR_ACTIVE;

    /* The MCP2515 goes bus-off if the transmit errors exceeds the bus-off limit. */
    if (CAN_MONITOR_BUS_OFF_LIMIT == canErrorReg.tx)
    {
        state = CAN_MONITOR_STATE_BUS_OFF;
    }
    /* If any of the error counters equals or exceeds the error passive limit,
     * the MCP2515 changes to error-passive state.
     */
    else if ((CAN_MONITOR_ERR_PASSIVE_LIMIT <= canErrorReg.tx) ||
             (CAN_MONITOR_ERR_PASSIVE_LIMIT <= canErrorReg.rx))
    {
        state = CAN_MONITOR_STATE_ERR_PASSIVE;
    }

    return state;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
