/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2024 Andreas Merkle
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
@file   vscp_type_weather_forecast.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Weather reporting forecast. The class have the same types as CLASS1.WEATHER

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_TYPE_WEATHER_FORECAST_H__
#define __VSCP_TYPE_WEATHER_FORECAST_H__

/** @defgroup vscp_type_weather_forecast Weather forecast
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
#define VSCP_TYPE_WEATHER_FORECAST_GENERAL                0

/**
 * The winter season has started.
 */
#define VSCP_TYPE_WEATHER_FORECAST_SEASONS_WINTER         1

/**
 * The spring season has started.
 */
#define VSCP_TYPE_WEATHER_FORECAST_SEASONS_SPRING         2

/**
 * The summer season has started.
 */
#define VSCP_TYPE_WEATHER_FORECAST_SEASONS_SUMMER         3

/**
 * The autumn season has started.
 */
#define VSCP_TYPE_WEATHER_FORECAST_SEASONS_AUTUMN         4

/**
 * No wind
 */
#define VSCP_TYPE_WEATHER_FORECAST_WIND_NONE              5

/**
 * Low wind speed conditions.
 */
#define VSCP_TYPE_WEATHER_FORECAST_WIND_LOW               6

/**
 * Medium wind speed conditions.
 */
#define VSCP_TYPE_WEATHER_FORECAST_WIND_MEDIUM            7

/**
 * High wind speed conditions.
 */
#define VSCP_TYPE_WEATHER_FORECAST_WIND_HIGH              8

/**
 * Very high wind speed conditions.
 */
#define VSCP_TYPE_WEATHER_FORECAST_WIND_VERY_HIGH         9

/**
 * Fogg.
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_FOGGY              10

/**
 * Freezing.
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_FREEZING           11

/**
 * Cold
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_VERY_COLD          12

/**
 * Very cold
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_COLD               13

/**
 * Air normal
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_NORMAL             14

/**
 * Air hot
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_HOT                15

/**
 * Air very hot
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_VERY_HOT           16

/**
 * Pollution low
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_POLLUTION_LOW      17

/**
 * Pollution medium
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_POLLUTION_MEDIUM   18

/**
 * Pollution high
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_POLLUTION_HIGH     19

/**
 * Air humid
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_HUMID              20

/**
 * Air dry
 */
#define VSCP_TYPE_WEATHER_FORECAST_AIR_DRY                21

/**
 * soil humid
 */
#define VSCP_TYPE_WEATHER_FORECAST_SOIL_HUMID             22

/**
 * soil dry
 */
#define VSCP_TYPE_WEATHER_FORECAST_SOIL_DRY               23

/**
 * Rain none
 */
#define VSCP_TYPE_WEATHER_FORECAST_RAIN_NONE              24

/**
 * Rain light
 */
#define VSCP_TYPE_WEATHER_FORECAST_RAIN_LIGHT             25

/**
 * Rain heavy
 */
#define VSCP_TYPE_WEATHER_FORECAST_RAIN_HEAVY             26

/**
 * Rain very heavy
 */
#define VSCP_TYPE_WEATHER_FORECAST_RAIN_VERY_HEAVY        27

/**
 * Sun none
 */
#define VSCP_TYPE_WEATHER_FORECAST_SUN_NONE               28

/**
 * Sun light
 */
#define VSCP_TYPE_WEATHER_FORECAST_SUN_LIGHT              29

/**
 * Sun heavy
 */
#define VSCP_TYPE_WEATHER_FORECAST_SUN_HEAVY              30

/**
 * Snow none.
 */
#define VSCP_TYPE_WEATHER_FORECAST_SNOW_NONE              31

/**
 * Snow light.
 */
#define VSCP_TYPE_WEATHER_FORECAST_SNOW_LIGHT             32

/**
 * Snow heavy.
 */
#define VSCP_TYPE_WEATHER_FORECAST_SNOW_HEAVY             33

/**
 * Dew point.
 */
#define VSCP_TYPE_WEATHER_FORECAST_DEW_POINT              34

/**
 * Storm.
 */
#define VSCP_TYPE_WEATHER_FORECAST_STORM                  35

/**
 * Flood.
 */
#define VSCP_TYPE_WEATHER_FORECAST_FLOOD                  36

/**
 * Earthquake
 */
#define VSCP_TYPE_WEATHER_FORECAST_EARTHQUAKE             37

/**
 * Nuclera disaster
 */
#define VSCP_TYPE_WEATHER_FORECAST_NUCLEAR_DISASTER       38

/**
 * Fire.
 */
#define VSCP_TYPE_WEATHER_FORECAST_FIRE                   39

/**
 * Lightning.
 */
#define VSCP_TYPE_WEATHER_FORECAST_LIGHTNING              40

/**
 * Radiation low.
 */
#define VSCP_TYPE_WEATHER_FORECAST_UV_RADIATION_LOW       41

/**
 * Radiation medium.
 */
#define VSCP_TYPE_WEATHER_FORECAST_UV_RADIATION_MEDIUM    42

/**
 * Radiation normal.
 */
#define VSCP_TYPE_WEATHER_FORECAST_UV_RADIATION_NORMAL    43

/**
 * Radiation high.
 */
#define VSCP_TYPE_WEATHER_FORECAST_UV_RADIATION_HIGH      44

/**
 * Radiation very high.
 */
#define VSCP_TYPE_WEATHER_FORECAST_UV_RADIATION_VERY_HIGH 45

/**
 * Warning level 1. This is the lowest varning level.
 */
#define VSCP_TYPE_WEATHER_FORECAST_WARNING_LEVEL1         46

/**
 * Warninglevel 2.
 */
#define VSCP_TYPE_WEATHER_FORECAST_WARNING_LEVEL2         47

/**
 * Warninglevel 3.
 */
#define VSCP_TYPE_WEATHER_FORECAST_WARNING_LEVEL3         48

/**
 * Warning level 4.
 */
#define VSCP_TYPE_WEATHER_FORECAST_WARNING_LEVEL4         49

/**
 * Warning level 5. This is the highest warning level.
 */
#define VSCP_TYPE_WEATHER_FORECAST_WARNING_LEVEL5         50

/**
 * The final warning level not seen by humans.
 */
#define VSCP_TYPE_WEATHER_FORECAST_ARMAGEDON              51

/**
 * UV Index is an international scale for UV intensity which can have the range of 1-15 where 1 is
 * very low radiation and a value over 10 is extremely high radiation.
 */
#define VSCP_TYPE_WEATHER_FORECAST_UV_INDEX               52

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

#endif  /* __VSCP_TYPE_WEATHER_FORECAST_H__ */

/** @} */