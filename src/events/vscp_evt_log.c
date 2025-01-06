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
@brief  Logging
@file   vscp_evt_log.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_evt_log.h

This file is automatically generated. Don't change it manually.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_evt_log.h"
#include "vscp_core.h"
#include "vscp_class_l1.h"
#include "vscp_type_log.h"

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
extern BOOL vscp_evt_log_sendGeneralEvent(void)
{
    vscp_TxMessage  txMsg;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_LOG, VSCP_TYPE_LOG_GENERAL, VSCP_PRIORITY_3_NORMAL);

    txMsg.dataSize = 0;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Log event
 * 
 * @param[in] id ID for event.
 * @param[in] logLevel Log level for message.
 * @param[in] index Idx for this message.
 * @param[in] msg Message. (array[5])
 * @param[in] msgsize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_log_sendLogEvent(uint8_t id, uint8_t logLevel, uint8_t index, uint8_t const * const msg, uint8_t msgSize)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;
    uint8_t         byteIndex   = 0;

    if ((NULL == msg) || (0 == msgSize))
    {
        return FALSE;
    }

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_LOG, VSCP_TYPE_LOG_MESSAGE, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.data[1] = logLevel;
    size += 1;

    txMsg.data[2] = index;
    size += 1;

    for(byteIndex = 0; byteIndex < msgSize; ++byteIndex)
    {
        txMsg.data[3 + byteIndex] = msg[byteIndex];
        size += 1;

        if (VSCP_L1_DATA_SIZE <= size)
        {
            break;
        }
    }

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Log Start
 * 
 * @param[in] id ID for log.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_log_sendLogStart(uint8_t id)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_LOG, VSCP_TYPE_LOG_START, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Log Stop
 * 
 * @param[in] id ID for log.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_log_sendLogStop(uint8_t id)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_LOG, VSCP_TYPE_LOG_STOP, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Log Level
 * 
 * @param[in] level ID for log byte 0 Log level.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_log_sendLogLevel(uint8_t level)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_LOG, VSCP_TYPE_LOG_LEVEL, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = level;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

