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
@brief  EEPROM simulation
@file   eeprom.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module simulates a EEPROM.

*******************************************************************************/
/** @defgroup eeprom EEPROM simulation
 * This module simulates a EEPROM.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __EEPROM_H__
#define __EEPROM_H__

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

/** The EEPROM storage has always to be a multiple of this number. */
#define EEPROM_STORAGE_MULTIPLE 8

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
 * This function intializes the EEPROM simulation.
 *
 * @param[in] size  EEPROM size in bytes
 */
extern void eeprom_init(uint16_t size);

/**
 * This function de-initializes the EEPROM simulation.
 */
extern void eeprom_deInit(void);

/**
 * Load file to EEPROM memory.
 *
 * @param[in] fileName  Filename of EEPROM file
 */
extern void eeprom_load(char* fileName);

/**
 * Save the EEPROM to file.
 *
 * @param[in] fileName  Filename of EEPROM file
 */
extern void eeprom_save(char* fileName);

/**
 * Read a single byte from the EEPROM.
 *
 * @param[in]   addr    Address in EEPROM
 * @return  Value
 */
extern uint8_t  eeprom_read8(uint16_t addr);

/**
 * Write a single byte to the EEPROM.
 *
 * @param[in]   addr    Address in EEPROM
 * @param[in]   value   Value to write
 */
extern void eeprom_write8(uint16_t addr, uint8_t value);

/**
 * Get base address of EEPROM storage in memory.
 *
 * @param[out] size EEPROM memory size
 * @return EEPROM memory base address
 */
extern uint8_t* eeprom_getBase(uint16_t* size);

/**
 * Dump the EEPROM to console.
 */
extern void eeprom_dump(void);

#ifdef __cplusplus
}
#endif

#endif  /* __EEPROM_H__ */

/** @} */
