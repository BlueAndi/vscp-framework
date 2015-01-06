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
@brief  Wind speed observer
@file   windObserver.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see windObserver.h

@section svn Subversion
$Author: amerkle $
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "windObserver.h"
#include <stddef.h>
#include "windDrv.h"
#include "vscp_weather.h"
#include "vscp_ps_user.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Minimum time the wind speed has to be stable in one state, before the state
 * changes.
 */
#define WINDOBSERVER_MIN_STABLE_TIME    2

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the different wind speed states. */
typedef enum
{
    WINDOBSERVER_STATE_INIT = 0,    /**< Initial state */
    WINDOBSERVER_STATE_LOW,         /**< Wind speed low */
    WINDOBSERVER_STATE_MEDIUM,      /**< Wind speed medium */
    WINDOBSERVER_STATE_HIGH,        /**< Wind speed high */
    WINDOBSERVER_STATE_VERY_HIGH    /**< Wind speed very high */
            
} WINDOBSERVER_STATE;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Current wind state */
static WINDOBSERVER_STATE   windObserver_state      = WINDOBSERVER_STATE_INIT;

/** Counter is used to determine the minimum stable wind speed. */
static uint8_t              windObserver_stableCnt  = 0;

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
extern void windObserver_init(void)
{
    /* Nothing to do */
    return;
}

/**
 * This function process the wind speed. Call it every second.
 */
extern void windObserver_process(void)
{
    uint8_t windSpeed   = windDrv_getWindSpeed();

    /* Wind measurement enabled? */
    if (1 == vscp_ps_user_readWindEnable())
    {
        /* Wind very high? */
        if (vscp_ps_user_readWindSpeedVeryHigh() < windSpeed)
        {
            if (WINDOBSERVER_STATE_VERY_HIGH != windObserver_state)
            {
                vscp_weather_sendVeryHighWindEvent(0, vscp_ps_user_readWindEventZone(), vscp_ps_user_readWindEventSubZone());
            }

            windObserver_state = WINDOBSERVER_STATE_VERY_HIGH;
        }
        /* Wind high? */
        else if (vscp_ps_user_readWindSpeedHigh() < windSpeed)
        {
            if (WINDOBSERVER_STATE_HIGH != windObserver_state)
            {
                vscp_weather_sendHighWindEvent(0, vscp_ps_user_readWindEventZone(), vscp_ps_user_readWindEventSubZone());
            }

            windObserver_state = WINDOBSERVER_STATE_HIGH;
        }
        /* Wind medium? */
        else if (vscp_ps_user_readWindSpeedMedium() < windSpeed)
        {
            if (WINDOBSERVER_STATE_MEDIUM != windObserver_state)
            {
                vscp_weather_sendMediumWindEvent(0, vscp_ps_user_readWindEventZone(), vscp_ps_user_readWindEventSubZone());
            }

            windObserver_state = WINDOBSERVER_STATE_MEDIUM;
        }
        /* Wind low */
        else
        {
            if (WINDOBSERVER_STATE_LOW != windObserver_state)
            {
                vscp_weather_sendLowWindEvent(0, vscp_ps_user_readWindEventZone(), vscp_ps_user_readWindEventSubZone());
            }

            windObserver_state = WINDOBSERVER_STATE_LOW;
        }
    }
    else
    {
        windObserver_state      = WINDOBSERVER_STATE_INIT;
        windObserver_stableCnt  = 0;
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
