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
@brief  Error
@file   vscp_evt_error.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Error reporting functionality that follow the errors defined in the vscp.h header file.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_EVT_ERROR_H__
#define __VSCP_EVT_ERROR_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_platform.h"

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

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * Success
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendSuccess(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Channel error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendChannelError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Fifo empty error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendFifoEmptyError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Fifo full error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendFifoFullError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Fifo size error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendFifoSizeError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Fifo wait error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendFifoWaitError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Generic error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendGenericError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Hardware error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendHardwareError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * initialization error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendInitializationError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Missing initialization error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendMissingInitializationError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Initialization ready
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendInitializationReady(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Not supported
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendNotSupported(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Overrun error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendOverrunError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Receiver empty error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendReceiverEmptyError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Register error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendRegisterError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Transmitter full error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendTransmitterFullError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Library error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendLibraryError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Procedural address error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendProceduralAddressError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Only one instance error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendOnlyOneInstanceError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Sub driver error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendSubDriverError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Timeout error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendTimeoutError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Not open error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendNotOpenError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Parameter error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendParameterError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Memory error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendMemoryError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Internal error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendInternalError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Communication error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendCommunicationError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * User error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendUserError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Password error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendPasswordError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Connection error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendConnectionError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Invalid handle error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendInvalidHandleError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Operation failed error
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendOperationFailedError(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Supplied buffer is to small to fit content
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendSuppliedBufferIsToSmallToFitContent(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Requested item is unknown
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendRequestedItemIsUnknown(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Name is already in use
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendNameIsAlreadyInUse(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Error when writing data
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendErrorWhenWritingData(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Operation stopped or aborted
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendOperationStoppedOrAborted(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

/**
 * Pointer with invalid value
 * 
 * @param[in] index Index. Often used as an index for channels/subdevices within a module.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] user Can be present or not be present. If present the bytes give additional user
 * specific information. (optional) (array[5])
 * @param[in] usersize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_error_sendPointerWithInvalidValue(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const user, uint8_t userSize);

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_EVT_ERROR_H__ */
