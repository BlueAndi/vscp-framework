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
@brief  CRC16-CCITT algorithm
@file   crc16ccitt.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains the CRC16-CCITT algorithm.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/
#ifndef __CRC16CCITT_H__
#define __CRC16CCITT_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/** The fast calculation uses a look-up table. */
/* #define CRC16CCITT_FAST */

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** The type of the CRC values. */
typedef uint16_t    Crc16CCITT;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * Calculate the initial crc value.
 *
 * @return The initial crc value.
 */
extern Crc16CCITT   crc16ccitt_init(void);

/**
 * Update the crc value with new data.
 *
 * @param crc   The current crc value.
 * @param data  Pointer to a buffer of @a size bytes.
 * @param size  Number of bytes in the @a data buffer.
 * @return      The updated crc value.
 */
extern Crc16CCITT   crc16ccitt_update(Crc16CCITT crc, const uint8_t *data, size_t size);

/**
 * Calculate the final crc value.
 *
 * @param crc  The current crc value.
 * @return     The final crc value.
 */
extern Crc16CCITT   crc16ccitt_finalize(Crc16CCITT crc);

/**
 * Calculates the CRC16-CCITT of the given data.
 *
 * @param data  Data buffer
 * @param size  Size of the data buffer
 * @return CRC16-CCITT
 */
extern Crc16CCITT   crc16ccitt_calculate(uint8_t *data, size_t size);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __CRC16CCITT_H__ */
