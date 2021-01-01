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
@brief  Logging
@file   log.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides some logging possibilities.

*******************************************************************************/
/** @defgroup log Logging
 * Use the logging for debugging or user notification.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __LOG_H__
#define __LOG_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>

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

/** Log a debug message. */
#define LOG_DEBUG(__msg)                    log_write(__FILE__, __LINE__, LOG_LEVEL_DEBUG, (__msg))

/** Log a debug message with one uint32_t parameter. */
#define LOG_DEBUG_UINT32(__msg, __par)      log_writeParUInt32(__FILE__, __LINE__, LOG_LEVEL_DEBUG, (__msg), (__par))

/** Log a debug message with one int32_t parameter. */
#define LOG_DEBUG_INT32(__msg, __par)       log_writeParInt32(__FILE__, __LINE__, LOG_LEVEL_DEBUG, (__msg), (__par))

/** Log a debug message with a string as parameter. */
#define LOG_DEBUG_STR(__msg, __par)         log_writeParStr(__FILE__, __LINE__, LOG_LEVEL_DEBUG, (__msg), (__par))

/** Log a info message. */
#define LOG_INFO(__msg)                     log_write(__FILE__, __LINE__, LOG_LEVEL_INFO, (__msg))

/** Log a info message with one parameter. */
#define LOG_INFO_UINT32(__msg, __par)       log_writeParUInt32(__FILE__, __LINE__, LOG_LEVEL_INFO, (__msg), (__par))

/** Log a info message with one int32_t parameter. */
#define LOG_INFO_INT32(__msg, __par)        log_writeParInt32(__FILE__, __LINE__, LOG_LEVEL_INFO, (__msg), (__par))

/** Log a info message with a string as parameter. */
#define LOG_INFO_STR(__msg, __par)          log_writeParStr(__FILE__, __LINE__, LOG_LEVEL_INFO, (__msg), (__par))

/** Log a warning message. */
#define LOG_WARNING(__msg)                  log_write(__FILE__, __LINE__, LOG_LEVEL_WARNING, (__msg))

/** Log a warning message with one parameter. */
#define LOG_WARNING_UINT32(__msg, __par)    log_writeParUInt32(__FILE__, __LINE__, LOG_LEVEL_WARNING, (__msg), (__par))

/** Log a warning message with one int32_t parameter. */
#define LOG_WARNING_INT32(__msg, __par)     log_writeParInt32(__FILE__, __LINE__, LOG_LEVEL_WARNING, (__msg), (__par))

/** Log a warning message with a string as parameter. */
#define LOG_WARNING_STR(__msg, __par)       log_writeParStr(__FILE__, __LINE__, LOG_LEVEL_WARNING, (__msg), (__par))

/** Log a error message. */
#define LOG_ERROR(__msg)                    log_write(__FILE__, __LINE__, LOG_LEVEL_ERROR, (__msg))

/** Log a error message with one parameter. */
#define LOG_ERROR_UINT32(__msg, __par)      log_writeParUInt32(__FILE__, __LINE__, LOG_LEVEL_ERROR, (__msg), (__par))

/** Log a error message with one int32_t parameter. */
#define LOG_ERROR_INT32(__msg, __par)       log_writeParInt32(__FILE__, __LINE__, LOG_LEVEL_ERROR, (__msg), (__par))

/** Log a error message with a string as parameter. */
#define LOG_ERROR_STR(__msg, __par)         log_writeParStr(__FILE__, __LINE__, LOG_LEVEL_ERROR, (__msg), (__par))

/** Log a fatal error message. */
#define LOG_FATAL(__msg)                    log_write(__FILE__, __LINE__, LOG_LEVEL_FATAL, (__msg))

/** Log a fatal error message with one parameter. */
#define LOG_FATAL_UINT32(__msg, __par)      log_writeParUInt32(__FILE__, __LINE__, LOG_LEVEL_FATAL, (__msg), (__par))

/** Log a fatal error message with one int32_t parameter. */
#define LOG_FATAL_INT32(__msg, __par)       log_writeParInt32(__FILE__, __LINE__, LOG_LEVEL_FATAL, (__msg), (__par))

/** Log a fatal error message with a string as parameter. */
#define LOG_FATAL_STR(__msg, __par)         log_writeParStr(__FILE__, __LINE__, LOG_LEVEL_FATAL, (__msg), (__par))

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the supported log levels. */
typedef enum
{
    LOG_LEVEL_INFO      = 0x01, /**< Info */
    LOG_LEVEL_DEBUG     = 0x02, /**< Debug */
    LOG_LEVEL_WARNING   = 0x04, /**< Warning */
    LOG_LEVEL_ERROR     = 0x08, /**< Error */
    LOG_LEVEL_FATAL     = 0x10  /**< Fatal error */

} LOG_LEVEL;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the module.
 */
extern void log_init(void);

/**
 * This function sets the log level. The log level can be a sum of several single
 * log level. Use OR to combine them.
 *
 * @param[in]   level   Log level
 */
extern void log_setLevel(uint8_t level);

/**
 * This function write a log message to the configured output stream. Don't call this
 * function direct, use the LOG_XXXXX macros instead!
 *
 * @param[in]   fileName    Module filename
 * @param[in]   line        Line number in the module
 * @param[in]   logLevel    Log level of the message
 * @param[in]   message     Message string
 */
extern void log_write(char const * const fileName, int line, uint8_t logLevel, char const * const message);

/**
 * This function write a log message to the configured output stream. Don't call this
 * function direct, use the LOG_XXXXX macros instead!
 *
 * @param[in]   fileName    Module filename
 * @param[in]   line        Line number in the module
 * @param[in]   logLevel    Log level of the message
 * @param[in]   message     Message string
 * @param[in]   par         Parameter
 */
extern void log_writeParUInt32(char const * const fileName, int line, uint8_t logLevel, char const * const message, uint32_t par);

/**
 * This function write a log message to the configured output stream. Don't call this
 * function direct, use the LOG_XXXXX macros instead!
 *
 * @param[in]   fileName    Module filename
 * @param[in]   line        Line number in the module
 * @param[in]   logLevel    Log level of the message
 * @param[in]   message     Message string
 * @param[in]   par         Parameter
 */
extern void log_writeParInt32(char const * const fileName, int line, uint8_t logLevel, char const * const message, int32_t par);

/**
 * This function write a log message to the configured output stream. Don't call this
 * function direct, use the LOG_XXXXX macros instead!
 *
 * @param[in]   fileName    Module filename
 * @param[in]   line        Line number in the module
 * @param[in]   logLevel    Log level of the message
 * @param[in]   message     Message string
 * @param[in]   par         Parameter
 */
extern void log_writeParStr(char const * const fileName, int line, uint8_t logLevel, char const * const message, char const * const par);

/**
 * This function behave like the standard printf, but it includes always the current
 * date and time in front.
 *
 * @param[in]   format  Format
 * @param[in]   ...     Variable number of parameters
 */
extern void log_printf(char const * const format, ...);

#ifdef __cplusplus
}
#endif

#endif  /* __LOG_H__ */

/** @} */
