/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2022 Andreas Merkle
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
#include "fsl_flexcan.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Used CAN peripheral */
#define VSCP_TP_ADAPTER_CAN						CAN0

/** CAN clock source */
#define VSCP_TP_ADAPTER_CAN_CLKSRC				kCLOCK_BusClk

/** CAN clock frequency */
#define VSCP_TP_ADAPTER_CAN_CLK_FREQ			CLOCK_GetFreq(VSCP_TP_ADAPTER_CAN_CLKSRC)

/** Number of CAN receive message buffers */
#define VSCP_TP_ADAPTER_CAN_RX_MB_COUNT			15

/** Number of CAN receive message buffers */
#define VSCP_TP_ADAPTER_CAN_TX_MB_COUNT			1

#if (16 < (VSCP_TP_ADAPTER_CAN_RX_MB_COUNT + VSCP_TP_ADAPTER_CAN_TX_MB_COUNT))
	#error Max. number of CAN message buffers exceeded!
#endif

#if (1 != VSCP_TP_ADAPTER_CAN_TX_MB_COUNT)
	#error Only one CAN transmit message buffer is supported!
#endif

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
    flexcan_config_t		flexcanConfig	= { 0 };
    flexcan_rx_mb_config_t	mbConfig		= { 0 };
    uint8_t					index			= 0;

    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
     * flexcanConfig.baudRate = 125000U;
     * flexcanConfig.maxMbNum = 16;
     * flexcanConfig.enableLoopBack = false;
     * flexcanConfig.enableSelfWakeup = false;
     * flexcanConfig.enableIndividMask = false;
     * flexcanConfig.enableDoze = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

#if (!defined(FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE)) || !FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE
    flexcanConfig.clkSrc = kFLEXCAN_ClkSrcPeri;
#endif /* FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE */

    FLEXCAN_Init(VSCP_TP_ADAPTER_CAN, &flexcanConfig, VSCP_TP_ADAPTER_CAN_CLK_FREQ);

    /* Setup all rx message buffer. */
    for(index = 0; index < VSCP_TP_ADAPTER_CAN_RX_MB_COUNT; ++index)
    {
		mbConfig.format = kFLEXCAN_FrameFormatExtend;
		mbConfig.type = kFLEXCAN_FrameTypeData;
		mbConfig.id = FLEXCAN_ID_EXT(0);

		/* Use lower message buffers */
		FLEXCAN_SetRxMbConfig(VSCP_TP_ADAPTER_CAN, index, &mbConfig, true);
    }

    /* Setup all tx message buffer. */
    for(index = 0; index < VSCP_TP_ADAPTER_CAN_TX_MB_COUNT; ++index)
    {
		/* Use upper message buffers */
		FLEXCAN_SetTxMbConfig(VSCP_TP_ADAPTER_CAN, (VSCP_TP_ADAPTER_CAN_RX_MB_COUNT + index - 1), true);
    }

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
    BOOL    		status  = FALSE;
    static uint8_t	mbIdx	= 0;

    if (NULL != msg)
    {
    	/* Walk through the receive message buffers and check whether a single message was received? */
    	do
    	{
    		/* Receive message buffer signaled? */
			if (FLEXCAN_GetMbStatusFlags(VSCP_TP_ADAPTER_CAN, 1 << mbIdx))
			{
				uint8_t 		index   = 0;
				flexcan_frame_t	rxFrame	= { 0 };

				/* Read CAN message from receive message buffer*/
				FLEXCAN_ReadRxMb(VSCP_TP_ADAPTER_CAN, mbIdx, &rxFrame);

				/* Build VSCP message */
	            msg->vscpClass  = (uint16_t)((rxFrame.id >> 16) & 0x01ff);
	            msg->vscpType   = (uint8_t)((rxFrame.id >> 8) & 0x00ff);
	            msg->oAddr      = (uint8_t)((rxFrame.id >> 0) & 0x00ff);
	            msg->hardCoded  = (uint8_t)((rxFrame.id >> 25) & 0x0001);
	            msg->priority   = (VSCP_PRIORITY)((rxFrame.id >> 26) & 0x0007);
	            msg->dataNum    = rxFrame.length;

	            for(index = 0; index < msg->dataNum; ++index)
	            {
	            	/* Byte 0 - 3 */
	            	if (4 < index)
	            	{
	            		msg->data[index] = (uint8_t)((rxFrame.dataWord0 >> (24 - index * 8)) & 0xff);
	            	}
	            	/* Byte 4 - 7 */
	            	else
	            	{
	            		msg->data[index] = (uint8_t)((rxFrame.dataWord0 >> (48 - index * 8)) & 0xff);
	            	}
	            }

	            /* Release receive message buffer */
				FLEXCAN_ClearMbStatusFlags(VSCP_TP_ADAPTER_CAN, 1 << mbIdx);

				status = TRUE;
			}
			else
			{
				++mbIdx;
			}
    	}
    	while((VSCP_TP_ADAPTER_CAN_RX_MB_COUNT < mbIdx) && (FALSE == status));

    	mbIdx %= VSCP_TP_ADAPTER_CAN_RX_MB_COUNT;
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
    	uint8_t			index	= 0;
    	flexcan_frame_t	txFrame = { 0 };
    	uint8_t			mbIdx	= VSCP_TP_ADAPTER_CAN_RX_MB_COUNT;	/* Right after the receive message buffers */

        txFrame.format = kFLEXCAN_FrameFormatExtend;
        txFrame.type = kFLEXCAN_FrameTypeData;
        txFrame.id = FLEXCAN_ID_EXT(((uint32_t)msg->priority << 26) |
									((uint32_t)msg->hardCoded << 25) |
									((uint32_t)msg->vscpClass << 16) |
									((uint32_t)msg->vscpType << 8) |
									((uint32_t)msg->oAddr));
        txFrame.length = msg->dataNum;

        for(index = 0; index < msg->dataNum; ++index)
        {
        	/* Byte 0 - 3 */
        	if (4 < index)
        	{
        		txFrame.dataWord0 |= msg->data[index] << (24 - index * 8);
        	}
        	/* Byte 4 - 7 */
        	else
        	{
        		txFrame.dataWord1 |= msg->data[index] << (48 - index * 8);
        	}
        }

        /* Send data through Tx Message Buffer using polling function. */
        if (kStatus_Success == FLEXCAN_TransferSendBlocking(VSCP_TP_ADAPTER_CAN, mbIdx, &txFrame))
        {
        	status = TRUE;
        }
    }

    return status;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
