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
@brief  User specific persistency access.
@file   vscp_ps_user.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains the user specific access to the persistent storage.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_PS_USER_H__
#define __VSCP_PS_USER_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_ps.h"

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/**
 * Address where the user specific persistent values are stored.
 */
#define VSCP_PS_USER_BASE_ADDR  VSCP_PS_ADDR_NEXT

/**
 * Persistent memory address: button enable
 */
#define VSCP_PS_USER_ADDR_BUTTON_ENABLE (VSCP_PS_USER_BASE_ADDR + 0)

/**
 * Persistent memory size of: button enable
 */
#define VSCP_PS_USER_SIZE_BUTTON_ENABLE 1

/**
 * Persistent memory address: wind enable
 */
#define VSCP_PS_USER_ADDR_WIND_ENABLE   (VSCP_PS_USER_BASE_ADDR + 1)

/**
 * Persistent memory size of: wind enable
 */
#define VSCP_PS_USER_SIZE_WIND_ENABLE   1

/**
 * Persistent memory address: relay enable
 */
#define VSCP_PS_USER_ADDR_RELAY_ENABLE  (VSCP_PS_USER_BASE_ADDR + 2)

/**
 * Persistent memory size of: relay enable
 */
#define VSCP_PS_USER_SIZE_RELAY_ENABLE  1

/**
 * Persistent memory address: shutter enable
 */
#define VSCP_PS_USER_ADDR_SHUTTER_ENABLE    (VSCP_PS_USER_BASE_ADDR + 3)

/**
 * Persistent memory size of: shutter enable
 */
#define VSCP_PS_USER_SIZE_SHUTTER_ENABLE    1

/**
 * Persistent memory address: relay event config
 */
#define VSCP_PS_USER_ADDR_RELAY_EVENT_CONFIG    (VSCP_PS_USER_BASE_ADDR + 4)

/**
 * Persistent memory size of: relay event config
 */
#define VSCP_PS_USER_SIZE_RELAY_EVENT_CONFIG    1

/**
 * Persistent memory address: shutter event config
 */
#define VSCP_PS_USER_ADDR_SHUTTER_EVENT_CONFIG  (VSCP_PS_USER_BASE_ADDR + 5)

/**
 * Persistent memory size of: shutter event config
 */
#define VSCP_PS_USER_SIZE_SHUTTER_EVENT_CONFIG  1

/**
 * Persistent memory address: relay switching duty cycle
 */
#define VSCP_PS_USER_ADDR_RELAY_SWITCHING_DUTY_CYCLE    (VSCP_PS_USER_BASE_ADDR + 6)

/**
 * Persistent memory size of: relay switching duty cycle
 */
#define VSCP_PS_USER_SIZE_RELAY_SWITCHING_DUTY_CYCLE    2

/**
 * Persistent memory address: relay holding duty cycle
 */
#define VSCP_PS_USER_ADDR_RELAY_HOLDING_DUTY_CYCLE  (VSCP_PS_USER_BASE_ADDR + 8)

/**
 * Persistent memory size of: relay holding duty cycle
 */
#define VSCP_PS_USER_SIZE_RELAY_HOLDING_DUTY_CYCLE  2

/**
 * Persistent memory address: button event zone
 */
#define VSCP_PS_USER_ADDR_BUTTON_EVENT_ZONE (VSCP_PS_USER_BASE_ADDR + 10)

/**
 * Persistent memory size of: button event zone
 */
#define VSCP_PS_USER_SIZE_BUTTON_EVENT_ZONE 8

/**
 * Persistent memory address: button event sub zone
 */
#define VSCP_PS_USER_ADDR_BUTTON_EVENT_SUB_ZONE (VSCP_PS_USER_BASE_ADDR + 18)

/**
 * Persistent memory size of: button event sub zone
 */
#define VSCP_PS_USER_SIZE_BUTTON_EVENT_SUB_ZONE 8

/**
 * Persistent memory address: relay event zone
 */
#define VSCP_PS_USER_ADDR_RELAY_EVENT_ZONE  (VSCP_PS_USER_BASE_ADDR + 26)

/**
 * Persistent memory size of: relay event zone
 */
#define VSCP_PS_USER_SIZE_RELAY_EVENT_ZONE  8

/**
 * Persistent memory address: relay event sub zone
 */
#define VSCP_PS_USER_ADDR_RELAY_EVENT_SUB_ZONE  (VSCP_PS_USER_BASE_ADDR + 34)

/**
 * Persistent memory size of: relay event sub zone
 */
#define VSCP_PS_USER_SIZE_RELAY_EVENT_SUB_ZONE  8

/**
 * Persistent memory address: shutter event zone
 */
#define VSCP_PS_USER_ADDR_SHUTTER_EVENT_ZONE    (VSCP_PS_USER_BASE_ADDR + 42)

/**
 * Persistent memory size of: shutter event zone
 */
#define VSCP_PS_USER_SIZE_SHUTTER_EVENT_ZONE    4

/**
 * Persistent memory address: shutter event sub zone
 */
#define VSCP_PS_USER_ADDR_SHUTTER_EVENT_SUB_ZONE    (VSCP_PS_USER_BASE_ADDR + 46)

/**
 * Persistent memory size of: shutter event sub zone
 */
#define VSCP_PS_USER_SIZE_SHUTTER_EVENT_SUB_ZONE    4

/**
 * Persistent memory address: wind event zone
 */
#define VSCP_PS_USER_ADDR_WIND_EVENT_ZONE   (VSCP_PS_USER_BASE_ADDR + 50)

/**
 * Persistent memory size of: wind event zone
 */
#define VSCP_PS_USER_SIZE_WIND_EVENT_ZONE   1

/**
 * Persistent memory address: wind event sub zone
 */
#define VSCP_PS_USER_ADDR_WIND_EVENT_SUB_ZONE   (VSCP_PS_USER_BASE_ADDR + 51)

/**
 * Persistent memory size of: wind event sub zone
 */
#define VSCP_PS_USER_SIZE_WIND_EVENT_SUB_ZONE   1

/**
 * Persistent memory address: wind speed medium
 */
#define VSCP_PS_USER_ADDR_WIND_SPEED_MEDIUM (VSCP_PS_USER_BASE_ADDR + 52)

/**
 * Persistent memory size of: wind speed medium
 */
#define VSCP_PS_USER_SIZE_WIND_SPEED_MEDIUM 1

/**
 * Persistent memory address: wind speed high
 */
#define VSCP_PS_USER_ADDR_WIND_SPEED_HIGH   (VSCP_PS_USER_BASE_ADDR + 53)

/**
 * Persistent memory size of: wind speed high
 */
#define VSCP_PS_USER_SIZE_WIND_SPEED_HIGH   1

/**
 * Persistent memory address: wind speed very high
 */
#define VSCP_PS_USER_ADDR_WIND_SPEED_VERY_HIGH  (VSCP_PS_USER_BASE_ADDR + 54)

/**
 * Persistent memory size of: wind speed very high
 */
#define VSCP_PS_USER_SIZE_WIND_SPEED_VERY_HIGH  1

/**
 * Persistent memory address: shutter max up time
 */
#define VSCP_PS_USER_ADDR_SHUTTER_MAX_UP_TIME   (VSCP_PS_USER_BASE_ADDR + 55)

/**
 * Persistent memory size of: shutter max up time
 */
#define VSCP_PS_USER_SIZE_SHUTTER_MAX_UP_TIME   8

/**
 * Persistent memory address: shutter max down time
 */
#define VSCP_PS_USER_ADDR_SHUTTER_MAX_DOWN_TIME (VSCP_PS_USER_BASE_ADDR + 63)

/**
 * Persistent memory size of: shutter max down time
 */
#define VSCP_PS_USER_SIZE_SHUTTER_MAX_DOWN_TIME 8

/**
 * Persistent memory address: shutter turn time
 */
#define VSCP_PS_USER_ADDR_SHUTTER_TURN_TIME (VSCP_PS_USER_BASE_ADDR + 71)

/**
 * Persistent memory size of: shutter turn time
 */
#define VSCP_PS_USER_SIZE_SHUTTER_TURN_TIME 8

/*******************************************************************************
    MACROS
*******************************************************************************/

/*******************************************************************************
    TYPES AND STRUCTURES
*******************************************************************************/

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function initializes this module.
 */
extern void vscp_ps_user_init(void);

/**
 * This function reads the button enable from persistent storage.
 * Enable or disable a button. There are up to 8 buttons enumerated by 1 to 8.
 * The button 1 corresponds to bit 0 and etc.
 * A 0 disables the button and a 1 enables it.
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readButtonEnable(void);

/**
 * This function writes the button enable to persistent storage.
 * Enable or disable a button. There are up to 8 buttons enumerated by 1 to 8.
 * The button 1 corresponds to bit 0 and etc.
 * A 0 disables the button and a 1 enables it.
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeButtonEnable(uint8_t value);

/**
 * This function reads the wind enable from persistent storage.
 * Enable or disable the wind measurement.
 * A 0 disables the wind measurement and a 1 enables it.
 * Note, that if the wind measurement is enabled, the button 8 functionality will be disabled.
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readWindEnable(void);

/**
 * This function writes the wind enable to persistent storage.
 * Enable or disable the wind measurement.
 * A 0 disables the wind measurement and a 1 enables it.
 * Note, that if the wind measurement is enabled, the button 8 functionality will be disabled.
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeWindEnable(uint8_t value);

/**
 * This function reads the relay enable from persistent storage.
 * Enable or disable a relay. There are up to 8 relays enumerated by 1 to 8.
 * The relay 1 corresponds to bit 0 and etc.
 * A 0 disables the relay and a 1 enables it.
 * Please note, that if a shutter is enabled, it doesn't matter that the relays are disabled here.
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readRelayEnable(void);

/**
 * This function writes the relay enable to persistent storage.
 * Enable or disable a relay. There are up to 8 relays enumerated by 1 to 8.
 * The relay 1 corresponds to bit 0 and etc.
 * A 0 disables the relay and a 1 enables it.
 * Please note, that if a shutter is enabled, it doesn't matter that the relays are disabled here.
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeRelayEnable(uint8_t value);

/**
 * This function reads the shutter enable from persistent storage.
 * Enable or disable a shutter. There are up to 4 shutter enumerated by 1 to 4.
 * The shutter 1 corresponds to bit 0 and etc.
 * A 0 disables the shutter and a 1 enables it.
 * If a shutter is enabled, the used relays can not be controlled by single relay actions!
 *
 * Bit 0: Enable shutter 1 and disable relay 1 and 2 to be controlled external.
 * Bit 1: Enable shutter 2 and disable relay 3 and 4 to be controlled external.
 * Bit 2: Enable shutter 3 and disable relay 5 and 6 to be controlled external.
 * Bit 3: Enable shutter 4 and disable relay 7 and 8 to be controlled external.
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readShutterEnable(void);

/**
 * This function writes the shutter enable to persistent storage.
 * Enable or disable a shutter. There are up to 4 shutter enumerated by 1 to 4.
 * The shutter 1 corresponds to bit 0 and etc.
 * A 0 disables the shutter and a 1 enables it.
 * If a shutter is enabled, the used relays can not be controlled by single relay actions!
 *
 * Bit 0: Enable shutter 1 and disable relay 1 and 2 to be controlled external.
 * Bit 1: Enable shutter 2 and disable relay 3 and 4 to be controlled external.
 * Bit 2: Enable shutter 3 and disable relay 5 and 6 to be controlled external.
 * Bit 3: Enable shutter 4 and disable relay 7 and 8 to be controlled external.
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeShutterEnable(uint8_t value);

/**
 * This function reads the relay event config from persistent storage.
 * Enable or disable a releay event. There are up to 8 relay enumerated by 1 to 8.
 * The relay 1 corresponds to bit 0 and etc.
 * A 0 disables the event and a 1 enables it.
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readRelayEventConfig(void);

/**
 * This function writes the relay event config to persistent storage.
 * Enable or disable a releay event. There are up to 8 relay enumerated by 1 to 8.
 * The relay 1 corresponds to bit 0 and etc.
 * A 0 disables the event and a 1 enables it.
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeRelayEventConfig(uint8_t value);

/**
 * This function reads the shutter event config from persistent storage.
 * Enable or disable a shutter event. There are up to 4 shutter enumerated by 1 to 4.
 * The shutter 1 corresponds to bit 0 and etc.
 * A 0 disables the event and a 1 enables it.
 * Note, that the shutter must be enabled.
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readShutterEventConfig(void);

/**
 * This function writes the shutter event config to persistent storage.
 * Enable or disable a shutter event. There are up to 4 shutter enumerated by 1 to 4.
 * The shutter 1 corresponds to bit 0 and etc.
 * A 0 disables the event and a 1 enables it.
 * Note, that the shutter must be enabled.
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeShutterEventConfig(uint8_t value);

/**
 * This function reads the relay switching duty cycle from persistent storage.
 * A monostable relay is activated by using the switching current.
 * The value is given as pwm duty cycle value (3-400).
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readRelaySwitchingDutyCycle(uint8_t index);

/**
 * This function writes the relay switching duty cycle to persistent storage.
 * A monostable relay is activated by using the switching current.
 * The value is given as pwm duty cycle value (3-400).
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeRelaySwitchingDutyCycle(uint8_t index, uint8_t value);

/**
 * This function reads the relay holding duty cycle from persistent storage.
 * A monostable relay is holded in active state by using the holding current.
 * The value is given as pwm duty cycle value (3-400).
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readRelayHoldingDutyCycle(uint8_t index);

/**
 * This function writes the relay holding duty cycle to persistent storage.
 * A monostable relay is holded in active state by using the holding current.
 * The value is given as pwm duty cycle value (3-400).
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeRelayHoldingDutyCycle(uint8_t index, uint8_t value);



/**
 * This function reads the button event zone from persistent storage.
 * The zone to which the button event belongs to (255 means all zones).
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readButtonEventZone(uint8_t index);

/**
 * This function writes the button event zone to persistent storage.
 * The zone to which the button event belongs to (255 means all zones).
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeButtonEventZone(uint8_t index, uint8_t value);

/**
 * This function reads the button event sub zone from persistent storage.
 * The sub-zone to which the button event belongs to (255 means all sub-zones).
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readButtonEventSubZone(uint8_t index);

/**
 * This function writes the button event sub zone to persistent storage.
 * The sub-zone to which the button event belongs to (255 means all sub-zones).
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeButtonEventSubZone(uint8_t index, uint8_t value);

/**
 * This function reads the relay event zone from persistent storage.
 * The zone to which the relay event belongs to (255 means all zones).
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readRelayEventZone(uint8_t index);

/**
 * This function writes the relay event zone to persistent storage.
 * The zone to which the relay event belongs to (255 means all zones).
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeRelayEventZone(uint8_t index, uint8_t value);

/**
 * This function reads the relay event sub zone from persistent storage.
 * The sub-zone to which the relay event belongs to (255 means all sub-zones).
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readRelayEventSubZone(uint8_t index);

/**
 * This function writes the relay event sub zone to persistent storage.
 * The sub-zone to which the relay event belongs to (255 means all sub-zones).
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeRelayEventSubZone(uint8_t index, uint8_t value);

/**
 * This function reads the shutter event zone from persistent storage.
 * The zone to which the shutter event belongs to (255 means all zones).
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readShutterEventZone(uint8_t index);

/**
 * This function writes the shutter event zone to persistent storage.
 * The zone to which the shutter event belongs to (255 means all zones).
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeShutterEventZone(uint8_t index, uint8_t value);

/**
 * This function reads the shutter event sub zone from persistent storage.
 * The sub-zone to which the shutter event belongs to  (255 means all sub-zones).
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readShutterEventSubZone(uint8_t index);

/**
 * This function writes the shutter event sub zone to persistent storage.
 * The sub-zone to which the shutter event belongs to  (255 means all sub-zones).
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeShutterEventSubZone(uint8_t index, uint8_t value);

/**
 * This function reads the wind event zone from persistent storage.
 * The zone to which the wind event belongs to (255 means all zones).
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readWindEventZone(void);

/**
 * This function writes the wind event zone to persistent storage.
 * The zone to which the wind event belongs to (255 means all zones).
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeWindEventZone(uint8_t value);

/**
 * This function reads the wind event sub zone from persistent storage.
 * The sub-zone to which the wind event belongs to  (255 means all sub-zones).
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readWindEventSubZone(void);

/**
 * This function writes the wind event sub zone to persistent storage.
 * The sub-zone to which the wind event belongs to  (255 means all sub-zones).
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeWindEventSubZone(uint8_t value);

/**
 * This function reads the wind speed medium from persistent storage.
 * Medium wind speed in [0,1m/s].
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readWindSpeedMedium(void);

/**
 * This function writes the wind speed medium to persistent storage.
 * Medium wind speed in [0,1m/s].
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeWindSpeedMedium(uint8_t value);

/**
 * This function reads the wind speed high from persistent storage.
 * High wind speed in [0,1m/s].
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readWindSpeedHigh(void);

/**
 * This function writes the wind speed high to persistent storage.
 * High wind speed in [0,1m/s].
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeWindSpeedHigh(uint8_t value);

/**
 * This function reads the wind speed very high from persistent storage.
 * Very high wind speed in [0,1m/s].
 *
 * @return Value
 */
extern uint8_t vscp_ps_user_readWindSpeedVeryHigh(void);

/**
 * This function writes the wind speed very high to persistent storage.
 * Very high wind speed in [0,1m/s].
 *
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeWindSpeedVeryHigh(uint8_t value);

/**
 * This function reads the shutter max up time from persistent storage.
 * Max. up-time (drive time from bottom to top) in 0.1s as 16-bit value.
 * Shutter 1 is at the lower address and the other shutters follow.
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readShutterMaxUpTime(uint8_t index);

/**
 * This function writes the shutter max up time to persistent storage.
 * Max. up-time (drive time from bottom to top) in 0.1s as 16-bit value.
 * Shutter 1 is at the lower address and the other shutters follow.
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeShutterMaxUpTime(uint8_t index, uint8_t value);

/**
 * This function reads the shutter max down time from persistent storage.
 * Max. down-time (drive time from top to bottom) in 0.1s as 16-bit value.
 * Shutter 1 is at the lower address and the other shutters follow.
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readShutterMaxDownTime(uint8_t index);

/**
 * This function writes the shutter max down time to persistent storage.
 * Max. down-time (drive time from top to bottom) in 0.1s as 16-bit value.
 * Shutter 1 is at the lower address and the other shutters follow.
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeShutterMaxDownTime(uint8_t index, uint8_t value);

/**
 * This function reads the shutter turn time from persistent storage.
 * Max. turn-time (turn 90°) in 0.1s as 16-bit value.
 * Shutter 1 is at the lower address and the other shutters follow.
 *
 * @param[in] index Index
 * @return Value
 */
extern uint8_t vscp_ps_user_readShutterTurnTime(uint8_t index);

/**
 * This function writes the shutter turn time to persistent storage.
 * Max. turn-time (turn 90°) in 0.1s as 16-bit value.
 * Shutter 1 is at the lower address and the other shutters follow.
 *
 * @param[in] index Index
 * @param[in] value Value to write
 */
extern void vscp_ps_user_writeShutterTurnTime(uint8_t index, uint8_t value);

#endif /* __VSCP_PS_USER_H__ */
