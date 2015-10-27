<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

if(SESSION_STATUS == 'ok' && isset($docinput['user_id'])) {
	/*
	?>
		<script language="javascript">
                        <!--
                        document.location = '2.html';
                        // -->
                        </script>
	<?
	*/
	}

if($profile['editor'] != '1') {

$recover = $_REQUEST['recover'];
$first_item = $_REQUEST['first_item'];
$subject = $_REQUEST['subject'];

?>
<p style="text-align: center" align="center">
<center>
<form style="font-size:11px;" action="<?=get_url($id)?>" method="post" class="pNormal">
<img src="img/pixel.gif" height="100" border="0" />
<br clear="all" />
<table border="0" cellpadding="0" cellspacing="0" width="324">
<tr>
<td background="img/login_top.gif" height="59" style="font-face: Tahoma, Helevetica, Arial; font-size: 12 px; font-weight: bold; text-align: center" valign="center" align="center">
<? 
if(SESSION_STATUS != 'ok') { 
	echo 'Bitte anmelden...'; 
	} 
else { 
	echo str_replace('%%email%%',SESSION_EMAIL,LANG_LOGGED_IN); 
	}
?>
</td>
</tr>
<tr>
<td background="img/login_middle.gif" style="font-face: Tahoma, Helevetica, Arial; font-size: 12 px; font-weight: normal; text-align: left">
<table border="0" cellpadding="0" background="img/pixel.gif" cellspacing="10" width="324"><tr><td width="100%">
<?
if($recover == 'true' && isset($docinput['user_id'])) {
	$qresult = mysql_query ("SELECT password FROM ".TABLE_USERS." WHERE email='".$docinput['user_id']."'");
	if ($qresult) {
		$row = mysql_fetch_row($qresult);
		$message = "$row[0]";
		mail($docinput['user_id'], '['.SITE_TITLE.'] Login', $message,
        	"From: ".WEBMASTER_MAIL."\nReply-To: ".WEBMASTER_MAIL);
		}
	echo '<span style="font-weight: bold; color: #00aa00">'.LANG_PASSWD_SENT.'</span><p>';
	}
if(FORCE_SSL && !SSL) {
	echo LANG_SSL_REQUIRED;
	}
elseif(SESSION_STATUS != 'ok') {
	// Prevent Netscape 4 from making the input fields too long...
	if(strstr($_SERVER['HTTP_USER_AGENT'], 'Mozilla/4') !== false && strstr($_SERVER['HTTP_USER_AGENT'], 'compatible') === false) {
		// Netscape 4.x
		$input_size = "25";
		}
	else {
		// NS6, Mozilla, MSIE
		$input_size = "50";
		}
        if(SESSION_STATUS == 'passwd' && $docinput['passwd2'] != '') { 
		?>
		<script language="javascript">
                        <!--
                        alert('Bad password.');
                        // -->
                        </script>
		<?
	
        }
        if(SESSION_STATUS != 'passwd' && $docinput['passwd2'] != '') {
		?>
		<script language="javascript">
                        <!--
                        alert('Invalid user name.');
                        // -->
                        </script>
		<?

        }
        ?>
        <input type="hidden" name="id" value="<?=$id?>" />
        <input type="hidden" name="first_item" value="<?=$first_item?>" />
        <input type="hidden" name="subject" value="<?=$subject?>" />
        <center>
	<table border="0" cellpadding="0" cellspacing="0"><tr><td align="left">
        <span class="pNormal">User Name</span><br />
        <input type="text" name="docinput[user_id]" size="<?=$input_size?>" value="<?=$docinput['user_id']?>" /><br /><br />
        <span class="pNormal"><?=LANG_PASSWORD?></span><br />
        <input type="password" name="docinput[passwd2]" size="<?=$input_size?>" /><br /><br />
	</td></tr></table>
	</center>
	<div style="text-align: right" align="right" />
        <input type="submit" style="width: 70 px; background: #d6d3ce" value="OK" />
	<input type="reset" style="width: 70 px; background: #d6d3ce" value="Cancel" />
	</div>
        <?php
        }
else { ?>
        <input type="hidden" name="id" value="<?=$id?>" />
        <input type="hidden" name="first_item" value="<?=$first_item?>" />
        <input type="hidden" name="docinput[logout]" value="true" />
	<p style="text-align: center" align="center">
        <input type="submit" value="<?=LANG_LOGOUT?>" style="width: 70 px; background: #d6d3ce" />
	</p>
        <?php
        }
?>
</td></tr></table>

</td>
</tr>
<tr>
<td><?=get_image_tag('img/login_bottom.gif')?></td>
</tr>
</table>
</form>
</center>
</p>
<? 

} 

?>
