<html>
<head>
<title></title>
<meta name="author" content="Dieter">
<meta name="generator" content="Ulli Meybohms HTML EDITOR">
</head>
<body text="#000000" bgcolor="#FFFFFF" link="#FF0000" alink="#FF0000" vlink="#FF0000">

<?php
echo "Server: ".$_SERVER["SERVER_ADDR"]." / Remote ".$_SERVER["REMOTE_ADDR"]."<br />";
if ( ! isset($_REQUEST["Code"]) )
{
?>
<form action="/cgi-bin/squid.pl" method="post" >
Freischaltcode <input type="Text" name="Code" value="" size="" maxlength=""/>
<input type="hidden" name="Aktion" value="allow" size="" maxlength=""/>
<input type="Submit" name="" value="Freischalten" />
</form>
<?php
}
else
{
error_reporting(E_ALL);
  // Freischaltung vornehmen
  if ( $_REQUEST["Code"] == "Test" )
  {
    echo "Freischaltung erfolgt:";
    passthru("/webdata/htdocs/squid.pl Rechner=Platz7 Aktion=allow");
  }
  else
  {
    echo "Falscher Code!";
    passthru("/webdata/htdocs/squid.pl Rechner=Platz7 Aktion=deny");
  }
}
?>
</body>
</html>
