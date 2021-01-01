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
@brief  VSCP threads
@file   vscp_thread.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains the VSCP threads.

*******************************************************************************/
/** @defgroup vscp_thread VSCP threads
 * Two threads will be created to process the VSCP framework and the VSCP timers.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __VSCP_THREAD_H__
#define __VSCP_THREAD_H__

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

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the possible function return values. */
typedef enum
{
    VSCP_THREAD_RET_OK = 0, /**< Successful */
    VSCP_THREAD_RET_ERROR   /**< Failed */

} VSCP_THREAD_RET;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the module.
 */
extern void vscp_thread_init(void);

/**
 * This function starts the VSCP threads.
 *
 * @return Status
 */
extern VSCP_THREAD_RET vscp_thread_start(void);

/**
 * This function stops the VSCP threads.
 */
extern void vscp_thread_stop(void);

/**
 * This function locks the threads.
 */
extern void vscp_thread_lock(void);

/**
 * This function unlocks the threads.
 */
extern void vscp_thread_unlock(void);

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_THREAD_H__ */

/** @} */
