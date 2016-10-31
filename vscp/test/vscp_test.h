/* The MIT License (MIT)
 *
 * Copyright (c) 2014 - 2016, Andreas Merkle
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
@brief  VSCP module tests
@file   vscp_test.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains all VSCP module tests. All tests are realized with CUnit.

*******************************************************************************/

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
*/

#ifndef __VSCP_TEST_H__
#define __VSCP_TEST_H__

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
 * Initializes the persistent memory with default values (0xFF).
 * Resets all call counters.
 * Clear rx and tx message buffers.
 * Resets all timers.
 * Reset the decision matrix.
 */
extern int  vscp_test_init(void);

/**
 * Precondition:
 *  - VSCP is not initialized yet.
 *
 * Action:
 *  - Initialize VSCP framework.
 *
 * Expectation:
 *  - All VSCP layers are initialized.
 *  - Persistent memory is initialized and contains default values.
 *  - Lamp blinks.
 */
extern void vscp_test_initNodeTheFirstTime(void);

/**
 * Precondition:
 *  - VSCP is initialized just one call before.
 *
 * Action:
 *  - Process VSCP the first time.
 *
 * Expectation:
 *  - Nickname discovery starts.
 *  - Timer for nickname discovery observation is started.
 *  - Lamp blinks.
 */
extern void vscp_test_processNodeTheFirstTime(void);

/**
 * Precondition:
 *  - Nickname discovery started and nickname is requested from segment master.
 *
 * Action:
 *  - Send nickname id back.
 *
 * Expectation:
 *  - Nickname is stored in persistent memory.
 *  - Node stops nickname discovery.
 *  - Lamp is on.
 */
extern void vscp_test_finisheNicknameDiscovery(void);

/**
 * Precondition:
 *  - Nickname discovery finished and nickname is set.
 *  - Node sent a new node online event.
 *
 * Action:
 *  - Send segment controller heartbeat event.
 *
 * Expectation:
 *  - Node stores the segment CRC.
 *  - Node doesn't initialize again.
 */
extern void vscp_test_firstSegCtrlHeartBeat(void);

/**
 * Precondition:
 *  - Nickname discovery finished and nickname is set.
 *  - Node sent a new node online event.
 *
 * Action:
 *  - Send a probe acknowledge with the same nickname.
 *
 * Expectation:
 *  - Nickname is cleared in persistent memory.
 *  - Node sends a probe event to segment master.
 */
extern void vscp_test_sendProbeAckInActiveState(void);

/**
 * Precondition:
 *  - Node sent a probe event to segment master.
 *
 * Action:
 *  - No response (no segment master).
 *
 * Expectation:
 *  - After a specific time, the node starts probing id for id.
 *  - But because every probe gets an answer, the node surrenders at the end.
 */
extern void vscp_test_noSegmentMaster(void);

/**
 * Precondition:
 *  - VSCP is not called yet.
 *
 * Action:
 *  - Limit number of available timers.
 *
 * Expectation:
 *  - VSCP core is in error state
 */
extern void vscp_test_init07(void);

/**
 * Initializes the persistent memory with default values (0xFF).
 * Resets all call counters.
 * Clear rx and tx message buffers.
 * Resets all timers.
 * Set a valid node nickname.
 */
extern int  vscp_test_initActive(void);

/**
 * Precondition:
 *  - VSCP is not initialized yet.
 *  - Nickname is set.
 *  - Node control flags are valid.
 *
 * Action:
 *  - Initialize VSCP.
 *
 * Expectation:
 *  - Persistent memory is initialized.
 *  - All VSCP layers are initialized.
 */
extern void vscp_test_active01(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send segment controller heartbeat event.
 *
 * Expectation:
 *  - Node stays in active mode.
 */
extern void vscp_test_active02(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send segment controller heartbeat event with different CRC.
 *
 * Expectation:
 *  - Node changes to initialization mode.
 */
extern void vscp_test_active03(void);

/**
 * Precondition:
 *  - Probe event is sent to the segment master.
 *
 * Action:
 *  - Proceed nickname disocvery.
 *
 * Expectation:
 *  - Node changes to active state.
 */
extern void vscp_test_active04(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send new node online / probe event (equal nickname id )
 *
 * Expectation:
 *  - Node sends probe acknowledge.
 */
extern void vscp_test_active05(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send new node online / probe event (different nickname id )
 *
 * Expectation:
 *  - Node do nothing.
 */
extern void vscp_test_active06(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send probe ACK.
 *
 * Expectation:
 *  - Node do nothing.
 */
extern void vscp_test_active07(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Set nickname id for node
 *
 * Expectation:
 *  - Node accepts new nickname id
 */
extern void vscp_test_active08(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Set illegal nickname id for node
 *
 * Expectation:
 *  - Node doesn't accept nickname (sends nothing)
 */
extern void vscp_test_active09(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Drop nickname
 *
 * Expectation:
 *  - Node changes to initialization state
 */
extern void vscp_test_active10(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Reset device, keep nickname
 *
 * Expectation:
 *  - Node restarts normal
 */
extern void vscp_test_active11(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Go idle, do not start up again
 *
 * Expectation:
 *  - Node is in idle state
 */
extern void vscp_test_active12(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Set persistent memory defaults
 *
 * Expectation:
 *  - Node set persistent memory defaults.
 *  - Node resets
 */
extern void vscp_test_active13(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Reset after specific time and keep nickname
 *
 * Expectation:
 *  - Node keeps nickname
 *  - Node resets after specific time
 */
extern void vscp_test_active14(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Read all registers.
 *  - Write to all read/write registers.
 *  - Check read only registers.
 *  - Check write only registers.
 *
 * Expectation:
 *  - All registers can be read and return the correct value.
 *  - All read/write registers can be written.
 *  - All read only registers can not be written.
 */
extern void vscp_test_active15(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Enter boot loader (wrong data)
 *
 * Expectation:
 *  - Node send a NACK and doesn't enter boot loader
 */
extern void vscp_test_active16(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Enter boot loader
 *
 * Expectation:
 *  - Boot loader shall be requested
 */
extern void vscp_test_active17(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - GUID drop nickname
 *
 * Expectation:
 *  - Node restarts in initialization state
 */
extern void vscp_test_active18(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Read page
 *
 * Expectation:
 *  - Page shall be read
 */
extern void vscp_test_active19(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Read page (7 byte)
 *
 * Expectation:
 *  - Page shall be read
 */
extern void vscp_test_active20(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Read page (8 byte)
 *
 * Expectation:
 *  - Page shall be read
 */
extern void vscp_test_active21(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Write page (4 byte)
 *
 * Expectation:
 *  - Page shall be read
 */
extern void vscp_test_active22(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Increment register
 *
 * Expectation:
 *  - Register is incremented
 */
extern void vscp_test_active23(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Decrement register
 *
 * Expectation:
 *  - Register is decremented
 */
extern void vscp_test_active24(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Who is there? (all)
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active25(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Who is there? (node addressed)
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active26(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Get decision matrix info
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active28(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Write data to decision matrix
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active29(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Read decision matrix data
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active30(void);

/**
 * Initialize decision matrix and the decision matrix extension.
 */
extern int vscp_test_initDM(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Node 0xaa sends a event
 *
 * Expectation:
 *  - Action 1 is executed.
 */
extern void vscp_test_dm01(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a event from a hard coded node.
 *
 * Expectation:
 *  - Action 2 is executed.
 */
extern void vscp_test_dm02(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a event with class 0x1f5.
 *
 * Expectation:
 *  - Action 3 is executed.
 */
extern void vscp_test_dm03(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a event with type 0x12.
 *
 * Expectation:
 *  - Action 4 is executed.
 */
extern void vscp_test_dm04(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a event with the nodes zone and sub-zone.
 *
 * Expectation:
 *  - Action 5 is executed.
 */
extern void vscp_test_dm05(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a class 0x14, type 0x01 event from node 0xbb.
 *
 * Expectation:
 *  - Action 6 is executed.
 */
extern void vscp_test_dm06(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send a class information, type button event from node 0xdd with zone 0x02, sub-zone 0x03
 *
 * Expectation:
 *  - Action 7 is executed.
 */
extern void vscp_test_dm07(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix configured
 *
 * Action:
 *  - Send class information, type button event from node 0xdd, par0 = 0x01, par3 = 0x04, par4 = 0x05, par5 = 0x06
 *
 * Expectation:
 *  - Action 8 is executed.
 */
extern void vscp_test_dm08(void);

/**
 * Initialize decision matrix next generation.
 */
extern int vscp_test_initDMNG(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix next generation configured
 *
 * Action:
 *  - Send class information, type button event with parameter "all released".
 *
 * Expectation:
 *  - No action is executed.
 */
extern void vscp_test_dmNG01(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix next generation configured
 *
 * Action:
 *  - Send class information, type button event with parameter "pressed".
 *
 * Expectation:
 *  - Action 1 is executed.
 */
extern void vscp_test_dmNG02(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Extended page read
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active31(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Extended page write
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active32(void);

/**
 * Precondition:
 *  - VSCP is active.
 *  - Decision matrix exists
 *
 * Action:
 *  - Wait for node heartbeat
 *
 * Expectation:
 *  - Response ...
 */
extern void vscp_test_active33(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Prepare tx message
 *  - Send message
 *
 * Expectation:
 *  - Message is sent to the transport layer.
 */
extern void vscp_test_active34(void);

/**
 * Precondition:
 *  - VSCP is active.
 *
 * Action:
 *  - Send a event to the core, which is not from the protocol class.
 *
 * Expectation:
 *  - Event is provided to the application.
 */
extern void vscp_test_active35(void);

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_TEST_H__ */
