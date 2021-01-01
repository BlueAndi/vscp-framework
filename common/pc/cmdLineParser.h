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
@brief  Command line parser
@file   cmdLineParser.h
@author Andreas Merkle, http://www.blue-andi.de

@section desc Description
This module provides a command line parser.

*******************************************************************************/
/** @defgroup cmdLineParser Command line parser
 * Functionality of a command line parser.
 * @{
 */

/*
 * Don't forget to set JAVADOC_AUTOBRIEF to YES in the doxygen file to generate
 * a correct module description.
 */

#ifndef __CMDLINEPARSER_H__
#define __CMDLINEPARSER_H__

/*******************************************************************************
	INCLUDES
*******************************************************************************/
#include <stdint.h>
#include "platform.h"

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

/** Use this define to get the program name as given by the command line argument. */
#define CMDLINEPARSER_PROG_NAME_STD "CLP-PN-STD"

/** Use this define to get the program name without path. */
#define CMDLINEPARSER_PROG_NAME_WP  "CLP-PN-WP"

/** Use this define to get every unknown command line argument. */
#define CMDLINEPARSER_UNKONWN       "CLP-UNKNOWN"

/*******************************************************************************
	MACROS
*******************************************************************************/

/*******************************************************************************
	TYPES AND STRUCTURES
*******************************************************************************/

/** This type defines the error handler callback. */
typedef void (*cmdLineParser_ErrorHandler)(int index, char const * const arg);

/** This type defines the possible function return values. */
typedef enum
{
    CMDLINEPARSER_RET_OK = 0,   /**< Successful */
    CMDLINEPARSER_RET_ENULL,    /**< Unexpected NULL pointer */
    CMDLINEPARSER_RET_ERROR     /**< Failed */

} CMDLINEPARSER_RET;

/** This type defines a command line argument handler. */
typedef CMDLINEPARSER_RET (*cmdLineParser_Handler)(void* const userData, char const * const arg, char const * const par);

/** This type defines a single command line argument. */
typedef struct
{
    char const *            name;           /**< Argument name */
    char const ** const     strPar;         /**< String parameter */
    BOOL * const            boolPar;        /**< Boolean parameter */
    cmdLineParser_Handler   handler;        /**< Argument handler */
    void*                   userData;       /**< User data */
    char const * const      description;    /**< Description of the argument */

} cmdLineParser_Arg;

/*******************************************************************************
	VARIABLES
*******************************************************************************/

/*******************************************************************************
	FUNCTIONS
*******************************************************************************/

/**
 * This function intializes the module.
 */
extern void cmdLineParser_init(void);

/**
 * This function parses the command line arguments and handle them accordingly
 * to the given parser configuration.
 *
 * @param[in]   config  Parser configuration
 * @param[in]   num     Number of configuration elements
 * @param[in]   argc    Number of command line arguments
 * @param[in]   argv    Command line argument vector
 *
 * @return Status
 */
extern CMDLINEPARSER_RET cmdLineParser_parse(cmdLineParser_Arg const * const config, uint32_t num, int argc, char* argv[]);

/**
 * This function shows all possible arguments with its description.
 *
 * @param[in]   config  Parser configuration
 * @param[in]   num     Number of configuration elements
 */
extern void cmdLineParser_show(cmdLineParser_Arg const * const config, uint32_t num);

#ifdef __cplusplus
}
#endif

#endif	/* __CMDLINEPARSER_H__ */

/** @} */
