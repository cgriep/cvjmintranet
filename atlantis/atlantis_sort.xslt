<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html" />

  <xsl:template match="/*">
    <html>
      <head><title>Atlantis-Datenauszug <xsl:value-of select="@title" /></title></head>
      <link rel="stylesheet" type="text/css" href="atlantis.css" />
      <body>
        <h1>
          <xsl:value-of select="@title" />
        </h1>
        <table class="Liste">
          <xsl:call-template name="tabellenkopf" />
          <xsl:call-template name="tabellenkoerper" />
        </table>
        <p>Stand: <xsl:value-of select="@stand" /></p>
      </body>
    </html>
  </xsl:template>

  <xsl:template name="tabellenkopf">
  <tr>
    <xsl:for-each select="/atlantis/*[1]/*">
    <th>
      <xsl:value-of select="name(current())" />
    </th>
    </xsl:for-each>
  </tr>
  </xsl:template>
  
  <xsl:template name="tabellenkoerper">
    <xsl:for-each select="/atlantis/*">
      <xsl:sort select="Fertigkeit_id" data-type="number" />
    <tr>
      <xsl:for-each select="./*">
        <td>
          <xsl:value-of select ="current()"/>
        </td>
      </xsl:for-each>
    </tr>
    </xsl:for-each>
  </xsl:template>
  
   
</xsl:stylesheet>