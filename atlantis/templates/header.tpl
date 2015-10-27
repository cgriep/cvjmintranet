<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
{popup_init src="/javascript/overlib.js"}

<title>{$PageTitle}</title>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1" />
<link rel="stylesheet" type="text/css" href="atlantis.css">
<link rel="shortcut icon" href="favicon.ico">
{if $mitAjax }
  {php}
  require('atlantisajax.php');
  $xajax->printJavascript('./');
  {/php}
{/if}

</head>
<body>
<div class="header">
  <a href="."><img src="atlantislogo001.png" class="nebeneinander" /></a>
  <h1>{$PageTitle}</h1>
</div>
<div class="headerRechts">
  {if isset($Character)}
    <strong>{$Character.Name}</strong><br />
    {$Character.Art} {$Character.Rasse}
    {if isset($Character.Ausrichtung) }
     <br />(Ausrichtung: {$Character.Ausrichtung})
    {/if}
  {/if}
</div>

