/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2025 Andreas Merkle
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
@brief  VSCP persistent memory access driver
@file   vscp_ps_access.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_ps_access.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_ps_access.h"

#include <stdlib.h>

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

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes the persistent memory access driver.
 * It doesn't write anything in the persistent memory! It only initializes
 * the module that read/write access is possible.
 */
extern void vscp_ps_access_init(void)
{
    /* Implement your code here ... */

    return;
}

/**
 * Read a single byte from the persistent memory.
 *
 * @param[in]   addr    Address in persistent memory
 * @return  Value
 */
extern uint8_t  vscp_ps_access_read8(uint16_t addr)
{
    uint8_t data    = 0;

    /* Implement your code here ... */

    return data;
}

/**
 * Write a single byte to the persistent memory.
 *
 * @param[in]   addr    Address in persistent memory
 * @param[in]   value   Value to write
 */
extern void vscp_ps_access_write8(uint16_t addr, uint8_t value)
{
    /* Implement your code here ... */

    return;
}

/**
 * Read several bytes from the persistent memory.
 *
 * @param[in]   addr    Address in persistent memory
 * @param[in]   buffer  Buffer which to read in
 * @param[in]   size    Buffer size in byte (Number of data to read)
 */
extern void vscp_ps_access_readMultiple(uint16_t addr, uint8_t* const buffer, uint8_t size)
{
    /* Note, this function uses single byte access by default.
     * If necessary, adapt this for optimized access according to your needs.
     */
    if ((NULL != buffer) &&
        (0 < size))
    {
        uint8_t index   = 0;

        for(index = 0; index < size; ++index)
        {
            buffer[index] = vscp_ps_access_read8(addr + index);
        }
    }

    return;
}

/**
 * Write several bytes to the persistent memory.
 *
 * @param[in]   addr    Address in persistent memory
 * @param[in]   buffer  Buffer which to write
 * @param[in]   size    Bufer size in byte (Number of data to write)
 */
extern void vscp_ps_access_writeMultiple(uint16_t addr, const uint8_t* const buffer, uint8_t size)
{
    /* Note, this function uses single byte access by default.
     * If necessary, adapt this for optimized access according to your needs.
     */
    if ((NULL != buffer) &&
        (0 < size))
    {
        uint8_t index   = 0;

        for(index = 0; index < size; ++index)
        {
            vscp_ps_access_write8(addr + index, buffer[index]);
        }
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
