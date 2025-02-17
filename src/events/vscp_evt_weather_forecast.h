/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2025 Andreas Merkle
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
@brief  Weather forecast
@file   vscp_evt_weather_forecast.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Weather reporting forecast. The class have the same types as CLASS1.WEATHER

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_EVT_WEATHER_FORECAST_H__
#define __VSCP_EVT_WEATHER_FORECAST_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_platform.h"

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
extern BOOL vscp_evt_weather_forecast_sendGeneralEvent(void);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSeasonWinter(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season spring
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSeasonSpring(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season summer
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSeasonSummer(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Autumn summer
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAutumnSummer(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * No wind
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendNoWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Low wind
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendLowWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Medium wind
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendMediumWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * High wind
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendHighWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Very high wind
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendVeryHighWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Air foggy
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirFoggy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Air freezing
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirFreezing(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Air Very cold
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirVeryCold(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Air cold
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirCold(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Air normal
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirNormal(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Air hot
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirHot(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Air very hot
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirVeryHot(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Pollution low
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendPollutionLow(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Pollution medium
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendPollutionMedium(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Pollution high
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendPollutionHigh(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Air humid
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirHumid(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Air dry
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirDry(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Soil humid
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSoilHumid(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Soil dry
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSoilDry(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Rain none
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendRainNone(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Rain light
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendRainLight(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Rain heavy
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendRainHeavy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Rain very heavy
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendRainVeryHeavy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Sun none
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSunNone(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Sun light
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSunLight(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Sun heavy
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSunHeavy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Snow none
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSnowNone(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Snow light
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSnowLight(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Snow heavy
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSnowHeavy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Dew point
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendDewPoint(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Storm
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendStorm(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Flood
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendFlood(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Earthquake
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendEarthquake(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Nuclear disaster
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendNuclearDisaster(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Fire
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendFire(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Lightning
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendLightning(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * UV Radiation low
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationLow(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * UV Radiation medium
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationMedium(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * UV Radiation normal
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationNormal(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * UV Radiation high
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationHigh(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * UV Radiation very high
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationVeryHigh(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Warning level 1
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel1(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Warning level 2
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel2(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Warning level 3
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel3(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Warning level 4
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel4(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Warning level 5
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel5(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Armageddon
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendArmageddon(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * UV Index
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-254). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-254). 255 is all sub-zones.
 * @param[in] uvIndex UV index (0-15)
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvIndex(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t uvIndex);

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_EVT_WEATHER_FORECAST_H__ */
