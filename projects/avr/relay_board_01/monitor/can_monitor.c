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
@file   can_monitor.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see can_monitor.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "can_monitor.h"
#include "can.h"
#include "vscp_core.h"
#include "vscp_portable.h"

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

static CAN_MONITOR_STATE can_monitor_determineCanBusState(void);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Current CAN bus state */
static CAN_MONITOR_STATE    can_monitor_state   = CAN_MONITOR_STATE_ERR_ACTIVE;

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
 * This function monitors the CAN bus state and in case of an
 * error passive or bus off, corresponding alarm bits are set.
 */
extern void can_monitor_process(void)
{
    CAN_MONITOR_STATE   state   = CAN_MONITOR_STATE_ERR_ACTIVE;

    /* Check the CAN bus and set alarms if necessary. */
    state = can_monitor_determineCanBusState();

    if (state != can_monitor_state)
    {
        if (CAN_MONITOR_STATE_ERR_PASSIVE == state)
        {
            vscp_core_setAlarm(VSCP_PORTABLE_ALARM_CAN_ERR_PASSIVE);
        }
        else if (CAN_MONITOR_STATE_BUS_OFF == state)
        {
            vscp_core_setAlarm(VSCP_PORTABLE_ALARM_CAN_BUS_OFF);
        }

        can_monitor_state = state;
    }

    return;
}

/**
 * This function returns the current CAN bus state.
 *
 * @return CAN bus state
 */
extern CAN_MONITOR_STATE can_monitor_getState(void)
{
    return can_monitor_state;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function determines the current CAN bus state by reading the error
 * counters of the CAN controller.
 *
 * @return CAN bus state
 */
static CAN_MONITOR_STATE can_monitor_determineCanBusState(void)
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
