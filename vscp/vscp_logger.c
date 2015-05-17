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
@brief  VSCP logger
@file   vscp_logger.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_logger.h

@section svn Subversion
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_logger.h"
#include "vscp_log.h"

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
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Current log level bitfield */
static uint8_t  vscp_logger_logLevel    = 0;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes the logger module.
 */
extern void vscp_logger_init(void)
{
    /* Nothing to do */

    return;
}

/**
 * This function gets the current log level(s).
 *
 * @return Current log level(s) (bitfield)
 */
extern uint8_t vscp_logger_getLogLevel(void)
{
    return vscp_logger_logLevel;
}

/**
 * This function sets the current log level(s).
 *
 * @param[in] level Log level(s) (bitfield)
 */
extern void vscp_logger_setLogLevel(uint8_t level)
{
    vscp_logger_logLevel = level;
    
    return;
}

/**
 * This function sends a log message, dependend on the active log level.
 *
 * @param[in] id    Message id
 * @param[in] level Log level
 * @param[in] msg   Message
 * @param[in] size  Message size in bytes
 */
extern void vscp_logger_log(uint8_t id, VSCP_LOGGER_LVL level, uint8_t const * const msg, uint8_t size)
{   
    if (0 != (vscp_logger_logLevel & (1 << level)))
    {
        (void)vscp_log_sendLogEvent(id, level, msg, size);
    }

    return;
}

/**
 * This function sends a log message, dependend on the active log level.
 *
 * @param[in] id    Message id
 * @param[in] level Log level
 * @param[in] value Value
 */
extern void vscp_logger_logUInt32(uint8_t id, VSCP_LOGGER_LVL level, uint32_t value)
{
    /* Value will be logged MSB first. */
    uint8_t buffer[4] = { ((uint8_t*)&value)[3], ((uint8_t*)&value)[2], ((uint8_t*)&value)[1], ((uint8_t*)&value)[0] };

    vscp_logger_log(id, level, buffer, sizeof(buffer));

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
