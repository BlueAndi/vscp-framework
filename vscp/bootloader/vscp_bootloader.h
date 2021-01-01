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
@brief  VSCP bootloader
@file   vscp_bootloader.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides a generic bootloader as specified by VSCP specification.

*******************************************************************************/
/** @defgroup vscp_bootloader VSCP bootloader
 * Generic bootloader algorithm as specified by VSCP specification.
 *
 * 1. Determine whether
 *     a) the user force bootloader mode,
 *     b) an valid application is present and shall be started,
 *     c) the application jumped to the bootloader,
 *     d) no application is present.
 *
 * 2. Conditions for case a):
 *     - If the segment initialization button is pressed during power-up, stay in the bootloader.
 *     Go to step 6.
 *
 * 3. Conditions for case b):
 *     - The segment initialization button is released.
 *     - The boot flag in the persistent memory shall contain 0xAA for destination application.
 *     If all conditions met, jump to the application otherwise go to step 6.
 *
 * 4. Conditions for case c):
 *     - The hardware jumper is not set.
 *     - The boot flag in the persistent memory shall contain 0xBB for destination bootloader.
 *     If all conditions met, go to step 9.
 *
 * 5. Conditions for case d):
 *     - The segment initialization button is released.
 *     - The boot flag in the persistent memory shall contain anything else than 0xAA and 0xBB.
 *     If all conditions met, go to step 6.
 *
 * 6. Send a "new node online/probe" event, so that the programming tool knows the node is available now.
 *     Node id 0xFE is used!
 *
 * 7. If any "ACK probe" event is received, the node shall go to sleep mode, because the
 *     nickname is already used.
 *
 * 8. Wait (infinite) for the "enter bootloader" event. Only if the received nickname
 *     and GUID are equal to the internal ones of the node, validate the proposed programming
 *     algorithm.
 *     If the algorithm is supported, continue with next step, otherwise a
 *     "NACK enter bootloader" event will be sent.
 *
 * 9. Because the application already received the "enter bootloader" event and validated its
 *     parameters, a "ACK enter bootloader" event shall be sent back now in the bootloader.
 *     Send the flash block size (= flash page size) and the number of blocks as parameters.
 *
 * 10. Wait (infinite) for the "start block data transfer" event. If the proposed memory type
 *     is correct acknowledge it via "ACK start block data transfer" event otherwise via
 *     "NACK start block data transfer" event.
 *
 * 11. Now several "block data" events will be received, until the complete block is transfered.
 *     Every received block data shall be written to RAM buffer. After the last data is transfered,
 *     a CRC16-CCITT shall be calculated over the whole block. And an "ACK block data" event shall
 *     be sent with the calculated CRC as parameter.
 *
 * 12. If a complete block is received, wait for a "program block" event. This is the
 *     trigger to delete a flash page and write the block RAM buffer to the flash page.
 *     If the block is successful written, send an "ACK program block" event to confirm it.
 *     Otherwise send an "NAK program block" event.
 *     Note that before writing the first block to flash, write 0xFF to boot flag in the persistent memory.
 *     This shall be done here, because after this, there is no way back.
 *
 * 13. The next block shall be handled (go to step 10), except an "drop nickname/reset device" event
 *     is received. In this case the block programming operation shall be stopped.
 *
 * 14. Validate the application in the flash.
 *     If the application is valid, a "ACK activate new image" event shall be sent, the boot flag
 *     in the persistent memory shall be set to 0xAA and reboot the node.
 *     Otherwise "NAK activate new image" event shall be sent and the node shall go to sleep.
 *
 * Notes:
 * - Unused bytes in a flash block shall be 0xFF.
 * - The host shall send always an image in the size of the whole application section.
 * - Because the AVR has flash pages, but the VSCP has blocks, both are considered as equal.
 * - If the programming procedure is aborted by "drop nickname/reset device" event
 *   the node reboots.
 *
 * Error codes:
 * 0   - Programming algorithm not supported
 * 1   - Memory type not supported
 * 2   - Bad block number
 * 3   - Invalid message
 *
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __VSCP_BOOTLOADER_H__
#define __VSCP_BOOTLOADER_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_types.h"

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

/** Bootloader algorithm id, which is send to the programming tool. The
 * programming tool determines with that id how the programming procedure
 * shall take place.
 */
#define VSCP_BOOTLOADER_ALGORITHM   (0x00)

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
 * This function initializes the module.
 */
extern void vscp_bootloader_init(void);

/**
 * This function runs the bootloader algorithm and will never return!
 */
extern void vscp_bootloader_run(void);

#ifdef __cplusplus
}
#endif

#endif  /* __VSCP_BOOTLOADER_H__ */

/** @} */
