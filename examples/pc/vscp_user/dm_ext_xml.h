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
@brief  Extended decision matrix XML parser
@file   dm_ext_xml.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains an xml parser for a extended decision matrix format.

*******************************************************************************/
/** @defgroup dm_ext_xml Extended decision matrix XML parser
 * Parse a extended decision matrix XML format.
 *
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __DM_EXT_XML_H__
#define __DM_EXT_XML_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "vscp_dm.h"

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

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

/** This type defines the supported function return values. */
typedef enum
{
    DM_EXT_XML_RET_OK = 0,  /**< Successful */
    DM_EXT_XML_RET_ERROR,   /**< Failed */
    DM_EXT_XML_RET_ENULL,   /**< Unexpected NULL pointer */
    DM_EXT_XML_RET_EFILE    /**< File not found */

} DM_EXT_XML_RET;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function initializes this module.
 */
extern void dm_ext_xml_init(void);

/**
 * This function loads the content of a extended decision matrix xml file to the
 * specified memory locations.
 *
 * @param[in]   fileName    Extended decision matrix xml file name
 * @param[out]  dm          Decision matrix storage
 * @param[out]  ext         Decision matrix extension storage
 * @param[in]   rows        Max. number of decision matrix rows in the storage
 * @return Status
 */
extern DM_EXT_XML_RET dm_ext_xml_load(char const * const fileName, vscp_dm_MatrixRow * const dm, vscp_dm_ExtRow * const ext, uint8_t rows);

#ifdef __cplusplus
}
#endif

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

#endif  /* __DM_EXT_XML_H__ */

/** @} */
