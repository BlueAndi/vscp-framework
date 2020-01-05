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
@brief  This module contains the shutter control.
@file   shutterDrv.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see shutterDrv.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "shutterDrv.h"
#include "relay.h"
#include <string.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/**
 * Shutter process cycle period in ms.
 */
#define SHUTTERDRV_PROCESS_PERIOD               10

/**
 * Wait cycles for transition SHUTTERDRV_STATE_DIR to SHUTTERDRV_STATE_POWER.
 */
#define SHUTTERDRV_WAIT_CYCLES_TO_POWER         (100 / SHUTTERDRV_PROCESS_PERIOD)

/**
 * Wait cycles for transition SHUTTERDRV_STATE_REMOVE_POWER to SHUTTERDRV_STATE_REMOVE_DIR.
 */
#define SHUTTERDRV_WAIT_CYCLES_TO_REMOVE_DIR    (100 / SHUTTERDRV_PROCESS_PERIOD)

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the different shutter states. */
typedef enum
{
    SHUTTERDRV_STATE_STOPPED = 0,   /**< Shutter is stopped */
    SHUTTERDRV_STATE_DIR,           /**< Shutter direction will be set: phase 1 */
    SHUTTERDRV_STATE_POWER,         /**< Shutter will be powered: phase 2 */
    SHUTTERDRV_STATE_DRIVE,         /**< Shutter drives */
    SHUTTERDRV_STATE_REMOVE_POWER,  /**< Shutter will be stopped */
    SHUTTERDRV_STATE_REMOVE_DIR     /**< Relay will fall back to power-off state. */

} SHUTTERDRV_STATE;

/** This type defines the parameter of a single shutter. */
typedef struct
{
    /* Configuration parameters */
    BOOL                isEnabled;  /**< TRUE: Shutter is enabled otherwise disabled. */
    uint8_t             relayPower; /**< 1. Relay: Used for powering */
    uint8_t             relayDir;   /**< 2. Relay: Used for up/down */
    shutterDrv_CallBack cb;         /**< User callback */
    void                *userData;  /**< User callback data */

    /* Operative parameters */
    SHUTTERDRV_STATE    state;          /**< Current state of the shutter */
    SHUTTERDRV_DIR      direction;      /**< Current direction of the shutter */
    SHUTTERDRV_DIR      nextDirection;  /**< Next direction the shutter shall drive to */
    uint8_t             waitCycles;     /**< Wait cycles for state machine transitions */

} shutterDrv_Context;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static void shutterDrv_stateMachine(uint8_t nr);

/*******************************************************************************
    LOCALE VARIABLES
*******************************************************************************/

/** All shutter contexts */
static shutterDrv_Context   shutterDrv_context[SHUTTERDRV_NUM];

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
extern void shutterDrv_init(void)
{
    /* Initialize all shutter contexts */
    memset(shutterDrv_context, 0, sizeof(shutterDrv_context));

    return;
}

/**
 * This function combines the relays to the shutter and set the maximum
 * duration times which the shutter needs to drive from top to bottom and
 * vice versa.
 *
 * @param[in]   nr          Shutter instance number
 * @param[in]   relayPower  Relay instance number. Relay used for powering.
 * @param[in]   relayDir    Relay instance number. Relay used for direction.
 * @param[in]   cb          Callback
 * @param[in]   userData    User specific data which comes via callback
 * @return Status
 */
extern SHUTTERDRV_RET   shutterDrv_configure(uint8_t nr, uint8_t relayPower, uint8_t relayDir, shutterDrv_CallBack cb, void * const userData)
{
    shutterDrv_Context  *con    = NULL;

    /* Invalid shutter instance or
     * invalid relay usage?
     */
    if ((SHUTTERDRV_NUM <= nr) ||
        (relayPower == relayDir))
    {
        return SHUTTERDRV_RET_EPAR;
    }

    con = &shutterDrv_context[nr];

    con->relayPower = relayPower;
    con->relayDir   = relayDir;
    con->cb         = cb;
    con->userData   = userData;

    return SHUTTERDRV_RET_OK;
}

/**
 * This function enables or disables a shutter instance. If the shutter
 * instance shall be disabled and it is driving at the moment, it will be
 * automatically stopped.
 *
 * @param[in]   nr          Shutter instance number
 * @param[in]   enableIt    TRUE: Enable it FALSE: Disable it
 */
extern void shutterDrv_enable(uint8_t nr, BOOL enableIt)
{
    shutterDrv_Context  *con    = NULL;

    /* Invalid shutter instance? */
    if (SHUTTERDRV_NUM <= nr)
    {
        return;
    }

    con = &shutterDrv_context[nr];

    /* If the shutter shall be disabled and it is driving at the
     * moment, it will be stopped now.
     */
    if (FALSE == enableIt)
    {
        shutterDrv_drive(nr, SHUTTERDRV_DIR_STOP);
    }

    con->isEnabled = enableIt;

    return;
}

/**
 * This function drives the shutter to the given direction.
 *
 * @param[in]   nr          Shutter instance number
 * @param[in]   direction   Shutter drive direction
 */
extern void shutterDrv_drive(uint8_t nr, SHUTTERDRV_DIR direction)
{
    shutterDrv_Context  *con    = NULL;

    /* Invalid shutter instance? */
    if (SHUTTERDRV_NUM <= nr)
    {
        return;
    }

    con = &shutterDrv_context[nr];

    /* Not configured? */
    if (con->relayPower == con->relayDir)
    {
        return;
    }

    /* Shutter disabled? */
    if (FALSE == con->isEnabled)
    {
        return;
    }

    con->nextDirection = direction;

    return;
}

/**
 * Get current drive direction of the shutter.
 *
 * @param[in]   nr  Shutter instance number
 * @return  Drive direction
 */
extern SHUTTERDRV_DIR   shutterDrv_getDriveDirection(uint8_t nr)
{
    shutterDrv_Context  *con    = NULL;

    /* Invalid shutter instance? */
    if (SHUTTERDRV_NUM <= nr)
    {
        return SHUTTERDRV_DIR_STOP;
    }

    con = &shutterDrv_context[nr];

    if (SHUTTERDRV_DIR_STOP == con->direction)
    {
        if (SHUTTERDRV_DIR_STOP != con->nextDirection)
        {
            return con->nextDirection;
        }
    }

    return con->direction;
}

/**
 * This function process all shutters. Call it in a 10 ms period.
 *
 * @return Idle state
 * @retval FALSE    Any shutter is driving
 * @retval TRUE     Idle, no shutter is driving
 */
extern BOOL shutterDrv_process(void)
{
    uint8_t index   = 0;
    uint8_t idle    = 0;
    BOOL    isIdle  = FALSE;

    /* Run state machine for every shutter. */
    for(index = 0; index < SHUTTERDRV_NUM; ++index)
    {
        shutterDrv_Context  * const con = &shutterDrv_context[index];

        shutterDrv_stateMachine(index);

        /* Shutter stopped and no new drive command is pending? */
        if ((SHUTTERDRV_STATE_STOPPED == con->state) &&
            (SHUTTERDRV_DIR_STOP == con->direction))
        {
            ++idle;
        }
    }

    /* If all shutter are stopped and no new drive command is pending,
     * the task will be suspended.
     */
    if (SHUTTERDRV_NUM == idle)
    {
        isIdle = TRUE;
    }

    return isIdle;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * Shutter state machine.
 *
 * @param[in]   nr  Instance number
 */
static void shutterDrv_stateMachine(uint8_t nr)
{
    shutterDrv_Context* con             = NULL;
    BOOL                fastTransition  = FALSE;    /* Use it for a fast state transition */

    /* Invalid shutter instance */
    if (SHUTTERDRV_NUM <= nr)
    {
        return;
    }

    con = &shutterDrv_context[nr];

    do
    {
        fastTransition = FALSE;

        /* Process current state */
        switch(con->state)
        {
        case SHUTTERDRV_STATE_STOPPED:

            /* Any new drive command? */
            if (con->direction != con->nextDirection)
            {
                /* Set direction */
                con->direction  = con->nextDirection;

                if (SHUTTERDRV_DIR_STOP != con->direction)
                {
                    con->state      = SHUTTERDRV_STATE_DIR;
                    con->waitCycles = 0;

                    fastTransition = TRUE;
                }
            }
            break;

        /* Set the direction */
        case SHUTTERDRV_STATE_DIR:

            /* Any new drive command? */
            if (con->direction != con->nextDirection)
            {
                /* Abort, go and get the new parameters. */
                if (0 < con->waitCycles)
                {
                    con->state = SHUTTERDRV_STATE_REMOVE_DIR;
                }
                else
                {
                    con->state = SHUTTERDRV_STATE_STOPPED;
                }

                fastTransition = TRUE;
            }
            else
            /* First time? */
            if (0 == con->waitCycles)
            {
                /* Which direction? */
                if (SHUTTERDRV_DIR_UP == con->direction)
                {
                    relay_activate(con->relayDir, FALSE);
                }
                else
                if (SHUTTERDRV_DIR_DOWN == con->direction)
                {
                    relay_activate(con->relayDir, TRUE);
                }
                else
                /* Error */
                {
                    /* For safety reason, remove power. */
                    con->direction  = SHUTTERDRV_DIR_STOP;
                    con->state      = SHUTTERDRV_STATE_REMOVE_POWER;

                    fastTransition = TRUE;
                }
            }

            /* Process wait cycles */
            if (SHUTTERDRV_WAIT_CYCLES_TO_POWER <= con->waitCycles)
            {
                con->waitCycles = 0;
                con->state      = SHUTTERDRV_STATE_POWER;

                fastTransition = TRUE;
            }
            else
            {
                con->waitCycles++;
            }
            break;

        case SHUTTERDRV_STATE_POWER:

            /* Any new drive command? */
            if (con->direction != con->nextDirection)
            {
                /* Abort, go get the new parameters. */
                con->state = SHUTTERDRV_STATE_REMOVE_DIR;

                fastTransition = TRUE;
            }
            else
            {
                /* Power it */
                relay_activate(con->relayPower, TRUE);

                con->state = SHUTTERDRV_STATE_DRIVE;

                /* Call callback */
                if (NULL != con->cb)
                {
                    con->cb(nr, con->direction, TRUE, con->userData);
                }
            }
            break;

        case SHUTTERDRV_STATE_DRIVE:

            /* Any new drive command? */
            if (con->direction != con->nextDirection)
            {
                /* Stop and get the new parameters. */
                con->state      = SHUTTERDRV_STATE_REMOVE_POWER;
                con->waitCycles = 0;

                fastTransition = TRUE;
            }
            break;

        case SHUTTERDRV_STATE_REMOVE_POWER:

            /* First time? */
            if (0 == con->waitCycles)
            {
                /* Remove power */
                relay_activate(con->relayPower, FALSE);

                /* Call callback */
                if (NULL != con->cb)
                {
                    con->cb(nr, con->direction, FALSE, con->userData);
                }
            }

            /* Process wait cycles */
            if (SHUTTERDRV_WAIT_CYCLES_TO_REMOVE_DIR <= con->waitCycles)
            {
                con->waitCycles = 0;
                con->state      = SHUTTERDRV_STATE_REMOVE_DIR;

                fastTransition = TRUE;
            }
            else
            {
                con->waitCycles++;
            }
            break;

        case SHUTTERDRV_STATE_REMOVE_DIR:
            /* Remove direction and let the relay fall back to the low
             * current state.
             */
            relay_activate(con->relayDir, FALSE);
            con->state = SHUTTERDRV_STATE_STOPPED;

            fastTransition = TRUE;
            break;

        default:
            break;
        }
    }
    while(TRUE == fastTransition);

    return;
}

