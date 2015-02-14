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
@brief  VSCP actions
@file   vscp_action.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_action.h

@section svn Subversion
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_action.h"

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) || VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_NEXT_GENERATION )

#include <stdio.h>
#include "platform.h"
#include "log.h"
#include "vscp_information.h"
#include "lamp_sim.h"

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
 */
extern void vscp_action_execute(uint8_t action, uint8_t par)
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
        
    default:
        platform_setTextColor(PLATFORM_COLOR_LIGHT_RED);
        log_printf("Unknown action %u with parameter 0x%02X.\n", action, par);
        platform_setTextColor(PLATFORM_COLOR_GREY);
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
                    (void)vscp_information_sendOnEvent(index, 255, 255);
                }
                else
                {                   
                    (void)vscp_information_sendOffEvent(index, 255, 255);
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
                platform_setTextColor(PLATFORM_COLOR_GREY);
                
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
 * @param[in]   par         Parameter, which contains the instance number
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
                (void)vscp_information_sendOnEvent(index, 255, 255);
            }
            else
            {                   
                (void)vscp_information_sendOffEvent(index, 255, 255);
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
            platform_setTextColor(PLATFORM_COLOR_GREY);
            
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
