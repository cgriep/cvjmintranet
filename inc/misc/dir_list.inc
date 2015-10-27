<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 21.08.2003
*/

global $subject;
global $design;

if(isset($subject)) {
        $subject = urldecode($subject);
        if (strstr ($subject, '.')) {
                echo ('<b>You try to cheat!</b>');
                $subject = '/';
                }
        if(substr($subject, -1) != '/') {
                $subject = $subject.'/';
                }
        }
else {
        $subject = '/';
        }
?>

<?=$design['table_begin']?>
<tr><td bgcolor="<?=$design['table_row_bright']?>">
<?=LANG_DIRECTORY?>:
<?
$buffer = split('/', $subject);
while(list($nr, $dir) = each($buffer)) {
  $real_path .= $dir.'/';
  if($nr==0) echo('<a href="'.get_url($id,'subject=%2f').'">'.$document['title'].'</a> > ');
  if($dir != '') {
        echo('<a href="'.get_url($id,'subject='.urlencode($real_path)).'">'.$dir.'</a> > ');
        }
  }
?>
</td></tr>
<?=$design['table_end']?>
<br />
<?
if (!strstr($subject, '.')) {
?>
<table width="100%" cellpadding="1" cellspacing="0" border="0">
<?

$file_counter = 0;
$total_size   = 0;
$dir_counter  = 0;
$dirlist = array();
$filelist= array();
$handle=opendir($value.$subject);
while (false != ($file = readdir($handle))) { 
	if(filetype($value.$subject.$file) == 'dir' || filetype($value.$subject.$file) == 'link') { $dirlist[] = $file; }
	if(filetype($value.$subject.$file) == 'file') { $filelist[] = $file; }
	}
sort($filelist);
sort($dirlist);
$filelist = array_merge($dirlist,$filelist);

while (list($nr,$file) = each($filelist)) {
    if ($file != "." && $file != ".." && strlen($file) > 0 && $file[0] != '.') { 
      $file_counter++;
      ?>
      <tr><td width="1%" align="left">
      <?
      if (filetype($value.$subject.$file) == 'dir' || filetype($value.$subject.$file) == 'link')  { 
        echo '<a href="'.get_url($id, 'subject='.urlencode($subject.$file)).'%2f"><img hspace="5" src="img/icons_16/folder.png" border="0"></a></td>
		<td width="33%" align="left"><a href="'.get_url($id, 'subject='.urlencode($subject.$file.'/')).'">'.$file.'</a></td>'; }
      if (filetype($value.$subject.$file) == 'file') { 	
	$file_extention = strtolower(substr($file,-3));
	$frame_file = false;
	switch ($file_extention) {
		case 'gif':
		case 'png':
		case 'peg':
		case 'jpg': $file_picture = 'img/icons_16/colorscm.png'; $frame_file = true; break;
		case 'gif': $file_picture = 'img/icons_16/colorscm.png'; $frame_file = true; break;
		case 'mp3': $file_picture = 'img/icons_16/package_multimedia.png'; break;
		case 'lnk': $file_picture = 'img/icons_16/link.png'; break;
		case 'mov':
		case 'avi':
		case 'peg':
		case 'mpg': $file_picture = 'img/icons_16/package_multimedia.png'; break;
		case 'exe': $file_picture = 'img/icons_16/gear.png'; break;
		case 'php': 
		case 'hp3': 
		case 'hp4': $file_picture = 'img/icons_16/file.png'; $frame_file = true; break; 
		case 'htm':
		case 'tml': $file_picture = 'img/icons_16/htmlfile.png'; $frame_file = true; break;
		case 'zip':
		case 'tgz':
		case '.gz': $file_picture = 'img/icons_16/tgz.png'; break;
		case '.ps': $file_picture = 'img/icons_16/kwrite.png'; break;
		case 'doc': $file_picture = 'img/icons_16/kwrite.png'; break;
		case 'xls': $file_picture = 'img/icons_16/kspread.png'; break;
		case 'ppt': $file_picture = 'img/icons_16/kpresenter.png'; break;
		case 'sdw': $file_picture = 'img/icons_16/apps/kwrite.png'; break;
		case 'sdc': $file_picture = 'img/icons_16/kspread.png'; break;
		case 'pdf': $file_picture = 'img/icons_16/pdf.png'; /* $frame_file = true; */ break;
	        case 'txt': $file_picture = 'img/icons_16/file.png'; $frame_file = true; break;
		default: $file_picture = 'img/icons_16/file.png'; $frame_file = false;		
		} 
	if($file_extention == 'lnk') {
		$fd = fopen($value.$subject.$file, "r");
    		$link = 'redirect.php?url='.trim(fread($fd, filesize ($value.$subject.$file)));
		fclose($fd);
        	echo '<a href="'.$link.'"><img hspace="5" width="16" height="16" src="'.$file_picture.'" border="0"></a>
		</td>
		<td width="33%" align="left"><a href="'.$link.'">'.substr($file,0,-4).'</a></td>'; 
		}
	elseif($frame_file == true) { 
        	echo '<a href="'.get_url($id, 'subject='.urlencode($subject)."&view=".urlencode($file)).'"><img hspace="5" width="16" height="16" src="'.$file_picture.'" border="0"></a>
		</td>
		<td width="33%" align="left"><a href="'.get_url($id, 'subject='.urlencode($subject)."&view=".urlencode($file)).'">'.$file.'</a></td>'; 
		}
	else {	
        	echo '<a href="redirect.php?url='.str_replace(' ', '%20',$value.$subject.$file).'"><img hspace="5" width="16" height="16" src="'.$file_picture.'" border="0"></a>
		</td>
		<td width="33%" align="left"><a href="redirect.php?url='.str_replace(' ','%20', $value.$subject.$file).'">'.$file.'</a></td>'; 
		}
	}
      ?>
      </td>
      <td width="20%" align="right" nowrap><? 
	if(filetype($value.$subject.$file) == 'dir') {
		echo htmlentities('<DIR>');
		$dir_counter++;
		}
	elseif(filetype($value.$subject.$file) == 'link') {
		echo htmlentities('<LINK>');
		$dir_counter++;
		}
	elseif($file_extention == 'lnk') {
		echo htmlentities('<URL>');
		}
        elseif(floor(filesize($value.$subject.$file) / 1024) < 1) {
		echo filesize($value.$subject.$file).' B';
		}
	else {
		echo floor(filesize($value.$subject.$file) / 1024).' KB'; 
		}
	$total_size += filesize($value.$subject.$file);
      ?>
      </td>
      <td width="5%" align="right">&nbsp;</td>
      <td width="20%" align="right" nowrap><?
      echo timestamp_to_string(filemtime($value.$subject.$file), 'datetime');
      
      ?>
      </td>
      <td width="29%" align="center">
      <?php
	if($profile['allow_delete'] == '1' || file_exists(BASE_PATH.$value.$subject.'.allow_delete') || $profile['editor'] == '1') {
		echo '[ <a href="'.get_url($id, 'subject='.urlencode($subject).'&docinput[deletefile]='.urlencode($file)).'">'.LANG_BROWSE_DELETE.'</a> ]';
		}
	else {
		echo '&nbsp;';
		}
      ?>
      </td>
      </tr>
      <?php
    } 
}
closedir($handle); 
?>
</table>
<br />
<?=$design['table_begin']?>
<table width="100%" cellpadding="2" cellspacing="0" border="0">
<tr><td bgcolor="<?=$design['table_row_bright']?>">
<?php
printf(LANG_BROWSE_USED, floor($total_size / 1024));
if($file_counter == 0) { echo LANG_BROWSE_NOFILES; } else { 
printf(' '.LANG_BROWSE_FILES, ($file_counter - $dir_counter), $dir_counter);
}
?>
</td></tr>
<?=$design['table_end']?>
<? } ?>
