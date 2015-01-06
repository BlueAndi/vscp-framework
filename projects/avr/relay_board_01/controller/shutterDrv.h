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
@brief  This module contains the shutter driver.
@file   shutterDrv.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Each shutter works with two relays. The relay which is used to define the
shutter drive direction and the relay which enables the power.
Possible drive directions are stop, up and down.

@section svn Subversion
$Author: amerkle $
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __SHUTTERDRV_H__
#define __SHUTTERDRV_H__

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
#define SHUTTERDRV_NUM  4

/*******************************************************************************
	MACROS
*******************************************************************************/

/*******************************************************************************
	TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines function return values. */
typedef enum
{
	SHUTTERDRV_RET_OK = 0,	/**< Execution successful */
	SHUTTERDRV_RET_ERROR,	/**< Execution failed (common error) */
	SHUTTERDRV_RET_ENULL,	/**< Unexpected NULL pointer */
	SHUTTERDRV_RET_EPAR	    /**< Invalid parameter */

} SHUTTERDRV_RET;

/** This type defines the possible shutter directions. */
typedef enum
{
	SHUTTERDRV_DIR_STOP = 0,	/**< Stop */
	SHUTTERDRV_DIR_UP,			/**< Drive shutter up */
	SHUTTERDRV_DIR_DOWN		    /**< Drive shutter down */

} SHUTTERDRV_DIR;

/** This type defines a shutter control callback. */
typedef void	(*shutterDrv_CallBack)(uint8_t nr, SHUTTERDRV_DIR dir, BOOL isDriving, void * const userData);

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
extern void shutterDrv_init(void);

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
extern SHUTTERDRV_RET shutterDrv_configure(uint8_t nr, uint8_t relayPower, uint8_t relayDir,	shutterDrv_CallBack cb, void * const userData);

/**
 * This function enables or disables a shutter instance. If the shutter
 * instance shall be disabled and it is driving at the moment, it will be
 * automatically stopped.
 *
 * @param[in]   nr          Shutter instance number
 * @param[in]   enableIt    TRUE: Enable it FALSE: Disable it
 */
extern void	shutterDrv_enable(uint8_t nr, BOOL enableIt);

/**
 * This function drives the shutter to the given direction.
 *
 * @param[in]   nr          Shutter instance number
 * @param[in]   direction   Shutter drive direction
 */
extern void	shutterDrv_drive(uint8_t nr, SHUTTERDRV_DIR direction);

/**
 * Get current drive direction of the shutter.
 *
 * @param[in]   nr  Shutter instance number
 * @return	Drive direction
 */
extern SHUTTERDRV_DIR shutterDrv_getDriveDirection(uint8_t nr);

/**
 * This function process all shutters. Call it in a 10 ms period.
 *
 * @return Idle state
 * @retval FALSE    Any shutter is driving
 * @retval TRUE     Idle, no shutter is driving
 */
extern BOOL shutterDrv_process(void);

#ifdef __cplusplus
}
#endif

#endif	/* __SHUTTERDRV_H__ */
