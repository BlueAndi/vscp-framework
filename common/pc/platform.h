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
@brief  Platform specific utility functions.
@file   platform.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides utility functions which are platform specific. The module
encapsulates the platform dependent code.

*******************************************************************************/
/** @defgroup platform Platform specific utility functions.
 * This module provides utility functions which are platform specific. The module
 * encapsulates the platform dependent code.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif  /* _WIN32 */

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

#ifndef BOOL
/** Boolean type */
#define BOOL    int
#endif  /* BOOL */

#ifndef FALSE
/** Boolean false value */
#define FALSE   (0)
#endif  /* FALSE */

#ifndef TRUE
/** Boolean true value */
#define TRUE    (!FALSE)
#endif  /* FALSE */

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the supported colors. */
typedef enum
{
    PLATFORM_COLOR_BLACK = 0,
    PLATFORM_COLOR_RED,
    PLATFORM_COLOR_GREEN,
    PLATFORM_COLOR_YELLOW,
    PLATFORM_COLOR_BLUE,
    PLATFORM_COLOR_MAGENTA,
    PLATFORM_COLOR_CYAN,
    PLATFORM_COLOR_GREY,
    PLATFORM_COLOR_LIGHT_RED,
    PLATFORM_COLOR_LIGHT_GREEN,
    PLATFORM_COLOR_LIGHT_YELLOW,
    PLATFORM_COLOR_LIGHT_BLUE,
    PLATFORM_COLOR_LIGHT_MAGENTA,
    PLATFORM_COLOR_LIGHT_CYAN,
    PLATFORM_COLOR_WHITE

} PLATFORM_COLOR;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function initializes the module.
 */
extern void platform_init(void);

/**
 * This function de-initializes the module.
 */
extern void platform_deInit(void);

/**
 * Disable the console echo.
 */
extern void platform_echoOff(void);

/**
 * Enable the console echo.
 */
extern void platform_echoOn(void);

/**
 * This function is not defined as part of the ANSI C/C++ standard.
 * It is generally used by Borland's family of compilers. It returns a positive
 * non-zero integer if a key is in the keyboard buffer. It will not wait for a
 * key to be pressed.
 *
 * @return Status
 * @retval 0    No key pressed
 * @retval > 0  Key pressed
 * @retval < 0  Error
 */
extern int platform_kbhit(void);

/**
 * This function is not defined as part of the ANSI C/C++ standard.
 * It is generally used by Borland's family of compilers. It waits until a key is
 * pressed and returns its key value.
 *
 * @return Key value
 * @retval 0    Aborted
 * @retval < 0  Key value
 * @retval -1   Error
 */
extern int platform_getch(void);

/**
 * This function suspends the execution of the current thread until the time-out
 * interval elapses.
 *
 * @param[in] valueMS   Timeout value in ms
 */
extern void platform_delay(long valueMS);

/**
 * Set text foreground color.
 *
 * @param[in] color Color
 */
extern void platform_setTextColor(PLATFORM_COLOR color);

/**
 * Set text background color.
 *
 * @param[in] color Color
 */
extern void platform_setTextBgColor(PLATFORM_COLOR color);

/**
 * Restore the text color to the default value.
 */
extern void platform_restoreTextColor(void);

/**
 * Restore the text background color to the default value.
 */
extern void platform_restoreTextBgColor(void);

#ifdef __cplusplus
}
#endif

#endif  /* __PLATFORM_H__ */

/** @} */
