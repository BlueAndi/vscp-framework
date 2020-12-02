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
@file   vscp_type_control.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Control functionality. One of the main concepts of VSCP is that it is an event driven protocol.
Commands are sent out as events to the network not as events to specific devices. A device can
belong to a zone which select limit events of interest for the particular node.. If there is a need
to control a specific device the registry model should be used. This is the only way to directly
control a device.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_TYPE_CONTROL_H__
#define __VSCP_TYPE_CONTROL_H__

/** @defgroup vscp_type_control Control
 * Level 1 protocol class types
 * @{
 * @ingroup vscp_l1
 */

/*******************************************************************************
    INCLUDES
*******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
    COMPILER SWITCHES
*******************************************************************************/

/*******************************************************************************
    CONSTANTS
*******************************************************************************/

/**
 * General Event.
 */
#define VSCP_TYPE_CONTROL_GENERAL                0

/**
 * Mute/Un-mute all sound generating nodes in a zone
 */
#define VSCP_TYPE_CONTROL_MUTE                   1

/**
 * Turn on/off lamps on nodes in zone.
 */
#define VSCP_TYPE_CONTROL_ALL_LAMPS              2

/**
 * Perform open on all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_OPEN                   3

/**
 * Perform close on all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_CLOSE                  4

/**
 * Turn On a nodes in a zone/subzone.
 */
#define VSCP_TYPE_CONTROL_TURNON                 5

/**
 * Turn Off a nodes in a zone/subzone.
 */
#define VSCP_TYPE_CONTROL_TURNOFF                6

/**
 * Start all nodes in a zone.
 */
#define VSCP_TYPE_CONTROL_START                  7

/**
 * Stop all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_STOP                   8

/**
 * Perform Reset on all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_RESET                  9

/**
 * Perform Interrupt on all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_INTERRUPT              10

/**
 * Perform Sleep on all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_SLEEP                  11

/**
 * Wakeup all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_WAKEUP                 12

/**
 * Resume all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_RESUME                 13

/**
 * Pause all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_PAUSE                  14

/**
 * Activate all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_ACTIVATE               15

/**
 * Deactivate all nodes in zone.
 */
#define VSCP_TYPE_CONTROL_DEACTIVATE             16

/**
 * Reserved.
 */
#define VSCP_TYPE_CONTROL_RESERVED17             17

/**
 * Reserved.
 */
#define VSCP_TYPE_CONTROL_RESERVED18             18

/**
 * Reserved.
 */
#define VSCP_TYPE_CONTROL_RESERVED19             19

/**
 * Dim all dimmer devices on a segment to a specified dim value.
 */
#define VSCP_TYPE_CONTROL_DIM_LAMPS              20

/**
 * This is typical for changing TV channels or for changing AV amp input source etc.
 */
#define VSCP_TYPE_CONTROL_CHANGE_CHANNEL         21

/**
 * Change an absolute level.
 */
#define VSCP_TYPE_CONTROL_CHANGE_LEVEL           22

/**
 * Relative Change Level request
 */
#define VSCP_TYPE_CONTROL_RELATIVE_CHANGE_LEVEL  23

/**
 * Measurement Request
 */
#define VSCP_TYPE_CONTROL_MEASUREMENT_REQUEST    24

/**
 * Request to stream data
 */
#define VSCP_TYPE_CONTROL_STREAM_DATA            25

/**
 * Synchronize events on a segment.
 */
#define VSCP_TYPE_CONTROL_SYNC                   26

/**
 * Request streamed data from nodes identified by zone/subzone.
 */
#define VSCP_TYPE_CONTROL_ZONED_STREAM_DATA      27

/**
 * Some nodes may have pre-set configurations to choose from. With this event a pre-set can be set for
 * a zone/sub-zone.
 * A node that receive and act on this event send CLASS1.INFORMATION,
 * Type=48 as a response event.
 */
#define VSCP_TYPE_CONTROL_SET_PRESET             28

/**
 * Toggle the state of a node.
 * Note: This may be a bad design option as it often demands that the state should be known for the
 * node on beforehand.
 */
#define VSCP_TYPE_CONTROL_TOGGLE_STATE           29

/**
 * With this event it is possible to generate a timed pulse that is on for a specified time.
 */
#define VSCP_TYPE_CONTROL_TIMED_PULSE_ON         30

/**
 * With this event it is possible to generate a timed pulse that is off for a specified time.
 */
#define VSCP_TYPE_CONTROL_TIMED_PULSE_OFF        31

/**
 * Set country and language.
 */
#define VSCP_TYPE_CONTROL_SET_COUNTRY_LANGUAGE   32

/**
 * Big Change level can be used in situations when the one byte level of CLASS1.CONTROL, Type=22 is
 * not enough.
 */
#define VSCP_TYPE_CONTROL_BIG_CHANGE_LEVEL       33

/**
 * Move shutter up.
 */
#define VSCP_TYPE_CONTROL_SHUTTER_UP             34

/**
 * Move shutter down.
 */
#define VSCP_TYPE_CONTROL_SHUTTER_DOWN           35

/**
 * Move shutter left.
 */
#define VSCP_TYPE_CONTROL_SHUTTER_LEFT           36

/**
 * Move shutter right.
 */
#define VSCP_TYPE_CONTROL_SHUTTER_RIGHT          37

/**
 * Move shutter to middle position.
 */
#define VSCP_TYPE_CONTROL_SHUTTER_MIDDLE         38

/**
 * Move shutter to preset position.
 */
#define VSCP_TYPE_CONTROL_SHUTTER_PRESET         39

/**
 * Turn on all lamps in a zone.
 */
#define VSCP_TYPE_CONTROL_ALL_LAMPS_ON           40

/**
 * Turn off all lamps in a zone.
 */
#define VSCP_TYPE_CONTROL_ALL_LAMPS_OFF          41

/**
 * Lock devices in a zone.
 */
#define VSCP_TYPE_CONTROL_LOCK                   42

/**
 * Unlock devices in a zone.
 */
#define VSCP_TYPE_CONTROL_UNLOCK                 43

/**
 * With this event it is possible to set duty cycle output such as PWM.
 */
#define VSCP_TYPE_CONTROL_PWM                    44

/**
 * Lock devices with token.
 */
#define VSCP_TYPE_CONTROL_TOKEN_LOCK             45

/**
 * Unlock devices with token.
 */
#define VSCP_TYPE_CONTROL_TOKEN_UNLOCK           46

/**
 * Set security level.
 */
#define VSCP_TYPE_CONTROL_SET_SECURITY_LEVEL     47

/**
 * Set security pin.
 */
#define VSCP_TYPE_CONTROL_SET_SECURITY_PIN       48

/**
 * Set security password.
 */
#define VSCP_TYPE_CONTROL_SET_SECURITY_PASSWORD  49

/**
 * Set security token.
 */
#define VSCP_TYPE_CONTROL_SET_SECURITY_TOKEN     50

/**
 * Request new security token.
 */
#define VSCP_TYPE_CONTROL_REQUEST_SECURITY_TOKEN 51

/**
 * Increment value.
 */
#define VSCP_TYPE_CONTROL_INCREMENT              52

/**
 * Decrement value.
 */
#define VSCP_TYPE_CONTROL_DECREMENT              53

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

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_TYPE_CONTROL_H__ */

/** @} */
