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
@brief  Main entry point
@file   main.c
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains the main entry point.

*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "vscp_test.h"

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
    PROTOTYPES
*******************************************************************************/

/*******************************************************************************
    LOCAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/

/*******************************************************************************
    GLOBAL FUNCTIONS
*******************************************************************************/

/**
 * Main entry point.
 */
int main(int argc, char* argv[])
{
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        printf("Initialize of test registry failed.\n");
    }
    else
    {
        CU_pSuite   pSuite  = NULL;

        pSuite  = CU_add_suite("Initialization and nickname discovery", vscp_test_init, NULL);
        (void)CU_add_test(pSuite, "Initialize the node the first time", vscp_test_initNodeTheFirstTime);
        (void)CU_add_test(pSuite, "Process the node the first time", vscp_test_processNodeTheFirstTime);
        (void)CU_add_test(pSuite, "Finish nickname discovery", vscp_test_finisheNicknameDiscovery);
        (void)CU_add_test(pSuite, "First segment controller heartbeat", vscp_test_firstSegCtrlHeartBeat);
        (void)CU_add_test(pSuite, "Send probe ack with same nickname", vscp_test_sendProbeAckInActiveState);
        (void)CU_add_test(pSuite, "Segment master removed", vscp_test_noSegmentMaster);

        pSuite  = CU_add_suite("Force error state", vscp_test_init, NULL);
        (void)CU_add_test(pSuite, "Limit number of available timers", vscp_test_init07);

        pSuite  = CU_add_suite("Segment controller heartbeat tests", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Segment controller heartbeat (CRC equal)", vscp_test_active02);
        (void)CU_add_test(pSuite, "Segment controller heartbeat (CRC different)", vscp_test_active03);
        (void)CU_add_test(pSuite, "Proceed nickname disovery", vscp_test_active04);

        pSuite  = CU_add_suite(".", NULL, NULL);
        (void)CU_add_test(pSuite, "New node online (equal nickname id)", vscp_test_active05);
        (void)CU_add_test(pSuite, "New node online (different nickname id)", vscp_test_active06);
        (void)CU_add_test(pSuite, "Probe ACK", vscp_test_active07);
        (void)CU_add_test(pSuite, "Set new nickname id for node", vscp_test_active08);
        (void)CU_add_test(pSuite, "Set illegal nickname id for node", vscp_test_active09);
        (void)CU_add_test(pSuite, "Drop nickname", vscp_test_active10);

        pSuite  = CU_add_suite("Drop nickname part 1", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Reset device, keep nickname", vscp_test_active11);

        pSuite  = CU_add_suite("Drop nickname part 2", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Go idle state", vscp_test_active12);

        pSuite  = CU_add_suite("Drop nickname part 3", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Set persistent storage to default", vscp_test_active13);

        pSuite  = CU_add_suite("Drop nickname part 4", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Reset after specific time", vscp_test_active14);

        pSuite  = CU_add_suite("VSCP registers", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Read/Write all registers", vscp_test_active15);

        pSuite  = CU_add_suite("Jump to boot loader", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Enter boot loader (wrong data)", vscp_test_active16);
        (void)CU_add_test(pSuite, "Enter boot loader", vscp_test_active17);

        pSuite  = CU_add_suite("GUID drop nickname part 1", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Drop nickname and reset", vscp_test_active18);

        pSuite  = CU_add_suite("Page read/write", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Read page (4 byte)", vscp_test_active19);
        (void)CU_add_test(pSuite, "Read page (7 byte)", vscp_test_active20);
        (void)CU_add_test(pSuite, "Read page (8 byte)", vscp_test_active21);
        (void)CU_add_test(pSuite, "Write page (4 byte)", vscp_test_active22);

        pSuite  = CU_add_suite("Increment/Decrement register", NULL, NULL);
        (void)CU_add_test(pSuite, "Increment register", vscp_test_active23);
        (void)CU_add_test(pSuite, "Decrement register", vscp_test_active24);

        pSuite  = CU_add_suite("Who is there?", NULL, NULL);
        (void)CU_add_test(pSuite, "Who is there? (all)", vscp_test_active25);
        (void)CU_add_test(pSuite, "Who is there? (node addressed)", vscp_test_active26);

        pSuite  = CU_add_suite("Decision matrix access", NULL, NULL);
        (void)CU_add_test(pSuite, "Get decision matrix info (matrix exists)", vscp_test_active28);
        (void)CU_add_test(pSuite, "Write data to decision matrix", vscp_test_active29);
        (void)CU_add_test(pSuite, "Read decision matrix data", vscp_test_active30);

        pSuite  = CU_add_suite("Decision matrix", vscp_test_initDM, NULL);
        (void)CU_add_test(pSuite, "Any event from node 0xaa triggers.", vscp_test_dm01);
        (void)CU_add_test(pSuite, "Any event from a hard coded node triggers.", vscp_test_dm02);
        (void)CU_add_test(pSuite, "Any event with class 0x1f5 triggers.", vscp_test_dm03);
        (void)CU_add_test(pSuite, "Any event with type 0x12 triggers.", vscp_test_dm04);
        (void)CU_add_test(pSuite, "Any event with the nodes zone/sub-zone triggers.", vscp_test_dm05);
        (void)CU_add_test(pSuite, "Any class 0x02, type 0x01 event from node 0xbb triggers.", vscp_test_dm06);
        (void)CU_add_test(pSuite, "Only class 0x14, type 0x01 event from node 0xdd, zone 0x02, sub-zone 0x03 triggers.", vscp_test_dm07);
        (void)CU_add_test(pSuite, "Only class 0x14, type 0x01 event from node 0xdd, with par0 = 0x01, par3 = 0x04, par4 = 0x05 and par5 = 0x06 triggers.", vscp_test_dm08);

        pSuite  = CU_add_suite("Extended page read/write", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Extended page read", vscp_test_active31);
        (void)CU_add_test(pSuite, "Extended page write", vscp_test_active32);

        pSuite  = CU_add_suite("Node heartbeat", vscp_test_initActive, NULL);
        (void)CU_add_test(pSuite, "Start up", vscp_test_active01);
        (void)CU_add_test(pSuite, "Node heartbeat", vscp_test_active33);

        pSuite  = CU_add_suite("Transparent sending of events", NULL, NULL);
        (void)CU_add_test(pSuite, "Prepare tx message and send event", vscp_test_active34);

        pSuite  = CU_add_suite("Application gets events from the core", NULL, NULL);
        (void)CU_add_test(pSuite, "Receive transparent events", vscp_test_active35);

        pSuite  = CU_add_suite("Decision matrix next generation", vscp_test_initDMNG, NULL);
        (void)CU_add_test(pSuite, "Send class information, type button event with parameter \"all released\".", vscp_test_dmNG01);
        (void)CU_add_test(pSuite, "Send class information, type button event with parameter \"pressed\".", vscp_test_dmNG02);

        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();

        CU_cleanup_registry();
    }

    return 0;
}

/*******************************************************************************
    LOCAL FUNCTIONS
*******************************************************************************/
