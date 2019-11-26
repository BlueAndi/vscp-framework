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
@brief  Multimedia
@file   vscp_evt_multimedia.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Dedicated class for multimedia functionality. This class was introduced to supplement the control
class and to offer multimedia specific control events.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_EVT_MULTIMEDIA_H__
#define __VSCP_EVT_MULTIMEDIA_H__

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
extern BOOL vscp_evt_protocol_sendGeneralEvent(void);

/**
 * Playback
 * 
 * @param[in] data0 Function (See below)
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendPlayback(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * NavigatorKey English
 * 
 * @param[in] data0 Function
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendNavigatorkeyEnglish(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Contrast
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific contrast level to set. A value
 * between 128 and 159 is change down by the specified number of contrast levels. A value between 160
 * and 191 is change up by the specified number of contrast levels. A value of 255 means that this is
 * and extended event and that the specific contrast level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustContrast(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Focus
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific focus level to set. A value
 * between 128 and 159 is change down by the specified number of focus levels. A value between 160 and
 * 191 is change up by the specified number of focus levels. A value of 255 means that this is and
 * extended event and that the specific focus level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustFocus(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Tint
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific tint level to set. A value
 * between 128 and 159 is change down by the specified number of tint levels. A value between 160 and
 * 191 is change up by the specified number of tint levels. A value of 255 means that this is and
 * extended event and that the specific tint level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustTint(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Color Balance
 * 
 * @param[in] data0 Reserved.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustColorBalance(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Brightness
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific brightness level to set. A value
 * between 128 and 159 is change down by the specified number of brightness levels. A value between
 * 160 and 191 is change up by the specified number of brightness levels. A value of 255 means that
 * this is and extended event and that the specific brightness level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustBrightness(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Hue
 * 
 * @param[in] data0 Reserved.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustHue(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Bass
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific bass level to set. A value
 * between 128 and 159 is change down by the specified number of bass levels. A value between 160 and
 * 191 is change up by the specified number of bass levels. A value of 255 means that this is and
 * extended event and that the specific bass level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustBass(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Treble
 * 
 * @param[in] data0 0 A value between 0 and 127 indicates the specific treble level to set. A value
 * between 128 and 159 is change down by the specified number of treble levels. A value between 160
 * and 191 is change up by the specified number of treble levels. A value of 255 means that this is
 * and extended event and that the specific treble level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustTreble(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Master Volume
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific volume level to set. A value
 * between 128 and 159 is change down by the specified number of volume levels. A value between 160
 * and 191 is change up by the specified number of volume levels. A value of 255 means that this is
 * and extended event and that the specific volume level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustMasterVolume(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Front Volume
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific volume level to set. A value
 * between 128 and 159 is change down by the specified number of volume levels. A value between 160
 * and 191 is change up by the specified number of volume levels. A value of 255 means that this is
 * and extended event and that the specific volume level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustFrontVolume(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Center Volume
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific volume level to set. A value
 * between 128 and 159 is change down by the specified number of volume levels. A value between 160
 * and 191 is change up by the specified number of volume levels. A value of 255 means that this is
 * and extended event and that the specific volume level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustCenterVolume(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Rear Volume
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific volume level to set. A value
 * between 128 and 159 is change down by the specified number of volume levels. A value between 160
 * and 191 is change up by the specified number of volume levels. A value of 255 means that this is
 * and extended event and that the specific volume level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustRearVolume(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Adjust Side Volume
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific volume level to set. A value
 * between 128 and 159 is change down by the specified number of volume levels. A value between 160
 * and 191 is change up by the specified number of volume levels. A value of 255 means that this is
 * and extended event and that the specific volume level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendAdjustSideVolume(uint8_t data0, uint8_t zone, uint8_t subZone);

/* "Reserved" not supported. No frame defined. */

/* "Reserved" not supported. No frame defined. */

/* "Reserved" not supported. No frame defined. */

/* "Reserved" not supported. No frame defined. */

/**
 * Select Disk
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific disk number. A value between 128
 * and 159 is change down by the specified number of disks. A value between 160 and 191 is change up
 * by the specified number of disks. A value of 200 means select a random disk. A value of 255 means
 * that this is and extended event and that the disk number is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSelectDisk(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Select Track
 * 
 * @param[in] data0 A value between 0 and 127 indicates the track number. A value between 128 and 159
 * is change down by the specified number of tracks. A value between 160 and 191 is change up by the
 * specified number of tracks. A value of 200 means select a random track. A value of 255 means that
 * this is and extended event and that the track number is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSelectTrack(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Select Album/Play list
 * 
 * @param[in] data0 A value between 0 and 127 indicates the album/play-list number. A value between
 * 128 and 159 is change down by the specified number of albums/play-lists. A value between 160 and
 * 191 is change up by the specified number of albums. A value of 200 means select a random album. A
 * value of 255 means that this is and extended event and that the album number is sent in byte 3 and
 * after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSelectAlbumPlayList(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Select Channel
 * 
 * @param[in] data0 A value between 0 and 127 indicates the channel number. A value between 128 and
 * 159 is change down by the specified number of channels. A value between 160 and 191 is change up by
 * the specified number of channels. A value of 200 means select a random channel. A value of 255
 * means that this is and extended event and that the channel number is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSelectChannel(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Select Page
 * 
 * @param[in] data0 A value between 0 and 127 indicates the page number. A value between 128 and 159
 * is change down by the specified number of pages. A value between 160 and 191 is change up by the
 * specified number of pages. A value of 200 means select a random page. A value of 255 means that
 * this is and extended event and that the page number is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSelectPage(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Select Chapter
 * 
 * @param[in] data0 A value between 0 and 127 indicates the chapter number. A value between 128 and
 * 159 is change down by the specified number of chapters. A value between 160 and 191 is change up by
 * the specified number of chapters. A value of 200 means select a random chapter. A value of 255
 * means that this is and extended event and that the chapter number is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSelectChapter(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Select Screen Format
 * 
 * @param[in] data0 0 = Auto, 1 = Just, 2 = Norma,l 3 = Zoom.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSelectScreenFormat(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Select Input Source
 * 
 * @param[in] data0 Device code
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSelectInputSource(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Select Output
 * 
 * @param[in] data0 Output Code
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSelectOutput(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Record
 * 
 * @param[in] data0 0 - Start to record, 1 - Stop record, 2 - Disable, AGC 3 - Enable AGC.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendRecord(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Set Recording Volume
 * 
 * @param[in] data0 A value between 0 and 127 indicates the specific contrast level to set. A value
 * between 128 and 159 is change down by the specified number of contrast levels. A value between 160
 * and 191 is change up by the specified number of contrast levels. A value of 255 means that this is
 * and extended event and that the specific contrast level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSetRecordingVolume(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Tivo Function
 * 
 * @param[in] data0 TIVO Code
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendTivoFunction(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Get Current Title
 * 
 * @param[in] data0 Reserved.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendGetCurrentTitle(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Set media position in milliseconds
 * 
 * @param[in] data0 Reserved
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data3 Position in milliseconds, This is an integer with a size specified by the event
 * size. This 0xFF, 0xFFFF, 0xFFFFFF, 0xFFFFFFFF and 0xFFFFFFFFFF is the maximum that can be sent for
 * different sizes. (array[2])
 * @param[in] data3size Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSetMediaPositionInMilliseconds(uint8_t data0, uint8_t zone, uint8_t subZone, uint8_t const * const data3, uint8_t data3Size);

/**
 * Get media information
 * 
 * @param[in] data0 Type of media information requested. 1 - Current Title, 1 - Get Folders, 2 - Get
 * Disks, 3 - Get Tracks, 4 - Get Albums/Play list,s 5 - Get Channels, 6 - Get Pages, 7 - Get Chapters
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendGetMediaInformation(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Remove Item from Album
 * 
 * @param[in] data0 0-128 - Pos to remove from album/play-list A value of 255 means that this is and
 * extended event and that the specific contrast level is sent in byte 3 and after.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendRemoveItemFromAlbum(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Remove all Items from Album
 * 
 * @param[in] data0 Reserved.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendRemoveAllItemsFromAlbum(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Save Album/Play list
 * 
 * @param[in] data0 0 - Do not overwrite if it already exists 1 - Overwrite if it exist.
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendSaveAlbumPlayList(uint8_t data0, uint8_t zone, uint8_t subZone);

/**
 * Multimedia Control
 * 
 * @param[in] data0 Control codes
 * @param[in] zone Zone for which event applies to (0-255). 255 is all zones.
 * @param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.
 * @param[in] data3 Index. Base 0. Increase by one for every fragment of the title sent.
 * @param[in] data4 Data. (array[2])
 * @param[in] data4size Size in byte.
 * 
 * @return If event is sent, it will return TRUE otherwise FALSE.
 */
extern BOOL vscp_evt_protocol_sendMultimediaControl(uint8_t data0, uint8_t zone, uint8_t subZone, uint8_t data3, uint8_t const * const data4, uint8_t data4Size);

/* "Multimedia Control response" not supported. No frame defined. */

#endif	/* __VSCP_EVT_MULTIMEDIA_H__ */
