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
@brief  Temperature simulation
@file   temperature_sim.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see temperature_sim.h

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "temperature_sim.h"
#include <memory.h>
#include <pthread.h>
#include <time.h>
#include "platform.h"
#include "vscp_thread.h"
#include "vscp_core.h"
#include "vscp_evt_measurement.h"
#include "vscp_evt_measurezone.h"
#include "vscp_data_coding.h"
#include "vscp_class_l1.h"
#include "vscp_type_measurement.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/*******************************************************************************
    MACROS
*******************************************************************************/

/** Number of elements in a array */
#define TEMPERATURE_SIM_ARRAY_NUM(__arr)    (sizeof(__arr) / sizeof((__arr)[0]))

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

} temperature_sim_Context;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static void* temperature_sim_thread(void* par);
static uint32_t temperature_sim_getTemperature(void);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Mutex used to protect the whole VSCP framework against concurrent access. */
static pthread_mutex_t          temperature_sim_mutex   = PTHREAD_MUTEX_INITIALIZER;

/** Thread data */
static temperature_sim_Context  temperature_sim_thrdData;

/** Simple temperature average in 0,01 degree celsius over months for one year. */
static uint32_t                 temperature_sim_temperatureAvgPerMonth[12] =
{
    800, 1000, 1300, 1650, 1920, 2100, 2100, 1920, 1650, 1300, 1000, 980
};

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the module.
 */
extern void temperature_sim_init(void)
{
    /* Clear thread data */
    memset(&temperature_sim_thrdData, 0, sizeof(temperature_sim_thrdData));

    /* Set error value here, to be able to check if temperature_sim_start is called and
     * the thread is already started.
     */
    temperature_sim_thrdData.status = 1;

    /* Set mutex */
    temperature_sim_thrdData.mutex = &temperature_sim_mutex;

    return;
}

/**
 * This function starts the temperature simulation.
 *
 * @return Status
 */
extern TEMPERATURE_SIM_RET temperature_sim_start(void)
{
    TEMPERATURE_SIM_RET status  = TEMPERATURE_SIM_RET_OK;

    if (0 != temperature_sim_thrdData.status)
    {
        /* Avoid that the thread stops immediately */
        temperature_sim_thrdData.quitFlag   = FALSE;

        /* Create thread with default attributes */
        temperature_sim_thrdData.status = pthread_create(&temperature_sim_thrdData.id, NULL, temperature_sim_thread, (void*)&temperature_sim_thrdData);

        /* Failed to create the thread? */
        if (0 != temperature_sim_thrdData.status)
        {
            status = TEMPERATURE_SIM_RET_ERROR;
        }
    }

    return status;
}

/**
 * This function stops the temperature simulation.
 */
extern void temperature_sim_stop(void)
{
    /* Is thread running? */
    if (0 == temperature_sim_thrdData.status)
    {
        (void)pthread_mutex_lock(&temperature_sim_mutex);
        temperature_sim_thrdData.quitFlag = TRUE;
        (void)pthread_mutex_unlock(&temperature_sim_mutex);

        /* Wait for the VSCP framework thread until its finished. */
        (void)pthread_join(temperature_sim_thrdData.id, NULL);

        /* Set error value here, to be able to check if temperature_sim_start is called and
         * the thread is already started.
         */
        temperature_sim_thrdData.status = 1;
    }

    return;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This thread simulates the temperature and sends periodically VSCP temperature
 * events.
 *
 * @param[in]   par Thread parameters
 * @return Not used
 */
static void* temperature_sim_thread(void* par)
{
    BOOL                        quitFlag    = FALSE;
    temperature_sim_Context*    threadData  = (temperature_sim_Context*)par;
    const uint32_t              sleepTime   = 100;  /* [ms] */
    const uint32_t              period      = 5000; /* [ms] */
    uint32_t                    timeCnt     = 0;

    (void)pthread_mutex_lock(threadData->mutex);
    quitFlag = threadData->quitFlag;
    (void)pthread_mutex_unlock(threadData->mutex);

    while(FALSE == quitFlag)
    {
        (void)pthread_mutex_lock(threadData->mutex);
        quitFlag = threadData->quitFlag;
        (void)pthread_mutex_unlock(threadData->mutex);

        if (0 == timeCnt)
        {
            int32_t temperatureC = temperature_sim_getTemperature();    /* [0,01 degree celsius] */
            int32_t temperatureK = 27315 + temperatureC;                /* [0,01 kelvin] */

            vscp_thread_lock();

            if (TRUE == vscp_core_isActive())
            {
                (void)vscp_evt_measurement_sendTemperature(0, 1, temperatureC, -2);
                (void)vscp_evt_measurezone_sendTemperature(0, 255, 255, temperatureK, -2);
            }

            vscp_thread_unlock();

            timeCnt = period / sleepTime;
        }
        else
        {
            --timeCnt;
        }

        platform_delay(sleepTime);
    }

    pthread_exit(NULL);

    return NULL;
}

/**
 * This function returns the current simulated temperature in 0,01 degree celsius.
 *
 * @return Temperature [0,01 degree celsius]
 */
static uint32_t temperature_sim_getTemperature(void)
{
    time_t      rawtime;
    struct tm * timeInfo;
    uint32_t    temperature     = 0;    /* Temperature this month */
    uint32_t    temperaturePrev = 0;    /* Temperature previous month */
    uint32_t    temperatureNext = 0;    /* Temperature next month */
    uint8_t     monthIndex      = 0;
    uint8_t     monthPrevIndex  = 0;
    uint8_t     monthNextIndex  = 0;

    time(&rawtime);
    timeInfo = localtime(&rawtime);

    /* Attention!
     * Any weather frog who see the following calculation, please close both
     * eyes and forget it. :-)
     */

    /* Get average temperature of the current month */
    monthIndex = timeInfo->tm_mon;
    temperature = temperature_sim_temperatureAvgPerMonth[monthIndex];

    /* Get average temperature of the previous month */
    if (0 == monthIndex)
    {
        monthPrevIndex = TEMPERATURE_SIM_ARRAY_NUM(temperature_sim_temperatureAvgPerMonth) - 1;
    }
    else
    {
        monthPrevIndex = monthIndex - 1;
    }
    temperaturePrev = temperature_sim_temperatureAvgPerMonth[monthPrevIndex];

    /* Get average temperature of the next month */
    if (TEMPERATURE_SIM_ARRAY_NUM(temperature_sim_temperatureAvgPerMonth) == monthIndex)
    {
        monthNextIndex = 0;
    }
    else
    {
        monthNextIndex = monthIndex + 1;
    }
    temperatureNext = temperature_sim_temperatureAvgPerMonth[monthNextIndex];

    if (15 >= timeInfo->tm_mday)
    {
        /* Calculate average temperature of the day. */
        if (temperature < temperaturePrev)
        {
            temperature -= ((temperaturePrev - temperature) * timeInfo->tm_mday) / 30;
        }
        else
        {
            temperature += ((temperature - temperaturePrev) * timeInfo->tm_mday) / 30;
        }
    }
    else
    {
        /* Calculate average temperature of the day. */
        if (temperature < temperatureNext)
        {
            temperature += ((temperatureNext - temperature) * timeInfo->tm_mday) / 30;
        }
        else
        {
            temperature -= ((temperature - temperatureNext) * timeInfo->tm_mday) / 30;
        }
    }

    return temperature;
}

