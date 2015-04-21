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
@brief  Wind speed driver
@file   windDrv.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see windDrv.h

@section svn Subversion
$Author: amerkle $
$Rev: 452 $
$Date: 2015-01-06 10:45:56 +0100 (Di, 06 Jan 2015) $
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "windDrv.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** Calibration frequency in 0.1Hz which corresponds to 1m/s. This value depends
 * on the mechanical construction of the windmill.
 */
#define WINDDRV_CALIB_FREQ          21u

/** Lower wind speed limit in m/s, which has a mechanical cause. */
#define WINDDRV_SPEED_LIMIT_LOW     1u

/** Upper wind speed limit in m/s, which has a mechanical cause. */
#define WINDDRV_SPEED_LIMIT_HIGH    25u

/** Maximum period in ms (low speed) of the windmill */
#define WINDDRV_PERIOD_LIMIT_LOW    ((uint16_t)(10000u / (WINDDRV_SPEED_LIMIT_LOW * WINDDRV_CALIB_FREQ)))

/** Minimal period in ms (high speed) of the windmill */
#define WINDDRV_PERIOD_LIMIT_HIGH   ((uint16_t)(10000u / (WINDDRV_SPEED_LIMIT_HIGH * WINDDRV_CALIB_FREQ)))

/** Every sample period in ms the number of turns of the windmill is checked. */
#define WINDDRV_SAMPLE_PERIOD       500u

#if (16000000UL == F_CPU)

/* CPU clock = 16 MHz
 * Prescaler = 256
 * Timer clock = CPU clock / Prescaler = 62,5 kHz
 * => Timer increase every 1 / 62,5 kHz = 16 us
 * => Overflow happens after 256, therefore every (256 * 16 us) = 4,096 ms
 *
 * Generate a interrupt every 4 ms, that means the interrupt shall be 0,096 ms faster.
 * => 0,096 ms / 16 us = 6
 * => Preload the counter register with 6.
 *
 * Now increase every 125th interrupt the tick counter to get a 500 ms tick.
 */

/* Prescaler of 256 */
#define WINDDRV_PRESCALER       ((1 << CS22) | (1 << CS21) | (0 << CS20))

/* Preload value */
#define WINDDRV_PRELOAD_VALUE   6

/* Timer threshold value */
#define WINDDRV_THRESHOLD_VALUE 125

#else

#error CPU clock not supported!

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

/** Count every timer interrupt */
static volatile uint8_t windDrv_timerInterruptCnt   = 0;

/** Count every turn of the windmill. */
static volatile uint8_t windDrv_turnCnt             = 0;

/** Wind speed in [dm/s] */
static volatile uint8_t windDrv_speed               = 0;

/******************************************************************************\
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * This function initialize the module (static initialization). Don't call
 * other functions before this function is executed.
 */
extern void windDrv_init(void)
{
    windDrv_timerInterruptCnt    = 0;

    /* Set timer prescaler */
    TCCR2B = WINDDRV_PRESCALER;

    /* Preload timer counter */
    TCNT2 = WINDDRV_PRELOAD_VALUE;

    return;
}

/**
 * This function enable or disable the wind measurement.
 *
 * @param[in]   enableIt    Enable/disable wind measurement
 */
extern void windDrv_enable(BOOL enableIt)
{
    if (FALSE == enableIt)
    {
        /* Disable INT1 interrupt */
        BIT_CLR(EIMSK, INT1);

        /* Disable timer overflow interrupt */
        BIT_CLR(TIMSK2, TOIE2);
    }
    else
    {
        /* Enable INT1 interrupt */
        BIT_SET(EIMSK, INT1);

        /* Enable timer overflow interrupt */
        BIT_SET(TIMSK2, TOIE2);
    }

    return;
}

/**
 * This function returns the current wind speed in [dm/s].
 * Note, that [dm/s] is deci-meter per second!!
 * Divide it by 10 to get [m/s].
 */
extern uint8_t  windDrv_getWindSpeed(void)
{
    return windDrv_speed;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This interrupt service routine calculates the wind speed in a defined time slot.
 */
ISR(TIMER2_OVF_vect)
{
    ++windDrv_timerInterruptCnt;

    if (WINDDRV_THRESHOLD_VALUE <= windDrv_timerInterruptCnt)
    {
        /* If the wind speed is higher than the windmill maximum, the wind speed
         * will be set to the limit.
         */
        if ((WINDDRV_SAMPLE_PERIOD / WINDDRV_PERIOD_LIMIT_HIGH) < windDrv_turnCnt)
        {
            windDrv_speed = 10u * WINDDRV_SPEED_LIMIT_HIGH; /* [dm/s] */
        }
        /* Is windless (calm)? */
        else if (0 == windDrv_turnCnt)
        {
            windDrv_speed = 0u; /* [dm/s] */
        }
        /* Calculate wind speed */
        else
        {
            /* Calculate frequency = number of turns / delta time */
            uint16_t    frequency = (uint8_t)( ( 1000u * (uint16_t)windDrv_turnCnt ) / ( WINDDRV_SAMPLE_PERIOD / 10u ) );  /* [0,1 Hz] */

            /* Calculate the wind speed = ( 10 dm/s * frequency ) / calibration frequency */
            windDrv_speed = (10u * frequency) / WINDDRV_CALIB_FREQ; /* [dm/s] */
        }

        /* Reset turn counter */
        windDrv_turnCnt = 0;

        /* Reset timer interrupt counter */
        windDrv_timerInterruptCnt = 0;
    }

    /* Preload timer counter */
    TCNT2 = WINDDRV_PRELOAD_VALUE;
}

/**
 * This interrupt service routine counts every turn of the windmill.
 */
ISR(INT1_vect)
{
    if (0xffu > windDrv_turnCnt)
    {
        ++windDrv_turnCnt;
    }
}
