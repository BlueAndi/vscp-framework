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

#include <stdio.h>
#include "log.h"
#include "vscp_evt_information.h"
#include "lamp_sim.h"
#include "vscp_class_l1.h"
#include "vscp_type_control.h"

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
#define VSCP_ACTION_NO_OPERATION    0

/**
 * Action: Enable lamp
 * Parameter: Bit 0 is lamp 0 and etc.
 * Note, a 1 enables a lamp, a 0 doesn't care.
 */
#define VSCP_ACTION_ENABLE_LAMP     1

/**
 * Action: Disable lamp
 * Parameter: Bit 0 is lamp 0 and etc.
 * Note, a 1 disable a lamp, a 0 doesn't care.
 */
#define VSCP_ACTION_DISABLE_LAMP    2

/**
 * Action: Toggle lamp
 * Parameter: Bit 0 is lamp 0 and etc.
 * Note, a 1 toggles a lamp, a 0 doesn't care.
 */
#define VSCP_ACTION_TOGGLE_LAMP     3

/**
 * Action: Dim lamp
 * Parameter: Bit 0 is lamp 0 and etc.
 * Note, a 1 dim a lamp, a 0 doesn't care.
 */
#define VSCP_ACTION_DIM_LAMP        4

/** Class1.Control event type dim lamp - magic number for dimming down one step. */
#define VSCP_ACTION_DIM_DOWN_ONE_STEP   254

/** Class1.Control event type dim lamp - magic number for dimming up one step. */
#define VSCP_ACTION_DIM_UP_ONE_STEP     255

/** Minimum dim step in percent */
#define VSCP_ACTION_DIM_STEP            10

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static void vscp_action_enableLamp(uint8_t par, BOOL enableIt);
static void vscp_action_toggleLamp(uint8_t par);
static void vscp_action_dimLamp(uint8_t par, vscp_RxMessage const * const msg);

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
    switch(action)
    {
    case VSCP_ACTION_NO_OPERATION:
        /* Nothing to do */
        break;

    case VSCP_ACTION_ENABLE_LAMP:
        vscp_action_enableLamp(par, TRUE);
        break;

    case VSCP_ACTION_DISABLE_LAMP:
        vscp_action_enableLamp(par, FALSE);
        break;

    case VSCP_ACTION_TOGGLE_LAMP:
        vscp_action_toggleLamp(par);
        break;

    case VSCP_ACTION_DIM_LAMP:
        vscp_action_dimLamp(par, msg);
        break;

    default:
        platform_setTextColor(PLATFORM_COLOR_LIGHT_RED);
        log_printf("Unknown action %u with parameter 0x%02X.\n", action, par);
        platform_restoreTextColor();
        break;
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function enables or disables a lamp.
 *
 * @param[in]   par         Parameter, which contains the instance number
 * @param[in]   enableIt    Enable lamp or disable it
 */
static void vscp_action_enableLamp(uint8_t par, BOOL enableIt)
{
    uint8_t index       = 0;
    BOOL    anyChange   = FALSE;

    for(index = 0; index < LAMP_SIM_NUM; ++index)
    {
        uint8_t executeBit  = (par >> index) & 0x01;

        if (0 != executeBit)
        {
            if (lamp_sim_getState(index) != enableIt)
            {
                lamp_sim_setState(index, enableIt);

                /* Send lamp state */
                if (FALSE == lamp_sim_getState(index))
                {
                    (void)vscp_evt_information_sendOn(index, 255, 255);
                }
                else
                {
                    (void)vscp_evt_information_sendOff(index, 255, 255);
                }

                /* Show the user the current lamp state */
                platform_setTextColor(PLATFORM_COLOR_LIGHT_RED);
                if (FALSE == lamp_sim_getState(index))
                {
                    log_printf("Lamp %u is off.\n", index + 1);
                }
                else
                {
                    log_printf("Lamp %u is %u%% on.\n", index + 1, lamp_sim_getBrightness(index));
                }
                platform_restoreTextColor();

                /* Set dirty flag */
                anyChange = TRUE;
            }
        }
    }

    if (TRUE == anyChange)
    {
        /* Show all lamps */
        printf("\n");
        lamp_sim_show();
        printf("\n");
    }

    return;
}

/**
 * This function toggles a lamp.
 *
 * @param[in]   par Parameter, which contains the instance number
 */
static void vscp_action_toggleLamp(uint8_t par)
{
    uint8_t index       = 0;
    BOOL    anyChange   = FALSE;

    for(index = 0; index < LAMP_SIM_NUM; ++index)
    {
        uint8_t executeBit  = (par >> index) & 0x01;

        if (0 != executeBit)
        {
            BOOL state  = lamp_sim_getState(index);

            if (FALSE == state)
            {
                lamp_sim_setState(index, TRUE);
            }
            else
            {
                lamp_sim_setState(index, FALSE);
            }

            /* Send lamp state */
            if (FALSE == lamp_sim_getState(index))
            {
                (void)vscp_evt_information_sendOn(index, 255, 255);
            }
            else
            {
                (void)vscp_evt_information_sendOff(index, 255, 255);
            }

            /* Show the user the current lamp state */
            platform_setTextColor(PLATFORM_COLOR_LIGHT_RED);
            if (FALSE == lamp_sim_getState(index))
            {
                log_printf("Lamp %u is off.\n", index + 1);
            }
            else
            {
                log_printf("Lamp %u is %u%% on.\n", index + 1, lamp_sim_getBrightness(index));
            }
            platform_restoreTextColor();

            /* Set dirty flag */
            anyChange = TRUE;
        }
    }

    if (TRUE == anyChange)
    {
        /* Show all lamps */
        printf("\n");
        lamp_sim_show();
        printf("\n");
    }

    return;
}

/**
 * This function dims a lamp.
 *
 * @param[in]   par Parameter, which contains the instance number
 */
static void vscp_action_dimLamp(uint8_t par, vscp_RxMessage const * const msg)
{
    uint8_t index       = 0;
    BOOL    anyChange   = FALSE;

    if (NULL == msg)
    {
        return;
    }

    /* In case of class1.control dim lamp events, use the provided brightness
     * percentage value.
     * Any other event is not supported by this action.
     */
    if ((VSCP_CLASS_L1_CONTROL != msg->vscpClass) ||
        (VSCP_TYPE_CONTROL_DIM_LAMPS != msg->vscpType) ||
        (3 != msg->dataNum))
    {
        return;
    }

    /* Validate event data for brightness */
    if (100 < msg->data[0])
    {
        if ((VSCP_ACTION_DIM_DOWN_ONE_STEP != msg->data[0]) &&
            (VSCP_ACTION_DIM_UP_ONE_STEP == msg->data[0]))
        {
            return;
        }
    }

    for(index = 0; index < LAMP_SIM_NUM; ++index)
    {
        uint8_t executeBit  = (par >> index) & 0x01;

        if (0 != executeBit)
        {
            uint8_t brightness  = 0;
            BOOL    oldState    = lamp_sim_getState(index);
            BOOL    newState    = FALSE;

            if (VSCP_ACTION_DIM_DOWN_ONE_STEP == msg->data[0])
            {
                brightness = lamp_sim_getBrightness(index);

                if (VSCP_ACTION_DIM_STEP <= brightness)
                {
                    brightness -= VSCP_ACTION_DIM_STEP;
                }
                else
                {
                    brightness = 0;
                }
            }
            else if (VSCP_ACTION_DIM_UP_ONE_STEP == msg->data[0])
            {
                brightness = lamp_sim_getBrightness(index);

                if (90 >= brightness)
                {
                    brightness += VSCP_ACTION_DIM_STEP;
                }
                else
                {
                    brightness = 100;
                }
            }
            else
            {
                brightness = msg->data[0];
            }

            lamp_sim_setBrightness(index, brightness);

            /* If the lamp changed from off to on or vice versa, a corresponding
             * event will be sent.
             */
            newState = lamp_sim_getState(index);

            if (oldState != newState)
            {
                /* Send lamp state */
                if (FALSE == newState)
                {
                    (void)vscp_evt_information_sendOn(index, 255, 255);
                }
                else
                {
                    (void)vscp_evt_information_sendOff(index, 255, 255);
                }

                /* Show the user the current lamp state */
                platform_setTextColor(PLATFORM_COLOR_LIGHT_RED);
                if (FALSE == lamp_sim_getState(index))
                {
                    log_printf("Lamp %u is off.\n", index + 1);
                }
                else
                {
                    log_printf("Lamp %u is %u%% on.\n", index + 1, lamp_sim_getBrightness(index));
                }
                platform_restoreTextColor();
            }

            /* Set dirty flag */
            anyChange = TRUE;
        }
    }

    if (TRUE == anyChange)
    {
        /* Show all lamps */
        printf("\n");
        lamp_sim_show();
        printf("\n");
    }

    return;
}

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) || VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION ) */
