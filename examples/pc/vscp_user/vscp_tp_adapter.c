/* The MIT License (MIT)
 * 
 * Copyright (c) 2014 - 2015, Andreas Merkle
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

@section svn Subversion
$Author: amerkle $
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_tp_adapter.h"
#include <stdio.h>
#include <memory.h>
#include "log.h"
#include "vscphelperlib.h"
#include "vscp_class_l1_l2.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Size of the interface GUID in byte */
#define VSCP_TP_ADAPTER_INTERFACE_GUID_SIZE 16

/** Base class id of L2 events */
#define VSCP_TP_ADAPTER_CLASS_L2_BASE       1024

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

} vscp_tp_adapter_NetPar;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static BOOL vscp_tp_adapter_handleL1Event(vscp_RxMessage * const msg, vscpEventEx const * const daemonEvent);
static BOOL vscp_tp_adapter_handleL1OverL2Event(vscp_RxMessage * const msg, vscpEventEx const * const daemonEvent);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Flag which signals that a message is received. */
static BOOL                     vscp_tp_adapter_messageReceived  = FALSE;

/** Receive message */
static vscp_RxMessage           vscp_tp_adapter_rxMessage;

/** Network client parameter */
static vscp_tp_adapter_NetPar   vscp_tp_adapter_clientPar;

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
        uint8_t                 index   = 0;
        
        /* Connected to a VSCP daemon? */
        if (0 != client->hSession)
        {
            uint32_t    count       = 0;
            vscpEventEx daemonEvent;
            int         vscphlpRet  = 0;
            
            /* Check for available events. */
            if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_isDataAvailable(client->hSession, &count)))
            {
                LOG_WARNING_INT32("Connection lost: ", vscphlpRet);
            }
            /* Any event available? */
            else if (0 < count)
            {
                if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_receiveEventEx(client->hSession, &daemonEvent)))
                {
                    LOG_WARNING_INT32("Connection lost: ", vscphlpRet);
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
            log_printf("Rx: class=0x%02X, type=0x%02X, prio=%2d, oAddr=0x%02X, %c, num=%u, data=",
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
            printf("\n");
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
        
        log_printf("Tx: class=0x%02X, type=0x%02X, prio=%2d, oAddr=0x%02X, %c, num=%u, data=",
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
        printf("\n");
            
        /* Connected to a VSCP daemon? */
        if (0 != client->hSession)
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
            daemonEvent.GUID[15]    = msg->oAddr; /* Node GUID LSB */
            daemonEvent.sizeData    = msg->dataNum;
            
            for(index = 0; index < daemonEvent.sizeData; ++index)
            {
                daemonEvent.data[index] = msg->data[index];
            }
            
            if (VSCP_ERROR_SUCCESS != (vscphlpRet = vscphlp_sendEventEx(client->hSession, &daemonEvent)))
            {
                LOG_WARNING_INT32("Couldn't send event to daemon:", vscphlpRet);
            }
        }

        status = TRUE;
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
    else if (VSCP_ERROR_SUCCESS != vscphlp_setResponseTimeout(client->hSession, 6))
    {
        LOG_ERROR("Couldn't set response timeout.");
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
    else
    /* Successful connected */
    {
        LOG_INFO("Connected.");
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
        if (VSCP_TP_ADAPTER_INTERFACE_GUID_SIZE < msg->dataNum)
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
