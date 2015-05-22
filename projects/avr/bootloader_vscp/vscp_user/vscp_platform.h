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
@brief  VSCP platform specific stuff
@file   vscp_platform.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This header file contains platform specific header files, types and etc.

*******************************************************************************/
/** @defgroup vscp_platform VSCP platform specific stuff
 * This header file contains platform specific header files, types and etc.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __VSCP_PLATFORM_H__
#define __VSCP_PLATFORM_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "system.h"

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

/** Define here the boot size configuration */
#define VSCP_PLATFORM_BOOTSZ_CONFIG 0

#if defined(__AVR_ATmega328P__)

#if (0 == VSCP_PLATFORM_BOOTSZ_CONFIG)

/** Boot section size in bytes */
#define VSCP_PLATFORM_BL_SECTION_SIZE   (4096u)

#elif (1 == VSCP_PLATFORM_BOOTSZ_CONFIG)

/** Boot section size in bytes */
#define VSCP_PLATFORM_BL_SECTION_SIZE   (2048u)

#elif (2 == VSCP_PLATFORM_BOOTSZ_CONFIG)

/** Boot section size in bytes */
#define VSCP_PLATFORM_BL_SECTION_SIZE   (1024u)

#elif (3 == VSCP_PLATFORM_BOOTSZ_CONFIG)

/** Boot section size in bytes */
#define VSCP_PLATFORM_BL_SECTION_SIZE   (512u)

#else

#error Boot section size not supported!

#endif

#else   /* not defined(__AVR_ATmega328P__) */

#error AVR MCU not supported.

#endif  /* not defined(__AVR_ATmega328P__) */

/** Flash page size */
#define VSCP_PLATFORM_FLASH_PAGE_SIZE (SPM_PAGESIZE)

/** Number of pages for the application section */
#define VSCP_PLATFORM_FLASH_NUM_PAGES ((FLASHEND + 1u - VSCP_PLATFORM_BL_SECTION_SIZE) / VSCP_PLATFORM_FLASH_PAGE_SIZE)

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

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_PLATFORM_H__ */

/** @} */
