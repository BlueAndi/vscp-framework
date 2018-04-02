/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2018, Andreas Merkle
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
@brief  VSCP test stubs
@file   vscp_stubs.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains all VSCP related stubbed functions, which are called by
VSCP.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __VSCP_STUBS_H__
#define __VSCP_STUBS_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_types.h"
#include "vscp_dm.h"

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

extern void vscp_test_psAccessInit(void);
extern uint8_t  vscp_test_psAccessRead8(uint16_t addr);
extern void vscp_test_psAccessWrite8(uint16_t addr, uint8_t value);

extern void vscp_test_appRegInit(void);

extern void vscp_test_tpAdatperInit(void);
extern BOOL vscp_test_tpAdatperReadMessage(vscp_RxMessage * const msg);
extern BOOL vscp_test_tpAdatperWriteMessage(vscp_TxMessage const * const msg);

extern void vscp_test_portableInit(void);
extern void vscp_test_portableRestoreFactoryDefaultSettings(void);
extern void vscp_test_timerInit(void);
extern uint8_t  vscp_test_timerCreate(void);
extern void vscp_test_timerStart(uint8_t id, uint16_t value);
extern void vscp_test_timerStop(uint8_t id);
extern BOOL vscp_test_timerGetStatus(uint8_t id);
extern void vscp_test_portableResetRequest(void);
extern void vscp_test_portableIdleStateEntered(void);
extern void vscp_test_portableErrorStateEntered(void);
extern uint8_t  vscp_test_portableGetBootLoaderAlgorithm(void);
extern uint8_t  vscp_test_portableGetPagesUsed(void);
extern void vscp_test_portableBootLoaderRequest(void);
extern uint8_t  vscp_test_portableGetMdfUrl(uint8_t index);
extern void vscp_test_portableProvideEvent(vscp_RxMessage const * const msg);
extern void vscp_test_portableUpdateTimeSinceEpoch(uint32_t timestamp);
extern void vscp_test_portableSetLampState(VSCP_LAMP_STATE state);

extern void vscp_test_actionInit(void);
extern void vscp_test_actionExecute(uint8_t action, uint8_t par, vscp_RxMessage const * const msg);

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_STUBS_H__ */
