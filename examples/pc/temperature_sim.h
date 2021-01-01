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
@brief  Temperature simulation
@file   temperature_sim.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides a temperature simulation.

*******************************************************************************/
/** @defgroup temperature_sim Temperature simulation
 * Simulates a temperature and sends periodically vscp events.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __TEMPERATURE_SIM_H__
#define __TEMPERATURE_SIM_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>

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

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the supported function return values. */
typedef enum
{
    TEMPERATURE_SIM_RET_OK = 0, /**< Successful */
    TEMPERATURE_SIM_RET_ERROR   /**< Failed */

} TEMPERATURE_SIM_RET;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the module.
 */
extern void temperature_sim_init(void);

/**
 * This function starts the temperature simulation.
 *
 * @return Status
 */
extern TEMPERATURE_SIM_RET temperature_sim_start(void);

/**
 * This function stops the temperature simulation.
 */
extern void temperature_sim_stop(void);

#ifdef __cplusplus
}
#endif

#endif  /* __TEMPERATURE_SIM_H__ */

/** @} */
