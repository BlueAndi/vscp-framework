<?xml version="1.0" encoding="UTF-8"?>

<!--

The MIT License (MIT)

Copyright (c) 2014 - 2020, Andreas Merkle
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
    <xsl:variable name="global.license" as="xs:string" select="unparsed-text('../xslt/utilities/license.txt', 'utf-8')" />

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
                    <xsl:with-param name="moduleName">vscp_type_alarm</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 2">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_security</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 10">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_measurement</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 15">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_data</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 20">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_information</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 30">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_control</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 40">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_multimedia</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 50">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_aol</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 60">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_measurement64</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 65">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_measurezone</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 70">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_measurement32</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 85">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_setvaluezone</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 90">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_weather</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 95">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_weather_forecast</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 100">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_phone</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 102">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_display</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 110">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_ir</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 206">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_gnss</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 212">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_wireless</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 506">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_diagnostic</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 508">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_error</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 509">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="moduleName">vscp_type_log</xsl:with-param>
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
                    <xsl:value-of select="$global.license" />
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

            <!-- Doxygen comment block -->
            <xsl:text>/** @defgroup </xsl:text><xsl:value-of select="$moduleName" /><xsl:text> </xsl:text><xsl:value-of select="name[@lang='en']" /><xsl:text>&LF;</xsl:text>
            <xsl:text> * Level 1 protocol class types&LF;</xsl:text>
            <xsl:text> * @{&LF;</xsl:text>
            <xsl:text> * @ingroup vscp_l1&LF;</xsl:text>
            <xsl:text> */&LF;</xsl:text>
            <xsl:text>&LF;</xsl:text>

            <!-- Includes -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>INCLUDES</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>

            <xsl:text>#ifdef __cplusplus&LF;</xsl:text>
            <xsl:text>extern "C"&LF;</xsl:text>
            <xsl:text>{&LF;</xsl:text>
            <xsl:text>#endif&LF;</xsl:text>
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
            <xsl:apply-templates select="vscp-types/vscp-type[token]">
                <xsl:with-param name="maxLen">
                    <xsl:for-each select="vscp-types/vscp-type">
                        <xsl:sort select="string-length(token)" order="descending" data-type="number"/>
                        <xsl:if test="position() = 1">
                            <xsl:value-of select="string-length(token)"/>
                        </xsl:if>
                    </xsl:for-each>
                </xsl:with-param> 
            </xsl:apply-templates>

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

            <xsl:text>#ifdef __cplusplus&LF;</xsl:text>
            <xsl:text>}&LF;</xsl:text>
            <xsl:text>#endif&LF;</xsl:text>
            <xsl:text>&LF;</xsl:text>

            <!-- Footer -->
            <xsl:call-template name="ctools.hFooterBlock">
                <xsl:with-param name="moduleName">
                    <xsl:value-of select="$moduleName" />
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>

            <xsl:text>/** @} */&LF;</xsl:text>

        </xsl:result-document>
    </xsl:template>

    <!-- Create a #define for each vscp-type  -->
    <xsl:template match="vscp-type">
        <xsl:param name="maxLen" />
        <xsl:call-template name="ctools.define">
            <xsl:with-param name="comment">
                <xsl:analyze-string select="description[@lang='en']" regex="\\n">
                    <xsl:matching-substring>
                        <xsl:text>&LF;</xsl:text>
                    </xsl:matching-substring>
                    <xsl:non-matching-substring>
                        <xsl:value-of select="." />
                    </xsl:non-matching-substring>
                </xsl:analyze-string>
            </xsl:with-param>
            <xsl:with-param name="name">
                <xsl:value-of select="token" />
            </xsl:with-param>
            <xsl:with-param name="value">
                <xsl:call-template name="addSpaces">
                    <xsl:with-param name="count">
                        <xsl:value-of select="$maxLen - string-length(token)" />
                    </xsl:with-param>
                </xsl:call-template>
                <xsl:value-of select="@id" />
            </xsl:with-param>
        </xsl:call-template>
        <xsl:text>&LF;</xsl:text>
    </xsl:template>

    <xsl:template name="addSpaces">
        <xsl:param name="count" />
        <xsl:if test="$count &gt; 0">
            <xsl:text> </xsl:text>
            <xsl:call-template name="addSpaces">
                <xsl:with-param name="count" select="$count - 1" />
            </xsl:call-template>
        </xsl:if>
    </xsl:template>

    <!-- Garbage collector -->
    <xsl:template match="text()">
    </xsl:template>

</xsl:stylesheet>
