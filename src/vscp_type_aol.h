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
@brief  Alert On LAN
@file   vscp_type_aol.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
AOL Event. The main idea of AOL is to send warnings to remote administrators about different PC
conditions using a LAN. Info here https://en.wikipedia.org/wiki/Alert_on_LAN

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_TYPE_AOL_H__
#define __VSCP_TYPE_AOL_H__

/** @defgroup vscp_type_aol Alert On LAN
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
#define VSCP_TYPE_AOL_GENERAL                     0

/**
 * This node was unplugged from its power source.
 */
#define VSCP_TYPE_AOL_UNPLUGGED_POWER             1

/**
 * This node was unplugged from the network.
 */
#define VSCP_TYPE_AOL_UNPLUGGED_LAN               2

/**
 * This node detected chassis intrusion.
 */
#define VSCP_TYPE_AOL_CHASSIS_INTRUSION           3

/**
 * This node detected processor removal.
 */
#define VSCP_TYPE_AOL_PROCESSOR_REMOVAL           4

/**
 * This node detected system environmental errors.
 */
#define VSCP_TYPE_AOL_ENVIRONMENT_ERROR           5

/**
 * This node detected high temperature.
 */
#define VSCP_TYPE_AOL_HIGH_TEMPERATURE            6

/**
 * This node detected Fan speed problem.
 */
#define VSCP_TYPE_AOL_FAN_SPEED                   7

/**
 * This node detected Voltage fluctuations.
 */
#define VSCP_TYPE_AOL_VOLTAGE_FLUCTUATIONS        8

/**
 * This node detected Operating system errors.
 */
#define VSCP_TYPE_AOL_OS_ERROR                    9

/**
 * This node detected System power-on errors.
 */
#define VSCP_TYPE_AOL_POWER_ON_ERROR              10

/**
 * This node detected System is hung.
 */
#define VSCP_TYPE_AOL_SYSTEM_HUNG                 11

/**
 * This node detected Component failure.
 */
#define VSCP_TYPE_AOL_COMPONENT_FAILURE           12

/**
 * This node detected Remote system reboot upon report of a critical failure.
 */
#define VSCP_TYPE_AOL_REBOOT_UPON_FAILURE         13

/**
 * This node detected Repair Operating System.
 */
#define VSCP_TYPE_AOL_REPAIR_OPERATING_SYSTEM     14

/**
 * This node detected Update BIOS image.
 */
#define VSCP_TYPE_AOL_UPDATE_BIOS_IMAGE           15

/**
 * This node detected Update Perform other diagnostic procedures.
 */
#define VSCP_TYPE_AOL_UPDATE_DIAGNOSTIC_PROCEDURE 16

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

#endif  /* __VSCP_TYPE_AOL_H__ */

/** @} */
