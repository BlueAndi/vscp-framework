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
#include "canDrv.h"
#include <util/delay.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Number of retries for sending a CAN message */
#define VSCP_TP_ADAPTER_TX_RETRY        3

/** The tx wait time in us is used after sending a CAN message failed. */
#define VSCP_TP_ADAPTER_TX_WAIT_TIME    500

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
 * This function initializes the transport layer.
 */
extern void vscp_tp_adapter_init(void)
{
    /* Initialize CAN driver */
    (void)can_Open(CAN_BITRATE_125K, 0, 0, 0);

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
        CANMsg  canMsg;

        if (ERROR_OK == can_readFrame(&canMsg))
        {
            uint8_t index   = 0;

            msg->vscpClass  = (uint16_t)((canMsg.id >> 16) & 0x01ff);
            msg->vscpType   = (uint8_t)((canMsg.id >> 8) & 0x00ff);
            msg->oAddr      = (uint8_t)((canMsg.id >> 0) & 0x00ff);
            msg->hardCoded  = (uint8_t)((canMsg.id >> 25) & 0x0001);
            msg->priority   = (VSCP_PRIORITY)((canMsg.id >> 26) & 0x0007);
            msg->dataNum    = canMsg.len;

            for(index = 0; index < msg->dataNum; ++index)
            {
                msg->data[index] = canMsg.byte[index];
            }

            status = TRUE;
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
        CANMsg  canMsg;
        uint8_t index       = 0;
        int8_t  rv          = 0;    /* Return value of CAN driver */
        uint8_t retryCnt    = 0;    /* Retry counter */

        canMsg.id       = ( (unsigned long)msg->priority << 26 ) |
                          ( (unsigned long)msg->hardCoded << 25) |
                          ( (unsigned long)msg->vscpClass << 16 ) |
                          ( (unsigned long)msg->vscpType << 8) |
                          ( (unsigned long)msg->oAddr);
        canMsg.flags    = CAN_IDFLAG_EXTENDED;
        canMsg.len      = msg->dataNum;

        for(index = 0; index < msg->dataNum; ++index)
        {
            canMsg.byte[index] = msg->data[index];
        }

        /* Send the CAN message */
        do
        {
            /* Before any retry, wait some time */
            if (0 < retryCnt)
            {
                _delay_us(VSCP_TP_ADAPTER_TX_WAIT_TIME);
            }

            rv = can_SendFrame(&canMsg);

            /* Failed to send CAN message? */
            if (ERROR_OK != rv)
            {
                ++retryCnt;
            }
        }
        while ((ERROR_OK != rv) && (VSCP_TP_ADAPTER_TX_RETRY > retryCnt));

        /* CAN message successful sent? */
        if (ERROR_OK == rv)
        {
            status = TRUE;
        }
    }

    return status;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

