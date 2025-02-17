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
@brief  Phone
@file   vscp_evt_phone.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_evt_phone.h

This file is automatically generated. Don't change it manually.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_evt_phone.h"
#include "vscp_core.h"
#include "vscp_class_l1.h"
#include "vscp_type_phone.h"

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
extern BOOL vscp_evt_phone_sendGeneralEvent(void)
{
    vscp_TxMessage  txMsg;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_PHONE, VSCP_TYPE_PHONE_GENERAL, VSCP_PRIORITY_3_NORMAL);

    txMsg.dataSize = 0;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Incoming call
 * 
 * @param[in] id Id for the call. This is an incremental identity number for each call.
 * @param[in] index Index of phone event (base = 0). Each call can be broken up into fragments. This
 * is the fragment number.
 * @param[in] total Total number of events (fragments) for this call information.
 * @param[in] info Caller information. Number or real text information. (array[5])
 * @param[in] infosize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_phone_sendIncomingCall(uint8_t id, uint8_t index, uint8_t total, uint8_t const * const info, uint8_t infoSize)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;
    uint8_t         byteIndex   = 0;

    if ((NULL == info) || (0 == infoSize))
    {
        return FALSE;
    }

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_PHONE, VSCP_TYPE_PHONE_INCOMING_CALL, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.data[1] = index;
    size += 1;

    txMsg.data[2] = total;
    size += 1;

    for(byteIndex = 0; byteIndex < infoSize; ++byteIndex)
    {
        txMsg.data[3 + byteIndex] = info[byteIndex];
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
 * Outgoing call
 * 
 * @param[in] id Id for the call. This is an incremental identity number for each call.
 * @param[in] index Index of phone event (base = 0). Each call can be broken up into fragments. This
 * is the fragment number.
 * @param[in] total Total number of events (fragments) for this call information.
 * @param[in] info Caller information. Number or real text information. (array[5])
 * @param[in] infosize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_phone_sendOutgoingCall(uint8_t id, uint8_t index, uint8_t total, uint8_t const * const info, uint8_t infoSize)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;
    uint8_t         byteIndex   = 0;

    if ((NULL == info) || (0 == infoSize))
    {
        return FALSE;
    }

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_PHONE, VSCP_TYPE_PHONE_OUTGOING_CALL, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.data[1] = index;
    size += 1;

    txMsg.data[2] = total;
    size += 1;

    for(byteIndex = 0; byteIndex < infoSize; ++byteIndex)
    {
        txMsg.data[3 + byteIndex] = info[byteIndex];
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
 * Ring
 * 
 * @param[in] id Id for the call. This is an incremental identity number for each call.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_phone_sendRing(uint8_t id)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_PHONE, VSCP_TYPE_PHONE_RING, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Answer
 * 
 * @param[in] id Id for the call. This is an incremental identity number for each call.
 * @param[in] index Index of phone event (base = 0). Each call can be broken up into fragments. This
 * is the fragment number.
 * @param[in] total Total number of events (fragments) for this call information.
 * @param[in] info Caller information. Number or real text information. (array[5])
 * @param[in] infosize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_phone_sendAnswer(uint8_t id, uint8_t index, uint8_t total, uint8_t const * const info, uint8_t infoSize)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;
    uint8_t         byteIndex   = 0;

    if ((NULL == info) || (0 == infoSize))
    {
        return FALSE;
    }

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_PHONE, VSCP_TYPE_PHONE_ANSWER, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.data[1] = index;
    size += 1;

    txMsg.data[2] = total;
    size += 1;

    for(byteIndex = 0; byteIndex < infoSize; ++byteIndex)
    {
        txMsg.data[3 + byteIndex] = info[byteIndex];
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
 * Hangup
 * 
 * @param[in] id Id for the call. This is an incremental identity number for each call.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_phone_sendHangup(uint8_t id)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_PHONE, VSCP_TYPE_PHONE_HANGUP, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Giveup
 * 
 * @param[in] id Id for the call. This is an incremental identity number for each call.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_phone_sendGiveup(uint8_t id)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_PHONE, VSCP_TYPE_PHONE_GIVEUP, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Transfer
 * 
 * @param[in] id Id for the call. This is an incremental identity number for each call.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_phone_sendTransfer(uint8_t id)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_PHONE, VSCP_TYPE_PHONE_TRANSFER, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Database Info
 * 
 * @param[in] id Id for the call. This is an incremental identity number for each call.
 * @param[in] index Index of phone event (base = 0). Each call can be broken up into fragments. This
 * is the fragment number.
 * @param[in] total Total number of events (fragments) for this call information.
 * @param[in] info Caller information. Number or real text information. (array[5])
 * @param[in] infosize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_phone_sendDatabaseInfo(uint8_t id, uint8_t index, uint8_t total, uint8_t const * const info, uint8_t infoSize)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;
    uint8_t         byteIndex   = 0;

    if ((NULL == info) || (0 == infoSize))
    {
        return FALSE;
    }

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_PHONE, VSCP_TYPE_PHONE_DATABASE_INFO, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = id;
    size += 1;

    txMsg.data[1] = index;
    size += 1;

    txMsg.data[2] = total;
    size += 1;

    for(byteIndex = 0; byteIndex < infoSize; ++byteIndex)
    {
        txMsg.data[3 + byteIndex] = info[byteIndex];
        size += 1;

        if (VSCP_L1_DATA_SIZE <= size)
        {
            break;
        }
    }

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

