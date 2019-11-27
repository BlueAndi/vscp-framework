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
This transformation script generates the VSCP event modules.

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
    xmlns:local="http://local"
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
        FUNCTIONS
    ****************************************************************************
    -->
    <xsl:function name="local:normalize">
        <xsl:param name="string" />
        <xsl:value-of select="normalize-space(replace($string, '[^a-zA-Z0-9]', ' '))" />
    </xsl:function>

    <xsl:function name="local:hungarianForm">
        <xsl:param name="string" />
        <xsl:call-template name="ctools.hungarianForm">
            <xsl:with-param name="string">
                <xsl:value-of select="local:normalize($string)" />
            </xsl:with-param>
        </xsl:call-template>
    </xsl:function>

    <xsl:function name="local:getVscpClassId">
        <xsl:param name="frameRef" />
        <xsl:analyze-string select="$frameRef" regex="specification/vscp-classes/([0-9]*).*">
            <xsl:matching-substring>
                <xsl:value-of select="regex-group(1)" />
            </xsl:matching-substring>
        </xsl:analyze-string>
    </xsl:function>

    <xsl:function name="local:getVscpTypeId">
        <xsl:param name="frameRef" />
        <xsl:analyze-string select="$frameRef" regex="specification/vscp-classes/[0-9]*/vscp-types/([0-9]*).*">
            <xsl:matching-substring>
                <xsl:value-of select="regex-group(1)" />
            </xsl:matching-substring>
        </xsl:analyze-string>
    </xsl:function>

    <!--
    ****************************************************************************
        ROOT TEMPLATE
    ****************************************************************************
    -->
    <xsl:template match="/">

        <xsl:apply-templates select="specification/vscp-classes/vscp-class" />

    </xsl:template>

    <!-- Create a C module per VSCP class. -->
    <xsl:template match="vscp-class">
        <xsl:choose>
            <xsl:when test="@id = 0">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">protocol</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 1">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">alarm</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 2">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">security</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 10">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">measurement</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 15">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">data</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 20">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">information</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 30">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">control</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 40">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">multimedia</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 50">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">aol</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 60">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">measurement64</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 65">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">measurezone</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 70">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">measurement32</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 85">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">setvaluezone</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 90">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">weather</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 95">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">weather_forecast</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 100">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">phone</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 102">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">display</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 110">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">ir</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 206">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">gnss</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 212">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">wireless</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 506">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">diagnostic</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 508">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">error</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="@id = 509">
                <xsl:call-template name="createFile">
                    <xsl:with-param name="baseName">log</xsl:with-param>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:text>No header for </xsl:text>
                <xsl:value-of select="@id" />
                <xsl:text> created.&LF;</xsl:text>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Create module files -->
    <xsl:template name="createFile">
        <xsl:param name="baseName" />

        <xsl:call-template name="createHeaderFile">
            <xsl:with-param name="baseName">
                <xsl:value-of select="$baseName" />
            </xsl:with-param>
        </xsl:call-template>

        <xsl:call-template name="createSourceFile">
            <xsl:with-param name="baseName">
                <xsl:value-of select="$baseName" />
            </xsl:with-param>
        </xsl:call-template>
    </xsl:template>

    <!--
    ****************************************************************************
        HEADER FILE
    ****************************************************************************
    -->

    <!-- Create header file. -->
    <xsl:template name="createHeaderFile">
        <xsl:param name="baseName" />

        <xsl:variable name="moduleName">
            <xsl:text>vscp_evt_</xsl:text>
            <xsl:value-of select="$baseName" />
        </xsl:variable>

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

            <!-- Includes -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>INCLUDES</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>#include &lt;stdint.h&gt;&LF;</xsl:text>
            <xsl:text>#include "vscp_platform.h"&LF;</xsl:text>
            <!-- Exception handling for special classes. -->
            <xsl:choose>
                <xsl:when test="@id = 60">
                    <xsl:text>#include &lt;math.h&gt;&LF;</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                </xsl:otherwise>
            </xsl:choose>
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
            <xsl:apply-templates select="vscp-types/vscp-type[token]" mode="h">
                <xsl:with-param name="baseName">
                    <xsl:value-of select="$baseName" />
                </xsl:with-param>
            </xsl:apply-templates>

            <!-- Footer -->
            <xsl:call-template name="ctools.hFooterBlock">
                <xsl:with-param name="moduleName">
                    <xsl:value-of select="$moduleName" />
                </xsl:with-param>
            </xsl:call-template>

        </xsl:result-document>
    </xsl:template>

    <!--
    ****************************************************************************
        SOURCE FILE
    ****************************************************************************
    -->

    <xsl:template name="createSourceFile">
        <xsl:param name="baseName" />

        <xsl:variable name="moduleName">
            <xsl:text>vscp_evt_</xsl:text>
            <xsl:value-of select="$baseName" />
        </xsl:variable>

        <xsl:text>Generate </xsl:text>
        <xsl:value-of select="$moduleName" />
        <xsl:text>.c&LF;</xsl:text>

        <xsl:result-document href="{$moduleName}.c">
            <!-- Header -->
            <xsl:call-template name="ctools.cHeaderBlock">
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
            </xsl:call-template>
            <xsl:text>&LF;</xsl:text>

            <!-- Includes -->
            <xsl:call-template name="ctools.generalBlock">
                <xsl:with-param name="name">
                    <xsl:text>INCLUDES</xsl:text>
                </xsl:with-param>
            </xsl:call-template>
            <xsl:text>#include "</xsl:text><xsl:value-of select="$moduleName" /><xsl:text>.h"&LF;</xsl:text>
            <xsl:text>#include "vscp_core.h"&LF;</xsl:text>
            <xsl:text>#include "vscp_class_l1.h"&LF;</xsl:text>
            <xsl:text>#include "vscp_type_</xsl:text>
            <xsl:value-of select="$baseName" />
            <xsl:text>.h"&LF;</xsl:text>
            <!-- Exception handling for special classes. -->
            <xsl:choose>
                <xsl:when test="@id = 10">
                    <xsl:text>#include "vscp_data_coding.h"&LF;</xsl:text>
                </xsl:when>
                <xsl:when test="@id = 15">
                    <xsl:text>#include "vscp_data_coding.h"&LF;</xsl:text>
                </xsl:when>
                <xsl:when test="@id = 65">
                    <xsl:text>#include "vscp_data_coding.h"&LF;</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                </xsl:otherwise>
            </xsl:choose>
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
            <xsl:apply-templates select="vscp-types/vscp-type[token]" mode="c">
                <xsl:with-param name="baseName">
                    <xsl:value-of select="$baseName" />
                </xsl:with-param>
            </xsl:apply-templates>

            <!-- Footer -->
            <xsl:call-template name="ctools.cFooterBlock">
            </xsl:call-template>

        </xsl:result-document>
    </xsl:template>

    <!--
    ****************************************************************************
        OTHER TEMPLATES
    ****************************************************************************
    -->

    <!-- Function comment -->
    <xsl:template name="funcComment">
        <xsl:param name="vscpClassId" />
        <xsl:param name="vscpTypeId" />

        <xsl:value-of select="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/name[@lang='en']" />
        <xsl:text>&LF;</xsl:text>

        <xsl:choose>

            <!-- Handle measurement class exceptional -->
            <xsl:when test="(../../@id = 10) and ($vscpTypeId &gt; 0)">
                <xsl:text>&LF;</xsl:text>
                <xsl:text>@param[in] index Index for sensor.&LF;</xsl:text>
                <xsl:text>@param[in] unit The unit of the data.&LF;</xsl:text>
                <xsl:text>@param[in] data The data as signed integer.&LF;</xsl:text>
                <xsl:text>@param[in] exp The exponent of the data (10^exponent).&LF;</xsl:text>
            </xsl:when>

            <!-- Handle data class exceptional -->
            <xsl:when test="(../../@id = 15) and ($vscpTypeId &gt; 0)">
                <xsl:text>&LF;</xsl:text>
                <xsl:text>@param[in] index Index for sensor.&LF;</xsl:text>
                <xsl:text>@param[in] unit The unit of the data.&LF;</xsl:text>
                <xsl:text>@param[in] data The data as signed integer.&LF;</xsl:text>
                <xsl:text>@param[in] exp The exponent of the data (10^exponent).&LF;</xsl:text>
            </xsl:when>

            <!-- Handle measurezone class exceptional -->
            <xsl:when test="(../../@id = 65) and ($vscpTypeId &gt; 0)">
                <xsl:text>&LF;</xsl:text>
                <xsl:text>@param[in] index Index for sensor.&LF;</xsl:text>
                <xsl:text>@param[in] zone Zone for which event applies to (0-255). 255 is all zones.&LF;</xsl:text>
                <xsl:text>@param[in] subZone Sub-zone for which event applies to (0-255). 255 is all sub-zones.&LF;</xsl:text>
                <xsl:text>@param[in] data The data as signed integer.&LF;</xsl:text>
                <xsl:text>@param[in] exp The exponent of the data (10^exponent).&LF;</xsl:text>
            </xsl:when>

            <xsl:otherwise>
                <!-- Any parameter? -->
                <xsl:if test="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element">
                    <xsl:text>&LF;</xsl:text>

                    <!-- Generate comment per parameter -->
                    <xsl:for-each select="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element">
                        <xsl:if test="position() &gt; 1">
                            <xsl:text>&LF;</xsl:text>
                        </xsl:if>
                        <xsl:text>@param[in] </xsl:text>
                        <xsl:value-of select="local:hungarianForm(name[@lang='en'])" />
                        <xsl:text> </xsl:text>
                        <xsl:value-of select="description[@lang='en']" />
                        <xsl:if test="@optional = 'true'">
                            <xsl:text> (optional)</xsl:text>
                        </xsl:if>
                        <xsl:if test="@length &gt; 1">
                            <xsl:text> (array[</xsl:text>
                            <xsl:value-of select="@length" />
                            <xsl:text>])</xsl:text>
                            <xsl:text>&LF;</xsl:text>
                            <xsl:text>@param[in] </xsl:text>
                            <xsl:value-of select="local:hungarianForm(concat(name[@lang='en'], 'size'))" />
                            <xsl:text> Size in byte.</xsl:text>
                        </xsl:if>
                    </xsl:for-each>
                    <xsl:text>&LF;</xsl:text>
                </xsl:if>
            </xsl:otherwise>

        </xsl:choose>

        <xsl:text>&LF;</xsl:text>
        <xsl:text>@return If event is sent, it will return TRUE otherwise FALSE.</xsl:text>
    </xsl:template>

    <!-- Function parameters -->
    <xsl:template name="funcParameter">
        <xsl:param name="vscpClassId" />
        <xsl:param name="vscpTypeId" />

        <xsl:choose>

            <!-- Handle measurement class exceptional -->
            <xsl:when test="(../../@id = 10) and ($vscpTypeId &gt; 0)">
                <xsl:text>uint8_t index, uint8_t unit, int32_t data, int8_t exp</xsl:text>
            </xsl:when>

            <!-- Handle data class exceptional -->
            <xsl:when test="(../../@id = 15) and ($vscpTypeId &gt; 0)">
                <xsl:text>uint8_t index, uint8_t unit, int32_t data, int8_t exp</xsl:text>
            </xsl:when>

            <!-- Handle measurezone class exceptional -->
            <xsl:when test="(../../@id = 65) and ($vscpTypeId &gt; 0)">
                <xsl:text>uint8_t index, uint8_t zone, uint8_t subZone, int32_t data, int8_t exp</xsl:text>
            </xsl:when>

            <xsl:otherwise>
                <xsl:choose>
                    <xsl:when test="(count(/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame) = 1) and /specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element">

                        <!-- Generate variable per parameter -->
                        <xsl:for-each select="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element">
                            <xsl:if test="position() &gt; 1">
                                <xsl:text>, </xsl:text>
                            </xsl:if>
                            <xsl:choose>
                                <xsl:when test="@length = 1">
                                    <xsl:value-of select="@type" />
                                    <xsl:text>_t</xsl:text>
                                    <xsl:if test="@optional = 'true'">
                                        <xsl:text> const * const</xsl:text>
                                    </xsl:if>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:value-of select="@type" />
                                    <xsl:text>_t const * const</xsl:text>
                                </xsl:otherwise>
                            </xsl:choose>
                            <xsl:text> </xsl:text>
                            <xsl:value-of select="local:hungarianForm(name[@lang='en'])" />
                            <xsl:if test="@length &gt; 1">
                                <xsl:text>, uint8_t </xsl:text>
                                <xsl:value-of select="local:hungarianForm(concat(name[@lang='en'], ' size'))" />
                            </xsl:if>
                        </xsl:for-each>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:text>void</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:otherwise>

        </xsl:choose>
    </xsl:template>

    <!-- Function body -->
    <xsl:template name="funcBody">
        <xsl:param name="vscpClassId" />
        <xsl:param name="vscpTypeId" />

        <xsl:choose>

            <!-- Handle measurement class exceptional -->
            <xsl:when test="(../../@id = 10) and ($vscpTypeId &gt; 0)">
                <!-- Function variables -->
                <xsl:text>&TAB;vscp_TxMessage&TAB;txMsg;&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>

                <!-- Prepare tx message -->
                <xsl:text>&TAB;vscp_core_prepareTxMessage(&amp;txMsg, </xsl:text>
                <xsl:value-of select="../../token" />
                <xsl:text>, </xsl:text>
                <xsl:value-of select="token" />
                <xsl:text>, VSCP_PRIORITY_3_NORMAL);&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>

                <xsl:text>&TAB;txMsg.dataNum = 1;&LF;</xsl:text>
                <xsl:text>&TAB;txMsg.data[0] = vscp_data_coding_getFormatByte(VSCP_DATA_CODING_REPRESENTATION_NORMALIZED_INTEGER, unit, index);&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;txMsg.dataNum += vscp_data_coding_int32ToNormalizedInteger(data, exp, &amp;txMsg.data[1], VSCP_L1_DATA_SIZE - txMsg.dataNum);&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;return vscp_core_sendEvent(&amp;txMsg);&LF;</xsl:text>
            </xsl:when>

            <!-- Handle data class exceptional -->
            <xsl:when test="(../../@id = 15) and ($vscpTypeId &gt; 0)">
                <!-- Function variables -->
                <xsl:text>&TAB;vscp_TxMessage&TAB;txMsg;&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>

                <!-- Prepare tx message -->
                <xsl:text>&TAB;vscp_core_prepareTxMessage(&amp;txMsg, </xsl:text>
                <xsl:value-of select="../../token" />
                <xsl:text>, </xsl:text>
                <xsl:value-of select="token" />
                <xsl:text>, VSCP_PRIORITY_3_NORMAL);&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>

                <xsl:text>&TAB;txMsg.dataNum = 1;&LF;</xsl:text>
                <xsl:text>&TAB;txMsg.data[0] = vscp_data_coding_getFormatByte(VSCP_DATA_CODING_REPRESENTATION_NORMALIZED_INTEGER, unit, index);&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;txMsg.dataNum += vscp_data_coding_int32ToNormalizedInteger(data, exp, &amp;txMsg.data[1], VSCP_L1_DATA_SIZE - txMsg.dataNum);&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;return vscp_core_sendEvent(&amp;txMsg);&LF;</xsl:text>
            </xsl:when>

            <!-- Handle measurezone class exceptional -->
            <xsl:when test="(../../@id = 65) and ($vscpTypeId &gt; 0)">
                <!-- Function variables -->
                <xsl:text>&TAB;vscp_TxMessage&TAB;txMsg;&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>

                <!-- Prepare tx message -->
                <xsl:text>&TAB;vscp_core_prepareTxMessage(&amp;txMsg, </xsl:text>
                <xsl:value-of select="../../token" />
                <xsl:text>, </xsl:text>
                <xsl:value-of select="token" />
                <xsl:text>, VSCP_PRIORITY_3_NORMAL);&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>

                <xsl:text>&TAB;txMsg.dataNum = 3;&LF;</xsl:text>
                <xsl:text>&TAB;txMsg.data[0] = index;&LF;</xsl:text>
                <xsl:text>&TAB;txMsg.data[1] = zone;&LF;</xsl:text>
                <xsl:text>&TAB;txMsg.data[2] = subZone;&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;txMsg.dataNum += vscp_data_coding_int32ToNormalizedInteger(data, exp, &amp;txMsg.data[3], VSCP_L1_DATA_SIZE - txMsg.dataNum);&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>
                <xsl:text>&TAB;return vscp_core_sendEvent(&amp;txMsg);&LF;</xsl:text>
            </xsl:when>

            <xsl:otherwise>
                <!-- Function variables -->
                <xsl:text>&TAB;vscp_TxMessage&TAB;txMsg;&LF;</xsl:text>
                <xsl:if test="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element">
                    <xsl:text>&TAB;uint8_t       &TAB;size&TAB;= 0;&LF;</xsl:text>
                </xsl:if>
                <xsl:if test="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element[@length &gt; 1]">
                    <xsl:text>&TAB;uint8_t       &TAB;byteIndex&TAB;= 0;&LF;</xsl:text>
                </xsl:if>
                <xsl:text>&LF;</xsl:text>

                <!-- Pointer checks -->
                <xsl:if test="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element[(@length &gt; 1) and (@optional = 'false')]">
                    <!-- Array pointer, which is not optional -->
                    <xsl:for-each select="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element[(@length &gt; 1) and (@optional = 'false')]">

                        <xsl:if test="position() &gt; 1">
                            <xsl:text>&LF;</xsl:text>
                        </xsl:if>

                        <xsl:text>&TAB;if ((NULL == </xsl:text>
                        <xsl:value-of select="local:hungarianForm(name[@lang = 'en'])" />
                        <xsl:text>) || (0 == </xsl:text>
                        <xsl:value-of select="local:hungarianForm(concat(name[@lang = 'en'], ' size'))" />
                        <xsl:text>))&LF;</xsl:text>
                        <xsl:text>&TAB;{&LF;</xsl:text>
                        <xsl:text>&TAB;&TAB;return FALSE;&LF;</xsl:text>
                        <xsl:text>&TAB;}&LF;</xsl:text>

                    </xsl:for-each>
                    <xsl:text>&LF;</xsl:text>
                </xsl:if>

                <!-- Prepare tx message -->
                <xsl:text>&TAB;vscp_core_prepareTxMessage(&amp;txMsg, </xsl:text>
                <xsl:value-of select="../../token" />
                <xsl:text>, </xsl:text>
                <xsl:value-of select="token" />
                <xsl:text>, VSCP_PRIORITY_3_NORMAL);&LF;</xsl:text>
                <xsl:text>&LF;</xsl:text>

                <!-- Handle each function parameter -->
                <xsl:for-each select="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element">

                    <!-- Variable name -->
                    <xsl:variable name="varName">
                        <xsl:value-of select="local:hungarianForm(name[@lang='en'])" />
                    </xsl:variable>

                    <!-- Variable content access. If variable is pointer, it will differ from variable name otherwise not. -->
                    <xsl:variable name="varContent">
                        <xsl:choose>
                            <xsl:when test="@optional = 'true'">
                                <xsl:text>*</xsl:text>
                                <xsl:value-of select="$varName" />
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:value-of select="$varName" />
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:variable>

                    <!-- Line indention -->
                    <xsl:variable name="indent">
                        <xsl:choose>
                            <xsl:when test="@optional = 'true'">
                                <xsl:text>&TAB;&TAB;</xsl:text>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:text>&TAB;</xsl:text>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:variable>

                    <!-- Every parameter section is separated with a empty line. -->
                    <xsl:if test="position() &gt; 1">
                        <xsl:text>&LF;</xsl:text>
                    </xsl:if>

                    <xsl:if test="@optional = 'true'">
                        <xsl:text>&TAB;if (NULL != </xsl:text>
                        <xsl:value-of select="$varName" />
                        <xsl:text>)&LF;</xsl:text>
                        <xsl:text>&TAB;{&LF;</xsl:text>
                    </xsl:if>

                    <xsl:choose>
                        <xsl:when test="@type = 'uint8'">

                            <xsl:choose>
                                <xsl:when test="@length = 1">
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>txMsg.data[</xsl:text>
                                    <xsl:value-of select="@pos + 0" />
                                    <xsl.text>] = </xsl.text>
                                    <xsl:value-of select="$varContent" />
                                    <xsl:text>;&LF;</xsl:text>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>size += </xsl:text>
                                    <xsl:value-of select="/specification/type-definitions/type-definition[name/text() = 'uint8']/size" />
                                    <xsl:text>;&LF;</xsl:text>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>for(byteIndex = 0; byteIndex &lt; </xsl:text>
                                    <xsl:value-of select="local:hungarianForm(concat(name[@lang='en'], ' size'))" />
                                    <xsl:text>; ++byteIndex)&LF;</xsl:text>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>{&LF;</xsl:text>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>&TAB;txMsg.data[</xsl:text>
                                    <xsl:value-of select="@pos" />
                                    <xsl.text> + byteIndex] = </xsl.text>
                                    <xsl:value-of select="$varName" />
                                    <xsl:text>[byteIndex];&LF;</xsl:text>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>&TAB;size += </xsl:text>
                                    <xsl:value-of select="/specification/type-definitions/type-definition[name/text() = 'uint8']/size" />
                                    <xsl:text>;&LF;</xsl:text>
                                    <xsl:text>&LF;</xsl:text>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>&TAB;if (VSCP_L1_DATA_SIZE &lt;= size)&LF;</xsl:text>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>&TAB;{&LF;</xsl:text>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>&TAB;&TAB;break;&LF;</xsl:text>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>&TAB;}&LF;</xsl:text>
                                    <xsl:value-of select="$indent" />
                                    <xsl:text>}&LF;</xsl:text>
                                </xsl:otherwise>
                            </xsl:choose>

                        </xsl:when>
                        <xsl:when test="@type = 'uint16'">

                            <xsl:value-of select="$indent" />
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 0" />
                            <xsl.text>] = (uint8_t)((</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text> &gt;&gt; 8) &amp; 0xff);&LF;</xsl:text>

                            <xsl:value-of select="$indent" />
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 1" />
                            <xsl.text>] = (uint8_t)((</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text> &gt;&gt; 0) &amp; 0xff);&LF;</xsl:text>

                            <xsl:value-of select="$indent" />
                            <xsl:text>size += </xsl:text>
                            <xsl:value-of select="/specification/type-definitions/type-definition[name/text() = 'uint16']/size" />
                            <xsl:text>;&LF;</xsl:text>

                        </xsl:when>
                        <xsl:when test="@type = 'uint32'">

                            <xsl:value-of select="$indent" /> 
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 0" />
                            <xsl.text>] = (uint8_t)((</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text> &gt;&gt; 24) &amp; 0xff);&LF;</xsl:text>

                            <xsl:value-of select="$indent" />
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 1" />
                            <xsl.text>] = (uint8_t)((</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text> &gt;&gt; 18) &amp; 0xff);&LF;</xsl:text>

                            <xsl:value-of select="$indent" />
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 2" />
                            <xsl.text>] = (uint8_t)((</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text> &gt;&gt; 8) &amp; 0xff);&LF;</xsl:text>

                            <xsl:value-of select="$indent" />
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 3" />
                            <xsl.text>] = (uint8_t)((</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text> &gt;&gt; 0) &amp; 0xff);&LF;</xsl:text>

                            <xsl:value-of select="$indent" />
                            <xsl:text>size += </xsl:text>
                            <xsl:value-of select="/specification/type-definitions/type-definition[name/text() = 'uint32']/size" />
                            <xsl:text>;&LF;</xsl:text>

                        </xsl:when>
                        <xsl:when test="@type = 'double'">

                            <xsl:value-of select="$indent" /> 
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 0" />
                            <xsl.text>] = ((uint8_t*)&amp;</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text>)[7];&LF;</xsl:text>

                            <xsl:value-of select="$indent" /> 
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 1" />
                            <xsl.text>] = ((uint8_t*)&amp;</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text>)[6];&LF;</xsl:text>

                            <xsl:value-of select="$indent" /> 
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 2" />
                            <xsl.text>] = ((uint8_t*)&amp;</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text>)[5];&LF;</xsl:text>

                            <xsl:value-of select="$indent" /> 
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 3" />
                            <xsl.text>] = ((uint8_t*)&amp;</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text>)[4];&LF;</xsl:text>

                            <xsl:value-of select="$indent" /> 
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 4" />
                            <xsl.text>] = ((uint8_t*)&amp;</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text>)[3];&LF;</xsl:text>

                            <xsl:value-of select="$indent" /> 
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 5" />
                            <xsl.text>] = ((uint8_t*)&amp;</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text>)[2];&LF;</xsl:text>

                            <xsl:value-of select="$indent" /> 
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 6" />
                            <xsl.text>] = ((uint8_t*)&amp;</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text>)[1];&LF;</xsl:text>

                            <xsl:value-of select="$indent" /> 
                            <xsl:text>txMsg.data[</xsl:text>
                            <xsl:value-of select="@pos + 7" />
                            <xsl.text>] = ((uint8_t*)&amp;</xsl.text>
                            <xsl:value-of select="$varContent" />
                            <xsl:text>)[0];&LF;</xsl:text>

                            <xsl:value-of select="$indent" />
                            <xsl:text>size += </xsl:text>
                            <xsl:value-of select="/specification/type-definitions/type-definition[name/text() = 'double']/size" />
                            <xsl:text>;&LF;</xsl:text>

                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:value-of select="$indent" />
                            <xsl:text>/* </xsl:text>
                            <xsl:value-of select="$varName" />
                            <xsl:text> not supported! */&LF;</xsl:text>
                        </xsl:otherwise>
                    </xsl:choose>

                    <xsl:if test="@optional = 'true'">
                        <xsl:text>&TAB;}&LF;</xsl:text>
                    </xsl:if>

                </xsl:for-each>

                <xsl:if test="not(/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element)">
                    <xsl:text>&TAB;txMsg.dataNum = 0;&LF;</xsl:text>
                    <xsl:text>&LF;</xsl:text>
                </xsl:if>

                <xsl:if test="/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame/elements/element">
                    <xsl:text>&LF;</xsl:text>
                    <xsl:text>&TAB;txMsg.dataNum = size;&LF;</xsl:text>
                    <xsl:text>&LF;</xsl:text>
                </xsl:if>

                <xsl:text>&TAB;return vscp_core_sendEvent(&amp;txMsg);&LF;</xsl:text>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Output function declaration -->
    <xsl:template name="funcDeclaration">
        <xsl:param name="baseName" />
        <xsl:param name="vscpClassId" />
        <xsl:param name="vscpTypeId" />

        <xsl:call-template name="ctools.function">

            <!-- Comment -->
            <xsl:with-param name="comment">
                <xsl:call-template name="funcComment">
                    <xsl:with-param name="vscpClassId">
                        <xsl:value-of select="$vscpClassId" />
                    </xsl:with-param>
                    <xsl:with-param name="vscpTypeId">
                        <xsl:value-of select="$vscpTypeId" />
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:with-param>

            <!-- Return type -->
            <xsl:with-param name="returnType">
                <xsl:text>extern BOOL</xsl:text>
            </xsl:with-param>

            <!-- Module name -->
            <xsl:with-param name="moduleName">
                <xsl:text>vscp_evt_</xsl:text>
                <xsl:value-of select="$baseName" />
            </xsl:with-param>
            
            <!-- Name -->
            <xsl:with-param name="name">
                <xsl:text>send </xsl:text>
                <xsl:value-of select="local:normalize(name[@lang='en'])" />
            </xsl:with-param>

            <!-- Parameter -->
            <xsl:with-param name="parameter">
                <xsl:call-template name="funcParameter">
                    <xsl:with-param name="vscpClassId">
                        <xsl:value-of select="$vscpClassId" />
                    </xsl:with-param>
                    <xsl:with-param name="vscpTypeId">
                        <xsl:value-of select="$vscpTypeId" />
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:with-param>

            <!-- Body -->
            <xsl:with-param name="body">
            </xsl:with-param>

        </xsl:call-template>
    </xsl:template>

    <!-- Create function prototype for vscp-type  -->
    <xsl:template match="vscp-type" mode="h">
        <xsl:param name="baseName" />

        <!-- Generate a function only if at least one frame is defined. -->
        <xsl:choose>

            <!-- Single frame -->
            <xsl:when test="count(frames/frame) = 1">

                <xsl:call-template name="funcDeclaration">
                    <xsl:with-param name="baseName">
                        <xsl:value-of select="$baseName" />
                    </xsl:with-param>
                    <xsl:with-param name="vscpClassId">
                        <xsl:value-of select="../../@id" />
                    </xsl:with-param>
                    <xsl:with-param name="vscpTypeId">
                        <xsl:value-of select="@id" />
                    </xsl:with-param>
                </xsl:call-template>

            </xsl:when>

            <!-- Multi frame -->
            <xsl:when test="count(frames/frame) &gt; 1">
                <xsl:text>/* "</xsl:text>
                <xsl:value-of select="name[@lang='en']" />
                <xsl:text>" not supported, because of multi frame. */&LF;</xsl:text>
            </xsl:when>
            
            <!-- Frame reference -->
            <xsl:when test="frames-ref">

                <xsl:variable name="vscpClassId">
                    <xsl:value-of select="local:getVscpClassId(frames-ref)" />
                </xsl:variable>

                <xsl:variable name="vscpTypeId">
                    <xsl:value-of select="local:getVscpTypeId(frames-ref)" />
                </xsl:variable>

                <xsl:choose>
                    <!-- Single frame -->
                    <xsl:when test="count(/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame) = 1">
                        <xsl:call-template name="funcDeclaration">
                            <xsl:with-param name="baseName">
                                <xsl:value-of select="$baseName" />
                            </xsl:with-param>
                            <xsl:with-param name="vscpClassId">
                                <xsl:value-of select="$vscpClassId" />
                            </xsl:with-param>
                            <xsl:with-param name="vscpTypeId">
                                <xsl:value-of select="$vscpTypeId" />
                            </xsl:with-param>
                        </xsl:call-template>
                    </xsl:when>

                    <!-- Multi frame -->
                    <xsl:otherwise>
                        <xsl:text>/* "</xsl:text>
                        <xsl:value-of select="name[@lang='en']" />
                        <xsl:text>" not supported, because of multi frame. */&LF;</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>

            </xsl:when>

            <xsl:otherwise>
                <xsl:text>/* "</xsl:text>
                <xsl:value-of select="name[@lang='en']" />
                <xsl:text>" not supported. No frame defined. */&LF;</xsl:text>
            </xsl:otherwise>

        </xsl:choose>

        <xsl:text>&LF;</xsl:text>
    </xsl:template>

    <!-- Output function definition -->
    <xsl:template name="funcDefinition">
        <xsl:param name="baseName" />
        <xsl:param name="vscpClassId" />
        <xsl:param name="vscpTypeId" />

        <xsl:call-template name="ctools.function">
            
            <!-- Comment -->
            <xsl:with-param name="comment">
                <xsl:call-template name="funcComment">
                    <xsl:with-param name="vscpClassId">
                        <xsl:value-of select="$vscpClassId" />
                    </xsl:with-param>
                    <xsl:with-param name="vscpTypeId">
                        <xsl:value-of select="$vscpTypeId" />
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:with-param>

            <!-- Return type -->
            <xsl:with-param name="returnType">
                <xsl:text>extern BOOL</xsl:text>
            </xsl:with-param>

            <!-- Module name -->
            <xsl:with-param name="moduleName">
                <xsl:text>vscp_evt_</xsl:text>
                <xsl:value-of select="$baseName" />
            </xsl:with-param>
            
            <!-- Name -->
            <xsl:with-param name="name">
                <xsl:text>send </xsl:text>
                <xsl:value-of select="local:normalize(name[@lang='en'])" />
            </xsl:with-param>

            <!-- Parameter -->
            <xsl:with-param name="parameter">
                <xsl:call-template name="funcParameter">
                    <xsl:with-param name="vscpClassId">
                        <xsl:value-of select="$vscpClassId" />
                    </xsl:with-param>
                    <xsl:with-param name="vscpTypeId">
                        <xsl:value-of select="$vscpTypeId" />
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:with-param>

            <!-- Body -->
            <xsl:with-param name="body">
                <xsl:call-template name="funcBody">
                    <xsl:with-param name="vscpClassId">
                        <xsl:value-of select="$vscpClassId" />
                    </xsl:with-param>
                    <xsl:with-param name="vscpTypeId">
                        <xsl:value-of select="$vscpTypeId" />
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:with-param>

        </xsl:call-template>
    </xsl:template>

    <!-- Create function for vscp-type  -->
    <xsl:template match="vscp-type" mode="c">
        <xsl:param name="baseName" />

        <!-- Generate a function only if at least one frame is defined. -->
        <xsl:choose>

            <!-- Single frame -->
            <xsl:when test="count(frames/frame) = 1">

                <xsl:call-template name="funcDefinition">
                    <xsl:with-param name="baseName">
                        <xsl:value-of select="$baseName" />
                    </xsl:with-param>
                    <xsl:with-param name="vscpClassId">
                        <xsl:value-of select="../../@id" />
                    </xsl:with-param>
                    <xsl:with-param name="vscpTypeId">
                        <xsl:value-of select="@id" />
                    </xsl:with-param>
                </xsl:call-template>

            </xsl:when>
            
            <!-- Multi frame -->
            <xsl:when test="count(frames/frame) &gt; 1">
                <xsl:text>/* "</xsl:text>
                <xsl:value-of select="name[@lang='en']" />
                <xsl:text>" not supported, because of multi frame. */&LF;</xsl:text>
            </xsl:when>

            <!-- Frame reference -->
            <xsl:when test="frames-ref">

                <xsl:variable name="vscpClassId">
                    <xsl:value-of select="local:getVscpClassId(frames-ref)" />
                </xsl:variable>

                <xsl:variable name="vscpTypeId">
                    <xsl:value-of select="local:getVscpTypeId(frames-ref)" />
                </xsl:variable>

                <xsl:choose>
                    <!-- Single frame -->
                    <xsl:when test="count(/specification/vscp-classes/vscp-class[@id = $vscpClassId]/vscp-types/vscp-type[@id = $vscpTypeId]/frames/frame) = 1">
                        <xsl:call-template name="funcDefinition">
                            <xsl:with-param name="baseName">
                                <xsl:value-of select="$baseName" />
                            </xsl:with-param>
                            <xsl:with-param name="vscpClassId">
                                <xsl:value-of select="$vscpClassId" />
                            </xsl:with-param>
                            <xsl:with-param name="vscpTypeId">
                                <xsl:value-of select="$vscpTypeId" />
                            </xsl:with-param>
                        </xsl:call-template>
                    </xsl:when>

                    <!-- Multi frame -->
                    <xsl:otherwise>
                        <xsl:text>/* "</xsl:text>
                        <xsl:value-of select="name[@lang='en']" />
                        <xsl:text>" not supported, because of multi frame. */&LF;</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>

            </xsl:when>

            <xsl:otherwise>
                <xsl:text>/* "</xsl:text>
                <xsl:value-of select="name[@lang='en']" />
                <xsl:text>" not supported. No frame defined. */&LF;</xsl:text>
            </xsl:otherwise>

        </xsl:choose>

        <xsl:text>&LF;</xsl:text>
    </xsl:template>

    <!-- Garbage collector -->
    <xsl:template match="text()">
    </xsl:template>

</xsl:stylesheet>
