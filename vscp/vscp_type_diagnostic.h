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
@brief  Diagnostic
@file   vscp_type_diagnostic.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Diagnostic functionality.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_TYPE_DIAGNOSTIC_H__
#define __VSCP_TYPE_DIAGNOSTIC_H__

/** @defgroup vscp_type_diagnostic Diagnostic
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
#define VSCP_TYPE_DIAGNOSTIC_GENERAL            0

/**
 * Over voltage has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_OVERVOLTAGE        1

/**
 * Under voltage has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_UNDERVOLTAGE       2

/**
 * Low voltage on USB VBUS has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_VBUS_LOW           3

/**
 * Low battery voltage has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_BATTERY_LOW        4

/**
 * Battery full voltage has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_BATTERY_FULL       5

/**
 * Battery error has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_BATTERY_ERROR      6

/**
 * Functional battery has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_BATTERY_OK         7

/**
 * Over current has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_OVERCURRENT        8

/**
 * Circuit error has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_CIRCUIT_ERROR      9

/**
 * Short circuit has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_SHORT_CIRCUIT      10

/**
 * Open Circuit has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_OPEN_CIRCUIT       11

/**
 * Moist has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_MOIST              12

/**
 * Wire failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_WIRE_FAIL          13

/**
 * Wireless faliure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_WIRELESS_FAIL      14

/**
 * IR failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_IR_FAIL            15

/**
 * 1-wire failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_1WIRE_FAIL         16

/**
 * RS-222 failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_RS222_FAIL         17

/**
 * RS-232 failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_RS232_FAIL         18

/**
 * RS-423 failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_RS423_FAIL         19

/**
 * RS-485 failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_RS485_FAIL         20

/**
 * CAN failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_CAN_FAIL           21

/**
 * LAN failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_LAN_FAIL           22

/**
 * USB failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_USB_FAIL           23

/**
 * Wifi failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_WIFI_FAIL          24

/**
 * NFC/RFID failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_NFC_RFID_FAIL      25

/**
 * Low signal has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_LOW_SIGNAL         26

/**
 * High signal has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_HIGH_SIGNAL        27

/**
 * ADC failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_ADC_FAIL           28

/**
 * ALU failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_ALU_FAIL           29

/**
 * An assert has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_ASSERT             30

/**
 * DAC failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_DAC_FAIL           31

/**
 * DMA failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_DMA_FAIL           32

/**
 * Ethernet failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_ETH_FAIL           33

/**
 * Exception has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_EXCEPTION          34

/**
 * Floating point unit (FPU) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_FPU_FAIL           35

/**
 * General purpose I/O (GPIO) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_GPIO_FAIL          36

/**
 * I2C failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_I2C_FAIL           37

/**
 * I2C failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_I2S_FAIL           38

/**
 * Invalid configuration has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_INVALID_CONFIG     39

/**
 * Memory Management Unit (MMU) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_MMU_FAIL           40

/**
 * Non mask-able interrupt (NMI) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_NMI                41

/**
 * Overheat has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_OVERHEAT           42

/**
 * Phased Locked Loop (PLL) fail has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_PLL_FAIL           43

/**
 * Power ON Reset (POR) fail has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_POR_FAIL           44

/**
 * Pulse Width Modulation (PWM) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_PWM_FAIL           45

/**
 * Random Access Memory (RAM) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_RAM_FAIL           46

/**
 * Read only memory (ROM) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_ROM_FAIL           47

/**
 * Serial peripheral interface (SPI) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_SPI_FAIL           48

/**
 * Stack failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_STACK_FAIL         49

/**
 * LIN bus failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_LIN_FAIL           50

/**
 * UART failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_UART_FAIL          51

/**
 * Unhandled interrupt has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_UNHANDLED_INT      52

/**
 * Memory failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_MEMORY_FAIL        53

/**
 * Variable range failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_VARIABLE_RANGE     54

/**
 * Watch Dog Timer (WDT) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_WDT                55

/**
 * EEPROM failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_EEPROM_FAIL        56

/**
 * Encryption failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_ENCRYPTION_FAIL    57

/**
 * Bad user input failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_BAD_USER_INPUT     58

/**
 * Decryption failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_DECRYPTION_FAIL    59

/**
 * Noise has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_NOISE              60

/**
 * Boot loader failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_BOOTLOADER_FAIL    61

/**
 * Program flow failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_PROGRAMFLOW_FAIL   62

/**
 * Real Time Clock (RTC) failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_RTC_FAIL           63

/**
 * System test failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_SYSTEM_TEST_FAIL   64

/**
 * Sensor failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_SENSOR_FAIL        65

/**
 * Safe state entered has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_SAFESTATE          66

/**
 * Signal implausible has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_SIGNAL_IMPLAUSIBLE 67

/**
 * Storage fail has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_STORAGE_FAIL       68

/**
 * Self test OK has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_SELFTEST_FAIL      69

/**
 * ESD/EMC/EMI failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_ESD_EMC_EMI        70

/**
 * Timeout has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_TIMEOUT            71

/**
 * LCD failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_LCD_FAIL           72

/**
 * Touch panel failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_TOUCHPANEL_FAIL    73

/**
 * No load has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_NOLOAD             74

/**
 * Cooling failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_COOLING_FAIL       75

/**
 * Heating failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_HEATING_FAIL       76

/**
 * Transmission failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_TX_FAIL            77

/**
 * Receiption failure has been diagnosed.
 */
#define VSCP_TYPE_DIAGNOSTIC_RX_FAIL            78

/**
 * A failure in an external IC circuit has been detected.
 */
#define VSCP_TYPE_DIAGNOSTIC_EXT_IC_FAIL        79

/**
 * Charging of battery or similar has started or is in progress.
 */
#define VSCP_TYPE_DIAGNOSTIC_CHARGING_ON        80

/**
 * Charging of battery or similar has ended.
 */
#define VSCP_TYPE_DIAGNOSTIC_CHARGING_OFF       81

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

#endif  /* __VSCP_TYPE_DIAGNOSTIC_H__ */

/** @} */
