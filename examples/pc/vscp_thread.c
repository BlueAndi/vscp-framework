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
@brief  VSCP threads
@file   vscp_thread.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_thread.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_thread.h"
#include <pthread.h>
#include <memory.h>
#include "platform.h"
#include "vscp_core.h"
#include "vscp_timer.h"
#include "vscp_portable.h"
#include "vscp_bootloader.h"

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

/** This type contains the necessary thread data. */
typedef struct
{
    pthread_t           id;             /**< Thread id */
    int                 status;         /**< Thread creation status */
    pthread_mutex_t*    mutex;          /**< Mutex to protect the thread data */
    BOOL                quitFlag;       /**< Flag to signal the thread to quit */

} vscp_thread_Context;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static void* vscp_thread_frameworkThread(void* par);
static void* vscp_thread_vscpTimerThread(void* par);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Mutex used to protect the whole VSCP framework against concurrent access. */
static pthread_mutex_t      vscp_thread_mutex   = PTHREAD_MUTEX_INITIALIZER;

/** VSCP framework thread data */
static vscp_thread_Context  vscp_thread_frameworkThrdData;

/** VSCP timers thread data */
static vscp_thread_Context  vscp_thread_timerThrdData;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the module.
 */
extern void vscp_thread_init(void)
{
    /* Clear thread data */
    memset(&vscp_thread_frameworkThrdData, 0, sizeof(vscp_thread_frameworkThrdData));
    memset(&vscp_thread_timerThrdData, 0, sizeof(vscp_thread_timerThrdData));

    /* Set error value here, to be able to check if vscp_thread_start is called and
     * the threads are already started.
     */
    vscp_thread_frameworkThrdData.status    = 1;
    vscp_thread_timerThrdData.status        = 1;

    /* Set mutex */
    vscp_thread_frameworkThrdData.mutex = &vscp_thread_mutex;
    vscp_thread_timerThrdData.mutex     = &vscp_thread_mutex;

    return;
}

/**
 * This function starts the VSCP threads.
 *
 * @return Status
 */
extern VSCP_THREAD_RET vscp_thread_start(void)
{
    VSCP_THREAD_RET status  = VSCP_THREAD_RET_OK;

    /* ----- Start VSCP framework thread ----- */

    if (0 != vscp_thread_frameworkThrdData.status)
    {
        /* Avoid that the thread stops immediately */
        vscp_thread_frameworkThrdData.quitFlag    = FALSE;

        /* Create vscp framework thread with default attributes */
        vscp_thread_frameworkThrdData.status = pthread_create(&vscp_thread_frameworkThrdData.id, NULL, vscp_thread_frameworkThread, (void*)&vscp_thread_frameworkThrdData);

        /* Failed to create the thread? */
        if (0 != vscp_thread_frameworkThrdData.status)
        {
            status = VSCP_THREAD_RET_ERROR;
        }
        else
        {
            /* ----- Start VSCP timer thread ----- */

            if (0 != vscp_thread_timerThrdData.status)
            {
                /* Avoid that the thread stops immediately */
                vscp_thread_timerThrdData.quitFlag    = FALSE;

                /* Create vscp framework thread with default attributes */
                vscp_thread_timerThrdData.status = pthread_create(&vscp_thread_timerThrdData.id, NULL, vscp_thread_vscpTimerThread, (void*)&vscp_thread_timerThrdData);

                /* Failed to create the thread? */
                if (0 != vscp_thread_timerThrdData.status)
                {
                    status = VSCP_THREAD_RET_ERROR;

                    /* Stop framework thread */
                    vscp_thread_lock();
                    vscp_thread_frameworkThrdData.quitFlag = TRUE;
                    vscp_thread_unlock();

                    (void)pthread_join(vscp_thread_frameworkThrdData.id, NULL);
                }
            }
        }
    }

    return status;
}

/**
 * This function stops the VSCP threads.
 */
extern void vscp_thread_stop(void)
{
    /* Is VSCP framework thread running? */
    if (0 == vscp_thread_frameworkThrdData.status)
    {
        vscp_thread_lock();
        vscp_thread_frameworkThrdData.quitFlag = TRUE;
        vscp_thread_unlock();

        /* Wait for the VSCP framework thread until its finished. */
        (void)pthread_join(vscp_thread_frameworkThrdData.id, NULL);

        /* Set error value here, to be able to check if vscp_thread_start is called and
         * the threads are already started.
         */
        vscp_thread_frameworkThrdData.status = 1;
    }

    /* Is VSCP timer thread running? */
    if (0 == vscp_thread_timerThrdData.status)
    {
        (void)pthread_mutex_lock(vscp_thread_timerThrdData.mutex);
        vscp_thread_timerThrdData.quitFlag = TRUE;
        (void)pthread_mutex_unlock(vscp_thread_timerThrdData.mutex);

        /* Wait for the VSCP timer thread until its finished. */
        (void)pthread_join(vscp_thread_timerThrdData.id, NULL);

        /* Set error value here, to be able to check if vscp_thread_start is called and
         * the threads are already started.
         */
        vscp_thread_timerThrdData.status = 1;
    }

    return;
}

/**
 * This function locks the threads.
 */
extern void vscp_thread_lock(void)
{
    (void)pthread_mutex_lock(&vscp_thread_mutex);
    return;
}

/**
 * This function unlocks the threads.
 */
extern void vscp_thread_unlock(void)
{
    (void)pthread_mutex_unlock(&vscp_thread_mutex);
    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This thread process the VSCP framework.
 *
 * @param[in]   par Thread parameters
 * @return Not used
 */
static void* vscp_thread_frameworkThread(void* par)
{
    BOOL                    quitFlag        = FALSE;
    vscp_thread_Context*    threadData      = (vscp_thread_Context*)par;
    BOOL                    bootloaderMode  = FALSE;

    (void)pthread_mutex_lock(threadData->mutex);
    quitFlag = threadData->quitFlag;
    (void)pthread_mutex_unlock(threadData->mutex);

    while(FALSE == quitFlag)
    {
        /* Process the whole VSCP framework */
        (void)pthread_mutex_lock(threadData->mutex);

        /* Application */
        if (FALSE == bootloaderMode)
        {
            (void)vscp_core_process();

            bootloaderMode = vscp_portable_isBootloaderRequested();
        }
        /* Bootloader */
        else
        {
            vscp_bootloader_init();
            vscp_bootloader_run();
            bootloaderMode = FALSE;

            /* Simulate reboot */
            vscp_core_init();
        }

        quitFlag = threadData->quitFlag;

        (void)pthread_mutex_unlock(threadData->mutex);

        /* Give a other threads a minimal chance. */
        platform_delay(1);
    }

    pthread_exit(NULL);

    return NULL;
}

/**
 * This thread process the VSCP framework.
 *
 * @param[in]   par Thread parameters
 * @return  Not used
 */
static void* vscp_thread_vscpTimerThread(void* par)
{
    BOOL                    quitFlag    = FALSE;
    vscp_thread_Context*    threadData  = (vscp_thread_Context*)par;
    uint16_t                waitTime    = 100;

    (void)pthread_mutex_lock(threadData->mutex);
    quitFlag = threadData->quitFlag;
    (void)pthread_mutex_unlock(threadData->mutex);

    while(FALSE == quitFlag)
    {
        /* Wait a specific time, until the timers are processed again. */
        platform_delay(waitTime);

        (void)pthread_mutex_lock(threadData->mutex);

        /* Process the VSCP framework timers */
        vscp_timer_process(waitTime);

        quitFlag = threadData->quitFlag;

        (void)pthread_mutex_unlock(threadData->mutex);
    }

    pthread_exit(NULL);

    return NULL;
}
