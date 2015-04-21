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
@brief  Button observer
@file   buttonObserver.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides a button observer. The observer sends VSCP events
in case of button state changes.

Differences between on and off with counted triggers:

<pre>
    Type 1
    If the max. on time is exceed, the callback will inform
    about on state with 0 pulse.

          max. on time
         <------------>
         *-------------------------*
         |                         |
    -----*                         *---------
                    callback    callback
                    (on, 0)     (off, 0)
                       |           |
    -----------------------------------------

    Type 2
    If the max. off time is exceed, the callback will inform
    about off state with 0 pulse. Precondition is type 1.

          max. off time
         <------------>
    -----*
         |
         *--------------------

                    callback
                       |
    -----------------------------------------


    Type 3
    If on time is lower or equal to the max. on time, the callback will
    inform about on state with 1 pulse.

                 max. off time
                <------------>
          max. on time
         <------------>
         *------*
         |      |
    -----*      *----------------------------

                           callback
                              |
    -----------------------------------------


    Type 4
    Only a example for 2 pulse.

                               max. off time
                              <------------>
                        max. on time
                       <------------>
                 max. off time
                <------------>
          max. on time
         <------------>
         *------*      *------*
         |      |      |      |
    -----*      *------*      *-----------------

                                         callback
                                            |
    --------------------------------------------
</pre>

@section svn Subversion
$Author: amerkle $
$Rev: 401 $
$Date: 2014-12-27 23:42:44 +0100 (Sa, 27 Dez 2014) $
*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __BUTTONOBSERVER_H__
#define __BUTTONOBSERVER_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "system.h"
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

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the different pushbutton states. */
typedef enum
{
    BUTTONOBSERVER_STATE_DISABLED   = 0,    /**< Button is disabled */
    BUTTONOBSERVER_STATE_RELEASED,          /**< Button is released */
    BUTTONOBSERVER_STATE_PRESSED            /**< Button pressed */

} BUTTONOBSERVER_STATE;

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
extern void buttonObserver_init(void);

/**
 * This function set a filter to disable some button observations.
 * The filter is configured bitwise. Bit 0 corresponds to button 1.
 * A 0 means, no observation.
 *
 * @param[in]   filter  Observation filter
 */
extern void buttonObserver_setFilter(uint8_t filter);

/**
 * This function process the pushbuttons. Call it every 10ms.
 */
extern void buttonObserver_process(void);

#ifdef __cplusplus
}
#endif

#endif  /* __BUTTONOBSERVER_H__ */
