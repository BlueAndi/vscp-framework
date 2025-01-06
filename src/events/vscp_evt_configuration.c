/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2025 Andreas Merkle
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
@file   vscp_evt_configuration.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_evt_configuration.h

This file is automatically generated. Don't change it manually.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_evt_configuration.h"
#include "vscp_core.h"
#include "vscp_class_l1.h"
#include "vscp_type_configuration.h"

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
extern BOOL vscp_evt_configuration_sendGeneralEvent(void)
{
    vscp_TxMessage  txMsg;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_GENERAL, VSCP_PRIORITY_3_NORMAL);

    txMsg.dataSize = 0;

    return vscp_core_sendEvent(&txMsg);
}

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
extern BOOL vscp_evt_configuration_sendLoadConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_LOAD, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subzone;
    size += 1;

    txMsg.data[3] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[4] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    if (NULL != control)
    {
        txMsg.data[5] = *control;
        size += 1;
    }

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Load configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendLoadConfigurationAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_LOAD_ACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Load configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendLoadConfigurationNegativeAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_LOAD_NACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

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
extern BOOL vscp_evt_configuration_sendSaveConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_SAVE, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subzone;
    size += 1;

    txMsg.data[3] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[4] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    if (NULL != control)
    {
        txMsg.data[5] = *control;
        size += 1;
    }

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Save configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSaveConfigurationAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_SAVE_ACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Save configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSaveConfigurationNegativeAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_SAVE_NACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

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
extern BOOL vscp_evt_configuration_sendCommitConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_COMMIT, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subzone;
    size += 1;

    txMsg.data[3] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[4] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    if (NULL != control)
    {
        txMsg.data[5] = *control;
        size += 1;
    }

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Commit configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendCommitConfigurationAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_COMMIT_ACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Commit configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendCommitConfigurationNegativeAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_COMMIT_NACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

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
extern BOOL vscp_evt_configuration_sendReloadConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_RELOAD, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subzone;
    size += 1;

    txMsg.data[3] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[4] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    if (NULL != control)
    {
        txMsg.data[5] = *control;
        size += 1;
    }

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Reload configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendReloadConfigurationAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_REALOD_ACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Reload configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendReloadConfigurationNegativeAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_RELOAD_NACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

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
extern BOOL vscp_evt_configuration_sendRestoreConfiguration(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t configurationid, uint8_t const * const control)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_RESTORE, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subzone;
    size += 1;

    txMsg.data[3] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[4] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    if (NULL != control)
    {
        txMsg.data[5] = *control;
        size += 1;
    }

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Restore configuration acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendRestoreConfigurationAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_RESTORE_ACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Restore configuration negative acknowledge
 * 
 * @param[in] configurationid Configuration Id
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendRestoreConfigurationNegativeAcknowledge(uint16_t configurationid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_RESTORE_NACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((configurationid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((configurationid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

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
extern BOOL vscp_evt_configuration_sendSetParameter(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t parameterid, uint16_t parametervalue)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_SET_PARAMETER, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subzone;
    size += 1;

    txMsg.data[3] = (uint8_t)((parameterid >> 8) & 0xff);
    txMsg.data[4] = (uint8_t)((parameterid >> 0) & 0xff);
    size += 2;

    txMsg.data[5] = (uint8_t)((parametervalue >> 8) & 0xff);
    txMsg.data[6] = (uint8_t)((parametervalue >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

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
extern BOOL vscp_evt_configuration_sendSetParameterToDefault(uint8_t index, uint8_t zone, uint8_t subzone, uint16_t parameterid)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_SET_PARAMETER_DEFAULT, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subzone;
    size += 1;

    txMsg.data[3] = (uint8_t)((parameterid >> 8) & 0xff);
    txMsg.data[4] = (uint8_t)((parameterid >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Set parameter acknowledge
 * 
 * @param[in] parameterid Parameter Id
 * @param[in] parametervalue Parameter value
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSetParameterAcknowledge(uint16_t parameterid, uint16_t parametervalue)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_SET_PARAMETER_ACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((parameterid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((parameterid >> 0) & 0xff);
    size += 2;

    txMsg.data[2] = (uint8_t)((parametervalue >> 8) & 0xff);
    txMsg.data[3] = (uint8_t)((parametervalue >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Set paramter negative acknowledge
 * 
 * @param[in] parameterid Parameter Id
 * @param[in] parametervalue Parameter value
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_configuration_sendSetParamterNegativeAcknowledge(uint16_t parameterid, uint16_t parametervalue)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_CONFIGURATION, VSCP_TYPE_CONFIGURATION_SET_PARAMETER_NACK, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = (uint8_t)((parameterid >> 8) & 0xff);
    txMsg.data[1] = (uint8_t)((parameterid >> 0) & 0xff);
    size += 2;

    txMsg.data[2] = (uint8_t)((parametervalue >> 8) & 0xff);
    txMsg.data[3] = (uint8_t)((parametervalue >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

