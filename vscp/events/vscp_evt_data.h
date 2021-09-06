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
@brief  Data
@file   vscp_evt_data.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Representation for different general data types. Byte 0 is the data coding byte described here.
Unit may not have meaning for some of the types and should be set to zero in that case.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_EVT_DATA_H__
#define __VSCP_EVT_DATA_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_platform.h"

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

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * General event
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_data_sendGeneralEvent(void);

/**
 * I/O value
 * 
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_data_sendIOValue(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * A/D value
 * 
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_data_sendADValue(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * D/A value
 * 
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_data_sendDAValue(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Relative strength
 * 
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_data_sendRelativeStrength(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Signal Level
 * 
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_data_sendSignalLevel(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Signal Quality
 * 
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_data_sendSignalQuality(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

#endif  /* __VSCP_EVT_DATA_H__ */
