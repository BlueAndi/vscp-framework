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
@brief  Lamp simulation
@file   lamp_sim.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see lamp_sim.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "lamp_sim.h"
#include <memory.h>
#include <stdio.h>
#include "platform.h"

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

/** This type defines the data of a single simulated lamp. */
typedef struct
{
    BOOL    enabled;    /**< Enabled or not */
    uint8_t brightness; /**< Brightness in percent */

} lamp_sim_Lamp;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Simulated lamps */
static lamp_sim_Lamp    lamp_sim_context[LAMP_SIM_NUM];

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the module.
 */
extern void lamp_sim_init(void)
{
    /* Initialize simulated lamp data */
    memset(lamp_sim_context, 0, sizeof(lamp_sim_context));

    return;
}

/**
 * This function enables or disables a lamp.
 *
 * @param[in]   index       Lamp index [0-7]
 * @param[in]   enableIt    Enable or disable lamp
 */
extern void lamp_sim_setState(uint8_t index, BOOL enableIt)
{
    if (LAMP_SIM_NUM > index)
    {
        lamp_sim_context[index].enabled = enableIt;

        if (0 == lamp_sim_context[index].brightness)
        {
            lamp_sim_context[index].brightness = 100;
        }
    }

    return;
}

/**
 * This function returns the lamp state.
 *
 * @param[in]   index       Lamp index [0-7]
 *
 * @return Lamp state
 * @retval FALSE    Lamp off
 * @retval TRUE     Lamp on
 */
extern BOOL lamp_sim_getState(uint8_t index)
{
    BOOL    state   = FALSE;

    if (LAMP_SIM_NUM > index)
    {
        state = lamp_sim_context[index].enabled;
    }

    return state;
}

/**
 * This function set the brightness of a lamp.
 *
 * @param[in]   index       Lamp index [0-7]
 * @param[in]   percentage  Brightness [0-100]
 */
extern void lamp_sim_setBrightness(uint8_t index, uint8_t brightness)
{
    if ((LAMP_SIM_NUM > index) &&
        (100 >= brightness))
    {
        lamp_sim_context[index].brightness = brightness;

        if (0 == lamp_sim_context[index].brightness)
        {
            lamp_sim_context[index].enabled = FALSE;
        }
        else if (0 < lamp_sim_context[index].brightness)
        {
            lamp_sim_context[index].enabled = TRUE;
        }
    }

    return;
}

/**
 * This function returns the lamp brightness.
 *
 * @param[in]   index       Lamp index [0-7]
 *
 * @return Lamp brightness in percent [0-100]
 */
extern uint8_t lamp_sim_getBrightness(uint8_t index)
{
    uint8_t brightness  = 0;

    if (LAMP_SIM_NUM > index)
    {
        brightness = lamp_sim_context[index].brightness;
    }

    return brightness;
}

/**
 * This function shows the status of every simulated lamp to the user.
 */
extern void lamp_sim_show(void)
{
    uint8_t index   = 0;

    printf("Lamps\n");

    for(index = 0; index < LAMP_SIM_NUM; ++index)
    {
        uint8_t brightness  = lamp_sim_context[index].brightness;

        if (FALSE == lamp_sim_context[index].enabled)
        {
            brightness = 0;
        }

        /* Lamp off? */
        if (0 == brightness)
        {
            platform_restoreTextColor();
            platform_restoreTextBgColor();
        }
        /* Lamp on with brightness lower than 50% ? */
        else if (50 > brightness)
        {
            platform_setTextColor(PLATFORM_COLOR_BLACK);
            platform_setTextBgColor(PLATFORM_COLOR_YELLOW);
        }
        /* Lamp on with brightness greater or equal than 50% */
        else
        {
            platform_setTextColor(PLATFORM_COLOR_BLACK);
            platform_setTextBgColor(PLATFORM_COLOR_LIGHT_YELLOW);
        }

        printf("   %u  ", index + 1);
    }

    /* Reset colors */
    platform_restoreTextColor();
    platform_restoreTextBgColor();

    printf("\n");

    for(index = 0; index < LAMP_SIM_NUM; ++index)
    {
        uint8_t brightness  = lamp_sim_context[index].brightness;

        if (FALSE == lamp_sim_context[index].enabled)
        {
            brightness = 0;
        }

        /* Lamp off? */
        if (0 == brightness)
        {
            platform_restoreTextColor();
            platform_restoreTextBgColor();
        }
        /* Lamp on with brightness lower than 50% ? */
        else if (50 > brightness)
        {
            platform_setTextColor(PLATFORM_COLOR_BLACK);
            platform_setTextBgColor(PLATFORM_COLOR_YELLOW);
        }
        /* Lamp on with brightness greater or equal than 50% */
        else
        {
            platform_setTextColor(PLATFORM_COLOR_BLACK);
            platform_setTextBgColor(PLATFORM_COLOR_LIGHT_YELLOW);
        }

        printf(" %03u%% ", brightness);
    }

    /* Reset colors */
    platform_restoreTextColor();
    platform_restoreTextBgColor();

    printf("\n");

    return;
}


/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
