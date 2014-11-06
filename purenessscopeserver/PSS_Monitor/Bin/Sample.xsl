<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
  <body>
    <h2>Monitor(<xsl:value-of select="Group/@Name"/>)</h2>
    <table border="1">
    <tr bgcolor="#9acd32">
	  <th align="left">PSS Time</th> 
	  <th align="left">State</th>	  
      <th align="left">Active Client</th>
      <th align="left">Pool Client</th>
	  <th align="left">Max Handler Client</th>
	  <th align="left">Flow In(Byte)</th>
	  <th align="left">Flow Out(Byte)</th>
    </tr>
    <xsl:for-each select="Group/Monitor">
    <tr>
	  <td><xsl:value-of select="@Time"/></td>
	  <xsl:choose>
	    <xsl:when test="@State = 'Connected'" >
			<td><font color="green" ><xsl:value-of select="@State"/></font></td>
		</xsl:when>
	    <xsl:otherwise>
			<td><font color="red" ><xsl:value-of select="@State"/></font></td>
		</xsl:otherwise>		
	  </xsl:choose>	
	  <td><xsl:value-of select="@ActiveClient"/></td>
	  <td><xsl:value-of select="@PoolClient"/></td>
	  <td><xsl:value-of select="@MaxHandlerCount"/></td>
	  <td><xsl:value-of select="@FlowIn"/></td>
	  <td><xsl:value-of select="@FlowOut"/></td>
    </tr>
    </xsl:for-each>
    </table>
  </body>
  </html>
</xsl:template>
</xsl:stylesheet>
