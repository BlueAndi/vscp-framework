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
@brief  VSCP transport layer adapter
@file   vscp_tp_adapter.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module adapts the project specific underlying physical transport medium to
the transport layer of VSCP.

*******************************************************************************/
/** @defgroup vscp_tp_adapter Transport driver adapter
 * The transport layer adapter adapts the transport layer of VSCP to the
 * underlying physical transport medium, e.g. CAN.
 *
 * In case of receiving a message, the core reads only one message from the
 * transport layer per process call and handle it complete. If more than one
 * message are received, the transport layer has to implement some kind of
 * buffer mechanism.
 *
 * In case of transmitting a message, the core can write several messages to
 * the transport layer, in one processing cycle. If the transport layer can't
 * send a message, in some cases the core will get into trouble, because right
 * now now fall-back mechanism exists.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __VSCP_TP_ADAPTER_H__
#define __VSCP_TP_ADAPTER_H__

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

/** This type defines the supported function return values. */
typedef enum
{
    VSCP_TP_ADAPTER_RET_OK = 0,             /**< Successful */
    VSCP_TP_ADAPTER_RET_INVALID_USER,       /**< Invalid user */
    VSCP_TP_ADAPTER_RET_INVALID_PASSWORD,   /**< Invalid password */
    VSCP_TP_ADAPTER_RET_TIMEOUT,            /**< Connection timeout */
    VSCP_TP_ADAPTER_RET_ERROR,              /**< Error */
    VSCP_TP_ADAPTER_RET_ENULL               /**< Unexpected NULL pointer */

} VSCP_TP_ADAPTER_RET;

/** This type defines the network level. */
typedef enum
{
    VSCP_TP_ADAPTER_LVL_1 = 0,      /**< Send level 1 events */
    VSCP_TP_ADAPTER_LVL_1_OVER_2    /**< Send level 1 events over level 2 */

} VSCP_TP_ADAPTER_LVL;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function initializes the transport layer.
 */
extern void vscp_tp_adapter_init(void);

/**
 * This function reads a message from the transport layer.
 *
 * @param[out]  msg Message storage
 * @return  Message received or not
 * @retval  FALSE   No message received
 * @retval  TRUE    Message received
 */
extern BOOL vscp_tp_adapter_readMessage(vscp_RxMessage * const msg);

/**
 * This function writes a message to the transport layer.
 *
 * @param[in]   msg Message storage
 * @return  Message sent or not
 * @retval  FALSE   Couldn't send message
 * @retval  TRUE    Message successful sent
 */
extern BOOL vscp_tp_adapter_writeMessage(vscp_TxMessage const * const msg);

/**
 * This function connects to a VSCP daemon.
 *
 * @param[in]   ipAddr      IP address of the daemon
 * @param[in]   user        User name
 * @param[in]   password    Password
 * @param[in]   lvl         Supported network level
 *
 * @return Status
 */
extern VSCP_TP_ADAPTER_RET vscp_tp_adapter_connect(char const * const ipAddr, char const * const user, char const * const password, VSCP_TP_ADAPTER_LVL lvl);

/**
 * This function disconnects a connection to a VSCP daemon.
 */
extern void vscp_tp_adapter_disconnect(void);

/**
 * This function simulates a received message.
 *
 * @param[in]   msg Received message
 */
extern void vscp_tp_adapter_simulateReceivedMessage(vscp_RxMessage const * const msg);

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_TP_ADAPTER_H__ */

/** @} */
