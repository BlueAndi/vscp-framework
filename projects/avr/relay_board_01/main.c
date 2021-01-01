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
#include "vscp_logger.h"
#include "swTimer.h"
#include "time.h"
#include "pushButton.h"
#include "relay.h"
#include "windDrv.h"
#include "windObserver.h"
#include "buttonObserver.h"
#include "vscp_ps_user.h"
#include "shutterDrv.h"
#include "shutter.h"
#include <util/delay.h>
#include "sys_sm.h"
#include "watchdog.h"
#include "can_monitor.h"
#include "vscp_evt_information.h"
#include <avr/sleep.h>

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

/** 1 s software timer period, based on the 250 ms software timer */
#define MAIN_SWTIMER_1S_PERIOD      (1000 / MAIN_SWTIMER_250MS_PERIOD)

/** Shutter timer period in ms */
#define MAIN_SHUTTER_TIMER_PERIOD   100

/** VSCP logger default log level */
#define MAIN_LOG_LEVEL_DEFAULT      VSCP_LOGGER_LVL_ERROR

/** VSCP log functionality is disabled by default */
#define MAIN_LOGGER_ENABLED_DEFAULT FALSE

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
static void main_loadConfiguration(void);
static MAIN_RET main_initRunLevel2(void);
static void main_timerCb(void);
static void main_processStatusLamp(void);
static void main_checkSegInitButton(void);
static void main_shutterCb(uint8_t nr, SHUTTER_DIR dir, SHUTTER_POS pos);

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/** Segment initialization button state */
static volatile BOOL    main_isInitButtonPressed    = FALSE;

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
    BOOL    swTimer10MSTriggered    = FALSE;                    /* Signals that the 10 ms software timer triggered */
    BOOL    swTimer1STriggered      = FALSE;                    /* Signals that the 1s software timer triggered */
    uint8_t swTimer1S               = MAIN_SWTIMER_1S_PERIOD;   /* The 1s timer is based on 250 ms software timer */
    uint8_t index                   = 0;
    BOOL    shuttersAreIdle         = FALSE;

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

    /* Enable watchdog monitoring */
    watchdog_enable(TRUE);

    /* Main loop */
    for(;;)
    {
        SYS_SM_ACTION   sysSmAction = SYS_SM_ACTION_NOTHING;

        /* ----- System state independent jobs ----- */

        /* Process VSCP framework */
        (void)vscp_core_process();

        /* 10 ms period */
        if (TRUE == swTimer_getStatus(MAIN_SWTIMER_10MS_ID))
        {
            swTimer10MSTriggered = TRUE;

            /* Process watchdog */
            watchdog_process();

            /* Process every 10ms the shutter module. */
            shuttersAreIdle = shutter_process();

            /* Process every 10ms the shutter control. */
            if (TRUE == shutterDrv_process())
            {
                shuttersAreIdle = TRUE;
            }
        }

        /* 250 ms period */
        if (TRUE == swTimer_getStatus(MAIN_SWTIMER_250MS_ID))
        {
            /* Process VSCP timers */
            vscp_timer_process(MAIN_SWTIMER_250MS_PERIOD);

            /* Process CAN monitor */
            can_monitor_process();

            /* Process VSCP lamp blinking */
            main_processStatusLamp();

            /* Process the time */
            time_process(MAIN_SWTIMER_250MS_PERIOD);

            /* Derive a 1 s timer, because this can't be done with a software timer. */
            if (0 < swTimer1S)
            {
                --swTimer1S;
            }
            else
            {
                swTimer1STriggered = TRUE;
                swTimer1S = MAIN_SWTIMER_1S_PERIOD;
            }
        }

        /* ----- System state dependent jobs ----- */

        switch(sys_sm_getState(&sysSmAction))
        {
        /* Node starts up and try to get online */
        case SYS_SM_STATE_START_UP:
            main_checkSegInitButton();

            /* Wait until node is online */
            if (TRUE == vscp_core_isActive())
            {
                sys_sm_requestState(SYS_SM_STATE_ACTIVE, SYS_SM_ACTION_NOTHING);
            }

            break;

        /* Node is online */
        case SYS_SM_STATE_ACTIVE:
            main_checkSegInitButton();

            /* If the node is offline, shutdown the system */
            if (FALSE == vscp_core_isActive())
            {
                sys_sm_requestState(SYS_SM_STATE_SHUTDOWN, SYS_SM_ACTION_HALT);
            }
            else
            {
                /* 10 ms period */
                if (TRUE == swTimer10MSTriggered)
                {
                    /* Process every 10ms the button observer. */
                    buttonObserver_process();
                }

                /* 1 s period */
                if (TRUE == swTimer1STriggered)
                {
                    /* Observe the wind speed and send VSCP events. */
                    windObserver_process();
                }

                /* Enter power saving mode? */
                if (TRUE == shuttersAreIdle)
                {
                    /*
                    set_sleep_mode(SLEEP_MODE_IDLE);
                    sleep_mode();
                    */
                }
            }

            break;

        /* Node left online state and went offline */
        case SYS_SM_STATE_SHUTDOWN:
            /* Stop all shutter */
            for(index = 0; index < SHUTTER_NUM; ++index)
            {
                shutter_drive(index, SHUTTER_DIR_STOP, 0);
                shutter_enable(index, FALSE);
            }

            sys_sm_requestState(SYS_SM_STATE_IDLE, SYS_SM_ACTION_KEEP);
            break;

        case SYS_SM_STATE_ERROR:
            /* Stop all shutter */
            for(index = 0; index < SHUTTER_NUM; ++index)
            {
                shutter_drive(index, SHUTTER_DIR_STOP, 0);
                shutter_enable(index, FALSE);
            }

            sys_sm_requestState(SYS_SM_STATE_IDLE, SYS_SM_ACTION_KEEP);
            break;

        /* Node waits for a reset */
        case SYS_SM_STATE_IDLE:

            /* Wait until all shutters are stopped before any further action
             * takes place.
             */
            if (TRUE == shuttersAreIdle)
            {
                if (SYS_SM_ACTION_REBOOT == sysSmAction)
                {
                    REBOOT();
                }
                else if (SYS_SM_ACTION_HALT == sysSmAction)
                {
                    HALT();
                }
                else
                {
                    /* Nothing to do */
                }
            }
            break;

        default:
            /* Should never happen */
            sys_sm_requestState(SYS_SM_STATE_ERROR, SYS_SM_ACTION_REBOOT);
            break;
        }

        /* Reset software timer flags */
        swTimer10MSTriggered    = FALSE;
        swTimer1STriggered      = FALSE;
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

    /* Initialize watchdog */
    watchdog_init();

    /* Initialize the system state machine */
    sys_sm_init();

    /* Initialize a hardware timer */
    timerDrv_init();
    timerDrv_setCallback(main_timerCb);

    /* Initialize push button driver */
    pushButton_init();

    /* Initialize relay driver */
    relay_init();

    /* Initialize wind driver */
    windDrv_init();

    /* Initialize the VSCP framework */
    if (VSCP_CORE_RET_OK != vscp_core_init())
    {
        status = MAIN_RET_ERROR;
    }

    /* Set VSCP logger default log level */
    vscp_logger_setLogLevel(MAIN_LOG_LEVEL_DEFAULT);
    vscp_logger_enable(MAIN_LOGGER_ENABLED_DEFAULT);

    /* Initialize CAN monitor */
    can_monitor_init();

    /* Initialize software timer */
    swTimer_init();

    /* Initialize the time module */
    time_init();

    /* Initialize wind speed observer */
    windObserver_init();

    /* Initialize button observer */
    buttonObserver_init();

    /* Initialize the shutter control */
    shutterDrv_init();

    /* Initialize the shutter module */
    shutter_init();

    /* Register shutter info callback */
    shutter_register(main_shutterCb, NULL);

    /* Configure all software timer */
    swTimer_start(MAIN_SWTIMER_10MS_ID, MAIN_SWTIMER_10MS_PERIOD, FALSE);
    swTimer_start(MAIN_SWTIMER_250MS_ID, MAIN_SWTIMER_250MS_PERIOD, FALSE);

    /* Load configuration */
    main_loadConfiguration();

    return status;
}

/**
 * This function loads the configuration from the persistent memory and configures
 * all peripheries.
 */
static void main_loadConfiguration(void)
{
    uint8_t     index               = 0;
    uint16_t    switchingPwmValue   = 0;
    uint16_t    holdingPwmValue     = 0;
    uint8_t     shutterEnableMask   = 0;
    uint8_t     buttonFilter        = 0;
    uint8_t     relayEnableMask     = 0;

    /* Get button filter */
    buttonFilter = vscp_ps_user_readButtonEnable();

    /* If the wind measurement is enabled, disable the button which uses the same pin. */
    if ((1 == vscp_ps_user_readWindEnable()) &&
        (TRUE == IS_BIT_SET(buttonFilter, WINDDRV_BUTTON_INDEX)))
    {
        BIT_CLR(buttonFilter, WINDDRV_BUTTON_INDEX);
        vscp_ps_user_writeButtonEnable(buttonFilter);
    }

    /* Set button observation filter */
    buttonObserver_setFilter(buttonFilter);

    /* Configure wind measurement */
    if (0 == vscp_ps_user_readWindEnable())
    {
        /* Disable wind measurement */
        windDrv_enable(FALSE);
    }
    else
    {
        /* Enable wind measurement */
        windDrv_enable(TRUE);
    }

    /* Configure relays:
     * - Switching current, 16-bit value, LSB first stored in persistent memory
     * - Holding current, 16-bit value, LSB first stored in persistent memory
     */
    switchingPwmValue  = ((uint16_t)vscp_ps_user_readRelaySwitchingDutyCycle(0)) << 0;
    switchingPwmValue |= ((uint16_t)vscp_ps_user_readRelaySwitchingDutyCycle(1)) << 8;
    relay_setSwitchingPwm(switchingPwmValue);

    holdingPwmValue  = ((uint16_t)vscp_ps_user_readRelayHoldingDutyCycle(0)) << 0;
    holdingPwmValue |= ((uint16_t)vscp_ps_user_readRelayHoldingDutyCycle(1)) << 8;
    relay_setHoldingPwm(holdingPwmValue);

    relayEnableMask = vscp_ps_user_readRelayEnable();
    for(index = 0; index < RELAY_NUM; ++index)
    {
        if (FALSE == IS_BIT_SET(relayEnableMask, index))
        {
            relay_enable(index, FALSE);
        }
        else
        {
            relay_enable(index, TRUE);
        }
    }

    /* Configure shutters */
    shutterEnableMask = vscp_ps_user_readShutterEnable();
    for(index = 0; index < SHUTTER_NUM; ++index)
    {
        uint16_t    maxUpTime       = 0;
        uint16_t    maxDownTime     = 0;
        uint16_t    turnTime        = 0;
        uint8_t     relayPowIndex   = index * 2 + 0;    /* Relay which handles the power */
        uint8_t     relayDirIndex   = index * 2 + 1;    /* Relay which sets the drive direction */

        /* Max. up time, 16-bit value, LSB first stored in persistent memory */
        maxUpTime  = (uint16_t)vscp_ps_user_readShutterMaxUpTime(index * 2 + 0);
        maxUpTime |= (uint16_t)vscp_ps_user_readShutterMaxUpTime(index * 2 + 1) << 8;

        /* Max. down time, 16-bit value, LSB first stored in persistent memory */
        maxDownTime  = (uint16_t)vscp_ps_user_readShutterMaxDownTime(index * 2 + 0);
        maxDownTime |= (uint16_t)vscp_ps_user_readShutterMaxDownTime(index * 2 + 1) << 8;

        /* Turn time, 16-bit value, LSB first stored in persistent memory */
        turnTime  = (uint16_t)vscp_ps_user_readShutterTurnTime(index * 2 + 0);
        turnTime |= (uint16_t)vscp_ps_user_readShutterTurnTime(index * 2 + 1) << 8;

        /* Configure shutter */
        shutter_configure(index, relayPowIndex, relayDirIndex, maxUpTime, maxDownTime, turnTime);

        /* Disable shutter? */
        if (FALSE == IS_BIT_SET(shutterEnableMask, index))
        {
            shutter_enable(index, FALSE);
        }
        /* Enable shutter */
        else
        {
            shutter_enable(index, TRUE);

            /* Go sure that the necessary relays are enabled */
            relay_enable(relayPowIndex, TRUE);
            relay_enable(relayDirIndex, TRUE);
        }
    }

    return;
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
 * This function is called by the timer ISR every tick!
 */
static void main_timerCb(void)
{
    static volatile uint8_t segmentInitButtonDebounceCnt    = 0;
    static volatile uint8_t shutterCnt                      = 0;

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

    /* Process all shutter timers in a 100 ms cycle period.
     * It is important to do that in the ISR, because the shutter timers are used
     * to determine the shutter position. Every deviation in time is bad.
     * Also the timer is used to drive the shutter a specific time in one direction.
     */
    if (0 < shutterCnt)
    {
        --shutterCnt;
    }
    else
    {
        shutterCnt = MAIN_SHUTTER_TIMER_PERIOD;

        shutter_processTimers();
    }

    return;
}

/**
 * This function process the status lamp.
 */
static void main_processStatusLamp(void)
{
    VSCP_LAMP_STATE state   = vscp_portable_getLampState();
    static uint8_t  slowCnt = 0;
    static uint8_t  busOff  = 0;

    /* Is the CAN bus off? */
    if (CAN_MONITOR_STATE_BUS_OFF == can_monitor_getState())
    {
        /* Blink a special sequence to notify the user. */
        if (0 == busOff)
        {
            HW_ENABLE_STATUS_LED();
        }
        else if (1 == busOff)
        {
            HW_DISABLE_STATUS_LED();
        }

        busOff = (busOff + 1) % 7;
    }
    /* CAN bus is ready */
    else
    {
        /* Blink accordingly to the VSCP framework state */
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

        /* Reset bus off special counter */
        busOff = 0;
    }

    return;
}

/**
 * This function checks the segment initialization button.
 * If the button is pressed, the segment initialization will be executed.
 */
static void main_checkSegInitButton(void)
{
    static BOOL lastSegInitButtonState  = FALSE;    /* Used for "raising edge" detection */

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

    return;
}

/**
 * This function informs about any shutter action.
 *
 * @param[in] nr    Shutter instance number
 * @param[in] dir   Shutter direction
 * @param[in] pos   Shutter position
 */
static void main_shutterCb(uint8_t nr, SHUTTER_DIR dir, SHUTTER_POS pos)
{
    /* Shutter event enabled? */
    if (TRUE == IS_BIT_SET(vscp_ps_user_readShutterEventConfig(), nr))
    {
        if (SHUTTER_DIR_UP == dir)
        {
            (void)vscp_evt_information_sendShutterUp(nr,
                                                     vscp_ps_user_readShutterEventZone(nr),
                                                     vscp_ps_user_readShutterEventSubZone(nr));
        }
        else if (SHUTTER_DIR_DOWN == dir)
        {
            (void)vscp_evt_information_sendShutterDown(nr,
                                                       vscp_ps_user_readShutterEventZone(nr),
                                                       vscp_ps_user_readShutterEventSubZone(nr));
        }
        else if (SHUTTER_DIR_STOP == dir)
        {
            if (SHUTTER_POS_TOP == pos)
            {
                (void)vscp_evt_information_sendShutterReachedTopEnd(nr,
                                                                    vscp_ps_user_readShutterEventZone(nr),
                                                                    vscp_ps_user_readShutterEventSubZone(nr));
            }
            else if (SHUTTER_POS_BOTTOM == pos)
            {
                (void)vscp_evt_information_sendShutterReachedBottomEnd(nr,
                                                                       vscp_ps_user_readShutterEventZone(nr),
                                                                       vscp_ps_user_readShutterEventSubZone(nr));
            }
            else
            {
                (void)vscp_evt_information_sendStop(nr,
                                                    vscp_ps_user_readShutterEventZone(nr),
                                                    vscp_ps_user_readShutterEventSubZone(nr));
            }
        }
        else
        {
            ;
        }
    }

    return;
}