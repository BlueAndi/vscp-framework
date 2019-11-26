/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2019, Andreas Merkle
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
@file   vscp_evt_data.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_evt_data.h

This file is automatically generated. Don't change it manually.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_evt_data.h"
#include "vscp_core.h"
#include "vscp_class_l1.h"
#include "vscp_type_data.h"

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
extern BOOL vscp_evt_protocol_sendGeneralEvent(void)
{
	vscp_TxMessage	txMsg;

	vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DATA, VSCP_TYPE_DATA_GENERAL, VSCP_PRIORITY_3_NORMAL);

	txMsg.dataNum = 0;

	return vscp_core_sendEvent(&txMsg);
}

/**
 * Count
 * 
 * @param[in] dataCoding Data coding.
 * @param[in] data Data with format defined by byte 0.  (array[7])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendIOValue(uint8_t dataCoding, uint8_t const * const data, uint8_t dataSize)
{
	vscp_TxMessage	txMsg;
	uint8_t       	size	= 0;
	uint8_t       	index	= 0;

	if (NULL == data) || (0 == dataSize)
	{
		return FALSE;
	}

	vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DATA, VSCP_TYPE_DATA_IO, VSCP_PRIORITY_3_NORMAL);

	txMsg.data[0] = dataCoding;
	size += 1;

	for(index = 0; index < dataSize; ++index)
	{
		txMsg.data[1 + index] = data[index];
		size += 1;

		if (VSCP_L1_DATA_SIZE <= size)
		{
			break;
		}
	}

	txMsg.dataNum = size;

	return vscp_core_sendEvent(&txMsg);
}

/**
 * Count
 * 
 * @param[in] dataCoding Data coding.
 * @param[in] data Data with format defined by byte 0.  (array[7])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendADValue(uint8_t dataCoding, uint8_t const * const data, uint8_t dataSize)
{
	vscp_TxMessage	txMsg;
	uint8_t       	size	= 0;
	uint8_t       	index	= 0;

	if (NULL == data) || (0 == dataSize)
	{
		return FALSE;
	}

	vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DATA, VSCP_TYPE_DATA_AD, VSCP_PRIORITY_3_NORMAL);

	txMsg.data[0] = dataCoding;
	size += 1;

	for(index = 0; index < dataSize; ++index)
	{
		txMsg.data[1 + index] = data[index];
		size += 1;

		if (VSCP_L1_DATA_SIZE <= size)
		{
			break;
		}
	}

	txMsg.dataNum = size;

	return vscp_core_sendEvent(&txMsg);
}

/**
 * Count
 * 
 * @param[in] dataCoding Data coding.
 * @param[in] data Data with format defined by byte 0.  (array[7])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendDAValue(uint8_t dataCoding, uint8_t const * const data, uint8_t dataSize)
{
	vscp_TxMessage	txMsg;
	uint8_t       	size	= 0;
	uint8_t       	index	= 0;

	if (NULL == data) || (0 == dataSize)
	{
		return FALSE;
	}

	vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DATA, VSCP_TYPE_DATA_DA, VSCP_PRIORITY_3_NORMAL);

	txMsg.data[0] = dataCoding;
	size += 1;

	for(index = 0; index < dataSize; ++index)
	{
		txMsg.data[1 + index] = data[index];
		size += 1;

		if (VSCP_L1_DATA_SIZE <= size)
		{
			break;
		}
	}

	txMsg.dataNum = size;

	return vscp_core_sendEvent(&txMsg);
}

/**
 * Count
 * 
 * @param[in] dataCoding Data coding.
 * @param[in] data Data with format defined by byte 0.  (array[7])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendRelativeStrength(uint8_t dataCoding, uint8_t const * const data, uint8_t dataSize)
{
	vscp_TxMessage	txMsg;
	uint8_t       	size	= 0;
	uint8_t       	index	= 0;

	if (NULL == data) || (0 == dataSize)
	{
		return FALSE;
	}

	vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DATA, VSCP_TYPE_DATA_RELATIVE_STRENGTH, VSCP_PRIORITY_3_NORMAL);

	txMsg.data[0] = dataCoding;
	size += 1;

	for(index = 0; index < dataSize; ++index)
	{
		txMsg.data[1 + index] = data[index];
		size += 1;

		if (VSCP_L1_DATA_SIZE <= size)
		{
			break;
		}
	}

	txMsg.dataNum = size;

	return vscp_core_sendEvent(&txMsg);
}

/**
 * Count
 * 
 * @param[in] dataCoding Data coding.
 * @param[in] data Data with format defined by byte 0.  (array[7])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSignalLevel(uint8_t dataCoding, uint8_t const * const data, uint8_t dataSize)
{
	vscp_TxMessage	txMsg;
	uint8_t       	size	= 0;
	uint8_t       	index	= 0;

	if (NULL == data) || (0 == dataSize)
	{
		return FALSE;
	}

	vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DATA, VSCP_TYPE_DATA_SIGNAL_LEVEL, VSCP_PRIORITY_3_NORMAL);

	txMsg.data[0] = dataCoding;
	size += 1;

	for(index = 0; index < dataSize; ++index)
	{
		txMsg.data[1 + index] = data[index];
		size += 1;

		if (VSCP_L1_DATA_SIZE <= size)
		{
			break;
		}
	}

	txMsg.dataNum = size;

	return vscp_core_sendEvent(&txMsg);
}

/**
 * Count
 * 
 * @param[in] dataCoding Data coding.
 * @param[in] data Data with format defined by byte 0.  (array[7])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSignalQuality(uint8_t dataCoding, uint8_t const * const data, uint8_t dataSize)
{
	vscp_TxMessage	txMsg;
	uint8_t       	size	= 0;
	uint8_t       	index	= 0;

	if (NULL == data) || (0 == dataSize)
	{
		return FALSE;
	}

	vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_DATA, VSCP_TYPE_DATA_SIGNAL_QUALITY, VSCP_PRIORITY_3_NORMAL);

	txMsg.data[0] = dataCoding;
	size += 1;

	for(index = 0; index < dataSize; ++index)
	{
		txMsg.data[1 + index] = data[index];
		size += 1;

		if (VSCP_L1_DATA_SIZE <= size)
		{
			break;
		}
	}

	txMsg.dataNum = size;

	return vscp_core_sendEvent(&txMsg);
}

