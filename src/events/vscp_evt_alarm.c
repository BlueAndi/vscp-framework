/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2022 Andreas Merkle
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
@brief  Alarm functionality
@file   vscp_evt_alarm.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
@see vscp_evt_alarm.h

This file is automatically generated. Don't change it manually.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include "vscp_evt_alarm.h"
#include "vscp_core.h"
#include "vscp_class_l1.h"
#include "vscp_type_alarm.h"

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
extern BOOL vscp_evt_alarm_sendGeneralEvent(void)
{
    vscp_TxMessage  txMsg;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_GENERAL, VSCP_PRIORITY_3_NORMAL);

    txMsg.dataSize = 0;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Warning
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendWarning(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_WARNING, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Alarm occurred
 * 
 * @param[in] alarmRegister Alarm byte from standard register 128/0x80 or a code of choice describing
 * the alarm.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendAlarmOccurred(uint8_t alarmRegister, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_ALARM, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = alarmRegister;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Alarm sound on/off
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendAlarmSoundOnOff(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_SOUND, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Alarm light on/off
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendAlarmLightOnOff(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_LIGHT, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Power on/off
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendPowerOnOff(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_POWER, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Emergency Stop
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendEmergencyStop(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_EMERGENCY_STOP, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Emergency Pause
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendEmergencyPause(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_EMERGENCY_PAUSE, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Emergency Reset
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendEmergencyReset(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_EMERGENCY_RESET, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Emergency Resume
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendEmergencyResume(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_EMERGENCY_RESUME, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Arm
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendArm(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_ARM, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Disarm
 * 
 * @param[in] onOff Alarm is on/off.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendDisarm(uint8_t onOff, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_DISARM, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = onOff;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Watchdog
 * 
 * @param[in] index index.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendWatchdog(uint8_t index, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_WATCHDOG, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = index;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

/**
 * Alarm reset
 * 
 * @param[in] alarmRegister Alarm byte from standard register 128/0x80 or a code of choice describing
 * the alarm.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_alarm_sendAlarmReset(uint8_t alarmRegister, uint8_t zone, uint8_t subZone)
{
    vscp_TxMessage  txMsg;
    uint8_t         size    = 0;

    vscp_core_prepareTxMessage(&txMsg, VSCP_CLASS_L1_ALARM, VSCP_TYPE_ALARM_RESET, VSCP_PRIORITY_3_NORMAL);

    txMsg.data[0] = alarmRegister;
    size += 1;

    txMsg.data[1] = zone;
    size += 1;

    txMsg.data[2] = subZone;
    size += 1;

    txMsg.dataSize = size;

    return vscp_core_sendEvent(&txMsg);
}

