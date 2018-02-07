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
@brief  VSCP transport layer adapter
@file   vscp_tp_adapter.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_tp_adapter.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_tp_adapter.h"
#include <avr/pgmspace.h>
#include "can.h"

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

/** CAN filter: Only messages with a 29-bit identifier shall be received. */
const uint8_t PROGMEM vscp_tp_adapter_canFilter[] =
{
    /* Group 0 */
    MCP2515_FILTER_EXTENDED(0), /* Filter 0 */
    MCP2515_FILTER_EXTENDED(0), /* Filter 1 */

    /* Group 1 */
    MCP2515_FILTER_EXTENDED(0), /* Filter 2 */
    MCP2515_FILTER_EXTENDED(0), /* Filter 3 */
    MCP2515_FILTER_EXTENDED(0), /* Filter 4 */
    MCP2515_FILTER_EXTENDED(0), /* Filter 5 */

    MCP2515_FILTER_EXTENDED(0), /* Mask 0 (for group 0) */
    MCP2515_FILTER_EXTENDED(0), /* Mask 1 (for group 1) */
};

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes the transport layer.
 */
extern void vscp_tp_adapter_init(void)
{
    /* Initialize CAN controller */
    (void)can_init(BITRATE_125_KBPS);

    /* Load filters and masks */
    can_static_filter(vscp_tp_adapter_canFilter);

    return;
}

/**
 * This function reads a message from the transport layer.
 *
 * @param[out]  msg Message storage
 * @return  Message received or not
 * @retval  FALSE   No message received
 * @retval  TRUE    Message received
 */
extern BOOL vscp_tp_adapter_readMessage(vscp_RxMessage * const msg)
{
    BOOL    status  = FALSE;

    if (NULL != msg)
    {
        /* New message received? */
        if (can_check_message())
        {
            can_t   canMsg; /* CAN message */

            if (can_get_message(&canMsg))
            {
                msg->vscpClass  = (canMsg.id >> 16) & 0x01FF;
                msg->vscpType   = (canMsg.id >>  8) & 0x00FF;
                msg->oAddr      = (canMsg.id >>  0) & 0x00FF;
                msg->hardCoded  = (canMsg.id >> 25) & 0x0001;
                msg->priority   = (uint16_t)( (canMsg.id >> 26) & 0x07 );
                msg->dataNum    = canMsg.length;

                if (0 < canMsg.length)
                {
                    uint8_t index   = 0;    /* Run variable */

                    for(index = 0; index < canMsg.length; ++index)
                    {
                        msg->data[index] = canMsg.data[index];
                    }
                }

                status = TRUE;
            }
        }
    }

    return status;
}

/**
 * This function writes a message to the transport layer.
 *
 * @param[in]   msg Message storage
 * @return  Message sent or not
 * @retval  FALSE   Couldn't send message
 * @retval  TRUE    Message successful sent
 */
extern BOOL vscp_tp_adapter_writeMessage(vscp_TxMessage const * const msg)
{
    BOOL    status  = FALSE;

    if ((NULL != msg) &&                        /* Message shall exists */
        (VSCP_L1_DATA_SIZE >= msg->dataNum))    /* Number of data bytes is limited */
    {
        can_t   canMsg; /* CAN message */

        canMsg.id = (((uint32_t)msg->priority)  << 26) |
                    (((uint32_t)msg->hardCoded) << 25) |
                    (((uint32_t)msg->vscpClass) << 16) |
                    (((uint32_t)msg->vscpType)  <<  8) |
                    msg->oAddr;

        canMsg.flags.rtr        = 0;
        canMsg.flags.extended   = 1;
        canMsg.length           = msg->dataNum;

        if (0 < canMsg.length)
        {
            uint8_t index   = 0;    /* Run variable */

            for(index = 0; index < canMsg.length; ++index)
            {
                canMsg.data[index] = msg->data[index];
            }
        }

        status = can_send_message(&canMsg);
    }

    return status;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

