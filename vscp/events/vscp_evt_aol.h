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
@brief  Alert On LAN
@file   vscp_evt_aol.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
AOL Event. The main idea of AOL is to send warnings to remote administrators about different PC
conditions using a LAN. Info here https://en.wikipedia.org/wiki/Alert_on_LAN

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_EVT_AOL_H__
#define __VSCP_EVT_AOL_H__

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
extern BOOL vscp_evt_aol_sendGeneralEvent(void);

/**
 * System unplugged from power source
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendSystemUnpluggedFromPowerSource(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * System unplugged from network
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendSystemUnpluggedFromNetwork(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Chassis intrusion
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendChassisIntrusion(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Processor removal
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendProcessorRemoval(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * System environmental errors
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendSystemEnvironmentalErrors(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * High temperature
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendHighTemperature(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Fan speed problem
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendFanSpeedProblem(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Voltage fluctuations
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendVoltageFluctuations(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Operating system errors
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendOperatingSystemErrors(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * System power-on error
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendSystemPowerOnError(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * System is hung
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendSystemIsHung(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Component failure
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendComponentFailure(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Remote system reboot upon report of a critical failure
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendRemoteSystemRebootUponReportOfACriticalFailure(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Repair Operating System
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendRepairOperatingSystem(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Update BIOS image
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendUpdateBiosImage(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Update Perform other diagnostic procedures
 * 
 * @param[in] index Index for record.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_aol_sendUpdatePerformOtherDiagnosticProcedures(uint8_t index, uint8_t zone, uint8_t subZone);

#endif  /* __VSCP_EVT_AOL_H__ */
