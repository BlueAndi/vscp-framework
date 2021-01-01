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
@brief  VSCP bootloader adapter
@file   vscp_bl_adapter.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module adapts the project specific environment according to the bootloader
its needs.

*******************************************************************************/
/** @defgroup vscp_bl_adapter VSCP bootloader adapter
 * This module adapts the project specific environment according to the bootloader
 * its needs.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __VSCP_BL_ADAPTER_H__
#define __VSCP_BL_ADAPTER_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_types.h"

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
 * This function initializes the module.
 */
extern void vscp_bl_adapter_init(void);

/**
 * This function reads the nickname id of the node.
 *
 * @return  Nickname id
 */
extern uint8_t  vscp_bl_adapter_readNicknameId(void);

/**
 * This function jumps to the application and will never return.
 */
extern void vscp_bl_adapter_jumpToApp(void);

/**
 * This function enable or disable the initialization lamp.
 *
 * @param[in]   enableIt    Enable (true) or disable (false) lamp
 */
extern void vscp_bl_adapter_enableLamp(BOOL enableIt);

/**
 * This function stops the MCU and will never return.
 * It is called in any error case.
 */
extern void vscp_bl_adapter_halt(void);

/**
 * This function reboot the MCU and will never return.
 */
extern void vscp_bl_adapter_reboot(void);

/**
 * This function returns the state of the segment initialization button.
 *
 * @return State
 * @retval FALSE    Released
 * @retval TRUE     Pressed
 */
extern BOOL vscp_bl_adapter_getSegInitButtonState(void);

/**
 * This function reads the boot flag from persistent memory.
 *
 * @return Boot flag
 */
extern uint8_t vscp_bl_adapter_readBootFlag(void);

/**
 * This function writes the boot flag to persistent memory.
 *
 * @param[in]   bootFlag    Boot flag
 */
extern void vscp_bl_adapter_writeBootFlag(uint8_t bootFlag);

/**
 * Read byte @see index of the GUID from persistent memory.
 * Note that index 0 is the LSB and index 15 the MSB.
 *
 * @param[in]   index   Index of the GUID [0-15]
 * @return  GUID byte @see index
 */
extern uint8_t  vscp_bl_adapter_readGUID(uint8_t index);

/**
 * This function writes a complete block to the program memory.
 *
 * @param[in]   blockNo Block number of block which shall be written
 * @param[in]   buffer  Pointer to the buffer with the data
 */
extern void vscp_bl_adapter_programBlock(uint32_t blockNo, uint8_t *buffer);

/**
 * This function read a byte from program memory.
 *
 * @param[in] address   Program memory address
 * @return Value
 */
extern uint8_t  vscp_bl_adapter_readProgMem(uint16_t address);

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_BL_ADAPTER_H__ */

/** @} */
