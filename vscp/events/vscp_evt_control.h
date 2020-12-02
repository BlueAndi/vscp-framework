/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2020, Andreas Merkle
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
@brief  Control
@file   vscp_evt_control.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Control functionality. One of the main concepts of VSCP is that it is an event driven protocol.
Commands are sent out as events to the network not as events to specific devices. A device can
belong to a zone which select limit events of interest for the particular node.. If there is a need
to control a specific device the registry model should be used. This is the only way to directly
control a device.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_EVT_CONTROL_H__
#define __VSCP_EVT_CONTROL_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_platform.h"

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

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * General event
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendGeneralEvent(void);

/**
 * Mute on/off
 * 
 * @param[in] command If equal to zero no mute else mute.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendMuteOnOff(uint8_t command, uint8_t zone, uint8_t subZone);

/**
 * (All) Lamp(s) on/off
 * 
 * @param[in] state If equal to zero off else on.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendAllLampSOnOff(uint8_t state, uint8_t zone, uint8_t subZone);

/**
 * Open
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendOpen(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Close
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendClose(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * TurnOn
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendTurnon(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * TurnOff
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendTurnoff(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Start
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendStart(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Stop
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendStop(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Reset
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendReset(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Interrupt
 * 
 * @param[in] interruptLevel Interrupt level. (0 – 255 , zero is lowest interrupt level. ).
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendInterrupt(uint8_t interruptLevel, uint8_t zone, uint8_t subZone);

/**
 * Sleep
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendSleep(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Wakeup
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendWakeup(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Resume
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendResume(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Pause
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendPause(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Activate
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendActivate(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Deactivate
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendDeactivate(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/* "Reserved for future use" not supported. No frame defined. */

/* "Reserved for future use" not supported. No frame defined. */

/* "Reserved for future use" not supported. No frame defined. */

/**
 * Dim lamp(s)
 * 
 * @param[in] value Value (0 – 100) . 0 = off, 100 = full on. 254 dim down one step. 255 dim up one
 * step.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendDimLampS(uint8_t value, uint8_t zone, uint8_t subZone);

/**
 * Change Channel
 * 
 * @param[in] channel A value between 0 and 127 indicates the channel number. A value between 128 to
 * 157 is change down by the specified number of channels. A value between 160 to 191 is change up by
 * the specified number of channels. A value of 255 means that this is an extended change channel
 * event and that the channel number is sent in byte 3 and after if needed.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendChangeChannel(uint8_t channel, uint8_t zone, uint8_t subZone);

/**
 * Change Level
 * 
 * @param[in] level Absolute level.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendChangeLevel(uint8_t level, uint8_t zone, uint8_t subZone);

/**
 * Relative Change Level
 * 
 * @param[in] level Relative level.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendRelativeChangeLevel(uint8_t level, uint8_t zone, uint8_t subZone);

/**
 * Measurement Request
 * 
 * @param[in] index Zero indicates all measurements supported by node should be sent (as separate
 * events). Non-zero indicates a node specific index specifying which measurement to send.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendMeasurementRequest(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Stream Data
 * 
 * @param[in] index Sequence number which is increase by one for each stream data event sent.
 * @param[in] data  Stream data. (optional) (array[7])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendStreamData(uint8_t index, uint8_t const * const data, uint8_t dataSize);

/**
 * Sync
 * 
 * @param[in] index Sensor index for a sensor within a module (see data coding). 255 is all sensors.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendSync(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Zoned Stream Data
 * 
 * @param[in] index Sequence number which is increase by one for each stream data event sent.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data  Stream data. (optional) (array[5])
 * @param[in] datasize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendZonedStreamData(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t const * const data, uint8_t dataSize);

/**
 * Set Pre-set
 * 
 * @param[in] presetCode Code for pre-set to set.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendSetPreSet(uint8_t presetCode, uint8_t zone, uint8_t subZone);

/**
 * Toggle state
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendToggleState(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Timed pulse on
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] control Control byte.
 * @param[in] time Set time as a long with MSB in the first byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendTimedPulseOn(uint8_t userSpecific, uint8_t zone, uint8_t subZone, uint8_t control, uint32_t time);

/**
 * Timed pulse off
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] control Control byte.
 * @param[in] time Set time as a long with MSB in the first byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendTimedPulseOff(uint8_t userSpecific, uint8_t zone, uint8_t subZone, uint8_t control, uint32_t time);

/**
 * Set country/language
 * 
 * @param[in] code Country/Language code.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] codeSpecific Country/Language code specific (array[4])
 * @param[in] codeSpecificsize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendSetCountryLanguage(uint8_t code, uint8_t zone, uint8_t subZone, uint8_t const * const codeSpecific, uint8_t codeSpecificSize);

/**
 * Big Change level
 * 
 * @param[in] index Index.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] level Level as signed Integer. The range can be adjusted by the user by sending the
 * needed number of bytes 1-5.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendBigChangeLevel(uint8_t index, uint8_t zone, uint8_t subZone, int32_t level);

/**
 * Move shutter up
 * 
 * @param[in] index Index.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendMoveShutterUp(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Move shutter down
 * 
 * @param[in] index Index.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendMoveShutterDown(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Move shutter left
 * 
 * @param[in] index Index.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendMoveShutterLeft(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Move shutter right
 * 
 * @param[in] index Index.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendMoveShutterRight(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Move shutter to middle position
 * 
 * @param[in] index Index.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendMoveShutterToMiddlePosition(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Move shutter to preset position
 * 
 * @param[in] index Index.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendMoveShutterToPresetPosition(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * (All) Lamp(s) on
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendAllLampSOn(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * (All) Lamp(s) off
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendAllLampSOff(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Lock
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendLock(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * Unlock
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-Zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendUnlock(uint8_t userSpecific, uint8_t zone, uint8_t subZone);

/**
 * PWM set
 * 
 * @param[in] repeats Repeat/counter: 0=repeat forever, >0 number of repeats
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] control Control byte.
 * @param[in] timeOn Time-On
 * @param[in] timeOff Time-Off
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendPwmSet(uint8_t repeats, uint8_t zone, uint8_t subZone, uint8_t control, uint16_t timeOn, uint16_t timeOff);

/**
 * Lock with token
 * 
 * @param[in] reserved Not used.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] token Token. This token can be 1-5 bytes and length of event is set accordingly. It
 * should be interpreted as an unsigned integer in the range 0-1099511627775. MSB byte is stored in
 * first byte. (array[5])
 * @param[in] tokensize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendLockWithToken(uint8_t reserved, uint8_t zone, uint8_t subZone, uint8_t const * const token, uint8_t tokenSize);

/**
 * Unlock with token
 * 
 * @param[in] reserved Not used.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] token Token. This token can be 1-5 bytes and length of event is set accordingly. It
 * should be interpreted as an unsigned integer in the range 0-1099511627775. MSB byte is stored in
 * first byte. (array[5])
 * @param[in] tokensize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendUnlockWithToken(uint8_t reserved, uint8_t zone, uint8_t subZone, uint8_t const * const token, uint8_t tokenSize);

/**
 * Set security level
 * 
 * @param[in] securityLevel Security level to set. 0-255 (Higher value is higher security level).
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendSetSecurityLevel(uint8_t securityLevel, uint8_t zone, uint8_t subZone);

/**
 * Set security pin
 * 
 * @param[in] reserved Not used.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] securityPin Security pin. This pin can be 1-5 bytes and length of event is set
 * accordingly. It should be interpreted as an unsigned integer in the range 0-1099511627775. MSB byte
 * is stored in first byte. (array[5])
 * @param[in] securityPinsize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendSetSecurityPin(uint8_t reserved, uint8_t zone, uint8_t subZone, uint8_t const * const securityPin, uint8_t securityPinSize);

/**
 * Set security password
 * 
 * @param[in] reserved Not used.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] securityPassword Security password. This password can be 1-5 bytes and length of event
 * is set accordingly. It should be interpreted as an UTF-8 string with a length set bt event data
 * length - 3 (array[5])
 * @param[in] securityPasswordsize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendSetSecurityPassword(uint8_t reserved, uint8_t zone, uint8_t subZone, uint8_t const * const securityPassword, uint8_t securityPasswordSize);

/**
 * Set security token
 * 
 * @param[in] reserved Not used.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] token Token. This token can be 1-5 bytes and length of event is set accordingly. It
 * should be interpreted as an unsigned integer in the range 0-1099511627775. MSB byte is stored in
 * first byte. (array[5])
 * @param[in] tokensize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendSetSecurityToken(uint8_t reserved, uint8_t zone, uint8_t subZone, uint8_t const * const token, uint8_t tokenSize);

/**
 * Request new security token
 * 
 * @param[in] reserved Not used.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendRequestNewSecurityToken(uint8_t reserved, uint8_t zone, uint8_t subZone);

/**
 * Increment
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones
 * @param[in] subzone Sub-zone for which event applies to (0-255). 255 is all sub-zones
 * @param[in] incrementValue Increment as unsigned integer. The range can be adjusted by the user by
 * sending just the needed number of bytes (1-5) which form the unsigned integer (MSB first). If
 * omitted (or 0) 1 is assumed as default increment value (optional) (array[5])
 * @param[in] incrementValuesize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendIncrement(uint8_t userSpecific, uint8_t zone, uint8_t subzone, uint8_t const * const incrementValue, uint8_t incrementValueSize);

/**
 * Decrement
 * 
 * @param[in] userSpecific User specific value.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones
 * @param[in] subzone Sub-zone for which event applies to (0-255). 255 is all sub-zones
 * @param[in] decrementValue Decrement as unsigned integer. The range can be adjusted by the user by
 * sending just the needed number of bytes (1-5) which form the unsigned integer (MSB first). If
 * omitted (or 0) 1 is assumed as default decrement value (optional) (array[5])
 * @param[in] decrementValuesize Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_control_sendDecrement(uint8_t userSpecific, uint8_t zone, uint8_t subzone, uint8_t const * const decrementValue, uint8_t decrementValueSize);

#endif  /* __VSCP_EVT_CONTROL_H__ */
