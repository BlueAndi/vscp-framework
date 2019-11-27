/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2019, Andreas Merkle
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
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSeasonWinter(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSeasonSpring(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSeasonSummer(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAutumnSummer(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendNoWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendLowWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendMediumWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendHighWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendVeryHighWind(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirFoggy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirFreezing(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirVeryCold(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirCold(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirNormal(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirHot(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirVeryHot(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendPollutionLow(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendPollutionMedium(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendPollutionHigh(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirHumid(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendAirDry(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSoilHumid(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSoilDry(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendRainNone(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendRainLight(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendRainHeavy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendRainVeryHeavy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSunNone(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSunLight(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSunHeavy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSnowNone(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSnowLight(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendSnowHeavy(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendDewPoint(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendStorm(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendFlood(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendEarthquake(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendNuclearDisaster(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendFire(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendLightning(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationLow(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationMedium(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationNormal(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationHigh(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvRadiationVeryHigh(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel1(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel2(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel3(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel4(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendWarningLevel5(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * Season winter
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendArmageddon(uint8_t index, uint8_t zone, uint8_t subZone);

/**
 * UV Index
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] uvIndex UV index (0-15)
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_weather_forecast_sendUvIndex(uint8_t index, uint8_t zone, uint8_t subZone, uint8_t uvIndex);

#endif  /* __VSCP_EVT_WEATHER_FORECAST_H__ */