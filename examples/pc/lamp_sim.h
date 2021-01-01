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
@file   lamp_sim.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides a lamp simulation.

*******************************************************************************/
/** @defgroup lamp_sim Lamp simulation
 * Simulates several lamps in a house.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __LAMP_SIM_H__
#define __LAMP_SIM_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "platform.h"

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

/** Number of lamps */
#define LAMP_SIM_NUM    8

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the module.
 */
extern void lamp_sim_init(void);

/**
 * This function enables or disables a lamp.
 *
 * @param[in]   index       Lamp index [0-7]
 * @param[in]   enableIt    Enable or disable lamp
 */
extern void lamp_sim_setState(uint8_t index, BOOL enableIt);

/**
 * This function returns the lamp state.
 *
 * @param[in]   index       Lamp index [0-7]
 *
 * @return Lamp state
 * @retval FALSE    Lamp off
 * @retval TRUE     Lamp on
 */
extern BOOL lamp_sim_getState(uint8_t index);

/**
 * This function set the brightness of a lamp.
 *
 * @param[in]   index       Lamp index [0-7]
 * @param[in]   percentage  Brightness [0-100]
 */
extern void lamp_sim_setBrightness(uint8_t index, uint8_t brightness);

/**
 * This function returns the lamp brightness.
 *
 * @param[in]   index       Lamp index [0-7]
 *
 * @return Lamp brightness in percent [0-100]
 */
extern uint8_t lamp_sim_getBrightness(uint8_t index);

/**
 * This function shows the status of every simulated lamp to the user.
 */
extern void lamp_sim_show(void);

#ifdef __cplusplus
}
#endif

#endif  /* __LAMP_SIM_H__ */

/** @} */
