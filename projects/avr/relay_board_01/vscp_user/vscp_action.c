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

#include "relay.h"
#include "shutter.h"
#include "vscp_evt_information.h"
#include "vscp_ps_user.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/**
 * Action: No operation
 * Parameter: Not used
 */
#define VSCP_ACTION_NO_OPERATION            0

/**
 * Action: Enable relay
 * Parameter: Bit 0 is relay 0 and etc.
 * Note, a 1 enables a relay, a 0 doesn't care.
 */
#define VSCP_ACTION_ENABLE_RELAY            1

/**
 * Action: Disable relay
 * Parameter: Bit 0 is relay 0 and etc.
 * Note, a 1 disable a relay, a 0 doesn't care.
 */
#define VSCP_ACTION_DISABLE_RELAY           2

/**
 * Action: Toggle relay
 * Parameter: Bit 0 is relay 0 and etc.
 * Note, a 1 toggles a relay, a 0 doesn't care.
 */
#define VSCP_ACTION_TOGGLE_RELAY            3

/**
 * Action: Stop shutter
 * Parameter: Bit 0 is shutter 0 and etc.
 * Note, a 1 stops a shutter, a 0 does nothing.
 */
#define VSCP_ACTION_STOP_SHUTTER            4

/**
 * Action: Drive shutter up
 * Parameter: Bit 0 is shutter 0 and etc.
 * Note, a 1 drives a shutter, a 0 does nothing.
 */
#define VSCP_ACTION_DRIVE_SHUTTER_UP        5

/**
 * Action: Drive shutter down
 * Parameter: Bit 0 is shutter 0 and etc.
 * Note, a 1 drives a shutter, a 0 does nothing.
 */
#define VSCP_ACTION_DRIVE_SHUTTER_DOWN      6

/**
 * Action: Drive shutter to top
 * Parameter: Bit 0 is shutter 0 and etc.
 * Note, a 1 drives a shutter, a 0 does nothing.
 */
#define VSCP_ACTION_DRIVE_SHUTTER_TOP       7

/**
 * Action: Drive shutter to bottom
 * Parameter: Bit 0 is shutter 0 and etc.
 * Note, a 1 drives a shutter, a 0 does nothing.
 */
#define VSCP_ACTION_DRIVE_SHUTTER_BOTTOM    8

/**
 * Action: Calibration drive of the shutter
 * Parameter: Bit 0 is shutter 0 and etc.
 * Note, a 1 drives a shutter, a 0 does nothing.
 */
#define VSCP_ACTION_CALIBRATE_SHUTTER       9

/**
 * Action: Alert shutter about high wind speed
 * Parameter: Bit 0 is shutter 0 and etc.
 * Note, a 1 alerts a shutter, a 0 does nothing.
 */
#define VSCP_ACTION_ALERT_SHUTTER           10

/**
 * Action: Remove a shutter shutter alert about high wind speed
 * Parameter: Bit 0 is shutter 0 and etc.
 * Note, a 1 removes the shutter alert, a 0 does nothing.
 */
#define VSCP_ACTION_REMOVE_SHUTTER_ALERT    11

/**
 * Action: Drive shutter to shade position
 * Parameter: Bit 0 is shutter 0 and etc.
 * Note, a 1 drives a shutter, a 0 does nothing.
 */
#define VSCP_ACTION_DRIVE_SHUTTER_SHADE     12

/**
 * Action: Turn shutter (jalousie)
 * Parameter: Bit 0-3 are shutter 1-4, bit 4-7 are used for angle.
 * Note, a 1 drives a shutter, a 0 does nothing.
 */
#define VSCP_ACTION_TURN_SHUTTER            13

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static void vscp_action_activateRelay(uint8_t par, BOOL activate);
static void vscp_action_toggleRelay(uint8_t par);
static void vscp_action_driveShutter(uint8_t par, SHUTTER_DIR dir, uint16_t duration);
static void vscp_action_alertShutter(uint8_t par, BOOL alert);
static void vscp_action_driveAbsShutter(uint8_t par, uint8_t pos);
static void vscp_action_turnShutter(uint8_t par);

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
    /* Nothing to do */

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
    NOT_USED(msg);

    switch(action)
    {
    case VSCP_ACTION_NO_OPERATION:
        /* Do nothing! */
        break;

    case VSCP_ACTION_ENABLE_RELAY:
        vscp_action_activateRelay(par, TRUE);
        break;

    case VSCP_ACTION_DISABLE_RELAY:
        vscp_action_activateRelay(par, FALSE);
        break;

    case VSCP_ACTION_TOGGLE_RELAY:
        vscp_action_toggleRelay(par);
        break;

    case VSCP_ACTION_STOP_SHUTTER:
        vscp_action_driveShutter(par, SHUTTER_DIR_STOP, 0);
        break;

    case VSCP_ACTION_DRIVE_SHUTTER_UP:
        vscp_action_driveShutter(par, SHUTTER_DIR_UP, 0);
        break;

    case VSCP_ACTION_DRIVE_SHUTTER_DOWN:
        vscp_action_driveShutter(par, SHUTTER_DIR_DOWN, 0);
        break;

    case VSCP_ACTION_DRIVE_SHUTTER_TOP:
        vscp_action_driveShutter(par, SHUTTER_DIR_TOP, 0);
        break;

    case VSCP_ACTION_DRIVE_SHUTTER_BOTTOM:
        vscp_action_driveShutter(par, SHUTTER_DIR_BOTTOM, 0);
        break;

    case VSCP_ACTION_CALIBRATE_SHUTTER:
        vscp_action_driveShutter(par, SHUTTER_DIR_CALIBRATE, 0);
        break;

    case VSCP_ACTION_ALERT_SHUTTER:
        vscp_action_alertShutter(par, TRUE);
        break;

    case VSCP_ACTION_REMOVE_SHUTTER_ALERT:
        vscp_action_alertShutter(par, FALSE);
        break;

    case VSCP_ACTION_DRIVE_SHUTTER_SHADE:
        /* Close shutter for 2/3 % */
        vscp_action_driveAbsShutter(par, (255 / 3) * 2);
        break;

    case VSCP_ACTION_TURN_SHUTTER:
        vscp_action_turnShutter(par);
        break;

    default:
        break;
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function executes a relay action.
 *
 * @param[in]   par         Parameter, which contains the instance number
 * @param[in]   activate    Enable relay or disable it
 */
static void vscp_action_activateRelay(uint8_t par, BOOL activate)
{
    uint8_t index   = 0;

    for(index = 0; index < RELAY_NUM; ++index)
    {
        /* Relay masked? */
        if (TRUE == IS_BIT_SET(par, index))
        {
            /* Control the relay only in case that no enabled shutter use it. */
            if (FALSE == shutter_isEnabled(index / 2))
            {
                /* Change relay state only if relay is enabled and relay state is different. */
                if ((TRUE == relay_isEnabled(index)) &&
                    (relay_isActivated(index) != activate))
                {
                    relay_activate(index, activate);

                    /* If enabled, send a VSCP event for any relay state change. */
                    if (0 != ((vscp_ps_user_readRelayEventConfig() >> index) & 0x01))
                    {
                        if (FALSE == activate)
                        {
                            vscp_evt_information_sendOff(   index,
                                                            vscp_ps_user_readRelayEventZone(index),
                                                            vscp_ps_user_readRelayEventSubZone(index));
                        }
                        else
                        {
                            vscp_evt_information_sendOn(    index,
                                                            vscp_ps_user_readRelayEventZone(index),
                                                            vscp_ps_user_readRelayEventSubZone(index));
                        }
                    }
                }
            }
        }
    }

    return;
}

/**
 * This function executes a relay action.
 *
 * @param[in]   par         Parameter, which contains the instance number
 */
static void vscp_action_toggleRelay(uint8_t par)
{
    uint8_t index   = 0;

    for(index = 0; index < RELAY_NUM; ++index)
    {
        /* Relay masked? */
        if (TRUE == IS_BIT_SET(par, index))
        {
            /* Control the relay only in case that no enabled shutter use it. */
            if (FALSE == shutter_isEnabled(index / 2))
            {
                /* Change relay state only if relay is enabled. */
                if (TRUE == relay_isEnabled(index))
                {
                    BOOL isActive = relay_isActivated(index);

                    if (FALSE == isActive)
                    {
                        relay_activate(index, TRUE);
                    }
                    else
                    {
                        relay_activate(index, FALSE);
                    }

                    /* If enabled, send a VSCP event for any relay state change. */
                    if (0 != ((vscp_ps_user_readRelayEventConfig() >> index) & 0x01))
                    {
                        if (TRUE == isActive)
                        {
                            vscp_evt_information_sendOff(   index,
                                                            vscp_ps_user_readRelayEventZone(index),
                                                            vscp_ps_user_readRelayEventSubZone(index));
                        }
                        else
                        {
                            vscp_evt_information_sendOn(    index,
                                                            vscp_ps_user_readRelayEventZone(index),
                                                            vscp_ps_user_readRelayEventSubZone(index));
                        }
                    }
                }
            }
        }
    }

    return;
}

/**
 * This function executes a shutter action.
 *
 * @param[in]   par         Parameter, which contains the instance numbers
 * @param[in]   dir         Shutter drive direction
 * @param[in]   duration    Shutter drive duration in 0,1s
 */
static void vscp_action_driveShutter(uint8_t par, SHUTTER_DIR dir, uint16_t duration)
{
    uint8_t index   = 0;

    for(index = 0; index < SHUTTER_NUM; ++index)
    {
        /* Shutter masked? */
        if (TRUE == IS_BIT_SET(par, index))
        {
            shutter_drive(index, dir, duration);
        }
    }

    return;
}

/**
 * This function executes a alert shutter action.
 *
 * @param[in]   par     Parameter, which contains the instance numbers
 * @param[in]   alert   Alert a shutter or remove the alert
 */
static void vscp_action_alertShutter(uint8_t par, BOOL alert)
{
    uint8_t index   = 0;

    for(index = 0; index < SHUTTER_NUM; ++index)
    {
        /* Shutter masked? */
        if (TRUE == IS_BIT_SET(par, index))
        {
            shutter_windAlert(index, alert);
        }
    }

    return;
}

/**
 * This function executes a shade shutter action.
 *
 * @param[in]   par Parameter, which contains the instance numbers
 * @param[in]   pos Position
 */
static void vscp_action_driveAbsShutter(uint8_t par, uint8_t pos)
{
    uint8_t index   = 0;

    for(index = 0; index < SHUTTER_NUM; ++index)
    {
        /* Shutter masked? */
        if (TRUE == IS_BIT_SET(par, index))
        {
            shutter_driveAbs(index, pos);
        }
    }

    return;
}

/**
 * This function executes a turn shutter action.
 *
 * @param[in]   par     Parameter, which contains the instance numbers in the low nibble
 *                      and the angle in the high nibble.
 */
static void vscp_action_turnShutter(uint8_t par)
{
    uint8_t index       = 0;
    uint8_t instances   = (par >> 0) & 0x0f;
    uint8_t angle       = ((par >> 4) & 0x0f) * 6;  /* One digit corresponds to 6° */

    for(index = 0; index < SHUTTER_NUM; ++index)
    {
        /* Shutter masked? */
        if (TRUE == IS_BIT_SET(instances, index))
        {
            shutter_turn(index, angle);
        }
    }

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) || VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION ) */
