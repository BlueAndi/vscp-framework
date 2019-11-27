/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2019, Andreas Merkle
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
@brief  IR Remote I/f
@file   vscp_evt_ir.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This is the IR code sent/received from common remote controls.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_EVT_IR_H__
#define __VSCP_EVT_IR_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_platform.h"

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
 * General event
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_ir_sendGeneralEvent(void);

/**
 * RC5 Send/Receive
 * 
 * @param[in] code RC5 code.
 * @param[in] adress RC5 Address.
 * @param[in] repeat Repeat count if any.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_ir_sendRc5SendReceive(uint8_t code, uint8_t adress, uint8_t repeat);

/**
 * SONY 12-bit Send/Receive
 * 
 * @param[in] code SONY code.
 * @param[in] adress SONY Address.
 * @param[in] repeat Repeat count if any.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_ir_sendSony12BitSendReceive(uint8_t code, uint8_t adress, uint8_t repeat);

/**
 * LIRC (Linux Infrared Remote Control)
 * 
 * @param[in] code6 LIRC Code, MSB
 * @param[in] code5 LIRC Code
 * @param[in] code4 LIRC Code
 * @param[in] code3 LIRC Code
 * @param[in] code2 LIRC Code
 * @param[in] code1 LIRC Code
 * @param[in] code0 LIRC Code, LSB
 * @param[in] repeat Repeat count if any.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_ir_sendLircLinuxInfraredRemoteControl(uint8_t code6, uint8_t code5, uint8_t code4, uint8_t code3, uint8_t code2, uint8_t code1, uint8_t code0, uint8_t repeat);

/**
 * VSCP Abstract Remote Format
 * 
 * @param[in] code Code.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which the event applies to (0-255). 255 is all sub-zones.
 * @param[in] repeat Repeat count if any.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_ir_sendVscpAbstractRemoteFormat(uint16_t code, uint8_t zone, uint8_t subZone, uint8_t repeat);

/**
 * MAPito Remote Format
 * 
 * @param[in] repeat Repeat count if any.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which the event applies to (0-255). 255 is all sub-zones.
 * @param[in] address Control address.
 * @param[in] keyCode Key-code.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_ir_sendMapitoRemoteFormat(uint8_t repeat, uint8_t zone, uint8_t subZone, uint32_t address, uint8_t keyCode);

#endif  /* __VSCP_EVT_IR_H__ */