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
@brief  Configuration
@file   vscp_evt_configuration.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Events for configuration of sets and parameters on nodes without register manipulation.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_EVT_CONFIGURATION_H__
#define __VSCP_EVT_CONFIGURATION_H__

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
extern BOOL vscp_evt_configuration_sendGeneralEvent(void);

/**
 * Load configuration
 * 
 * @param[in] index Index
 * @param[in] zone Zone
 * @param[in] subzone Subzone
 * @param[in] configurationid Configuration Id
 * @param[in] control Control byte (optional). (optional)
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendLoadConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control);

/**
 * Load configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendLoadConfigurationAcknowledge(uint16_t configurationid);

/**
 * Load configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendLoadConfigurationNegativeAcknowledge(uint16_t configurationid);

/**
 * Save configuration
 * 
 * @param[in] index Index
 * @param[in] zone Zone
 * @param[in] subzone Subzone
 * @param[in] configurationid Configuration Id
 * @param[in] control Control byte (optional). (optional)
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSaveConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control);

/**
 * Save configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSaveConfigurationAcknowledge(uint16_t configurationid);

/**
 * Save configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSaveConfigurationNegativeAcknowledge(uint16_t configurationid);

/**
 * Commit configuration
 * 
 * @param[in] index Index
 * @param[in] zone Zone
 * @param[in] subzone Subzone
 * @param[in] configurationid Configuration Id
 * @param[in] control Control byte (optional). (optional)
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendCommitConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control);

/**
 * Commit configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendCommitConfigurationAcknowledge(uint16_t configurationid);

/**
 * Commit configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendCommitConfigurationNegativeAcknowledge(uint16_t configurationid);

/**
 * Reload configuration
 * 
 * @param[in] index Index
 * @param[in] zone Zone
 * @param[in] subzone Subzone
 * @param[in] configurationid Configuration Id
 * @param[in] control Control byte (optional). (optional)
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendReloadConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control);

/**
 * Reload configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendReloadConfigurationAcknowledge(uint16_t configurationid);

/**
 * Reload configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendReloadConfigurationNegativeAcknowledge(uint16_t configurationid);

/**
 * Restore configuration
 * 
 * @param[in] index Index
 * @param[in] zone Zone
 * @param[in] subzone Subzone
 * @param[in] configurationid Configuration Id
 * @param[in] control Control byte (optional). (optional)
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendRestoreConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control);

/**
 * Restore configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendRestoreConfigurationAcknowledge(uint16_t configurationid);

/**
 * Restore configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendRestoreConfigurationNegativeAcknowledge(uint16_t configurationid);

/**
 * Set parameter
 * 
 * @param[in] index Index
 * @param[in] zone Zone
 * @param[in] subzone Subzone
 * @param[in] parameterid Parameter Id
 * @param[in] parametervalue Parameter value
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSetParameter(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t parameterid, uint16_t parametervalue);

/**
 * Set parameter to default
 * 
 * @param[in] index Index
 * @param[in] zone Zone
 * @param[in] subzone Subzone
 * @param[in] parameterid Parameter Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSetParameterToDefault(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t parameterid);

/**
 * Set parameter acknowledge
 * 
 * @param[in] parameterid Parameter Id
 * @param[in] parametervalue Parameter value
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSetParameterAcknowledge(uint16_t parameterid, uint16_t parametervalue);

/**
 * Set paramter negative acknowledge
 * 
 * @param[in] parameterid Parameter Id
 * @param[in] parametervalue Parameter value
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSetParamterNegativeAcknowledge(uint16_t parameterid, uint16_t parametervalue);

#endif  /* __VSCP_EVT_CONFIGURATION_H__ */