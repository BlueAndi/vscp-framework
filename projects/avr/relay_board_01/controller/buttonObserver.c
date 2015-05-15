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
@brief  Button observer
@file   buttonObserver.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see buttonObserver.h

@section svn Subversion
$Author: amerkle $
$Rev: 401 $
$Date: 2014-12-27 23:42:44 +0100 (Sa, 27 Dez 2014) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "buttonObserver.h"
#include <stddef.h>
#include <string.h>
#include "pushButton.h"
#include "vscp_ps_user.h"
#include "vscp_information.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Number of buttons */
#define BUTTONOBSERVER_NUM          8

/** Default max. on time (button is pressed) in cycle per digit */
#define BUTTONOBSERVER_MAX_ON_TIME  40

/** Default max. off time (button is released) in cycle per digit */
#define BUTTONOBSERVER_MAX_OFF_TIME 40

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type contains the parameter about a single pushbutton. */
typedef struct
{
    uint8_t                 maxOnTime;  /**< Max. on time (pressed) for pulse detection */
    uint8_t                 maxOffTime; /**< Max. off time (released) for pulse detection */
    BOOL                    isActive;   /**< A button is processed only in case of it is active. */
    BUTTONOBSERVER_STATE    state;      /**< Current button state (pressed/released) */
    uint8_t                 onOffTime;  /**< Current on/off time counter in 10ms per digit */
    uint8_t                 numPulse;   /**< Current number of detected pulse */

} buttonObserver_Button;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static void buttonObserver_processButton(buttonObserver_Button * const button, BUTTONOBSERVER_STATE state);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Context of all buttons */
static buttonObserver_Button    buttonObserver_buttons[BUTTONOBSERVER_NUM];

/** Previous button status */
static uint8_t                  buttonObserver_status   = 0;

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
extern void buttonObserver_init(void)
{
    uint8_t index   = 0;

    memset(buttonObserver_buttons, 0, sizeof(buttonObserver_buttons));

    /* Set default maximum times */
    for(index = 0; index < BUTTONOBSERVER_NUM; ++index)
    {
        buttonObserver_Button   *button = &buttonObserver_buttons[index];

        button->state       = BUTTONOBSERVER_STATE_RELEASED;
        button->maxOffTime  = BUTTONOBSERVER_MAX_OFF_TIME;
        button->maxOnTime   = BUTTONOBSERVER_MAX_ON_TIME;
    }

    /* Get current button status */
    buttonObserver_status = pushButton_readStatus();

    return;
}

/**
 * This function set a filter to disable some button observations.
 * The filter is configured bitwise. Bit 0 corresponds to button 1.
 * A 0 means, no observation.
 *
 * @param[in]   filter  Observation filter
 */
extern void buttonObserver_setFilter(uint8_t filter)
{
    uint8_t index   = 0;

    for(index = 0; index < BUTTONOBSERVER_NUM; ++index)
    {
        buttonObserver_Button   *button = &buttonObserver_buttons[index];

        /* Disable the button? */
        if (FALSE == IS_BIT_SET(filter, index))
        {
            button->state = BUTTONOBSERVER_STATE_DISABLED;
        }
        /* Enable the button */
        else
        {
            button->state = BUTTONOBSERVER_STATE_RELEASED;
        }
    }

    return;
}

/**
 * This function process the pushbuttons. Call it every 10ms.
 */
extern void buttonObserver_process(void)
{
    uint8_t index   = 0;
    uint8_t stable  = 0;
    uint8_t status  = pushButton_readStatus();

    /* Which buttons are stable?
     * Example:
     * status     = 1 <-- Current state
     * old status = 0 <-- Previous state
     * stable = 1 ^ 0 = 1 --> State not stable
     *
     * status     = 1
     * old status = 1
     * stable = 1 ^ 1 = 0 --> State is stable
     *
     * etc.
     *
     */
    stable = status ^ buttonObserver_status;

    /* Remember the current status for debouncing. */
    buttonObserver_status = status;

    /* Process all buttons */
    for(index = 0; index < BUTTONOBSERVER_NUM; ++index)
    {
        buttonObserver_Button   *button = &buttonObserver_buttons[index];

        /* Button enabled? */
        if (BUTTONOBSERVER_STATE_DISABLED != button->state)
        {
            /* Is the state of this button stable? */
            if (FALSE == IS_BIT_SET(stable, index))
            {
                /* Is the button released? */
                if (FALSE == IS_BIT_SET(status, index))
                {
                    buttonObserver_processButton(button, BUTTONOBSERVER_STATE_RELEASED);
                }
                else
                /* Button is pressed. */
                {
                    buttonObserver_processButton(button, BUTTONOBSERVER_STATE_PRESSED);
                }
            }
            /* Button state is not stable. Call it with the previous state. */
            else
            {
                buttonObserver_processButton(button, button->state);
            }
        }
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function process all button state changes.
 *
 * @param[in]   button  Button context
 * @param[in]   state   Current state
 */
static void buttonObserver_processButton(buttonObserver_Button * const button, BUTTONOBSERVER_STATE state)
{
    BOOL    notify      = FALSE;
    uint8_t numPulse    = 0;

    if (NULL == button)
    {
        return;
    }

    /* Button state changed? */
    if (state != button->state)
    {
        /* Reset on/off time counter */
        button->onOffTime = 0;

        /* Changed to released? */
        if (BUTTONOBSERVER_STATE_RELEASED == state)
        {
            /* If max. off time is not exceed, increase the number of detected pulse. */
            if (TRUE == button->isActive)
            {
                ++button->numPulse;
            }
            else
            /* If max. on time is exceed, notify the off state of the button immediately. */
            {
                notify      = TRUE;
                numPulse    = 0;
            }
        }

        /* Track the time? */
        if (FALSE == notify)
        {
            /* Yes */
            button->isActive = TRUE;
        }

        /* Save current state */
        button->state = state;
    }
    else
    /* Active? */
    if (TRUE == button->isActive)
    {
        /* Increase time counter */
        ++button->onOffTime;

        /* Verify the on time? */
        if (BUTTONOBSERVER_STATE_PRESSED == button->state)
        {
            /* Greater than the max. on time? */
            if (button->maxOnTime < button->onOffTime)
            {
                /* Notify */
                notify      = TRUE;
                numPulse    = button->numPulse;

                /* No further processing */
                button->isActive = FALSE;

                /* Reset */
                button->numPulse = 0;
            }
        }
        else
        /* Verify off time */
        {
            /* Greater than the max. off time? */
            if (button->maxOffTime < button->onOffTime)
            {
                /* Inform application */
                notify      = TRUE;
                numPulse    = button->numPulse;

                /* No further processing */
                button->isActive = FALSE;

                /* Reset */
                button->numPulse = 0;
            }
        }
    }

    /* Notify? */
    if (TRUE == notify)
    {
        uint8_t buttonIndex         = (uint8_t)(button - buttonObserver_buttons);   /* Calculate button index */
        uint8_t eventButtonState    = 0;

        /* Any button repeats? */
        if (0 < numPulse)
        {
            eventButtonState = 2;

            /* Limit number of repeats, caused by the VSCP event limitation. */
            if (32 < numPulse)
            {
                numPulse = 32;
            }

            eventButtonState |= (numPulse << 3);
        }
        else if (BUTTONOBSERVER_STATE_RELEASED == button->state)
        {
            eventButtonState = 0;
        }
        else if (BUTTONOBSERVER_STATE_PRESSED == button->state)
        {
            eventButtonState = 1;
        }

        (void)vscp_information_sendButtonEvent( eventButtonState,
                                                vscp_ps_user_readButtonEventZone(buttonIndex),
                                                vscp_ps_user_readButtonEventSubZone(buttonIndex),
                                                buttonIndex,
                                                NULL);
    }

    return;
}
