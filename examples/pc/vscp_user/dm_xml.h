/* The MIT License (MIT)
 * 
 * Copyright (c) 2014 - 2015, Andreas Merkle
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
@brief  Decision matrix XML loader
@file   dm_xml.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module contains function to load a standard decision matrix, a extended
decision matrix and a decision matrix next generation from a xml file.

@section svn Subversion
$Author: amerkle $
$Rev: 449 $
$Date: 2015-01-05 20:23:52 +0100 (Mo, 05 Jan 2015) $
*******************************************************************************/
/** @defgroup dm_xml Decision matrix XML loader
 * Load a standard decision matrix, a extended decision matrix or a decision
 * matrix next generation from a xml file.
 *
 * @{
 */
 
/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __DM_XML_H__
#define __DM_XML_H__

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <inttypes.h>
#include "vscp_dm.h"

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
    DM_XML_RET_OK = 0,  /**< Successful */
    DM_XML_RET_ERROR,   /**< Failed */
    DM_XML_RET_ENULL,   /**< Unexpected NULL pointer */
    DM_XML_RET_EFILE    /**< File not found */

} DM_XML_RET;

/*******************************************************************************
    VARIABLES
*******************************************************************************/

/*******************************************************************************
    FUNCTIONS
*******************************************************************************/

/**
 * This function initializes this module.
 */
extern void dm_xml_init(void);

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM )

/**
 * This function loads the content of a standard decision matrix xml file to the
 * specified memory location.
 *
 * @param[in]   fileName    Standard decision matrix xml file name
 * @param[out]  dm          Decision matrix storage
 * @param[in]   rows        Max. number of decision matrix rows in the storage
 * @return Status
 */
extern DM_XML_RET dm_xml_loadStd(char const * const fileName, vscp_dm_MatrixRow * const dm, uint8_t rows);

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM ) */

#if VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION )

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
extern DM_XML_RET dm_xml_loadExt(char const * const fileName, vscp_dm_MatrixRow * const dm, vscp_dm_ExtRow * const ext, uint8_t rows);

#endif  /* VSCP_CONFIG_BASE_IS_ENABLED( VSCP_CONFIG_ENABLE_DM_EXTENSION ) */

/**
 * This function loads the content of a decision matrix next generation xml file to the
 * specified memory location.
 *
 * @param[in]   fileName    Eecision matrix next generation xml file name
 * @param[out]  storage     Decision matrix next generation storage
 * @param[in]   size        Decision matrix next generation storage size in bytes
 * @return Status
 */
extern DM_XML_RET dm_xml_loadNG(char const * const fileName, uint8_t* storage, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif  /* __DM_XML_H__ */

/** @} */
