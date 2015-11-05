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
@brief  This module controls the shutter.
@file   shutter.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see shutter.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "shutter.h"
#include "shutterDrv.h"
#include <string.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/**
 * Time in 0.1s which will be added for any drive to top or bottom.
 * This is for balancing the drive tolerances.
 */
#define SHUTTER_BALANCE_TIME    10

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the shutter states. */
typedef enum
{
    SHUTTER_STATE_NORMAL = 0,       /**< Normal state */
    SHUTTER_STATE_BLOCKED,          /**< Shutter is blocked, because of wind alert. */
    SHUTTER_STATE_CALIB_PREPARE,    /**< Calibration state, shutter drives up, until any other drive command */
    SHUTTER_STATE_CALIB_DRIVE_DOWN, /**< Calibration state, shutter drives down and count the time, until any other drive command */
    SHUTTER_STATE_CALIB_DRIVE_UP    /**< Calibration state, shutter drives up and count the time, until any other drive command */

} SHUTTER_STATE;

/** This type defines the different position status. */
typedef enum
{
    SHUTTER_POS_STATUS_INVALID = 0,     /**< Position is invalid (unknown) */
    SHUTTER_POS_STATUS_VALID_TOP,       /**< Position is valid if shutter drives to the top. */
    SHUTTER_POS_STATUS_VALID_BOTTOM     /**< Position is valid if shutter drives to the bottom. */

} SHUTTER_POS_STATUS;

/** This type defines a timer. */
typedef struct
{
    BOOL                isEnabled;  /**< Timer enabled or disabled */
    volatile uint16_t   run;        /**< Timer value in 0.1s which is increased. Counts how long the shutter really drives. */
    volatile uint16_t   duration;   /**< Drive duration timer value in 0.1s, which is decreased. Countdown until the shutter stops. */

} shutter_Timer;

/** This type defines the parameter of a single shutter. */
typedef struct
{
    /* Configuration parameters */
    BOOL                isEnabled;  /**< TRUE: Configuration is enabled */
    BOOL                isUpdated;  /**< TRUE: Configuration updated by calibration drive */
    uint16_t            maxUp;      /**< Max. time which the shutter needs to drive from bottom to the top */
    uint16_t            maxDown;    /**< Max. time which the shutter needs to drive from the top to the bottom */
    uint16_t            maxTurn;    /**< Max. time which the shutter needs to turn 90°. Only for jalousie! */

    /* Operative parameters */
    SHUTTER_STATE       state;      /**< Shutter state machine state */
    shutter_Timer       timer;      /**< Shutter drive timer */

    /* Calibration parameters */
    SHUTTER_POS_STATUS  posStatus;  /**< Position status */
    uint16_t            pos;        /**< Current shutter position (only estimated) in 0.1s. 0 means shutter is at the top. */

} shutter_Context;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static void shutter_driveCb(uint8_t nr, SHUTTERDRV_DIR direction, BOOL isDriving, void * const userData);
static uint16_t shutter_getTimeToTop(shutter_Context const * const con);
static uint16_t shutter_getTimeToBottom(shutter_Context const * const con);
static uint16_t shutter_calcPos(uint16_t pos, SHUTTER_POS_STATUS status, SHUTTER_POS_STATUS newStatus, uint16_t maxUp, uint16_t maxDown);

/*******************************************************************************
    LOCALE VARIABLES
*******************************************************************************/

/** All shutter contexts */
static shutter_Context  shutter_context[SHUTTER_NUM];

/** Callback for any kind of shutter action */
static shutter_CallBack shutter_infoCb          = NULL;

/** User data used by the info callback */
static void*            shutter_infoCbUserData  = NULL;

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
extern void shutter_init(void)
{
    /* Initialize all shutter contexts */
    memset(shutter_context, 0, sizeof(shutter_context));

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
 * @param[in]   maxUp       Max. up-time (drive time from bottom to top) in 0.1s
 * @param[in]   maxDown     Max. down-time (drive time from top to bottom) in 0.1s
 * @param[in]   maxTurn     Max. turn-time (turn 90°) in 0.1s
 */
extern void shutter_configure(uint8_t nr, uint8_t relayPower, uint8_t relayDir, uint16_t maxUp, uint16_t maxDown, uint16_t maxTurn)
{
    shutter_Context *con    = NULL;

    /* Invalid shutter instance? */
    if (SHUTTER_NUM <= nr)
    {
        return;
    }

    con = &shutter_context[nr];

    if (SHUTTERDRV_RET_OK != shutterDrv_configure(nr, relayPower, relayDir, shutter_driveCb, con))
    {
        return;
    }

    con->maxUp      = maxUp;
    con->maxDown    = maxDown;
    con->maxTurn    = maxTurn;

    return;
}

/**
 * This function registers a callback, which will be informed for any kind of shutter action.
 *
 * @param[in]   cb          Callback
 * @param[in]   userData    User specific data which will get back via callback
 */
extern void shutter_register(shutter_CallBack cb, void* userData)
{
    shutter_infoCb          = cb;
    shutter_infoCbUserData  = userData;
    return;
}

/**
 * This function enables or disables a shutter instance. If the shutter
 * instance shall be disabled and it is driving at the moment, it will be
 * automatically stopped.
 *
 * @param[in]   nr          Shutter instance number
 * @param[in]   enableIt    TRUE: Enable it FALSE: Disable it
 */
extern void shutter_enable(uint8_t nr, BOOL enableIt)
{
    shutter_Context *con    = NULL;

    /* Invalid shutter instance? */
    if (SHUTTER_NUM <= nr)
    {
        return;
    }

    con = &shutter_context[nr];

    shutterDrv_enable(nr, enableIt);
    con->isEnabled = enableIt;

    return;
}

/**
 * This functions returns the shutter enable status.
 *
 * @param[in]   nr  Shutter instance number
 * @return Enabled or not
 * @retval FALSE    Shutter is disabled
 * @retval TRUE     Shutter is enabled
 */
extern BOOL shutter_isEnabled(uint8_t nr)
{
    shutter_Context *con    = NULL;

    /* Invalid shutter instance? */
    if (SHUTTER_NUM <= nr)
    {
        return FALSE;
    }

    con = &shutter_context[nr];

    return con->isEnabled;
}

/**
 * This function drives the shutter up or down. Additional to the up/down
 * command, a time duration can be specified how long the shutter shall drive
 * in this direction. If top or bottom is given as direction, duration is not
 * used.
 *
 * @param[in]   nr          Shutter instance number
 * @param[in]   dir         Shutter drive direction
 * @param[in]   duration    Drive duration (0: infinite) in 0.1s
 */
extern void shutter_drive(uint8_t nr, SHUTTER_DIR dir, uint16_t duration)
{
    shutter_Context *con            = NULL;
    SHUTTERDRV_DIR  ctrlDirection   = SHUTTERDRV_DIR_STOP;

    /* Invalid shutter instance number? */
    if (SHUTTER_NUM <= nr)
    {
        return;
    }

    con = &shutter_context[nr];

    /* If the shutter instance is disabled, we will return now. */
    if (FALSE == con->isEnabled)
    {
        return;
    }

    /* Handle state machine */
    if (SHUTTER_STATE_NORMAL == con->state)
    {
        switch(dir)
        {
        case SHUTTER_DIR_STOP:
            ctrlDirection = SHUTTERDRV_DIR_STOP;
            break;

        case SHUTTER_DIR_UP:
            ctrlDirection = SHUTTERDRV_DIR_UP;
            break;

        case SHUTTER_DIR_DOWN:
            ctrlDirection = SHUTTERDRV_DIR_DOWN;
            break;

        case SHUTTER_DIR_TOP:
            /* How many time is needed to be at the top? */
            duration = shutter_getTimeToTop(con);

            /* Is the shutter already at the top? */
            if (0 == duration)
            {
                ctrlDirection = SHUTTERDRV_DIR_STOP;
            }
            else
            {
                /* Add balance time to compensate tolerances. */
                if ((0xffff - duration) < SHUTTER_BALANCE_TIME)
                {
                    duration = 0xffff;
                }
                else
                {
                    duration += SHUTTER_BALANCE_TIME;
                }

                ctrlDirection = SHUTTERDRV_DIR_UP;
            }
            break;

        case SHUTTER_DIR_BOTTOM:
            /* How many time is needed to be at the bottom? */
            duration = shutter_getTimeToBottom(con);

            /* Is the shutter already at the bottom? */
            if (0 == duration)
            {
                ctrlDirection = SHUTTERDRV_DIR_STOP;
            }
            else
            {
                /* Add balance time to compensate tolerances. */
                if ((0xffff - duration) < SHUTTER_BALANCE_TIME)
                {
                    duration = 0xffff;
                }
                else
                {
                    duration += SHUTTER_BALANCE_TIME;
                }

                ctrlDirection = SHUTTERDRV_DIR_DOWN;
            }
            break;

        case SHUTTER_DIR_CALIBRATE:
            /* If shutter is driving, stop it now. */
            if (SHUTTERDRV_DIR_STOP != shutterDrv_getDriveDirection(nr))
            {
                ctrlDirection = SHUTTERDRV_DIR_STOP;
            }
            else
            {
                ctrlDirection   = SHUTTERDRV_DIR_UP;
                con->state      = SHUTTER_STATE_CALIB_PREPARE;

                /* Invalidate the position status */
                con->posStatus  = SHUTTER_POS_STATUS_INVALID;
            }
            break;

        default:
            /* Shall never happen */
            ctrlDirection = SHUTTERDRV_DIR_STOP;
            break;
        }

        /* If a drive command is already running and the new drive
         * command goes not into the opposite direction, the shutter
         * will be stopped.
         */
        if (SHUTTERDRV_DIR_UP == shutterDrv_getDriveDirection(nr))
        {
            if (SHUTTERDRV_DIR_DOWN != ctrlDirection)
            {
                /* Stop shutter */
                ctrlDirection   = SHUTTERDRV_DIR_STOP;
                duration        = 0;
            }
        }
        else
        if (SHUTTERDRV_DIR_DOWN == shutterDrv_getDriveDirection(nr))
        {
            if (SHUTTERDRV_DIR_UP != ctrlDirection)
            {
                /* Stop shutter */
                ctrlDirection   = SHUTTERDRV_DIR_STOP;
                duration        = 0;
            }
        }
    }
    else if (SHUTTER_STATE_BLOCKED == con->state)
    {
        /* Nothing to do */
    }
    else if (SHUTTER_STATE_CALIB_PREPARE == con->state)
    {
        /* User notified that the shutter is at the top now.
         * Drive down and count the time.
         */
        ctrlDirection   = SHUTTERDRV_DIR_DOWN;
        duration        = 0xffff;   /* Set to maximum to avoid that the shutter stops before it is in end position. */
        con->state      = SHUTTER_STATE_CALIB_DRIVE_DOWN;
    }
    else if (SHUTTER_STATE_CALIB_DRIVE_DOWN == con->state)
    {
        /* User notified that the shutter is at the bottom now.
         * Drive up and count the time.
         */
        ctrlDirection   = SHUTTERDRV_DIR_UP;
        duration        = 0xffff;   /* Set to maximum to avoid that the shutter stops before it is in end position. */
        con->state      = SHUTTER_STATE_CALIB_DRIVE_UP;

        /* Stop timer and store result */
        con->timer.isEnabled    = FALSE;    /* Avoids that a ISR changes the run counter. */
        con->maxDown            = con->timer.run;
    }
    else if (SHUTTER_STATE_CALIB_DRIVE_UP == con->state)
    {
        /* User notified that the shutter is at the top now.
         * Store the up and down time in persistent memory.
         */
        ctrlDirection   = SHUTTERDRV_DIR_STOP;
        con->state      = SHUTTER_STATE_NORMAL;

        /* Stop timer and store result */
        con->timer.isEnabled    = FALSE;    /* Avoids that a ISR changes the run counter. */
        con->maxUp              = con->timer.run;

        /* Shutter is now at the top */
        con->pos        = 0;
        con->posStatus  = SHUTTER_POS_STATUS_VALID_TOP;

        /* Configuration is updated */
        con->isUpdated = TRUE;
    }
    else
    {
        /* Should never happen */
        ctrlDirection   = SHUTTERDRV_DIR_STOP;
        con->state      = SHUTTER_STATE_NORMAL;
    }

    /* Command the shutter driver only in case of no wind alert. */
    if (SHUTTER_STATE_BLOCKED != con->state)
    {
        /* Set timer for drive duration. */
        if (SHUTTERDRV_DIR_STOP == ctrlDirection)
        {
            con->timer.duration = 0;
        }
        else
        {
            con->timer.duration = duration;
        }

        /* Drive the shutter */
        shutterDrv_drive(nr, ctrlDirection);
    }

    return;
}

/**
 * This function drives the shutter to a absolute position.
 * Note, the top is always the position 0 and the bottom is 255. This means
 * 256 absolute positions can be used.
 *
 * If the shutter position is not known, this function will cause a reference
 * drive to the top.
 *
 * @param[in]   nr  Shutter instance number
 * @param[in]   pos Absolute position
 */
extern void shutter_driveAbs(uint8_t nr, uint8_t pos)
{
    shutter_Context *con        = NULL;
    SHUTTER_DIR     dir         = SHUTTER_DIR_STOP;
    uint16_t        duration    = 0;

    /* Invalid shutter instance number? */
    if (SHUTTER_NUM <= nr)
    {
        return;
    }

    con = &shutter_context[nr];

    /* If the shutter instance is disabled, we will return now. */
    if (FALSE == con->isEnabled)
    {
        return;
    }

    /* Shutter position unknown? Calibration drive needed first? */
    if ((SHUTTER_POS_STATUS_VALID_TOP != con->posStatus) &&
        (SHUTTER_POS_STATUS_VALID_BOTTOM != con->posStatus))
    {
        dir         = SHUTTER_DIR_UP;
        duration    = con->maxUp;
    }
    else
    /* Shutter position is known. Calculate the difference and determine
     * the time duration and the direction to drive.
     */
    {
        uint16_t    oldPos      = 0;
        uint16_t    newPos      = 0;

        if (SHUTTER_POS_STATUS_VALID_TOP == con->posStatus)
        {
            newPos = (uint16_t)(((uint32_t)pos * (uint32_t)con->maxUp) / (uint32_t)255);
        }
        else
        {
            newPos = (uint16_t)(((uint32_t)pos * (uint32_t)con->maxDown) / (uint32_t)255);
        }

        /* Shall shutter drive up? */
        if (con->pos > newPos)
        {
            dir = SHUTTER_DIR_UP;

            oldPos = shutter_calcPos(   con->pos,
                                        con->posStatus,
                                        SHUTTER_POS_STATUS_VALID_TOP,
                                        con->maxUp,
                                        con->maxDown);

            newPos = shutter_calcPos(   newPos,
                                        con->posStatus,
                                        SHUTTER_POS_STATUS_VALID_TOP,
                                        con->maxUp,
                                        con->maxDown);

            duration = oldPos - newPos;
        }
        else
        /* Shall shutter drive down? */
        if (con->pos < newPos)
        {
            dir = SHUTTER_DIR_DOWN;

            oldPos = shutter_calcPos(   con->pos,
                                        con->posStatus,
                                        SHUTTER_POS_STATUS_VALID_BOTTOM,
                                        con->maxUp,
                                        con->maxDown);

            newPos = shutter_calcPos(   newPos,
                                        con->posStatus,
                                        SHUTTER_POS_STATUS_VALID_BOTTOM,
                                        con->maxUp,
                                        con->maxDown);

            duration = newPos - oldPos;
        }
        else
        /* Requested position is equal to the current position. */
        {
            dir         = SHUTTER_DIR_STOP;
            duration    = 0;
        }
    }

    shutter_drive(nr, dir, duration);

    return;
}

/**
 * This function drives set/reset the wind alert. If alert is set, the shutter
 * will be driven to the top and blocked for further drive commands. If the
 * alert is removed, the shutter will stay at top and drive commands are
 * possible again.
 *
 * @param[in]   nr      Shutter instance number
 * @param[in]   alert   Set or remove wind alert
 */
extern void shutter_windAlert(uint8_t nr, BOOL alert)
{
    shutter_Context *con    = NULL;

    /* Invalid shutter instance number? */
    if (SHUTTER_NUM <= nr)
    {
        return;
    }

    con = &shutter_context[nr];

    /* Remove alert? */
    if (FALSE == alert)
    {
        con->state = SHUTTER_STATE_NORMAL;
    }
    else
    {
        /* Shutter not blocked yet? */
        if (SHUTTER_STATE_BLOCKED != con->state)
        {
            /* If a calibration drive is running, it will be stopped. */
            con->state = SHUTTER_STATE_NORMAL;

            /* Drive shutter up! */
            shutter_drive(nr, SHUTTERDRV_DIR_STOP, 0);
            shutter_drive(nr, SHUTTER_DIR_TOP, 0);
        }

        con->state = SHUTTER_STATE_BLOCKED;
    }

    return;
}

/**
 * This function turns the shutter the given angle. It is only interesting for
 * a jalousie!
 *
 * @param[in]   nr      Shutter instance number
 * @param[in]   angle   Angle in degree [0;90]°
 */
extern void shutter_turn(uint8_t nr, uint8_t angle)
{
    shutter_Context *con        = NULL;
    uint16_t        duration    = 0;

    /* Invalid shutter instance number? */
    if (SHUTTER_NUM <= nr)
    {
        return;
    }

    con = &shutter_context[nr];

    /* If the shutter instance is disabled, we will return now. */
    if (FALSE == con->isEnabled)
    {
        return;
    }

    /* Angle greater than 90°? */
    if (90 < angle)
    {
        return;
    }

    /* No turn time available? */
    if (0 == con->maxTurn)
    {
        return;
    }

    /* Calculate duration */
    duration = (con->maxTurn * angle) / 90;

    if (0 < duration)
    {
        shutter_drive(nr, SHUTTER_DIR_UP, duration);
    }

    return;
}

/**
 * This function is called in an ISR to process the timers.
 * It shall be called every 100 ms.
 */
extern void shutter_processTimers(void)
{
    uint8_t index   = 0;

    for(index = 0; index < SHUTTER_NUM; ++index)
    {
        shutter_Context *con    = &shutter_context[index];

        /* Is the timer enabled? */
        if (TRUE == con->timer.isEnabled)
        {
            /* No timeout happened right now? */
            if (0 < con->timer.duration)
            {
                --con->timer.duration;
            }

            /* Count time up, independent of timeout. */
            if (0xffff > con->timer.run)
            {
                ++con->timer.run;
            }
        }
    }

    return;
}

/**
 * This function controls the shutters according to their given commands.
 * It shall be called every 10 ms.
 *
 * @return Idle state
 * @retval FALSE    Any shutter is driving
 * @retval TRUE     Idle, no shutter is driving
 */
extern BOOL shutter_process(void)
{
    uint8_t index   = 0;
    uint8_t idle    = 0;
    BOOL    isIdle  = FALSE;

    /* Run state machine for every shutter. */
    for(index = 0; index < SHUTTER_NUM; ++index)
    {
        shutter_Context *con    = &shutter_context[index];

        /* Timer enabled? */
        if (TRUE == con->timer.isEnabled)
        {
            /* Timeout? */
            if (0 == con->timer.duration)
            {
                /* Stop shutter */
                shutterDrv_drive(index, SHUTTERDRV_DIR_STOP);
            }
        }

        /* Process idle counter */
        if (SHUTTERDRV_DIR_STOP == shutterDrv_getDriveDirection(index))
        {
            ++idle;
        }
    }

    /* If all shutter instances are idle, the task can be suspended. */
    if (SHUTTER_NUM == idle)
    {
        isIdle = TRUE;
    }

    return isIdle;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function is called by the shutter control and signals start and stop
 * of the shutter.
 *
 * @param[in]   nr          Shutter instance number
 * @param[in]   direction   Shutter drive direction
 * @param[in]   isDriving   Shutter is driving or stopped
 * @param[in]   userData    User specific data
 */
static void shutter_driveCb(uint8_t nr, SHUTTERDRV_DIR direction, BOOL isDriving, void * const userData)
{
    shutter_Context * const con = (shutter_Context*)userData;

    if (NULL == userData)
    {
        return;
    }

    /* Shutter stopped? */
    if (FALSE == isDriving)
    {
        /* Stop counting the time the shutter runs. */
        con->timer.isEnabled = FALSE;

        if ((SHUTTER_STATE_NORMAL == con->state) ||
            (SHUTTER_STATE_BLOCKED == con->state))
        {
            /* Do we know where the shutter position is (top or bottom)? */
            if ((SHUTTERDRV_DIR_UP == direction) &&
                (con->maxUp <= con->timer.run))
            {
                /* At the top */
                con->pos        = 0;
                con->posStatus  = SHUTTER_POS_STATUS_VALID_TOP;
            }
            else if ((SHUTTERDRV_DIR_DOWN == direction) &&
                     (con->maxDown <= con->timer.run))
            {
                /* At the bottom */
                con->pos        = con->maxDown;
                con->posStatus  = SHUTTER_POS_STATUS_VALID_BOTTOM;
            }
            else
            /* Is the shutter already calibrated? */
            if ((SHUTTER_POS_STATUS_VALID_TOP == con->posStatus) ||
                (SHUTTER_POS_STATUS_VALID_BOTTOM == con->posStatus))
            {
                /* Calculate position */
                if (SHUTTERDRV_DIR_UP == direction)
                {
                    /* Position recalculation needed? */
                    if (SHUTTER_POS_STATUS_VALID_BOTTOM == con->posStatus)
                    {
                        con->pos        = shutter_calcPos(  con->pos,
                                                            con->posStatus,
                                                            SHUTTER_POS_STATUS_VALID_TOP,
                                                            con->maxUp,
                                                            con->maxDown);

                        con->posStatus  = SHUTTER_POS_STATUS_VALID_TOP;
                    }

                    if (con->pos <= con->timer.run)
                    {
                        con->pos = 0;
                    }
                    else
                    {
                        con->pos -= con->timer.run;
                    }
                }
                else
                /* Calculate position */
                if (SHUTTERDRV_DIR_DOWN == direction)
                {
                    /* Position recalculation needed? */
                    if (SHUTTER_POS_STATUS_VALID_TOP == con->posStatus)
                    {
                        con->pos        = shutter_calcPos(  con->pos,
                                                            con->posStatus,
                                                            SHUTTER_POS_STATUS_VALID_BOTTOM,
                                                            con->maxUp,
                                                            con->maxDown);

                        con->posStatus  = SHUTTER_POS_STATUS_VALID_BOTTOM;
                    }

                    if (con->maxDown <= con->timer.run)
                    {
                        con->pos = con->maxDown;
                    }
                    else
                    if ((con->maxDown - con->pos) <= con->timer.run)
                    {
                        con->pos = con->maxDown;
                    }
                    else
                    {
                        con->pos += con->timer.run;
                    }
                }
            }
        }
    }
    /* Shutter started to drive */
    else
    {
        /* Shall the shutter run a defined time or infinite? */
        if (0 < con->timer.duration)
        {
            /* Start to count the time the shutter runs.
             * At this point the timer duration is already set.
             */
            con->timer.run          = 0;
            con->timer.isEnabled    = TRUE;
        }
    }

    /* Inform application? */
    if (NULL != shutter_infoCb)
    {
        /* Shutter stopped? */
        if (FALSE == isDriving)
        {
            SHUTTER_POS shutterPos = SHUTTER_POS_UNKNOWN;

            /* Is shutter at the top? */
            if ((0 == con->pos) &&
                (SHUTTER_POS_STATUS_VALID_TOP == con->posStatus))
            {
                shutterPos = SHUTTER_POS_TOP;
            }
            /* Is shutter at the bottom? */
            else if ((con->maxDown == con->pos) &&
                     (SHUTTER_POS_STATUS_VALID_BOTTOM == con->posStatus))
            {
                shutterPos = SHUTTER_POS_BOTTOM;
            }
            else
            {
                shutterPos = SHUTTER_POS_UNKNOWN;
            }

            shutter_infoCb(nr, SHUTTER_DIR_STOP, shutterPos);
        }
        /* Shutter started to drive */
        else
        {
            SHUTTER_DIR shutterDir = SHUTTER_DIR_STOP;

            if (SHUTTERDRV_DIR_UP == direction)
            {
                shutterDir = SHUTTER_DIR_UP;
            }
            else if (SHUTTERDRV_DIR_DOWN == direction)
            {
                shutterDir = SHUTTER_DIR_DOWN;
            }
            else
            {
                shutterDir = SHUTTER_DIR_STOP;
            }

            shutter_infoCb(nr, shutterDir, SHUTTER_POS_UNKNOWN);
        }
    }

    return;
}

/**
 * Get the time which is needed to drive the shutter to the top.
 *
 * @param[in]   con Shutter context
 * @return  Duration time in 0.1s
 */
static uint16_t shutter_getTimeToTop(shutter_Context const * const con)
{
    uint16_t    duration    = 0;

    if (NULL != con)
    {
        /* Calibration drive needed first? */
        if ((SHUTTER_POS_STATUS_VALID_TOP != con->posStatus) &&
            (SHUTTER_POS_STATUS_VALID_BOTTOM != con->posStatus))
        {
            duration = con->maxUp;
        }
        else
        /* Shutter position is known. */
        {
            duration = shutter_calcPos( con->pos,
                                        con->posStatus,
                                        SHUTTER_POS_STATUS_VALID_TOP,
                                        con->maxUp,
                                        con->maxDown);
        }
    }

    return duration;
}

/**
 * Get the time which is needed to drive the shutter to the bottom.
 *
 * @param[in]   con Shutter context
 * @return  Duration time in 0.1s
 */
static uint16_t shutter_getTimeToBottom(shutter_Context const * const con)
{
    uint16_t    duration    = 0;

    if (NULL != con)
    {
        /* Calibration drive needed first? */
        if ((SHUTTER_POS_STATUS_VALID_TOP != con->posStatus) &&
            (SHUTTER_POS_STATUS_VALID_BOTTOM != con->posStatus))
        {
            duration = con->maxDown;
        }
        else
        /* Shutter position is known. */
        {
            duration = con->maxDown - shutter_calcPos(  con->pos,
                                                        con->posStatus,
                                                        SHUTTER_POS_STATUS_VALID_BOTTOM,
                                                        con->maxUp,
                                                        con->maxDown);
        }
    }

    return duration;
}

/**
 * Calculate the position which will be valid for the given new position
 * status.
 *
 * @param[in]   pos         Position
 * @param[in]   status      Current position status
 * @param[in]   newStatus   New position status
 * @param[in]   maxUp       Max. time to the top in 0.1s
 * @param[in]   maxDown     Max. time to the bottom 0.1s
 * @return  Position
 */
static uint16_t shutter_calcPos(uint16_t pos, SHUTTER_POS_STATUS status, SHUTTER_POS_STATUS newStatus, uint16_t maxUp, uint16_t maxDown)
{
    uint32_t    tmp = pos;

    if (status != newStatus)
    {
        if (SHUTTER_POS_STATUS_VALID_TOP == newStatus)
        {
            tmp *= maxUp;
            tmp /= maxDown;

            if (maxUp < tmp)
            {
                tmp = maxUp;
            }
        }
        else
        if (SHUTTER_POS_STATUS_VALID_BOTTOM == newStatus)
        {
            tmp *= maxDown;
            tmp /= maxUp;

            if (maxDown < tmp)
            {
                tmp = maxDown;
            }
        }
    }

    return (uint16_t)tmp;
}
