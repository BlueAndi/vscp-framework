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
@brief  System specific defines, types and constants.
@file   system.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This header file contains system specific defines, types and constants.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

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
/** The boolean type on a 8-bit microcontroller shall be 8-bit. Normally boolean
 * has a int type, but this would lead to a 16-bit value.
 */
#define BOOL    char
#endif  /* BOOL */

#ifndef FALSE
/** Boolean false */
#define FALSE   (0)
#endif  /* FALSE */

#ifndef TRUE
/** Boolean true */
#define TRUE    (!FALSE)
#endif  /* TRUE */

/*******************************************************************************
    MACROS
*******************************************************************************/

/** No operation */
#define _NOP()  do{ asm volatile ("nop"); }while(0)

/** Set bit */
#define BIT_SET(__value, __bit)  do{ (__value) |= (1 << (__bit)); }while(0)

/** Clear bit */
#define BIT_CLR(__value, __bit)  do{ (__value) &= ~(1 << (__bit)); }while(0)

/** Halt program */
#define HALT()  do{ _NOP(); }while(1)

/** Reboot via watchdog */
#define REBOOT()    do{ wdt_enable(WDTO_15MS); HALT(); }while(0)

/** Calculates the number of elements in the given array. */
#define ARRAY_NUM(__array)  (sizeof(__array)/sizeof((__array)[0]))

/** Validate a pointer (checks for NULL). If the pointer is invalid it will return immediately. */
#define VALIDATE_PTR(__ptr, __ret)  \
    do{                             \
        if (NULL == (__ptr))        \
        { return (__ret); }         \
    }while(0)

/** Validate a context. If the context is invalid, it will return immediately. */
#define VALIDATE_CON(__con, __ret)              \
    do{                                         \
        if ((void*)(__con) != (__con)->valid)   \
        { return (__ret); }                     \
    }while(0)

/** Not used macro */
#define NOT_USED(__var) ((void)(__var))

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif  /* __SYSTEM_H__ */
