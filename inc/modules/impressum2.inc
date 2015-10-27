<?php
  if ( file_exists(BASE_PATH.INC_PATH.DATA_PATH."impressum.txt") ) {
    readfile(BASE_PATH.INC_PATH.DATA_PATH."impressum.txt");
  }
  else
  {
?>
  <font size="-1">
  <p>Kommentare und Hinweise bitte an
  <script language=javascript>
  <!--
  var aka = "Christoph Griep"
  var prefix = "griep"
  var domain = "oszimt.de"
  document.write("<a href=" + "mail" + "to:" + prefix + "@" + domain + ">" + aka + "</a>" + ".")
  //-->
  </script>
  </p>
  </font>
<?php
 }
?>