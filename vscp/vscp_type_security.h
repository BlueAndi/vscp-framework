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
@brief  Security
@file   vscp_type_security.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Security related events for alarms and similar devices.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_TYPE_SECURITY_H__
#define __VSCP_TYPE_SECURITY_H__

/** @defgroup vscp_type_security Security
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
#define VSCP_TYPE_SECURITY_GENERAL               0

/**
 * A motion has been detected.
 */
#define VSCP_TYPE_SECURITY_MOTION                1

/**
 * A glass break event has been detected.
 */
#define VSCP_TYPE_SECURITY_GLASS_BREAK           2

/**
 * A beam break event has been detected.
 */
#define VSCP_TYPE_SECURITY_BEAM_BREAK            3

/**
 * A sensor tamper has been detected.
 */
#define VSCP_TYPE_SECURITY_SENSOR_TAMPER         4

/**
 * A shock sensor event has been detected.
 */
#define VSCP_TYPE_SECURITY_SHOCK_SENSOR          5

/**
 * A smoke sensor event has been detected.
 */
#define VSCP_TYPE_SECURITY_SMOKE_SENSOR          6

/**
 * A heat sensor event has been detected.
 */
#define VSCP_TYPE_SECURITY_HEAT_SENSOR           7

/**
 * A panic switch event has been detected.
 */
#define VSCP_TYPE_SECURITY_PANIC_SWITCH          8

/**
 * Indicates a door sensor reports that a door is open.
 */
#define VSCP_TYPE_SECURITY_DOOR_OPEN             9

/**
 * Indicates a window sensor reports that a window is open.
 */
#define VSCP_TYPE_SECURITY_WINDOW_OPEN           10

/**
 * CO sensor has detected CO at non secure level
 */
#define VSCP_TYPE_SECURITY_CO_SENSOR             11

/**
 * A frost sensor condition is detected
 */
#define VSCP_TYPE_SECURITY_FROST_DETECTED        12

/**
 * Flame is detected.
 */
#define VSCP_TYPE_SECURITY_FLAME_DETECTED        13

/**
 * Low oxygen level detected.
 */
#define VSCP_TYPE_SECURITY_OXYGEN_LOW            14

/**
 * Weight-detector triggered.
 */
#define VSCP_TYPE_SECURITY_WEIGHT_DETECTED       15

/**
 * Water has been detected.
 */
#define VSCP_TYPE_SECURITY_WATER_DETECTED        16

/**
 * Condensation (humidity) detected.
 */
#define VSCP_TYPE_SECURITY_CONDENSATION_DETECTED 17

/**
 * Noise (sound) has been detected.
 */
#define VSCP_TYPE_SECURITY_SOUND_DETECTED        18

/**
 * Harmful sound levels detected.
 */
#define VSCP_TYPE_SECURITY_HARMFUL_SOUND_LEVEL   19

/**
 * Tamper detected.
 */
#define VSCP_TYPE_SECURITY_TAMPER                20

/**
 * Authenticated. A user or a device has been authenticated.
 */
#define VSCP_TYPE_SECURITY_AUTHENTICATED         21

/**
 * Unauthenticated. A user or a device has failed authentication.
 */
#define VSCP_TYPE_SECURITY_UNAUTHENTICATED       22

/**
 * Authorized. A user or a device has been authorized.
 */
#define VSCP_TYPE_SECURITY_AUTHORIZED            23

/**
 * Unauthorized. A user or a device has failed authorization.
 */
#define VSCP_TYPE_SECURITY_UNAUTHORIZED          24

/**
 * ID Check. A user or a device has gone through an identification test and is either allowed or not
 * allowed access according to bits in byte 0.
 */
#define VSCP_TYPE_SECURITY_ID_CHECK              25

/**
 * Valid pin. A valid pin has been entered by a device or user.
 */
#define VSCP_TYPE_SECURITY_PIN_OK                26

/**
 * Invalid pin. An invalid pin has been entered by a device or user.
 */
#define VSCP_TYPE_SECURITY_PIN_FAIL              27

/**
 * Pin warning. An invalid pin has been entered by a device or user and a warning has been given. This
 * warning is typically a warning that the pin will be unusable if further failures are detected.
 */
#define VSCP_TYPE_SECURITY_PIN_WARNING           28

/**
 * Pin error. An invalid pin has been entered by a device or user and it has failed so many times that
 * the pin is now locked and unusable.
 */
#define VSCP_TYPE_SECURITY_PIN_ERROR             29

/**
 * Valid password. A valid password has been entered by a device or user.
 */
#define VSCP_TYPE_SECURITY_PASSWORD_OK           30

/**
 * Invalid password. An invalid password has been entered by a device or user.
 */
#define VSCP_TYPE_SECURITY_PASSWORD_FAIL         31

/**
 * Password warning. An invalid password has been entered by a device or user and a warning has been
 * given. This warning is typically a warning that the password will be unusable if further failures
 * are detected.
 */
#define VSCP_TYPE_SECURITY_PASSWORD_WARNING      32

/**
 * Password error. An invalid password has been entered by a device or user and it has failed so many
 * times that the password is now locked and unusable.
 */
#define VSCP_TYPE_SECURITY_PASSWORD_ERROR        33

/**
 * ...
 */
#define VSCP_TYPE_SECURITY_GAS                   34

/**
 * ...
 */
#define VSCP_TYPE_SECURITY_IN_MOTION             35

/**
 * ...
 */
#define VSCP_TYPE_SECURITY_NOT_IN_MOTION         36

/**
 * ...
 */
#define VSCP_TYPE_SECURITY_VIBRATION             37

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

#endif  /* __VSCP_TYPE_SECURITY_H__ */

/** @} */
