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
@file   log.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see log.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

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

static char const * const   log_getLogLevelStr(LOG_LEVEL level);
static const char * log_getFileNameOnly(char const * const fileName);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** The current log level in the system. */
static unsigned int     log_logLevel    = 0;

/** Mutex used to protect the log_printf() */
static pthread_mutex_t  log_mutex       = PTHREAD_MUTEX_INITIALIZER;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the module.
 */
extern void log_init(void)
{
    /* Nothing to do */

    return;
}

/**
 * This function sets the log level. The log level can be a sum of several single
 * log level. Use OR to combine them.
 *
 * @param[in]   level   Log level
 */
extern void log_setLevel(uint8_t level)
{
    log_logLevel = level;
    return;
}

/**
 * This function write a log message to the configured output stream. Don't call this
 * function direct, use the LOG_XXXXX macros instead!
 *
 * @param[in]   fileName    Module filename
 * @param[in]   line        Line number in the module
 * @param[in]   logLevel    Log level of the message
 * @param[in]   message     Message string
 */
extern void log_write(char const * const fileName, int line, uint8_t logLevel, char const * const message)
{
    if (0 != (log_logLevel & logLevel))
    {
        log_printf("%s (%d) - %s: %s\n", log_getFileNameOnly(fileName), line, log_getLogLevelStr(logLevel), message);
    }

    return;
}

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
extern void log_writeParUInt32(char const * const fileName, int line, uint8_t logLevel, char const * const message, uint32_t par)
{
    if (0 != (log_logLevel & logLevel))
    {
        log_printf("%s (%d) - %s: %s %u\n", log_getFileNameOnly(fileName), line, log_getLogLevelStr(logLevel), message, par);
    }

    return;
}

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
extern void log_writeParInt32(char const * const fileName, int line, uint8_t logLevel, char const * const message, int32_t par)
{
    if (0 != (log_logLevel & logLevel))
    {
        log_printf("%s (%d) - %s: %s %d\n", log_getFileNameOnly(fileName), line, log_getLogLevelStr(logLevel), message, par);
    }

    return;
}

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
extern void log_writeParStr(char const * const fileName, int line, uint8_t logLevel, char const * const message, char const * const par)
{
    if (0 != (log_logLevel & logLevel))
    {
        log_printf("%s (%d) - %s: %s %s\n", log_getFileNameOnly(fileName), line, log_getLogLevelStr(logLevel), message, par);
    }

    return;
}

/**
 * This function behave like the standard printf, but it includes always the current
 * date and time in front.
 *
 * @param[in]   format  Format
 * @param[in]   ...     Variable number of parameters
 */
extern void log_printf(char const * const format, ...)
{
    va_list     args;
    time_t      rawtime;
    struct tm * timeInfo;

    time(&rawtime);
    timeInfo = localtime(&rawtime);

    (void)pthread_mutex_lock(&log_mutex);

    printf("%04u-%02u-%02u %02u:%02u:%02u ",
        timeInfo->tm_year + 1900,
        timeInfo->tm_mon + 1,
        timeInfo->tm_mday,
        timeInfo->tm_hour,
        timeInfo->tm_min,
        timeInfo->tm_sec);

    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    (void)pthread_mutex_unlock(&log_mutex);

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * Get a user friendly string for a given log level.
 *
 * @param[in]   level   Log level
 * @return User friendly description of log level
 */
static char const * const   log_getLogLevelStr(LOG_LEVEL level)
{
    switch(level)
    {
    case LOG_LEVEL_INFO:
        return "Info";

    case LOG_LEVEL_DEBUG:
        return "Debug";

    case LOG_LEVEL_WARNING:
        return "Warning";

    case LOG_LEVEL_ERROR:
        return "Error";

    case LOG_LEVEL_FATAL:
        return "Fatal";

    default:
        break;
    }

    return "Unknown";
}

/**
 * This function returns the position of the file name, without path.
 *
 * @param[in] fileName  File name with path
 * @return Pointer to the position of the file name
 */
static const char * log_getFileNameOnly(char const * const fileName)
{
    char const *    fileNameOnly    = NULL;

    if (NULL != fileName)
    {
        fileNameOnly = strrchr(fileName, '/');

        if (NULL == fileNameOnly)
        {
            fileNameOnly = strrchr(fileName, '\\');
        }

        /* File name has no path? */
        if (NULL == fileNameOnly)
        {
            fileNameOnly = fileName;
        }
        else
        {
            /* Overstep the (back)slash */
            fileNameOnly++;
        }
    }

    return fileNameOnly;
}
