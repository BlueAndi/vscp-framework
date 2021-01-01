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
@brief  This module controls the shutter.
@file   shutter.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Each shutter works with two relays. The relay which is used to define the
shutter drive direction and the relay which enables the power.

Possible drive directions are stop, up and down. Additional a drive time can
be specified, how long the shutter will drive into the given direction.

On the other hand it is possible to drive to a absolute shutter position. This
feature will only work if the shutter knows its own position. To know the own
position, a reference drive is needed. A reference drive means to drive the
max. up-time up. Then we know the shutter is at the top and the position is
known.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __SHUTTER_H__
#define __SHUTTER_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Number of shutters */
#define SHUTTER_NUM 4

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines function return values. */
typedef enum
{
    SHUTTER_RET_OK = 0, /**< Execution successful */
    SHUTTER_RET_ERROR,  /**< Execution failed (common error) */
    SHUTTER_RET_ENULL,  /**< Unexpected NULL pointer */
    SHUTTER_RET_EPAR    /**< Invalid parameter */

} SHUTTER_RET;

/** This type defines the possible shutter directions. */
typedef enum
{
    SHUTTER_DIR_STOP = 0,   /**< Stop */
    SHUTTER_DIR_UP,         /**< Drive shutter up */
    SHUTTER_DIR_DOWN,       /**< Drive shutter down */
    SHUTTER_DIR_TOP,        /**< Drive to the top */
    SHUTTER_DIR_BOTTOM,     /**< Drive to the bottom */
    SHUTTER_DIR_CALIBRATE   /**< Calibration drive */

} SHUTTER_DIR;

/** This type defines the possible shutter positions. */
typedef enum
{
    SHUTTER_POS_UNKNOWN = 0,    /**< Position is unknown */
    SHUTTER_POS_TOP,            /**< Position is at the top */
    SHUTTER_POS_BOTTOM          /**< Position is at the bottom */

} SHUTTER_POS;

/** This type defines a shutter control callback. */
typedef void    (*shutter_CallBack)(uint8_t nr, SHUTTER_DIR dir, SHUTTER_POS pos);

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern void shutter_init(void);

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
extern void shutter_configure(uint8_t nr, uint8_t relayPower, uint8_t relayDir, uint16_t maxUp, uint16_t maxDown, uint16_t maxTurn);

/**
 * This function registers a callback, which will be informed for any kind of shutter action.
 *
 * @param[in]   cb          Callback
 * @param[in]   userData    User specific data which will get back via callback
 */
extern void shutter_register(shutter_CallBack cb, void* userData);

/**
 * This function enables or disables a shutter instance. If the shutter
 * instance shall be disabled and it is driving at the moment, it will be
 * automatically stopped.
 *
 * @param[in]   nr          Shutter instance number
 * @param[in]   enableIt    TRUE: Enable it FALSE: Disable it
 */
extern void shutter_enable(uint8_t nr, BOOL enableIt);

/**
 * This functions returns the shutter enable status.
 *
 * @param[in]   nr  Shutter instance number
 * @return Enabled or not
 * @retval FALSE    Shutter is disabled
 * @retval TRUE     Shutter is enabled
 */
extern BOOL shutter_isEnabled(uint8_t nr);

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
extern void shutter_drive(uint8_t nr, SHUTTER_DIR dir, uint16_t duration);

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
extern void shutter_driveAbs(uint8_t nr, uint8_t pos);

/**
 * This function drives set/reset the wind alert. If alert is set, the shutter
 * will be driven to the top and blocked for further drive commands. If the
 * alert is removed, the shutter will stay at top and drive commands are
 * possible again.
 *
 * @param[in]   nr      Shutter instance number
 * @param[in]   alert   Set or remove wind alert
 */
extern void shutter_windAlert(uint8_t nr, BOOL alert);

/**
 * This function turns the shutter the given angle. It is only interesting for
 * a jalousie!
 *
 * @param[in]   nr      Shutter instance number
 * @param[in]   angle   Angle in degree [0;90]°
 */
extern void shutter_turn(uint8_t nr, uint8_t angle);

/**
 * This function is called in an ISR to process the timers.
 * It shall be called every 100 ms.
 */
extern void shutter_processTimers(void);

/**
 * This function controls the shutters according to their given commands.
 * It shall be called every 10 ms.
 *
 * @return Idle state
 * @retval FALSE    Any shutter is driving
 * @retval TRUE     Idle, no shutter is driving
 */
extern BOOL shutter_process(void);

#ifdef __cplusplus
}
#endif

#endif  /* __SHUTTER_H__ */
