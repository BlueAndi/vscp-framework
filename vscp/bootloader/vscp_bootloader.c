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
@brief  VSCP bootloader
@file   vscp_bootloader.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_bootloader.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_bootloader.h"
#include "vscp_bl_adapter.h"
#include "vscp_tp_adapter.h"
#include "vscp_class_l1.h"
#include "vscp_type_protocol.h"
#include "crc16ccitt.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Node nickname id in case of no application is present. */
#define VSCP_BOOTLOADER_NODE_NICKNAME_ID    (0xFE)

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Build a uint32_t variable from bytes */
#define VSCP_BOOTLOADER_BUILD_UINT32(__byteMSB, __byte2, __byte1, __byteLSB)    \
    (((uint32_t)(__byteMSB) & 0xff) << 24) |                                    \
    (((uint32_t)(__byte2)   & 0xff) << 16) |                                    \
    (((uint32_t)(__byte1)   & 0xff) <<  8) |                                    \
    (((uint32_t)(__byteLSB) & 0xff) <<  0)

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines VSCP related error ids. */
typedef enum
{
    VSCP_BOOTLOADER_ERROR_PROGRAMMING_ALGO_NOT_SUPPORTED = 0,   /**< Programming algorithm not supported */
    VSCP_BOOTLOADER_ERROR_MEMORY_TYPE_NOT_SUPPORTED,            /**< Memory type not supported */
    VSCP_BOOTLOADER_ERROR_BAD_BLOCK_NUMBER,                     /**< Bad block number */
    VSCP_BOOTLOADER_ERROR_INVALID_MESSAGE                       /**< Invalid message */

} VSCP_BOOTLOADER_ERROR;

/** This type defines different memory types. */
typedef enum
{
    VSCP_BOOTLOADER_MEM_TYPE_PROGRAM = 0,   /**< Program memory */
    VSCP_BOOTLOADER_MEM_TYPE_EEPROM,        /**< Data EEPROM memory */
    VSCP_BOOTLOADER_MEM_TYPE_CONFIG,        /**< Configuation memory (fuses, etc.) */
    VSCP_BOOTLOADER_MEM_TYPE_DATA           /**< Data memory (RAM) */

} VSCP_BOOTLOADER_MEM_TYPE;

/** This type defines all necessary programming parameters. */
typedef struct
{
    uint8_t*    blockBuffer;        /**< Buffer contains one complete block */
    uint32_t    blockNumber;        /**< Start block number */
    uint32_t    programBlockNumber; /**< Block number to program */
    uint8_t     blockBufferIndex;   /**< Current index in the block buffer */
    BOOL        programmingStarted; /**< Programming started or not */

} vscp_bootloader_ProgParam;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static void vscp_bootloader_simApp(void);
static void vscp_bootloader_sendNewNodeOnlineEvent(void);
static void vscp_bootloader_sendAckEnterBootLoader(uint32_t blockSize, uint32_t numBlocks);
static void vscp_bootloader_sendNakEnterBootLoader(uint8_t errorCode);
static void vscp_bootloader_programmingProcedure(void);
static void vscp_bootloader_handleProtocolStartBlockDataTransfer(vscp_RxMessage const * const rxMsg, vscp_bootloader_ProgParam * const progParam);
static void vscp_bootloader_handleProtocolBlockData(vscp_RxMessage const * const rxMsg, vscp_bootloader_ProgParam * const progParam);
static void vscp_bootloader_handleProtocolProgramDataBlock(vscp_RxMessage const * const rxMsg, vscp_bootloader_ProgParam * const progParam);
static BOOL vscp_bootloader_handleProtocolDropNicknameId(vscp_RxMessage const * const rxMsg);
static void vscp_bootloader_sendAckStartBlockDataTransfer(void);
static void vscp_bootloader_sendNakStartBlockDataTransfer(void);
static void vscp_bootloader_sendAckBlockData(uint16_t crc, uint32_t writePtr);
static void vscp_bootloader_sendNakBlockData(uint8_t errorCode, uint32_t writePtr);
static void vscp_bootloader_sendAckProgramBlockData(uint32_t blockNumber);
static void vscp_bootloader_sendNakProgramBlockData(uint8_t errorCode, uint32_t blockNumber);
static void vscp_bootloader_sendAckActivateNewImage(void);
static void vscp_bootloader_sendNakActivateNewImage(void);
static BOOL vscp_bootloader_handleProtocolActivateNewImage(vscp_RxMessage const * const rxMsg, vscp_bootloader_ProgParam * const progParam);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Node nickname id */
static uint8_t  vscp_bootloader_nickname    = VSCP_BOOTLOADER_NODE_NICKNAME_ID;

/** Block buffer */
static uint8_t  vscp_bootloader_blockBuffer[VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE];

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes the module.
 */
extern void vscp_bootloader_init(void)
{
    vscp_bl_adapter_init();
    vscp_tp_adapter_init();

    return;
}

/**
 * This function runs the bootloader algorithm and will never return!
 */
extern void vscp_bootloader_run(void)
{
    BOOL    simApp  = FALSE;

    /* The user doesn't force to stay in the bootloader? */
    if (FALSE == vscp_bl_adapter_getSegInitButtonState())
    {
        uint8_t bootFlag    = vscp_bl_adapter_readBootFlag();

        /* Jump to application? */
        if (VSCP_BOOT_FLAG_APPLICATION == bootFlag)
        {
            vscp_bl_adapter_jumpToApp();
            /* This line will never be reached. */
            return;
        }
        /* No application present? Important here to compare with
         * "not equal", which handles all error cases too.
         */
        else if (VSCP_BOOT_FLAG_BOOTLOADER != bootFlag)
        {
            simApp = TRUE;
        }
        /* Application jumped to bootloader per request? */
        else
        {
            simApp = FALSE;

            /* Go sure that the application will start next time again. */
            vscp_bl_adapter_writeBootFlag(VSCP_BOOT_FLAG_APPLICATION);

            /* Get nickname id from persistent memory */
            vscp_bootloader_nickname = vscp_bl_adapter_readNicknameId();
        }
    }
    /* User force bootloader mode */
    else
    {
        simApp = TRUE;
    }

    /* Set lamp on */
    vscp_bl_adapter_enableLamp(TRUE);

    /* Simulate application? */
    if (TRUE == simApp)
    {
        vscp_bootloader_simApp();
    }

    /* Acknowledge the "enter bootloader event", which was received by the application. */
    vscp_bootloader_sendAckEnterBootLoader(VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE, VSCP_PLATFORM_PROG_MEM_NUM_BLOCKS);

    /* Programming procedure */
    vscp_bootloader_programmingProcedure();

    /* Set lamp off */
    vscp_bl_adapter_enableLamp(FALSE);

    vscp_bl_adapter_reboot();
    /* This line never will be reached. */

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function behaves like a application.
 * It will only return if a "enter boot loader mode" command is received.
 */
static void vscp_bootloader_simApp(void)
{
    BOOL            success = FALSE;
    vscp_RxMessage  rxMsg;

    /* Send an "new node online" event, so that the programming tool knows the node is available now.
     * The event needs the nickname id, but bootloader doesn't know it. And to avoid that the
     * bootloader program flash size is big, the nickname 0xFE will be used.
     */
    vscp_bootloader_sendNewNodeOnlineEvent();

    do
    {
        if (TRUE == vscp_tp_adapter_readMessage(&rxMsg))
        {
            if (VSCP_CLASS_L1_PROTOCOL == rxMsg.vscpClass)
            {
                switch(rxMsg.vscpType)
                {
                case VSCP_TYPE_PROTOCOL_PROBE_ACK:
                    /* Does another node has the same nickname id? */
                    if (vscp_bootloader_nickname == rxMsg.oAddr)
                    {
                        /* Set lamp off */
                        vscp_bl_adapter_enableLamp(FALSE);

                        /* Abort bootloader */
                        vscp_bl_adapter_halt();
                        /* This line will never be reached. */
                    }
                    break;

                case VSCP_TYPE_PROTOCOL_ENTER_BOOT_LOADER:
                    /* Is this event for this node? */
                    if ((vscp_bootloader_nickname == rxMsg.data[0]) &&
                        (vscp_bl_adapter_readGUID(15 - 0) == rxMsg.data[2]) &&
                        (vscp_bl_adapter_readGUID(15 - 3) == rxMsg.data[3]) &&
                        (vscp_bl_adapter_readGUID(15 - 5) == rxMsg.data[4]) &&
                        (vscp_bl_adapter_readGUID(15 - 7) == rxMsg.data[5]))
                    {
                        /* Is the bootloader programming algorithm supported? */
                        if (VSCP_BOOTLOADER_ALGORITHM == rxMsg.data[1])
                        {
                            /* Don't send the acknowledge here, this will be done
                             * in the calling function.
                             */
                            success = TRUE;
                        }
                        else
                        /* Bootloader programming algorithm not supported */
                        {
                            vscp_bootloader_sendNakEnterBootLoader(VSCP_BOOTLOADER_ERROR_PROGRAMMING_ALGO_NOT_SUPPORTED);
                        }
                    }
                    break;

                default:
                    /* Wait for next event */
                    break;
                }
            }
        }
    }
    while(FALSE == success);

    return;
}

/**
 * This functions sends the new node online / probe event.
 */
static void vscp_bootloader_sendNewNodeOnlineEvent(void)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_NEW_NODE_ONLINE;
    txMsg.priority  = VSCP_PRIORITY_0_HIGH;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 1;    /* 1 byte data */
    txMsg.data[0]   = vscp_bootloader_nickname;

    (void)vscp_tp_adapter_writeMessage(&txMsg);
}

/**
 * This function sends a "ACK enter bootloader" event.
 *
 * @param[in]   blockSize   Block size
 * @param[in]   numBlocks   Number of blocks
 */
static void vscp_bootloader_sendAckEnterBootLoader(uint32_t blockSize, uint32_t numBlocks)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_ACK_BOOT_LOADER;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 8;                        /* 8 byte data */
    txMsg.data[0]   = (blockSize >> 24) & 0xff; /* MSB of program memory block size. */
    txMsg.data[1]   = (blockSize >> 16) & 0xff; /* Program memory block size. */
    txMsg.data[2]   = (blockSize >>  8) & 0xff; /* Program memory block size. */
    txMsg.data[3]   = (blockSize >>  0) & 0xff; /* LSB of program memory block size. */
    txMsg.data[4]   = (numBlocks >> 24) & 0xff; /* MSB of number of block s available. */
    txMsg.data[5]   = (numBlocks >> 16) & 0xff; /* Number of block s available. */
    txMsg.data[6]   = (numBlocks >>  8) & 0xff; /* Number of block s available. */
    txMsg.data[7]   = (numBlocks >>  0) & 0xff; /* LSB of number of blocks available. */

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function sends a "NACK enter bootloader" event.
 *
 * @param[in]  errorCode   User defined error code
 */
static void vscp_bootloader_sendNakEnterBootLoader(uint8_t errorCode)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_NACK_BOOT_LOADER;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 1;            /* 1 byte data */
    txMsg.data[0]   = errorCode;    /* User defined error code */

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function process the programming procedure. It will block until the
 * programming procedure is finished or stopped by the remote host.
 */
static void vscp_bootloader_programmingProcedure(void)
{
    BOOL                        abortFlag   = FALSE;
    vscp_bootloader_ProgParam   progParam   = { vscp_bootloader_blockBuffer, 0, 0, 0, TRUE };
    vscp_RxMessage              rxMsg;

    /* Only with a "activate new image" or "drop nickname/reset device" event we can leave. */
    do
    {
        if (TRUE == vscp_tp_adapter_readMessage(&rxMsg))
        {
            if (VSCP_CLASS_L1_PROTOCOL == rxMsg.vscpClass)
            {
                switch(rxMsg.vscpType)
                {
                case VSCP_TYPE_PROTOCOL_START_BLOCK:
                    vscp_bootloader_handleProtocolStartBlockDataTransfer(&rxMsg, &progParam);
                    break;

                case VSCP_TYPE_PROTOCOL_BLOCK_DATA:
                    vscp_bootloader_handleProtocolBlockData(&rxMsg, &progParam);
                    break;

                case VSCP_TYPE_PROTOCOL_PROGRAM_BLOCK_DATA:
                    vscp_bootloader_handleProtocolProgramDataBlock(&rxMsg, &progParam);
                    break;
                
                case VSCP_TYPE_PROTOCOL_ACTIVATE_NEW_IMAGE:
                    abortFlag = vscp_bootloader_handleProtocolActivateNewImage(&rxMsg, &progParam);
                    break;

                case VSCP_TYPE_PROTOCOL_DROP_NICKNAME:
                    abortFlag = vscp_bootloader_handleProtocolDropNicknameId(&rxMsg);
                    break;

                default:
                    break;
                }
            }
        }
    }
    while(FALSE == abortFlag);

    return;
}

/**
 * This function handles the start block data transfer event.
 *
 * @param[in]   rxMsg       Received message
 * @param[in]   progParam   Programming parameter
 */
static void vscp_bootloader_handleProtocolStartBlockDataTransfer(vscp_RxMessage const * const rxMsg, vscp_bootloader_ProgParam * const progParam)
{
    if ((NULL == rxMsg) ||
        (NULL == progParam))
    {
        return;
    }

    if ((4 > rxMsg->dataNum) ||
        (6 < rxMsg->dataNum))
    {
        vscp_bootloader_sendNakStartBlockDataTransfer();
    }
    else
    {
        uint8_t memoryType  = VSCP_BOOTLOADER_MEM_TYPE_PROGRAM;

        /* Get block number */
        progParam->blockNumber = VSCP_BOOTLOADER_BUILD_UINT32(rxMsg->data[0],
                                                              rxMsg->data[1],
                                                              rxMsg->data[2],
                                                              rxMsg->data[3]);

        /* Determine memory type which to program */
        if (4 < rxMsg->dataNum)
        {
            memoryType = rxMsg->data[4];
        }
        else
        {
            memoryType = VSCP_BOOTLOADER_MEM_TYPE_PROGRAM;
        }

        /* Validate parameters:
            * - Only program flash is supported.
            * - Received block number shall be valid.
            */
        if (VSCP_BOOTLOADER_MEM_TYPE_PROGRAM != memoryType)
        {
            vscp_bootloader_sendNakStartBlockDataTransfer();
        }
        else if (VSCP_PLATFORM_PROG_MEM_NUM_BLOCKS <= progParam->blockNumber)
        {
            vscp_bootloader_sendNakStartBlockDataTransfer();
        }
        else
        {
            vscp_bootloader_sendAckStartBlockDataTransfer();

            /* Reset block buffer index */
            progParam->blockBufferIndex = 0;
        }
    }

    return;
}

/**
 * This function handles the block data event.
 *
 * @param[in]   rxMsg       Received message
 * @param[in]   progParam   Programming parameter
 */
static void vscp_bootloader_handleProtocolBlockData(vscp_RxMessage const * const rxMsg, vscp_bootloader_ProgParam * const progParam)
{
    if ((NULL == rxMsg) ||
        (NULL == progParam))
    {
        return;
    }

    /* Every block data must be a multiple of 8. */
    if (VSCP_L1_DATA_SIZE != rxMsg->dataNum)
    {
        vscp_bootloader_sendNakBlockData(VSCP_BOOTLOADER_ERROR_INVALID_MESSAGE,
                                         progParam->blockNumber * VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE);
    }
    /* Block buffer already full? */
    else if (VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE < (progParam->blockBufferIndex + rxMsg->dataNum))
    {
        vscp_bootloader_sendNakBlockData(VSCP_BOOTLOADER_ERROR_INVALID_MESSAGE,
                                         progParam->blockNumber * VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE);
    }
    else
    {
        uint8_t index   = 0;

        for(index = 0; index < rxMsg->dataNum; ++index)
        {
            progParam->blockBuffer[progParam->blockBufferIndex] = rxMsg->data[index];

            ++(progParam->blockBufferIndex);
        }

        /* Complete block received? */
        if (VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE <= progParam->blockBufferIndex)
        {
            /* Calculate CRC16-CCITT over the whole block and send it back for verification. */
            Crc16CCITT  crcCalculated = crc16ccitt_calculate(progParam->blockBuffer, VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE);

            vscp_bootloader_sendAckBlockData(crcCalculated, progParam->blockNumber * VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE);
        }
    }

    return;
}

/**
 * This function handles the program data block event.
 *
 * @param[in]   rxMsg       Received message
 * @param[in]   progParam   Programming parameter
 */
static void vscp_bootloader_handleProtocolProgramDataBlock(vscp_RxMessage const * const rxMsg, vscp_bootloader_ProgParam * const progParam)
{
    if ((NULL == rxMsg) ||
        (NULL == progParam))
    {
        return;
    }

    if (4 != rxMsg->dataNum)
    {
        vscp_bootloader_sendNakProgramBlockData(VSCP_BOOTLOADER_ERROR_INVALID_MESSAGE, progParam->blockNumber);
    }
    else
    {
        /* Get block number which to program */
        progParam->programBlockNumber = VSCP_BOOTLOADER_BUILD_UINT32(rxMsg->data[0],
                                                                     rxMsg->data[1],
                                                                     rxMsg->data[2],
                                                                     rxMsg->data[3]);

        /* The received block shall match with the block number. */
        if (progParam->blockNumber != progParam->programBlockNumber)
        {
            vscp_bootloader_sendNakProgramBlockData(VSCP_BOOTLOADER_ERROR_BAD_BLOCK_NUMBER, progParam->blockNumber);
        }
        else
        {
            /* First time? */
            if (FALSE != progParam->programmingStarted)
            {
                /* If any error happen during programming, the bootloader shall start again. */
                vscp_bl_adapter_writeBootFlag(VSCP_BOOT_FLAG_BOOTLOADER);

                progParam->programmingStarted = FALSE;
            }

            vscp_bl_adapter_programBlock(progParam->blockNumber, progParam->blockBuffer);

            vscp_bootloader_sendAckProgramBlockData(progParam->blockNumber);
        }
    }

    return;
}

/**
 * This function handles the GUID drop nickname id event.
 *
 * @param[in]   rxMsg   Received message
 * @return Reboot request
 * @retval FALSE    Continue
 * @retval TRUE     Reboot
 */
static BOOL vscp_bootloader_handleProtocolDropNicknameId(vscp_RxMessage const * const rxMsg)
{
    BOOL    reboot  = FALSE;

    if (NULL == rxMsg)
    {
        return FALSE;
    }

    /* Abort bootloader? */
    if ((0 < rxMsg->dataNum) &&
        (4 > rxMsg->dataNum) &&
        (vscp_bootloader_nickname == rxMsg->data[0]))
    {
        reboot = TRUE;
    }

    return reboot;
}

/**
 * This function sends a "ACK start block data transfer" event.
 */
static void vscp_bootloader_sendAckStartBlockDataTransfer(void)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_START_BLOCK_ACK;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 0;    /* 0 byte data */

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function sends a "NACK start block data transfer" event.
 */
static void vscp_bootloader_sendNakStartBlockDataTransfer(void)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_START_BLOCK_NACK;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 0;    /* 0 byte data */

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function sends a "ACK block data" event.
 *
 * @param[in]   crc         Block CRC
 * @param[in]   writePtr    Write pointer after the last data has been written
 */
static void vscp_bootloader_sendAckBlockData(uint16_t crc, uint32_t writePtr)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_BLOCK_DATA_ACK;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 6;    /* 6 byte data */
    txMsg.data[0]   = (uint8_t)((crc >> 8u) & 0xFFu);
    txMsg.data[1]   = (uint8_t)((crc >> 0u) & 0xFFu);
    txMsg.data[2]   = (uint8_t)((writePtr >> 24u) & 0xFFu);
    txMsg.data[3]   = (uint8_t)((writePtr >> 16u) & 0xFFu);
    txMsg.data[4]   = (uint8_t)((writePtr >>  8u) & 0xFFu);
    txMsg.data[5]   = (uint8_t)((writePtr >>  0u) & 0xFFu);

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function sends a "NACK block data" event.
 *
 * @param[in]   errorCode   User defined error code
 * @param[in]   writePtr    Write pointer after the last data has been written
 */
static void vscp_bootloader_sendNakBlockData(uint8_t errorCode, uint32_t writePtr)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_BLOCK_DATA_NACK;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 5;    /* 5 byte data */
    txMsg.data[0]   = errorCode;
    txMsg.data[1]   = (uint8_t)((writePtr >> 24u) & 0xFFu);
    txMsg.data[2]   = (uint8_t)((writePtr >> 16u) & 0xFFu);
    txMsg.data[3]   = (uint8_t)((writePtr >>  8u) & 0xFFu);
    txMsg.data[4]   = (uint8_t)((writePtr >>  0u) & 0xFFu);

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function sends a "ACK program block data" event.
 *
 * @param[in]  blockNumber Programmed block number
 */
static void vscp_bootloader_sendAckProgramBlockData(uint32_t blockNumber)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_PROGRAM_BLOCK_DATA_ACK;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 4;    /* 4 byte data */
    txMsg.data[0]   = (uint8_t)((blockNumber >> 24u) & 0xFFu);
    txMsg.data[1]   = (uint8_t)((blockNumber >> 16u) & 0xFFu);
    txMsg.data[2]   = (uint8_t)((blockNumber >>  8u) & 0xFFu);
    txMsg.data[3]   = (uint8_t)((blockNumber >>  0u) & 0xFFu);

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function sends a "NACK program block data" event.
 *
 * @param[in]  errorCode   User defined error code
 * @param[in]  blockNumber Block number
 */
static void vscp_bootloader_sendNakProgramBlockData(uint8_t errorCode, uint32_t blockNumber)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_PROGRAM_BLOCK_DATA_NACK;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 5;    /* 5 byte data */
    txMsg.data[0]   = errorCode;
    txMsg.data[1]   = (uint8_t)((blockNumber >> 24u) & 0xFFu);
    txMsg.data[2]   = (uint8_t)((blockNumber >> 16u) & 0xFFu);
    txMsg.data[3]   = (uint8_t)((blockNumber >>  8u) & 0xFFu);
    txMsg.data[4]   = (uint8_t)((blockNumber >>  0u) & 0xFFu);

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function sends a "ACK activate new image" event.
 */
static void vscp_bootloader_sendAckActivateNewImage(void)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_ACTIVATE_NEW_IMAGE_ACK;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 0;    /* 0 byte data */

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function sends a "NACK activate new image" event.
 */
static void vscp_bootloader_sendNakActivateNewImage(void)
{
    vscp_TxMessage  txMsg;

    txMsg.vscpClass = VSCP_CLASS_L1_PROTOCOL;
    txMsg.vscpType  = VSCP_TYPE_PROTOCOL_ACTIVATE_NEW_IMAGE_NACK;
    txMsg.priority  = VSCP_PRIORITY_7_LOW;
    txMsg.oAddr     = vscp_bootloader_nickname;
    txMsg.hardCoded = FALSE;
    txMsg.dataNum   = 0;    /* 0 byte data */

    (void)vscp_tp_adapter_writeMessage(&txMsg);

    return;
}

/**
 * This function handles the activate new image event.
 *
 * @param[in]   rxMsg       Received message
 * @param[in]   progParam   Programming parameter
 *
 * @return New image successful received or not.
 * @retval FALSE    Failed to activate new image
 * @retval TRUE     New image successful activated
 */
static BOOL vscp_bootloader_handleProtocolActivateNewImage(vscp_RxMessage const * const rxMsg, vscp_bootloader_ProgParam * const progParam)
{
    BOOL    success = FALSE;
    
    if ((NULL == rxMsg) ||
        (NULL == progParam))
    {
        return success;
    }

    if (2 != rxMsg->dataNum)
    {
        vscp_bootloader_sendNakActivateNewImage();
    }
    else
    {
        Crc16CCITT  crcReceived     = 0;
        Crc16CCITT  crcCalculated   = crc16ccitt_init();
        uint16_t    index           = 0;

        /* Calculate CRC from 0 to the last programmed block (inclusive) */
        for(index = 0; index < (VSCP_PLATFORM_PROG_MEM_BLOCK_SIZE * (progParam->programBlockNumber + 1)); ++index)
        {
            uint8_t data = vscp_bl_adapter_readProgMem(index);

            crcCalculated = crc16ccitt_update(crcCalculated, &data, 1);
        }

        crcCalculated = crc16ccitt_finalize(crcCalculated);

        crcReceived = rxMsg->data[0];
        crcReceived <<= 8u;
        crcReceived |= rxMsg->data[1];

        if (crcReceived != crcCalculated)
        {
            vscp_bootloader_sendNakActivateNewImage();
        }
        else
        {
            /* Write boot destination */
            vscp_bl_adapter_writeBootFlag(VSCP_BOOT_FLAG_APPLICATION);

            vscp_bootloader_sendAckActivateNewImage();

            success = TRUE;
        }
    }
    
    return success;
}
