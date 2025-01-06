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
@brief  Multimedia
@file   vscp_type_multimedia.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
Dedicated class for multimedia functionality. This class was introduced to supplement the control
class and to offer multimedia specific control events.

This file is automatically generated. Don't change it manually.

*******************************************************************************/

#ifndef __VSCP_TYPE_MULTIMEDIA_H__
#define __VSCP_TYPE_MULTIMEDIA_H__

/** @defgroup vscp_type_multimedia Multimedia
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
#define VSCP_TYPE_MULTIMEDIA_GENERAL                     0

/**
 * This is for controlling playback functionality
 */
#define VSCP_TYPE_MULTIMEDIA_PLAYBACK                    1

/**
 * This is typically for navigation functions or DVD controls
 */
#define VSCP_TYPE_MULTIMEDIA_NAVIGATOR_KEY_ENG           2

/**
 * This is typically for adjusting the contrast level of a display device
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_CONTRAST             3

/**
 * This is typically for adjusting the focus settings of a display device
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_FOCUS                4

/**
 * This is typically for adjusting the tint settings of a display device
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_TINT                 5

/**
 * This is typically for adjusting the color balance settings of a display device.
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_COLOUR_BALANCE       6

/**
 * This is typically for adjusting the tint settings of a display device
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_BRIGHTNESS           7

/**
 * This is typically for adjusting the hue settings of a display device
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_HUE                  8

/**
 * This is typically for adjusting the bass level settings of a sound device. Depending on the
 * implementation, this could automatically adjust the treble level. To adjust left and right bass
 * levels, a node would have to use separate zones or sub-zones for left and right.
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_BASS                 9

/**
 * This is typically for adjusting the treble level settings of a sound device. Depending on the
 * implementation, this could automatically adjust the bass level. To adjust left and right treble
 * levels, a node would have to use separate zones or sub-zones for left and right.
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_TREBLE               10

/**
 * This is typically for adjusting the master volume level. This could be used for adjusting the level
 * for all speakers.
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_MASTER_VOLUME        11

/**
 * This is typically for adjusting the front speaker volume level. This usually means the two front
 * speakers as opposed to the single center speaker.
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_FRONT_VOLUME         12

/**
 * This is typically for adjusting the front speaker volume level. This usually means the single
 * center speaker as opposed to the two front speakers.
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_CENTRE_VOLUME        13

/**
 * This is typically for adjusting the rear speaker volume level.
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_REAR_VOLUME          14

/**
 * This is typically for adjusting the side speaker volume level.
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SIDE_VOLUME          15

/**
 * These are reserved for other future speaker combinations
 */
#define VSCP_TYPE_MULTIMEDIA_RESERVED16                  16

/**
 * These are reserved for other future speaker combinations
 */
#define VSCP_TYPE_MULTIMEDIA_RESERVED17                  17

/**
 * These are reserved for other future speaker combinations
 */
#define VSCP_TYPE_MULTIMEDIA_RESERVED18                  18

/**
 * These are reserved for other future speaker combinations
 */
#define VSCP_TYPE_MULTIMEDIA_RESERVED19                  19

/**
 * This is typically for selecting a disk for playback
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SELECT_DISK          20

/**
 * This is typically for selecting a track for playback
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SELECT_TRACK         21

/**
 * This is typically for selecting an album or play-list for playback
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SELECT_ALBUM         22

/**
 * This is typically for selecting a TV Channel
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SELECT_CHANNEL       23

/**
 * This is typically for selecting a page of a film
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SELECT_PAGE          24

/**
 * This is typically for selecting a chapter of a film
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SELECT_CHAPTER       25

/**
 * This is for controlling screen format of a display device
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SELECT_SCREEN_FORMAT 26

/**
 * This is for controlling the input source of a playback device
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SELECT_INPUT_SOURCE  27

/**
 * This is for controlling the output of a playback device
 */
#define VSCP_TYPE_MULTIMEDIA_ADJUST_SELECT_OUTPUT        28

/**
 * Control a recording device.
 */
#define VSCP_TYPE_MULTIMEDIA_RECORD                      29

/**
 * Control a recording device.
 */
#define VSCP_TYPE_MULTIMEDIA_SET_RECORDING_VOLUME        30

/**
 * This is typically for accessing TIVO functions
 */
#define VSCP_TYPE_MULTIMEDIA_TIVO_FUNCTION               40

/**
 * Get the title for the current active media.
 */
#define VSCP_TYPE_MULTIMEDIA_GET_CURRENT_TITLE           50

/**
 * This is for controlling the position in the stream/file of a playback device
 */
#define VSCP_TYPE_MULTIMEDIA_SET_POSITION                51

/**
 * Get various media information from a device.
 */
#define VSCP_TYPE_MULTIMEDIA_GET_MEDIA_INFO              52

/**
 * Remove an item from an album.
 */
#define VSCP_TYPE_MULTIMEDIA_REMOVE_ITEM                 53

/**
 * Remove all items from an album.
 */
#define VSCP_TYPE_MULTIMEDIA_REMOVE_ALL_ITEMS            54

/**
 * Save album/play-list to permanent storage.
 */
#define VSCP_TYPE_MULTIMEDIA_SAVE_ALBUM                  55

/**
 * Send multimedia information. This can be the title for the current active media. It can be sent as
 * a response to a “Get Title” or similar event or by its own when a new title is playing or other
 * multimedia information has changed.
 * Response should be Type=61
 */
#define VSCP_TYPE_MULTIMEDIA_CONTROL                     60

/**
 * Response for multimedia control.
 */
#define VSCP_TYPE_MULTIMEDIA_CONTROL_RESPONSE            61

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

#endif  /* __VSCP_TYPE_MULTIMEDIA_H__ */

/** @} */
