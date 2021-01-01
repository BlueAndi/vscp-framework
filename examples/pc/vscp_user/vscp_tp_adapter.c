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
@file   vscp_tp_adapter.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_tp_adapter.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_tp_adapter.h"
#include <stdio.h>
#include <memory.h>
#include "log.h"
#include "vscphelperlib.h"
#include "vscp_class_l1.h"
#include "vscp_class_l1_l2.h"
#include "vscp_util.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Size of the interface GUID in byte */
#define VSCP_TP_ADAPTER_INTERFACE_GUID_SIZE VSCP_GUID_SIZE

/** Base class id of L2 events */
#define VSCP_TP_ADAPTER_CLASS_L2_BASE       1024

/** Daemon command response timeout in ms */
#define VSCP_TP_ADAPTER_RESPONSE_TIMEOUT    6000

/** Daemon wait time after a command was executed in ms */
#define VSCP_TP_ADAPTER_CMD_WAIT_TIME       250

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the necessary IP network parameter. */
typedef struct
{
    long                hSession;   /**< Session handle */
    VSCP_TP_ADAPTER_LVL lvl;        /**< Network level */
    uint8_t             interfaceGUID[VSCP_TP_ADAPTER_INTERFACE_GUID_SIZE]; /**< Interface GUID */

} vscp_tp_adapter_NetPar;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static BOOL vscp_tp_adapter_handleL1Event(vscp_RxMessage * const msg, vscpEventEx const * const daemonEvent);
static BOOL vscp_tp_adapter_handleL1OverL2Event(vscp_RxMessage * const msg, vscpEventEx const * const daemonEvent);
static void vscp_tp_adapter_showMessage(vscp_Message const * const msg, BOOL isReceived);
static const char* vscp_tp_adapter_getErrorStr(int value);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Flag which signals that a message is received. */
static BOOL                     vscp_tp_adapter_messageReceived  = FALSE;

/** Receive message */
static vscp_RxMessage           vscp_tp_adapter_rxMessage;

/** Network client parameter */
static vscp_tp_adapter_NetPar   vscp_tp_adapter_clientPar;

/** Connected or not */
static BOOL                     vscp_tp_adapter_isConnected     = FALSE;

/** User friendly strings for every CLASS Protocol type. */
static const char*              vscp_tp_adapter_protocolTypes[] =
{
    /*  0 */ "Undefined",
    /*  1 */ "Segment Controller Heartbeat",
    /*  2 */ "New Node Online",
    /*  3 */ "Probe ACK",
    /*  4 */ "Reserved",
    /*  5 */ "Reserved",
    /*  6 */ "Set Nickname Id",
    /*  7 */ "Nickname Id Accepted",
    /*  8 */ "Drop Nickname Id",
    /*  9 */ "Read Register",
    /* 10 */ "Read/Write Response",
    /* 11 */ "Write Register",
    /* 12 */ "Enter Boot Loader Mode",
    /* 13 */ "Enter Boot Loader Mode ACK",
    /* 14 */ "Enter Boot Loader Mode NACK",
    /* 15 */ "Start Block Data Transfer",
    /* 16 */ "Block Data",
    /* 17 */ "Block Data ACK",
    /* 18 */ "Block Data NACK",
    /* 19 */ "Program Data Block",
    /* 20 */ "Program Data Block ACK",
    /* 21 */ "Program Data Block NACK",
    /* 22 */ "Activate New Image",
    /* 23 */ "GUID Drop Nickname Id",
    /* 24 */ "Page Read",
    /* 25 */ "Page Write",
    /* 26 */ "Page Read/Write Response",
    /* 27 */ "High End Server Probe",
    /* 28 */ "High End Server Response",
    /* 29 */ "Increment Register",
    /* 30 */ "Decrement Register",
    /* 31 */ "Who Is There",
    /* 32 */ "Who Is There Response",
    /* 33 */ "Get Decision Matrix Info",
    /* 34 */ "Get Decision Matrix Info Response",
    /* 35 */ "Get Embedded MDF",
    /* 36 */ "Get Embedded MDF Response",
    /* 37 */ "Extended Page Read Register",
    /* 38 */ "Extended Page Write Register",
    /* 39 */ "Extended Page Read/Write Response",
    /* 40 */ "Get Event Interest",
    /* 41 */ "Get Event Interest Response",
    /* 42 */ "Reserved",
    /* 43 */ "Reserved",
    /* 44 */ "Reserved",
    /* 45 */ "Reserved",
    /* 46 */ "Reserved",
    /* 47 */ "Reserved",
    /* 48 */ "Activate New Image ACK",
    /* 49 */ "Activate New Image NACK",
    /* 50 */ "Start Block Data Transfer ACK",
    /* 51 */ "Start Block Data Transfer NACK"
};

/** User friendly strings for VSCP helper library function return status. */
static const char*                  vscp_tp_adapter_errorStr[]  =
{
    /*  0 */ "Successful",
    /*  1 */ "?",
    /*  2 */ "?",
    /*  3 */ "?",
    /*  4 */ "?",
    /*  5 */ "?",
    /*  6 */ "?",
    /*  7 */ "Invalid channel",
    /*  8 */ "FIFO is empty",
    /*  9 */ "FIFO is full",
    /* 10 */ "FIFO size error",
    /* 11 */ "FIFO wait",
    /* 12 */ "Generic error",
    /* 13 */ "Hardware error",
    /* 14 */ "Initialization failed",
    /* 15 */ "Initialization missing",
    /* 16 */ "Initialization ready",
    /* 17 */ "Not supported",
    /* 18 */ "Overrun",
    /* 19 */ "Receive buffer empty",
    /* 20 */ "Register value error",
    /* 21 */ "Transmit buffer full",
    /* 22 */ "?",
    /* 23 */ "?",
    /* 24 */ "?",
    /* 25 */ "?",
    /* 26 */ "?",
    /* 27 */ "?",
    /* 28 */ "Unable to load library",
    /* 29 */ "Unable to get library proc address",
    /* 30 */ "Only one instance allowed",
    /* 31 */ "Problem with sub driver call",
    /* 32 */ "Timeout",
    /* 33 */ "The device is not open",
    /* 34 */ "A parameter is invalid",
    /* 35 */ "Memory exhausted",
    /* 36 */ "Some kind of internal program error",
    /* 37 */ "Some kind of communication error",
    /* 38 */ "Login error username",
    /* 39 */ "Login error password",
    /* 40 */ "Could not connect",
    /* 41 */ "The handle is not valid",
    /* 42 */ "Operation failed for some reason"
};

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
    /* Nothing to do */

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
        vscp_tp_adapter_NetPar* client  = &vscp_tp_adapter_clientPar;

        /* Connected to a VSCP daemon? */
        if (0 != client->hSession)
        {
            uint32_t    count       = 0;
            vscpEventEx daemonEvent;
            int         vscphlpRet  = 0;

            /* Disconnected? */
            if (FALSE == vscp_tp_adapter_isConnected)
            {
                /* Nothing to do */
                ;
            }
            /* Check for available events. */
            else if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_isDataAvailable(client->hSession, &count)))
            {
                LOG_ERROR_INT32("Couldn't check for available data: ", vscphlpRet);
                LOG_ERROR_STR("vscphlp_isDataAvailable failed: ", vscp_tp_adapter_getErrorStr(vscphlpRet));

                log_printf("Connection lost.\n");
                vscp_tp_adapter_isConnected = FALSE;
            }
            /* Any event available? */
            else if (0 < count)
            {
                if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_receiveEventEx(client->hSession, &daemonEvent)))
                {
                    LOG_WARNING_INT32("Couldn't receive event: ", vscphlpRet);
                    LOG_WARNING_STR("vscphlp_receiveEventEx failed: ", vscp_tp_adapter_getErrorStr(vscphlpRet));
                }
                /* Handle all level 1 events? */
                else if (VSCP_TP_ADAPTER_LVL_1 == client->lvl)
                {
                    if (FALSE == vscp_tp_adapter_handleL1Event(msg, &daemonEvent))
                    {
                        status = TRUE;
                    }
                }
                /* Handle all level 1 events and level 1 over level 2 events? */
                else if (VSCP_TP_ADAPTER_LVL_1_OVER_2 == client->lvl)
                {
                    /* Level 1 event? */
                    if (VSCP_CLASS_L1_L2_BASE > daemonEvent.vscp_class)
                    {
                        if (FALSE == vscp_tp_adapter_handleL1Event(msg, &daemonEvent))
                        {
                            status = TRUE;
                        }
                    }
                    /* Level 1 over level 2 event? */
                    else if (VSCP_TP_ADAPTER_CLASS_L2_BASE > daemonEvent.vscp_class)
                    {
                        if (FALSE == vscp_tp_adapter_handleL1OverL2Event(msg, &daemonEvent))
                        {
                            status = TRUE;
                        }
                    }
                }
            }
        }
        /* Any simulated message available? */
        else if (TRUE == vscp_tp_adapter_messageReceived)
        {
            *msg = vscp_tp_adapter_rxMessage;

            vscp_tp_adapter_messageReceived = FALSE;

            status = TRUE;
        }

        if (FALSE != status)
        {
            vscp_tp_adapter_showMessage(msg, TRUE);
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
        vscp_tp_adapter_NetPar* client  = &vscp_tp_adapter_clientPar;
        uint8_t                 index   = 0;

        /* Connected to a VSCP daemon? */
        if (0 != client->hSession)
        {
            /* Connected? */
            if (TRUE == vscp_tp_adapter_isConnected)
            {
                vscpEventEx daemonEvent;
                int         vscphlpRet  = 0;

                memset(&daemonEvent, 0, sizeof(daemonEvent));

                /* Send always a level 1 event back, independent of the configured
                 * network level @see VSCP_TP_ADAPTER_LVL
                 */
                daemonEvent.vscp_class  = msg->vscpClass;
                daemonEvent.vscp_type   = msg->vscpType;
                daemonEvent.head        = 0;
                daemonEvent.head        |= (msg->priority & 0x07) << 5;
                daemonEvent.head        |= (msg->hardCoded & 0x01) << 4;
                daemonEvent.sizeData    = msg->dataNum;
                
                /* Assign interface GUID and node nickname at LSB byte */
                for(index = 0; index < (VSCP_TP_ADAPTER_INTERFACE_GUID_SIZE - 1); ++index)
                {
                    daemonEvent.GUID[index] = client->interfaceGUID[index];
                }
                daemonEvent.GUID[index] = msg->oAddr; /* Node nickname shall be at interface GUID LSB byte */

                /* Copy event data */
                for(index = 0; index < daemonEvent.sizeData; ++index)
                {
                    daemonEvent.data[index] = msg->data[index];
                }

                if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_sendEventEx(client->hSession, &daemonEvent)))
                {
                    LOG_WARNING_INT32("Couldn't send event to daemon:", vscphlpRet);
                    LOG_WARNING_STR("vscphlp_sendEventEx failed: ", vscp_tp_adapter_getErrorStr(vscphlpRet));
                }
                else
                {
                    status = TRUE;
                }
            }
        }
        else
        {
            status = TRUE;
        }

        if (FALSE != status)
        {
            vscp_tp_adapter_showMessage(msg, FALSE);
        }
    }

    return status;
}

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
extern VSCP_TP_ADAPTER_RET vscp_tp_adapter_connect(char const * const ipAddr, char const * const user, char const * const password, VSCP_TP_ADAPTER_LVL lvl)
{
    VSCP_TP_ADAPTER_RET     status      = VSCP_TP_ADAPTER_RET_OK;
    vscp_tp_adapter_NetPar* client      = &vscp_tp_adapter_clientPar;
    char const *            pUser       = "";
    char const *            pPassword   = "";
    vscpEventFilter         filter;
    int                     vscphlpRet  = 0;

    /* No daemon address given? */
    if (NULL == ipAddr)
    {
        return VSCP_TP_ADAPTER_RET_ENULL;
    }

    /* Already a connection established? */
    if (0 != client->hSession)
    {
        return VSCP_TP_ADAPTER_RET_ERROR;
    }

    if (NULL != user)
    {
        pUser = user;
    }

    if (NULL != password)
    {
        pPassword = password;
    }

    /* Set network level */
    client->lvl = lvl;

    /* Set event filter for level 1 events only */
    if (VSCP_TP_ADAPTER_LVL_1 == client->lvl)
    {
        /* The classes of level 1 events are lower than 512.
         * 512 - 1 = 0x01ff
         * mask    = 0xfe00
         * filter  = 0x0000
         */
        memset(&filter, 0, sizeof(filter));
        filter.mask_class   = 0xfe00;
        filter.filter_class = 0x0000;
    }
    /* Set event filter for level 1 events and level 1 over level 2 events. */
    else if (VSCP_TP_ADAPTER_LVL_1_OVER_2 == client->lvl)
    {
        /* The classes of level 1 events over level 2 are greater or equal than 512 and
         * lower than 1024.
         * 1024 - 1 = 0x03ff
         * mask     = 0xfc00
         * filter   = 0x0000
         */
        memset(&filter, 0, sizeof(filter));
        filter.mask_class   = 0xfc00;
        filter.filter_class = 0x0000;
    }
    else
    {
        return VSCP_TP_ADAPTER_RET_ERROR;
    }

    /* Create a session */
    client->hSession = vscphlp_newSession();

    /* No session handle? */
    if (0 == client->hSession)
    {
        LOG_ERROR("Couldn't get a session handle.");
        status = VSCP_TP_ADAPTER_RET_ERROR;
    }
    /* Set response timeout */
    else if (VSCP_ERROR_SUCCESS != vscphlp_setResponseTimeout(client->hSession, VSCP_TP_ADAPTER_RESPONSE_TIMEOUT))
    {
        LOG_ERROR("Couldn't set response timeout.");
        status = VSCP_TP_ADAPTER_RET_ERROR;
    }
    /* Set wait time after command execution */
    else if (VSCP_ERROR_SUCCESS != vscphlp_setAfterCommandSleep(client->hSession, VSCP_TP_ADAPTER_CMD_WAIT_TIME))
    {
        LOG_ERROR("Couldn't set wait time after command.");
        status = VSCP_TP_ADAPTER_RET_ERROR;
    }
    /* Open a channel */
    else if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_open(client->hSession, ipAddr, pUser, pPassword)))
    {
        if (VSCP_ERROR_INVALID_HANDLE == vscphlpRet)
        {
            LOG_ERROR("Invalid handle.");
            status = VSCP_TP_ADAPTER_RET_ERROR;
        }
        else if (VSCP_ERROR_USER == vscphlpRet)
        {
            LOG_ERROR("Invalid user.");
            status = VSCP_TP_ADAPTER_RET_INVALID_USER;
        }
        else if (VSCP_ERROR_PASSWORD == vscphlpRet)
        {
            LOG_ERROR("Invalid password");
            status = VSCP_TP_ADAPTER_RET_INVALID_PASSWORD;
        }
        else if (VSCP_ERROR_TIMEOUT == vscphlpRet)
        {
            LOG_ERROR("Connection timeout.");
            status = VSCP_TP_ADAPTER_RET_TIMEOUT;
        }
        else
        {
            LOG_ERROR_INT32("Failed to open a channel:", vscphlpRet);
            status = VSCP_TP_ADAPTER_RET_ERROR;
        }
    }
    /* Check channel */
    else if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_noop(client->hSession)))
    {
        LOG_ERROR_INT32("Channel communication error:", vscphlpRet);
        status = VSCP_TP_ADAPTER_RET_ERROR;
    }
    /* Set filter for configured event level */
    else if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_setFilter(client->hSession, &filter)))
    {
        LOG_ERROR_INT32("Set filter error:", vscphlpRet);
        status = VSCP_TP_ADAPTER_RET_ERROR;
    }
    /* Clear rx event queue and go sure that no L2 event is in the queue right now. */
    else if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_clearDaemonEventQueue(client->hSession)))
    {
        LOG_ERROR_INT32("Couldn't clear rx event queue:", vscphlpRet);
        status = VSCP_TP_ADAPTER_RET_ERROR;
    }
    /* Get interface GUID */
    else if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_getGUID(client->hSession, client->interfaceGUID)))
    {
        LOG_ERROR_INT32("Couldn't get interface GUID:", vscphlpRet);
        status = VSCP_TP_ADAPTER_RET_ERROR;
    }
    else
    /* Successful connected */
    {
        unsigned char   major       = 0;
        unsigned char   minor       = 0;
        unsigned char   subMinor    = 0;
        unsigned long   dllVersion  = 0;
        char            vendorStr[120];

        vscp_tp_adapter_isConnected = TRUE;

        LOG_INFO("Connected.");
        
        log_printf("Interface GUID: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n",
                    client->interfaceGUID[0],
                    client->interfaceGUID[1],
                    client->interfaceGUID[2],
                    client->interfaceGUID[3],
                    client->interfaceGUID[4],
                    client->interfaceGUID[5],
                    client->interfaceGUID[6],
                    client->interfaceGUID[7],
                    client->interfaceGUID[8],
                    client->interfaceGUID[9],
                    client->interfaceGUID[10],
                    client->interfaceGUID[11],
                    client->interfaceGUID[12],
                    client->interfaceGUID[13],
                    client->interfaceGUID[14],
                    client->interfaceGUID[15]);

        /* Get version of remote VSCP daemon */
        if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_getVersion(client->hSession, &major, &minor, &subMinor)))
        {
            LOG_WARNING_INT32("vscphlp_getVersion failed:", vscphlpRet);
        }
        else
        {
            log_printf("Remote VSCP daemon v%u.%u.%u\n", major, minor, subMinor);
        }

        /* Get dll version */
        if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_getDLLVersion(client->hSession, &dllVersion)))
        {
            LOG_WARNING_INT32("vscphlp_getDLLVersion failed:", vscphlpRet);
        }
        else
        {
            log_printf("Used VSCP dll version 0x%08lX\n", dllVersion);
        }

        /* Get vendor from driver */
        if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_getVendorString(client->hSession, vendorStr, sizeof(vendorStr))))
        {
            LOG_WARNING_INT32("vscphlp_getVendorString failed:", vscphlpRet);
        }
        else
        {
            log_printf("Vendor of driver: %s\n", vendorStr);
        }
    }

    /* Any error happened? */
    if (VSCP_TP_ADAPTER_RET_OK != status)
    {
        /* If a session is opened, close it. */
        if (0 != client->hSession)
        {
            (void)vscphlp_close(client->hSession);
            vscphlp_closeSession(client->hSession);
            client->hSession = 0;
        }
    }

    return status;
}

/**
 * This function disconnects a connection to a VSCP daemon.
 */
extern void vscp_tp_adapter_disconnect(void)
{
    vscp_tp_adapter_NetPar* client  = &vscp_tp_adapter_clientPar;

    /* If a session is opened, close it. */
    if (0 != client->hSession)
    {
        (void)vscphlp_close(client->hSession);
        vscphlp_closeSession(client->hSession);
        client->hSession = 0;

        LOG_INFO("Disconnected.");
    }

    vscp_tp_adapter_isConnected = FALSE;

    return;
}

/**
 * This function simulates a received message.
 *
 * @param[in]   msg Received message
 */
extern void vscp_tp_adapter_simulateReceivedMessage(vscp_RxMessage const * const msg)
{
    vscp_tp_adapter_rxMessage        = *msg;
    vscp_tp_adapter_messageReceived  = TRUE;
    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function handles a level 1 event from the daemon and converts it to the internal
 * vscp event structure.
 *
 * @param[in] msg           Internal vscp event
 * @param[in] daemonEvent   Daemon event
 * @return Error status
 * @retval FALSE    Successful
 * @retval TRUE     Failed
 */
static BOOL vscp_tp_adapter_handleL1Event(vscp_RxMessage * const msg, vscpEventEx const * const daemonEvent)
{
    BOOL isError = FALSE;

    if ((NULL == msg) ||
        (NULL == daemonEvent))
    {
        isError = TRUE;
    }
    else if (VSCP_CLASS_L1_L2_BASE <= daemonEvent->vscp_class)
    {
        LOG_WARNING("Invalid L1 event.");
        isError = TRUE;
    }
    else if (VSCP_L1_DATA_SIZE < daemonEvent->sizeData)
    {
        LOG_WARNING("Invalid L1 event.");
        isError = TRUE;
    }
    else
    {
        uint8_t index   = 0;

        msg->vscpClass  = daemonEvent->vscp_class;
        msg->vscpType   = (uint8_t)(daemonEvent->vscp_type & 0xff);
        msg->priority   = (daemonEvent->head >> 5) & 0x07;
        msg->oAddr      = daemonEvent->GUID[15]; /* Node GUID LSB */
        msg->hardCoded  = (daemonEvent->head >> 4) & 0x01;
        msg->dataNum    = (uint8_t)(daemonEvent->sizeData & 0xff);

        for(index = 0; index < msg->dataNum; ++index)
        {
            msg->data[index] = daemonEvent->data[index];
        }
    }

    return isError;
}

/**
 * This function handles a level 1 over level 2 event from the daemon and converts it to the internal
 * vscp event structure.
 *
 * @param[in] msg           Internal vscp event
 * @param[in] daemonEvent   Daemon event
 * @return Error status
 * @retval FALSE    Successful
 * @retval TRUE     Failed
 */
static BOOL vscp_tp_adapter_handleL1OverL2Event(vscp_RxMessage * const msg, vscpEventEx const * const daemonEvent)
{
    BOOL isError = FALSE;

    if ((NULL == msg) ||
        (NULL == daemonEvent))
    {
        isError = TRUE;
    }
    else if ((VSCP_CLASS_L1_L2_BASE > daemonEvent->vscp_class) ||
        (1024 <= daemonEvent->vscp_class))
    {
        LOG_WARNING("Invalid L1 event over L2.");
        isError = TRUE;
    }
    else if ((VSCP_L1_DATA_SIZE + VSCP_TP_ADAPTER_INTERFACE_GUID_SIZE) < daemonEvent->sizeData)
    {
        LOG_WARNING("Invalid L1 event over L2.");
        isError = TRUE;
    }
    else
    {
        uint8_t index       = 0;
        uint8_t dataOffset  = 0;

        msg->vscpClass  = daemonEvent->vscp_class - VSCP_CLASS_L1_L2_BASE;
        msg->vscpType   = (uint8_t)(daemonEvent->vscp_type & 0xff);
        msg->priority   = (daemonEvent->head >> 5) & 0x07;
        msg->oAddr      = daemonEvent->GUID[15]; /* Node GUID LSB */
        msg->hardCoded  = (daemonEvent->head >> 4) & 0x01;

        /* Determine whether the event has the interface GUID in the payload or not.
         * This is done quite simple by checking for greater or equal the
         * GUID size. Because all other level 1 events has a lower data size.
         */
        msg->dataNum    = (uint8_t)(daemonEvent->sizeData & 0xff);
        if (VSCP_TP_ADAPTER_INTERFACE_GUID_SIZE <= msg->dataNum)
        {
            /* Overstep the interface GUID */
            msg->dataNum -= VSCP_TP_ADAPTER_INTERFACE_GUID_SIZE;
            dataOffset = VSCP_TP_ADAPTER_INTERFACE_GUID_SIZE;
        }

        for(index = 0; index < msg->dataNum; ++index)
        {
            msg->data[index] = daemonEvent->data[index + dataOffset];
        }
    }

    return isError;
}

/**
 * This function prints a VSCP message to the screen.
 *
 * @param[in] msg           VSCP message
 * @param[in] isReceived    Is message received or transmitted?
 */
static void vscp_tp_adapter_showMessage(vscp_Message const * const msg, BOOL isReceived)
{
    uint8_t index = 0;

    log_printf("%cx: class=0x%04X type=0x%02X prio=%2d oAddr=0x%02X %c num=%u data=",
        (TRUE == isReceived) ? 'R' : 'T',
        msg->vscpClass,
        msg->vscpType,
        msg->priority,
        msg->oAddr,
        (FALSE == msg->hardCoded) ? '-' : 'h',
        msg->dataNum);

    for(index = 0; index < msg->dataNum; ++index)
    {
        printf("%02X", msg->data[index]);

        if ((index + 1) < msg->dataNum)
        {
            printf(" ");
        }
    }

    if ((VSCP_CLASS_L1_PROTOCOL == msg->vscpClass) ||
        (VSCP_CLASS_L1_L2_PROTOCOL == msg->vscpClass))
    {
        if (VSCP_UTIL_ARRAY_NUM(vscp_tp_adapter_protocolTypes) <= msg->vscpType)
        {
            printf(" ?");
        }
        else
        {
            printf(" %s", vscp_tp_adapter_protocolTypes[msg->vscpType]);
        }
    }

    printf("\n");

    return;
}

/**
 * This function returns a error from the VSCP helper library as user friendly string.
 *
 * @param[in] value Error value
 * @return User friendly description of the error value.
 */
static const char* vscp_tp_adapter_getErrorStr(int value)
{
    char const * str    = "!!Error unknown!!";

    if (VSCP_UTIL_ARRAY_NUM(vscp_tp_adapter_errorStr) > value)
    {
        str = vscp_tp_adapter_errorStr[value];
    }

    return str;
}

