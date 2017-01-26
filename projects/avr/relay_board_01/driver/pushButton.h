/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2017, Andreas Merkle
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
@brief  Push button driver
@file   pushButton.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
The pushbutton driver provides an easy function to read the current pushbutton
states.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __PUSHBUTTON_H__
#define __PUSHBUTTON_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <inttypes.h>

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
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern void pushButton_init(void);

/**
 *   Read the current hardware status of all pushbuttons and return it.
 *
 *   MSB-------------------------------------------LSB
 *   | PB8 | PB7 | PB6 | PB5 | PB4 | PB3 | PB2 | PB1 |
 *   -------------------------------------------------
 *   A low bit means the pushbutton is pressed and a high bit means the
 *   pushbutton is released.
 *
 *   @return Status of the pushbuttons.
 */
extern uint8_t pushButton_readStatus(void);

#ifdef __cplusplus
}
#endif

#endif  /* __PUSHBUTTON_H__ */
