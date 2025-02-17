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
@brief  Display
@file   vscp_evt_display.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_evt_display.h

This file is automatically generated. Don't change it manually.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_evt_display.h"
#include "vscp_core.h"
#include "vscp_class_l1.h"
#include "vscp_type_display.h"

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
extern BOOL vscp_evt_display_sendGeneralEvent(void)
{
    vscp_TxMessage  txMsg;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_GENERAL, VSCP_PRIORITY_3_NORMAL);

    txMsg.dataSize = 0;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Clear Display
 * 
 * @param[in] code Code - not yet defined.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_display_sendClearDisplay(uint8_t code, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_CLEAR_DISPLAY, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = code;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Position cursor
 * 
 * @param[in] index Index.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] row Row to move to (first row is 0).
 * @param[in] column Column to move to (first column is 0).
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_display_sendPositionCursor(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t row, uint8_t column)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_POSITION_CURSOR, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.data[3] = row;
    size += 1;

    txMsg.data[4] = column;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Write Display
 * 
 * @param[in] index Increase by one for each event sent for specific text to display.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] data Display data. (array[5])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_display_sendWriteDisplay(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const data, uint8_t dataSize)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;
    uint8_t         byteIndex   = 0;

    if ((NULL == data) || (0 == dataSize))
    {
        return FALSE;
    }

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_WRITE_DISPLAY, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    for(byteIndex = 0; byteIndex < dataSize; ++byteIndex)
    {
        txMsg.data[3 + byteIndex] = data[byteIndex];
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
 * Write Display buffer
 * 
 * @param[in] index Increase by one for each event sent for specific text to display.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] data Display data. (array[5])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_display_sendWriteDisplayBuffer(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const data, uint8_t dataSize)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;
    uint8_t         byteIndex   = 0;

    if ((NULL == data) || (0 == dataSize))
    {
        return FALSE;
    }

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_WRITE_DISPLAY_BUFFER, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    for(byteIndex = 0; byteIndex < dataSize; ++byteIndex)
    {
        txMsg.data[3 + byteIndex] = data[byteIndex];
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
 * Show Display Buffer
 * 
 * @param[in] index Increase by one for each event sent for specific text to display.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_display_sendShowDisplayBuffer(uint8_t index, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_SHOW_DISPLAY_BUFFER, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Set Display Buffer Parameter
 * 
 * @param[in] index Display parameter index.
 * @param[in] coding Data coding byte as of VSCP Specification.
 * @param[in] data Data as of coding. (array[6])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_display_sendSetDisplayBufferParameter(uint8_t index, uint8_t coding, uint8_t const * const data, uint8_t dataSize)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;
    uint8_t         byteIndex   = 0;

    if ((NULL == data) || (0 == dataSize))
    {
        return FALSE;
    }

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_SET_DISPLAY_BUFFER_PARAM, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = coding;
    size += 1;

    for(byteIndex = 0; byteIndex < dataSize; ++byteIndex)
    {
        txMsg.data[2 + byteIndex] = data[byteIndex];
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
 * Show Text
 * 
 * @param[in] index Increase by one for each event sent for specific text to display.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] event Event. (array[5])
 * @param[in] eventsize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_display_sendShowText(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const event, uint8_t eventSize)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;
    uint8_t         byteIndex   = 0;

    if ((NULL == event) || (0 == eventSize))
    {
        return FALSE;
    }

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_SHOW_TEXT, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    for(byteIndex = 0; byteIndex < eventSize; ++byteIndex)
    {
        txMsg.data[3 + byteIndex] = event[byteIndex];
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
 * Set LED
 * 
 * @param[in] index Increase by one for each event sent for specific text to display.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] state 0 =off, 1=on, 2=blink
 * @param[in] blinkOnPeriod Blink On period in milliseconds.
 * @param[in] blinkOffPeriod Blink Off period in milliseconds.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_display_sendSetLed(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t state, uint16_t blinkOnPeriod, uint16_t blinkOffPeriod)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_SET_LED, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.data[3] = state;
    size += 1;

    txMsg.data[4] = (uint8_t)((blinkOnPeriod >> 8) & 0xff);
    txMsg.data[5] = (uint8_t)((blinkOnPeriod >> 0) & 0xff);
    size += 2;

    txMsg.data[6] = (uint8_t)((blinkOffPeriod >> 8) & 0xff);
    txMsg.data[7] = (uint8_t)((blinkOffPeriod >> 0) & 0xff);
    size += 2;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Set RGB Color
 * 
 * @param[in] index Increase by one for each event sent for specific text to display.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] red Color R to display 0-255.
 * @param[in] green Color G to display 0-255.
 * @param[in] blue Color B to display 0-255.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_display_sendSetRgbColor(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t red, uint8_t green, uint8_t blue)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DISPLAY, VSCP_TYPE_DISPLAY_SET_COLOR, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.data[3] = red;
    size += 1;

    txMsg.data[4] = green;
    size += 1;

    txMsg.data[5] = blue;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

