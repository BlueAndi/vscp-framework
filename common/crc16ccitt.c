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
@file   crc16ccitt.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see crc16ccitt.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "crc16ccitt.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/*
 *                                16  12  5
 * The CCITT CRC 16 polynomial is X + X + X + 1.
 * In binary, this is the bit pattern 1 0001 0000 0010 0001, and in hex it
 * is 0x11021.
 * A 17 bit register is simulated by testing the MSB before shifting
 * the data, which affords us the luxury of specify the polynomial as a
 * 16 bit value, 0x1021.
 */
#define CRC16CCITT_POLYNOM  (0x1021)

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

#ifdef CRC16CCITT_FAST

static Crc16CCITT   crc16ccitt_updateFast(Crc16CCITT crc, const uint8_t *data, size_t size);

#else   /* Not defined CRC16CCITT_FAST */

static Crc16CCITT   crc16ccitt_updateAlgo(Crc16CCITT crc, const uint8_t *data, size_t size);

#endif  /* Not defined CRC16CCITT_FAST */

/*******************************************************************************
    LOCALE VARIABLES
*******************************************************************************/

#ifdef CRC16CCITT_FAST

/** Static table used for the table_driven implementation. */
static const Crc16CCITT crc_table[256] = {
    0x0000u, 0x1021u, 0x2042u, 0x3063u, 0x4084u, 0x50a5u, 0x60c6u, 0x70e7u,
    0x8108u, 0x9129u, 0xa14au, 0xb16bu, 0xc18cu, 0xd1adu, 0xe1ceu, 0xf1efu,
    0x1231u, 0x0210u, 0x3273u, 0x2252u, 0x52b5u, 0x4294u, 0x72f7u, 0x62d6u,
    0x9339u, 0x8318u, 0xb37bu, 0xa35au, 0xd3bdu, 0xc39cu, 0xf3ffu, 0xe3deu,
    0x2462u, 0x3443u, 0x0420u, 0x1401u, 0x64e6u, 0x74c7u, 0x44a4u, 0x5485u,
    0xa56au, 0xb54bu, 0x8528u, 0x9509u, 0xe5eeu, 0xf5cfu, 0xc5acu, 0xd58du,
    0x3653u, 0x2672u, 0x1611u, 0x0630u, 0x76d7u, 0x66f6u, 0x5695u, 0x46b4u,
    0xb75bu, 0xa77au, 0x9719u, 0x8738u, 0xf7dfu, 0xe7feu, 0xd79du, 0xc7bcu,
    0x48c4u, 0x58e5u, 0x6886u, 0x78a7u, 0x0840u, 0x1861u, 0x2802u, 0x3823u,
    0xc9ccu, 0xd9edu, 0xe98eu, 0xf9afu, 0x8948u, 0x9969u, 0xa90au, 0xb92bu,
    0x5af5u, 0x4ad4u, 0x7ab7u, 0x6a96u, 0x1a71u, 0x0a50u, 0x3a33u, 0x2a12u,
    0xdbfdu, 0xcbdcu, 0xfbbfu, 0xeb9eu, 0x9b79u, 0x8b58u, 0xbb3bu, 0xab1au,
    0x6ca6u, 0x7c87u, 0x4ce4u, 0x5cc5u, 0x2c22u, 0x3c03u, 0x0c60u, 0x1c41u,
    0xedaeu, 0xfd8fu, 0xcdecu, 0xddcdu, 0xad2au, 0xbd0bu, 0x8d68u, 0x9d49u,
    0x7e97u, 0x6eb6u, 0x5ed5u, 0x4ef4u, 0x3e13u, 0x2e32u, 0x1e51u, 0x0e70u,
    0xff9fu, 0xefbeu, 0xdfddu, 0xcffcu, 0xbf1bu, 0xaf3au, 0x9f59u, 0x8f78u,
    0x9188u, 0x81a9u, 0xb1cau, 0xa1ebu, 0xd10cu, 0xc12du, 0xf14eu, 0xe16fu,
    0x1080u, 0x00a1u, 0x30c2u, 0x20e3u, 0x5004u, 0x4025u, 0x7046u, 0x6067u,
    0x83b9u, 0x9398u, 0xa3fbu, 0xb3dau, 0xc33du, 0xd31cu, 0xe37fu, 0xf35eu,
    0x02b1u, 0x1290u, 0x22f3u, 0x32d2u, 0x4235u, 0x5214u, 0x6277u, 0x7256u,
    0xb5eau, 0xa5cbu, 0x95a8u, 0x8589u, 0xf56eu, 0xe54fu, 0xd52cu, 0xc50du,
    0x34e2u, 0x24c3u, 0x14a0u, 0x0481u, 0x7466u, 0x6447u, 0x5424u, 0x4405u,
    0xa7dbu, 0xb7fau, 0x8799u, 0x97b8u, 0xe75fu, 0xf77eu, 0xc71du, 0xd73cu,
    0x26d3u, 0x36f2u, 0x0691u, 0x16b0u, 0x6657u, 0x7676u, 0x4615u, 0x5634u,
    0xd94cu, 0xc96du, 0xf90eu, 0xe92fu, 0x99c8u, 0x89e9u, 0xb98au, 0xa9abu,
    0x5844u, 0x4865u, 0x7806u, 0x6827u, 0x18c0u, 0x08e1u, 0x3882u, 0x28a3u,
    0xcb7du, 0xdb5cu, 0xeb3fu, 0xfb1eu, 0x8bf9u, 0x9bd8u, 0xabbbu, 0xbb9au,
    0x4a75u, 0x5a54u, 0x6a37u, 0x7a16u, 0x0af1u, 0x1ad0u, 0x2ab3u, 0x3a92u,
    0xfd2eu, 0xed0fu, 0xdd6cu, 0xcd4du, 0xbdaau, 0xad8bu, 0x9de8u, 0x8dc9u,
    0x7c26u, 0x6c07u, 0x5c64u, 0x4c45u, 0x3ca2u, 0x2c83u, 0x1ce0u, 0x0cc1u,
    0xef1fu, 0xff3eu, 0xcf5du, 0xdf7cu, 0xaf9bu, 0xbfbau, 0x8fd9u, 0x9ff8u,
    0x6e17u, 0x7e36u, 0x4e55u, 0x5e74u, 0x2e93u, 0x3eb2u, 0x0ed1u, 0x1ef0u
};

#endif  /* CRC16CCITT_FAST */

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * Calculate the initial crc value.
 *
 * @return The initial crc value.
 */
extern Crc16CCITT   crc16ccitt_init(void)
{
    return 0xffffu;
}

/**
 * Update the crc value with new data.
 *
 * @param crc   The current crc value.
 * @param data  Pointer to a buffer of @a size bytes.
 * @param size  Number of bytes in the @a data buffer.
 * @return      The updated crc value.
 */
extern Crc16CCITT   crc16ccitt_update(Crc16CCITT crc, const uint8_t *data, size_t size)
{
#ifdef CRC16CCITT_FAST

    return crc16ccitt_updateFast(crc, data, size);

#else   /* Not defined CRC16CCITT_FAST */

    return crc16ccitt_updateAlgo(crc, data, size);

#endif  /* Not defined CRC16CCITT_FAST */
}

/**
 * Calculate the final crc value.
 *
 * @param crc  The current crc value.
 * @return     The final crc value.
 */
extern Crc16CCITT   crc16ccitt_finalize(Crc16CCITT crc)
{
    return crc ^ 0x0000u;
}

/**
 * Calculates the CRC16-CCITT of the given data.
 *
 * @param data  Data buffer
 * @param size  Size of the data buffer
 * @return CRC16-CCITT
 */
extern Crc16CCITT   crc16ccitt_calculate(uint8_t *data, size_t size)
{
    Crc16CCITT  crc = crc16ccitt_init();

    crc = crc16ccitt_update(crc, data, size);

    return crc16ccitt_finalize(crc);
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

#ifdef CRC16CCITT_FAST

/**
 * Update the crc value with new data.
 *
 * @param crc   The current crc value.
 * @param data  Pointer to a buffer of @a size bytes.
 * @param size  Number of bytes in the @a data buffer.
 * @return      The updated crc value.
 */
static Crc16CCITT   crc16ccitt_updateFast(Crc16CCITT crc, const uint8_t *data, size_t size)
{
    unsigned int    tblIdx = 0;

    if (NULL != data)
    {
        while (size--)
        {
            tblIdx = ((crc >> 8u) ^ *data) & 0xffu;
            crc = (crc_table[tblIdx] ^ (crc << 8u)) & 0xffffu;

            data++;
        }
    }

    return crc & 0xffffu;
}

#else   /* Not defined CRC16CCITT_FAST */

/**
 * Update the crc value with new data.
 * This routine generates the 16 bit remainder of a block of
 * data using the ccitt polynomial generator.
 *
 * Notes:
 * The CRC is preset to all 1's to detect errors involving a loss
 * of leading zero's.
 * The CRC (a 16 bit value) is generated in LSB MSB order.
 * Two ways to verify the integrity of a received message or block of data:
 *
 * 1) Calculate the crc on the data, and compare it to the crc
 *    calculated previously. The location of the saved crc must be
 *    known.
 *
 * 2) Append the calculated crc to the end of the data. Now calculate
 *    the crc of the data and its crc. If the new crc equals the
 *    value in "crc_ok", the data is valid.
 *
 * @param crc   The current crc value.
 * @param data  Pointer to a buffer of @a size bytes.
 * @param size  Number of bytes in the @a data buffer.
 * @return      The updated crc value.
 */
static Crc16CCITT   crc16ccitt_updateAlgo(Crc16CCITT crc, const uint8_t *data, size_t size)
{
    uint8_t dataIndex   = 0;
    uint8_t bitIndex    = 0;

    do
    {
        crc ^= ((uint16_t)data[dataIndex]) << 8;

        for (bitIndex = 0; bitIndex < 8; ++bitIndex)
        {
            if (crc & (1 << 15))
            {
                crc = (crc << 1) ^ CRC16CCITT_POLYNOM;
            }
            else
            {
                crc = crc << 1;
            }
        }

        ++dataIndex;
    }
    while(size > dataIndex);

    return crc;
}

#endif  /* Not defined CRC16CCITT_FAST */
