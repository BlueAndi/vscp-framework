/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2015, Andreas Merkle
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
@brief  VSCP class 1 type Log events
@file   vscp_log.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Logging functionality.

This file is automatically generated. Don't change it manually.

@section svn Subversion
$Author: amerkle $
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
*******************************************************************************/
/** @defgroup vscp_log Log events abstraction
 * Level 1 log events abstraction
 * @{
 * @ingroup vscp_l1_events_abstraction
 */

#ifndef __VSCP_LOG_H__
#define __VSCP_LOG_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_types.h"

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
 * General log event.
 *
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_log_sendUndefinedEvent(void);

/**
 * Message for Log. Several frames have to be sent for a event that take up more the
 * five bytes which is the maximum for each frame. In this case the zero based index
 * (byte 2) should be increased for each frame.
 *
 * @param[in] id ID for event.
 * @param[in] level Log level for message.
 * @param[in] msg Message.
 * @param[in] size Message size in bytes.
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_log_sendLogEvent(uint8_t id, uint8_t level, uint8_t const * const msg, uint8_t size);

/**
 * Start logging.
 *
 * @param[in] id ID for log.
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_log_sendLogStartEvent(uint8_t id);

/**
 * Stop logging.
 *
 * @param[in] id ID for log.
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_log_sendLogStopEvent(uint8_t id);

#endif /* __VSCP_LOG_H__ */

/** @} */
