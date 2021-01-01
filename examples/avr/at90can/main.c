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
@brief  Main entry point
@file   main.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains the main entry point.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "system.h"
#include <stdint.h>
#include "hw.h"
#include "timerDrv.h"
#include "vscp_core.h"
#include "vscp_timer.h"
#include "vscp_portable.h"
#include "swTimer.h"
#include "time.h"
#include "serialDrv.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/** 10 ms software timer id */
#define MAIN_SWTIMER_10MS_ID        0

/** 10 ms software timer period */
#define MAIN_SWTIMER_10MS_PERIOD    10

/** 250 ms software timer id */
#define MAIN_SWTIMER_250MS_ID       1

/** 250 ms software timer period */
#define MAIN_SWTIMER_250MS_PERIOD   250

/** Segment initialization button debouncing time in ms. */
#define MAIN_SEG_INIT_DEBOUNCE_TIME 100

/** Define the serial port which is used as terminal. */
#define MAIN_SERIAL_PORT            SERIALDRV_USART_0

/** Serial terminal rx storage size in byte */
#define MAIN_SERIAL_RX_SIZE         10

/** Serial terminal tx storage size in byte */
#define MAIN_SERIAL_TX_SIZE         10

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the supported function return values. */
typedef enum
{
    MAIN_RET_OK = 0,    /**< Successful */
    MAIN_RET_ERROR,     /**< Failed */
    MAIN_RET_ENULL      /**< Unexpected NULL pointer */

} MAIN_RET;

/*******************************************************************************
    PROTOTYPES
*******************************************************************************/

static MAIN_RET main_initRunLevel1(void);
static MAIN_RET main_initRunLevel2(void);
static void main_print(char const * const str);
static void main_timerCb(void);
static void main_processStatusLamp(void);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Handle to serial port */
static serialDrv_Handle     main_hSerial                = SERIALDRV_INVALID_HANDLE;

/** Serial rx storage */
static uint8_t              main_serialRxStorage[MAIN_SERIAL_RX_SIZE];

/** Serial tx storage */
static uint8_t              main_serialTxStorage[MAIN_SERIAL_TX_SIZE];

/** Segment init button state */
static volatile BOOL        main_isInitButtonPressed    = FALSE;

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * Main entry point.
 *
 * @return Not used
 */
int main(void)
{
    BOOL    lastSegInitButtonState  = FALSE;    /* Used for raising edge detection */

    /* ********** Run level 1 - interrupts disabled ********** */

    /* Enter run level 1 */
    if (MAIN_RET_OK != main_initRunLevel1())
    {
        /* Error */
        HALT();
    }

    /* ********** Run level 2 - interrupts enabled ********** */

    /* Enable interrupts */
    sei();

    /* Enter run level 2 */
    if (MAIN_RET_OK != main_initRunLevel2())
    {
        /* Error */
        HALT();
    }

    /* ********** Run level 3 - main loop starts ********** */

    /* Show VSCP protocol and framework version */
    main_print("VSCP framework ");
    main_print(VSCP_CORE_FRAMEWORK_VERSION);
    main_print("\n");
    main_print("VSCP ");
    main_print(VSCP_CORE_VERSION_STR);
    main_print("\n\n");

    /* Main loop */
    for(;;)
    {
        /* Process VSCP framework */
        vscp_core_process();

        /* Initialize the VSCP segment, because user pressed the segment
         * initialization button?
         *
         * If the user keeps the button pressed and it looks like the user never
         * release it, the initialization will anyway take place only once.
         */
        if ((TRUE == main_isInitButtonPressed) &&
            (FALSE == lastSegInitButtonState))
        {
            vscp_core_startNodeSegmentInit();
        }

        lastSegInitButtonState = main_isInitButtonPressed;

        /* 250 ms period */
        if (TRUE == swTimer_getStatus(MAIN_SWTIMER_250MS_ID))
        {
            /* Process VSCP timers */
            vscp_timer_process(MAIN_SWTIMER_250MS_PERIOD);

            /* Process VSCP lamp blinking */
            main_processStatusLamp();

            /* Process the time */
            time_process(MAIN_SWTIMER_250MS_PERIOD);
        }

        /* Some stuff shall only be done in case that VSCP is in ACTIVE state. */
        if (TRUE == vscp_core_isActive())
        {
            /* Implement your code here ... */


        }
    }

    return 0;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/

/**
 * This function initializes run level 1.
 * Precondition: Interrupts are disabled
 *
 * @return Status
 */
static MAIN_RET main_initRunLevel1(void)
{
    MAIN_RET    status  = MAIN_RET_OK;

    /* Initialize the hardware */
    hw_init();

    /* Initialize a hardware timer */
    timerDrv_init();
    timerDrv_setCallback(main_timerCb);

    /* Initialize serial driver */
    serialDrv_init();

    /* Initialize serial port */
    main_hSerial = serialDrv_get(MAIN_SERIAL_PORT);

    serialDrv_initUsart(main_hSerial,
                        main_serialRxStorage,
                        sizeof(main_serialRxStorage),
                        main_serialTxStorage,
                        sizeof(main_serialTxStorage));

    /* Open serial port */
    if (SERIALDRV_RET_OK != serialDrv_open( main_hSerial,
                                            SERIALDRV_BAUD_38400,
                                            SERIALDRV_DATA_8,
                                            SERIALDRV_PARITY_NONE,
                                            SERIALDRV_STOPBIT_1))
    {
        status = MAIN_RET_ERROR;
    }

    /* Initialize the VSCP framework */
    if (VSCP_CORE_RET_OK != vscp_core_init())
    {
        status = MAIN_RET_ERROR;
    }

    /* Initialize software timer */
    swTimer_init();

    /* Initialize the time module */
    time_init();

    /* Configure all software timer */
    swTimer_start(MAIN_SWTIMER_10MS_ID, MAIN_SWTIMER_10MS_PERIOD, FALSE);
    swTimer_start(MAIN_SWTIMER_250MS_ID, MAIN_SWTIMER_250MS_PERIOD, FALSE);

    return status;
}

/**
 * This function initializes run level 2.
 * Precondition: Interrupts are enabled
 *
 * @return Status
 */
static MAIN_RET main_initRunLevel2(void)
{
    MAIN_RET    status  = MAIN_RET_OK;

    /* Implement your code here ... */

    return status;
}

/**
 * This function prints a string to the serial port.
 * Safety feature: If the string has a length greater than 200 characters,
 * the function aborts.
 *
 * @param[in] str   String
 */
static void main_print(char const * const str)
{
    uint8_t         index   = 0;
    const uint8_t   maxLen  = 200;

    while((maxLen > index) && ('\0' != str[index]))
    {
        while(0 == serialDrv_write(main_hSerial, str[index]))
        {
            /* Wait */
            ;
        }

        ++index;
    }

    return;
}

/**
 * This function is called by the timer ISR every tick!
 */
static void main_timerCb(void)
{
    static volatile uint8_t     segmentInitButtonDebounceCnt    = 0;

    /* Is the segment initialization button released? */
    if (FALSE == hw_getSegInitButtonState())
    {
        segmentInitButtonDebounceCnt = 0;
        main_isInitButtonPressed = FALSE;
    }
    /* Segment initialization button is pressed.
     * Perform debouncing!
     */
    else if (TIMERDRV_MS(MAIN_SEG_INIT_DEBOUNCE_TIME) <= segmentInitButtonDebounceCnt)
    {
        main_isInitButtonPressed = TRUE;
    }
    else
    {
        ++segmentInitButtonDebounceCnt;
    }

    /* Process all software timer */
    swTimer_process();

    return;
}

/**
 * This function process the status lamp.
 */
static void main_processStatusLamp(void)
{
    VSCP_LAMP_STATE state   = vscp_portable_getLampState();
    static uint8_t  slowCnt = 0;

    switch(state)
    {
    case VSCP_LAMP_STATE_OFF:
        HW_DISABLE_STATUS_LED();
        break;

    case VSCP_LAMP_STATE_ON:
        HW_ENABLE_STATUS_LED();
        break;

    case VSCP_LAMP_STATE_BLINK_SLOW:
        if (0 == (slowCnt % 4))
        {
            HW_TOGGLE_STATUS_LED();
        }
        break;

    case VSCP_LAMP_STATE_BLINK_FAST:
        HW_TOGGLE_STATUS_LED();
        break;

    default:
        break;
    }

    /* Reset counter for slow blinking */
    if (VSCP_LAMP_STATE_BLINK_SLOW != state)
    {
        slowCnt = 0;
    }
    else
    {
        ++slowCnt;
    }

    return;
}
