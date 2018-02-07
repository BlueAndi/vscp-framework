/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2018, Andreas Merkle
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
@brief  VSCP class 1 type Measurement events
@file   vscp_measurement.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Measurement information.

This file is automatically generated. Don't change it manually.

@section svn Subversion
$Rev:  $
$Date:  $
*******************************************************************************/

#ifndef __VSCP_MEASUREMENT_H__
#define __VSCP_MEASUREMENT_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_types.h"

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
 * Undefined measurement value.
 *
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendUndefinedEvent(void);

/**
 * This is a discrete value typical for a count. There is no unit for this measurement just a discrete
 * value.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendCountEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Meter.
 *
 * This is a measurement of a length or a distance.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendLengthDistanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Kilogram.
 *
 * This is a measurement of a mass.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendMassEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Seconds.
 *
 * Opt. unit: (1) Millisecond . Absolute: (2) y-y-m-d-h-m-s (binary). String: (3) “HHMMSS” time since
 * epoch (00:00:00 UTC, January 1, 1970).
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendTimeEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Ampere.
 *
 * This is a measurement of an electric current.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendElectricCurrentEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Kelvin.
 *
 * Opt. unit: Degree Celsius (1), Fahrenheit (2)
 *
 * This is a measurement of a temperature.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendTemperatureEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Mole.
 *
 * This is a measurement of an amount of a substance.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendAmountOfSubstanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Candela.
 *
 * This is a measurement of luminous intensity.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendLuminousIntensityEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Hertz.
 *
 * This is a measurement of regular events during a second.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendFrequencyEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Becquerel.
 *
 * This is a measurement of rates of things, which happen randomly, or are unpredictable.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendRadioactivityAndOtherRandomEventsEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Newton.
 *
 * This is a measurement of force.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendForceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Pascal.
 *
 * Opt. unit: bar (1), psi (2)
 *
 * This is a measurement of pressure.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendPressureEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Joule.
 *
 * This is a measurement of energy.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendEnergyEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Watt.
 *
 * This is a measurement of power.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendPowerEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Coulomb.
 *
 * This is a measurement electrical charge.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendElectricalChargeEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Volt.
 *
 * This is a measurement of electrical potential.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendElectricalPotentialEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Farad.
 *
 * This is a measurement of electric capacitance.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendElectricalCapacitanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Ohm.
 *
 * This is a measurement of resistance.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendElectricalResistanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Siemens.
 *
 * This is a measurement of electrical conductance.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendElectricalConductanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Ampere meters.
 *
 * This is a measurement of magnetic field strength.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendMagneticFieldStrengthEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Weber.
 *
 * This is a measurement of magnetic flux.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendMagneticFluxEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Tesla.
 *
 * This is a measurement of flux density or field strength for magnetic fields (also called the
 * magnetic induction).
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendMagneticFluxDensityEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Henry.
 *
 * This is a measurement of inductance.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendInductanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Lumen (lm= cd * sr)
 *
 * This is a measurement of luminous Flux.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendLuminousFluxEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Lux ( lx = lm / m² )
 *
 * This is used to express both Illuminance (incidence of light) and Luminous Emittance (emission of
 * light).
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendIlluminanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Gray. Opt unit: Sievert.
 *
 * This is a measurement of a radiation dose (Absorbed dose of ionizing radiation).
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendRadiationDoseEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Katal.
 *
 * This is a measurement of catalytic activity used in biochemistry.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendCatalyticActivityEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: cubic meter
 *
 * Opt. unit: Liter (dm³) (1).
 *
 * This is a measurement of volume.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendVolumeEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Bel.
 *
 * Opt Unit: Neper (1), dB (2).
 *
 * This is a measurement of sound intensity.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendSoundIntensityEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Radian (Plane angles). Opt Unit: Degree (1) Opt Unit: Arcminute (2) Opt Unit:
 * Arcseconds (3)
 *
 * This is a measurement of an angle.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendAngleEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Longitude.
 *
 * Opt. unit: Latitude.
 *
 * This is a measurement of a position.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendPositionEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Meters per second.
 *
 * This is a measurement of a speed.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendSpeedEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Meters per second/second.
 *
 * This is a measurement of acceleration.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendAccelerationEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: N/m.
 *
 * This is a measurement of tension.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendTensionEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Relative percentage 0-100%.
 *
 * This is a measurement of relative moistness (Humidity).
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendDampMoistEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Cubic meters/second. Opt Unit: Liter/Second.
 *
 * This is a measurement of flow.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendFlowEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Thermal ohm K/W.
 *
 * This is a measurement of thermal resistance.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendThermalResistanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Dioptre (dpt) m-1.
 *
 * This is a measurement of refractive power.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendRefractivePowerEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Poiseuille (Pl) Pa . s.
 *
 * This is a measurement of dynamic viscosity.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendDynamicViscosityEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Rayal Pa . s/m.
 *
 * This is a measurement of sound impedance.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendSoundImpedanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Acoustic ohm Pa . s/ m³.
 *
 * This is a measurement of refractive power.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendSoundResistanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Darag F-1.
 *
 * This is a measurement of electric elasticity.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendElectricElastanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Talbot ( tb = lm * s)
 *
 * This is a measurement of luminous energy.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendLuminousEnergyEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Nit (nt = cd / m²)
 *
 * This is a measurement of luminance.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendLuminanceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Molal mol/kg.
 *
 * This is a measurement of chemical concentration.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendChemicalConcentrationEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Sievert J/Kg.
 *
 * This is a measurement of dose equivalent.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendDoseEquivalentEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Kelvin.
 *
 * Opt. unit: Degree Celsius (1), Fahrenheit (2)
 *
 * This is a measurement of the Dew Point.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendDewPointEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Relative value.
 *
 * This is a relative value for a level measurement without a unit. It is just relative to the min/max
 * value for the selected data representation.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendRelativeLevelEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: Meter.
 *
 * Opt. unit: Feet(1), inches (2)
 *
 * Altitude in meters.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendAltitude(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: square meter (m²)
 *
 * Area in square meter.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendAreaEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: watt per steradian ( W / sr )
 *
 * Radiated power per room angle.
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendRadiantIntensityEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Default unit: att per steradian per square metre ( W / (sr * m²) )
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendRadianceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Power emitted from or striking onto a surface or area.
 *
 * Default unit: watt per square metre ( W / m² )
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendIrradianceExitanceRadiosityEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Radiance of a surface per unit frequency or wavelength.
 *
 * Default unit: watt per steradian per square metre per nm (W·sr-1·m-2·nm-1)
 *
 * Opt. unit: watt per steradian per metre3 (W·sr-1·m-3) watt per steradian per square metre per hertz
 * (W·sr-1·m-3)
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendSpectralRadianceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Irradiance of a surface per unit frequency or wavelength.
 *
 * Default unit: watt per square metre per nm (W·m-2·nm-1)
 *
 * Opt. unit: watt per metre3 (W·m-3) watt per square metre per hertz (W·m-2·Hz-1)
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendSpectralIrradianceEvent(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * This is a measurement of sound pressure (acoustic pressure). 
 *
 * Default unit: pascal (Pa)
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendSoundPressure(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Sound energy density or sound density is the sound energy per unit volume.
 *
 * Default unit: pascal (Pa)
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendSoundEnergyDensity(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

/**
 * Sound level expressed in decibel. This event is supplied for convenience.
 *
 * Default unit: decibel (dB)
 *
 * @param[in] index Index for sensor.
 * @param[in] unit The unit of the data.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 *
 */
extern BOOL vscp_measurement_sendSoundLevel(uint8_t index, uint8_t unit, int32_t data, int8_t exp);

#endif /* __VSCP_MEASUREMENT_H__ */
