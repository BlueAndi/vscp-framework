/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
@brief  VSCP class 1 type Measurezone events
@file   vscp_measurezone.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Measurements with zone information.

This file is automatically generated. Don't change it manually.

@section svn Subversion
$Rev:  $
$Date:  $
*******************************************************************************/

#ifndef __VSCP_MEASUREZONE_H__
#define __VSCP_MEASUREZONE_H__

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
extern BOOL vscp_measurezone_sendUndefinedEvent(void);

/**
 * This is a discrete value typical for a count. There is no unit for this measurement just a discrete
 * value.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendCountEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Meter.
 * 
 * This is a measurement of a length or a distance.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendLengthDistanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Kilogram.
 * 
 * This is a measurement of a mass.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendMassEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Seconds.
 * 
 * Opt. unit: (1) Millisecond . Absolute: (2) y-y-m-d-h-m-s (binary). String: (3) “HHMMSS” time since
 * epoch (00:00:00 UTC, January 1, 1970).
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendTimeEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Ampere.
 * 
 * This is a measurement of an electric current.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendElectricCurrentEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Kelvin.
 * 
 * Opt. unit: Degree Celsius (1), Fahrenheit (2)
 * 
 * This is a measurement of a temperature.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendTemperatureEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Mole.
 * 
 * This is a measurement of an amount of a substance.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendAmountOfSubstanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Candela.
 * 
 * This is a measurement of luminous intensity.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendLuminousIntensityEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Hertz.
 * 
 * This is a measurement of regular events during a second.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendFrequencyEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Becquerel.
 * 
 * This is a measurement of rates of things, which happen randomly, or are unpredictable.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendRadioactivityAndOtherRandomEventsEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Newton.
 * 
 * This is a measurement of force.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendForceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Pascal.
 * 
 * Opt. unit: bar (1), psi (2)
 * 
 * This is a measurement of pressure.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendPressureEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Joule.
 * 
 * This is a measurement of energy.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendEnergyEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Watt.
 * 
 * This is a measurement of power.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendPowerEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Coulomb.
 * 
 * This is a measurement electrical charge.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendElectricalChargeEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Volt.
 * 
 * This is a measurement of electrical potential.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendElectricalPotentialEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Farad.
 * 
 * This is a measurement of electric capacitance.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendElectricalCapacitanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Ohm.
 * 
 * This is a measurement of resistance.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendElectricalResistanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Siemens.
 * 
 * This is a measurement of electrical conductance.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendElectricalConductanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Ampere meters.
 * 
 * This is a measurement of magnetic field strength.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendMagneticFieldStrengthEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Weber.
 * 
 * This is a measurement of magnetic flux.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendMagneticFluxEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Tesla.
 * 
 * This is a measurement of flux density or field strength for magnetic fields (also called the
 * magnetic induction).
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendMagneticFluxDensityEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Henry.
 * 
 * This is a measurement of inductance.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendInductanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Lumen (lm= cd * sr)
 * 
 * This is a measurement of luminous Flux.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendLuminousFluxEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Lux ( lx = lm / m² )
 * 
 * This is used to express both Illuminance (incidence of light) and Luminous Emittance (emission of
 * light).
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendIlluminanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Gray. Opt unit: Sievert.
 * 
 * This is a measurement of a radiation dose (Absorbed dose of ionizing radiation).
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendRadiationDoseEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Katal.
 * 
 * This is a measurement of catalytic activity used in biochemistry.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendCatalyticActivityEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: cubic meter
 * 
 * Opt. unit: Liter (dm³) (1).
 * 
 * This is a measurement of volume.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendVolumeEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Bel.
 * 
 * Opt Unit: Neper (1), dB (2).
 * 
 * This is a measurement of sound intensity.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendSoundIntensityEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Radian (Plane angles). Opt Unit: Degree (1) Opt Unit: Arcminute (2) Opt Unit:
 * Arcseconds (3)
 * 
 * This is a measurement of an angle.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendAngleEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Longitude.
 * 
 * Opt. unit: Latitude.
 * 
 * This is a measurement of a position.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendPositionEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Meters per second.
 * 
 * This is a measurement of a speed.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendSpeedEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Meters per second/second.
 * 
 * This is a measurement of acceleration.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendAccelerationEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: N/m.
 * 
 * This is a measurement of tension.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendTensionEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Relative percentage 0-100%.
 * 
 * This is a measurement of relative moistness (Humidity).
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendDampMoistEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Cubic meters/second. Opt Unit: Liter/Second.
 * 
 * This is a measurement of flow.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendFlowEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Thermal ohm K/W.
 * 
 * This is a measurement of thermal resistance.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendThermalResistanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Dioptre (dpt) m-1.
 * 
 * This is a measurement of refractive power.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendRefractivePowerEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Poiseuille (Pl) Pa . s.
 * 
 * This is a measurement of dynamic viscosity.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendDynamicViscosityEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Rayal Pa . s/m.
 * 
 * This is a measurement of sound impedance.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendSoundImpedanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Acoustic ohm Pa . s/ m³.
 * 
 * This is a measurement of refractive power.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendSoundResistanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Darag F-1.
 * 
 * This is a measurement of electric elasticity.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendElectricElastanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Talbot ( tb = lm * s)
 * 
 * This is a measurement of luminous energy.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendLuminousEnergyEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Nit (nt = cd / m²)
 * 
 * This is a measurement of luminance.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendLuminanceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Molal mol/kg.
 * 
 * This is a measurement of chemical concentration.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendChemicalConcentrationEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Sievert J/Kg.
 * 
 * This is a measurement of dose equivalent.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendDoseEquivalentEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Kelvin.
 * 
 * Opt. unit: Degree Celsius (1), Fahrenheit (2)
 * 
 * This is a measurement of the Dew Point.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendDewPointEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Relative value.
 * 
 * This is a relative value for a level measurement without a unit. It is just relative to the min/max
 * value for the selected data representation.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendRelativeLevelEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: Meter.
 * 
 * Opt. unit: Feet(1), inches (2)
 * 
 * Altitude in meters.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendAltitude(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: square meter (m²)
 * 
 * Area in square meter.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendAreaEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: watt per steradian ( W / sr )
 * 
 * Radiated power per room angle.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendRadiantIntensityEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: att per steradian per square metre ( W / (sr * m²) )
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendRadianceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: watt per square metre ( W / m² )
 * 
 * Power emitted from or striking onto a surface or area.
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendIrradianceExitanceRadiosityEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: watt per steradian per square metre per nm (W·sr-1·m-2·nm-1)
 * 
 * Opt. unit: watt per steradian per metre3 (W·sr-1·m-3) watt per steradian per square metre per hertz
 * (W·sr-1·m-3)
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendSpectralRadianceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

/**
 * Default unit: watt per square metre per nm (W·m-2·nm-1)
 * 
 * Opt. unit: watt per metre3 (W·m-3) watt per square metre per hertz (W·m-2·Hz-1)
 * 
 * @param[in] index Index for sensor.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data The data as signed integer.
 * @param[in] exp The exponent of the data (10^exponent).
 * @return Status
 * @retval FALSE Failed to send the event
 * @retval TRUE  Event successul sent
 * 
 */
extern BOOL vscp_measurezone_sendSpectralIrradianceEvent(uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, uint8_t exp);

#endif /* __VSCP_MEASUREZONE_H__ */