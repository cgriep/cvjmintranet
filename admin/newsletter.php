<html>
<head>
<title>Newsletter erstellen</title>
<meta name="author" content="Platz7">
<meta name="generator" content="Ulli Meybohms HTML EDITOR">
</head>
<body text="#000000" bgcolor="#FFFFFF" link="#FF0000" alink="#FF0000" vlink="#FF0000">

<?php
        include ("../inc/functions.inc");
        include("header.inc");
        include('../inc/database.inc');
        include('../inc/db_tables.inc');
        if(!defined("SQL_CONNECTION") || SQL_CONNECTION == '') {
                echo '<b><font color="#aa0000">Can\'t connect to SQL server or database.</font></b><br><br>';
        }
        else {
                // Read all contstants from database
                $qresult = sql_query ("SELECT name, value FROM ".TABLE_SETUP);
                if(sql_num_rows($qresult) > 0) {
                        while ($row = sql_fetch_row($qresult)) {
                                define(strtoupper($row[0]),$row[1]);
                        }
                        sql_free_result($qresult);
                }
        }
  if(!defined('WEBMASTER_MAIL')) define('WEBMASTER_MAIL', $_SERVER['SERVER_ADMIN']);
  if(!defined('SITE_TITLE')) define('SITE_TITLE', $_SERVER['SERVER_NAME']);
?>

<table border="0" width="100%">
<form action="<?php $_SERVER["PHP_SELF"];?>" methode="post">
<tr><td>Absender</td><td><input type="Text" name="Absender" value="<?=WEBMASTER_MAIL?>" size="40"></td></tr>
<tr><td>Betreff</td><td><input type="Text" name="Betreff" value="[Newsletter] <?=SITE_TITLE?>" size="60" maxlength=""></td></tr>
<tr><td>Text</td><td><textarea name="Inhalt" cols="75" rows="10"></textarea></td></tr>
<tr><td colspan="2"><input type="Submit" value="Absenden"></td>
</tr>
</form>
</table>

<?php
  if( isset($_REQUEST["Absender"] )) {
    // Newsletter verschicken
    $qres = sql_query("SELECT email FROM `awf_users` inner join awf_userdata on " .
      "awf_users.id=user_id where awf_userdata.name = 'newsletter' and " .
      "awf_userdata.value = 1 and valid = 1;");
    if(sql_num_rows($qres) > 0) {
      while($row = sql_fetch_row($qres)) {
        mail($row[0], htmlentities($_REQUEST['Betreff']),
          htmlentities($_REQUEST['Text']),
          "From: ".$_REQUEST['Absender']."\nReply-To: ".$_REQUEST['Absender']);
        echo "Mail an " . $row[0]." versendet.<br />";
      }
    }
    sql_free_result($qres);
  }
?>
</body>
</html>