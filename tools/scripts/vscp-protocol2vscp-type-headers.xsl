<?xml version="1.0" encoding="UTF-8"?>

<!--

The MIT License (MIT)

Copyright (c) 2014 - 2019, Andreas Merkle
http://www.blue-andi.de
vscp@blue-andi.de

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

********************************************************************************
This transformation script generates the VSCP type C header files.

********************************************************************************
-->

<!DOCTYPE stylesheet [
    <!ENTITY LF  "&#xA;" >
    <!ENTITY TAB "&#x9;" >
]>

<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xs="http://www.w3.org/2001/XMLSchema"
    xmlns:fn="http://www.w3.org/2005/xpath-functions"
    xpath-default-namespace="http://www.blue-andi.de/vscp/xml-schema/vscp-protocol/1.0"
    version="2.0">

    <!--
    ****************************************************************************
        INCLUDES
    ****************************************************************************
    -->
    <xsl:include href="../xslt/utilities/ctools.xsl"/>

    <!--
    ****************************************************************************
        OUTPUT CONFIGURATION
    ****************************************************************************
    -->
    <xsl:output method="text" encoding="UTF-8" omit-xml-declaration="yes" indent="no" />

    <!--
    ****************************************************************************
        GLOBAL VARIABLES
    ****************************************************************************
    -->
    <xsl:variable name="global.author">Andreas Merkle, http://www.blue-andi.de</xsl:variable>

    <!--
    ****************************************************************************
        ROOT TEMPLATE
    ****************************************************************************
    -->
    <xsl:template match="/">

        <xsl:apply-templates select="specification/vscp-classes/vscp-class" />

    </xsl:template>

    <!--
    ****************************************************************************
        HEADER FILE
    ****************************************************************************
    -->

    <!-- Create one header file per VSCP class. -->
    <xsl:template match="vscp-class">
        <xsl:choose>
            <xsl:when test="@id = 0">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_protocol</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 1">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_alarm_protocol</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 2">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_security_protocol</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:text>No header for </xsl:text>
                <xsl:value-of select="@id" />
                <xsl:text> created.&LF;</xsl:text>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Create header file. -->
    <xsl:template name="createFile">
        <xsl:param name="moduleName" />

        <xsl:text>Generate </xsl:text>
        <xsl:value-of select="$moduleName" />
        <xsl:text>.h&LF;</xsl:text>

        <xsl:result-document href="{$moduleName}.h">
            <!-- Header -->
            <xsl:call-template name="ctools.hHeaderBlock">
                <xsl:with-param name="license">
                <xsl:text>/* The MIT License (MIT)
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

</xsl:text>
                </xsl:with-param>
                <xsl:with-param name="briefDesc">
                    <xsl:value-of select="name[@lang='en']" />
                </xsl:with-param>
                <xsl:with-param name="moduleName">
                    <xsl:value-of select="$moduleName" />
                </xsl:with-param>
                <xsl:with-param name="author">
                    <xsl:value-of select="$global.author" />
                </xsl:with-param>
                <xsl:with-param name="description">
                    <xsl:value-of select="description[@lang='en']" />
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>

            <!-- Includes -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>INCLUDES</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>#include &lt;stdint.h&gt;&LF;</xsl:text>
            <xsl:text>&LF;</xsl:text>

            <!-- Compiler switches -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>COMPILER SWITCHES</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>

            <!-- Constants -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>CONSTANTS</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>
            <xsl:apply-templates select="vscp-types/vscp-type" />

            <!-- Macros -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>MACROS</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>

            <!-- Types and structures -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>TYPES AND STRUCTURES</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>

            <!-- Global variables -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>VARIABLES</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>

            <!-- Functions -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>FUNCTIONS</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>

            <!-- Footer -->
            <xsl:call-template name="ctools.hFooterBlock">
                <xsl:with-param name="moduleName">
                    <xsl:value-of select="$moduleName" />
                </xsl:with-param>
            </xsl:call-template>

        </xsl:result-document>
    </xsl:template>

    <xsl:template match="vscp-type">
        <xsl:call-template name="ctools.define">
            <xsl:with-param name="comment">
                <xsl:value-of select="description[@lang='en']" />
            </xsl:with-param>
            <xsl:with-param name="name">
                <xsl:value-of select="token" />
            </xsl:with-param>
            <xsl:with-param name="value">
                <xsl:value-of select="@id" />
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>

    <!-- Garbage collector -->
    <xsl:template match="text()">
    </xsl:template>

</xsl:stylesheet>
