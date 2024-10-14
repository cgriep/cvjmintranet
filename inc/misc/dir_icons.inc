<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 06.10.2003
				
				modified 15th October 2003
				Paul Maxwell - attempt to have output table restricted to 3 simple columns
				as the default dir_icons.inc supplied with awf 1.10 produces a set of tables which
				cause the page to become too wide (for IE 5x anyway)
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

<?php
if (!strstr($subject, '.')) {
// <table width="100%" cellpadding="1" cellspacing="0" border="0">
?>
<?php

$file_counter = 0;
$total_size   = 0;
$dir_counter  = 0;

$filelist = array();
$dirlist = array();

$handle=opendir($value.$subject);
while (false != ($file = readdir($handle))) { 
	if(filetype($value.$subject.$file) == 'dir' || filetype($value.$subject.$file) == 'link') { $dirlist[] = $file; }
	if(filetype($value.$subject.$file) == 'file') { $filelist[] = $file; }
	}

sort($filelist);
sort($dirlist);
$filelist = array_merge($dirlist,$filelist);

echo '<table width="100%" cellpadding="5" cellspacing="0" border="0"  align="center"><tr bgcolor="'.$design['table_row_dark'].'">';
$colorswitch = 0;


foreach ($filelist as $nr =>$file) {
    if ($file != "." && $file != ".." && strlen($file) > 0 && $file[0] != '.') { 
      $file_counter++;
			
    $xtrastring = '';
    if($profile['allow_delete'] == '1' || file_exists(BASE_PATH.$value.$subject.'.allow_delete') || $profile['editor'] == '1') {
        $xtrastring = '<br /><img src="img/pixel.gif" width="50" height="1" border="0" />[ <a href="'.get_url($id, 'subject='.urlencode($subject).'&docinput[deletefile]='.urlencode($file)).'">'.LANG_BROWSE_DELETE.'</a> ]';		
        }
		else {
            if(filetype($value.$subject.$file) == 'dir') {
                $xtrastring = '';
                }
            elseif(filetype($value.$subject.$file) == 'link') {
                $xtrastring = '';
                }
            elseif($file_extention == 'lnk') {
                $xtrastring = '';
                }
            elseif(floor(filesize($value.$subject.$file) / 1024) < 1) {
                $xtrastring = '<br /><img src="img/pixel.gif" width="50" height="1" border="0" /> '.filesize($value.$subject.$file).' B';
                }
            else {
                $xtrastring =  '<br /><img src="img/pixel.gif" width="50" height="1" border="0" /> '.floor(filesize($value.$subject.$file) / 1024).' KB'; 
                }
		    }		
				
      if (filetype($value.$subject.$file) == 'dir' || filetype($value.$subject.$file) == 'link')  { 
        echo '<td width="50%" valign="middle" align="left"><a href="'.get_url($id,'subject='.urlencode($subject.$file)).'%2f"><img src="img/icons_32/folder_yellow.png" border="0" align="left" alt="'.LANG_DIRECTORY.'">'
				.$file.'</a>'.$xtrastring.'</td>';
				}
      if (filetype($value.$subject.$file) == 'file') { 	
	$file_extention = strtolower(substr($file,-3));
	$frame_file = false;
	switch ($file_extention) {
		case 'gif':
		case 'png':
		case 'peg':
		case 'jpg': $file_picture = 'img/icons_32/appearance.png'; break;
		case 'gif': $file_picture = 'img/icons_32/appearance.png'; break;
		case 'wav':
		case 'mp3': $file_picture = 'img/icons_32/sound.png'; break;
		case 'lnk': $file_picture = 'img/icons_32/connect_established.png'; break;
		case 'mov':
		case 'avi':
		case 'peg':
		case 'exe': $file_picture = 'img/icons_32/gear.png'; break;
		case 'php': 
		case 'hp3': 
		case 'hp4': $file_picture = 'img/icons_32/html.png'; break; 
		case 'htm':
		case 'tml': $file_picture = 'img/icons_32/html.png'; break;
		case 'zip':
		case 'tgz':
		case '.gz': $file_picture = 'img/icons_32/package.png'; break;
		case '.ps': $file_picture = 'img/icons_32/kword.png'; break;
		case 'doc': $file_picture = 'img/icons_32/kword.png'; break;
		case 'ppt': $file_picture = 'img/icons_32/kpresenter.png'; break;
		case 'xls': $file_picture = 'img/icons_32/kspread.png'; break;
		case 'sdw': $file_picture = 'img/icons_32/kword.png'; break;
		case 'sdc': $file_picture = 'img/icons_32/kspread.png'; break;
		case 'pdf': $file_picture = 'img/icons_32/pdf.png'; break;
	  case 'txt': $file_picture = 'img/icons_32/txt.png'; break;
		default: $file_picture = 'img/icons_32/zip_unmount.png'; false;		
		} 
	if($file_extention == 'lnk') {
		$fd = fopen($value.$subject.$file, "r");
    $link = 'redirect.php?url='.trim(fread($fd, filesize ($value.$subject.$file)));
		fclose($fd);
    echo '<td width="50%" valign="middle" align="left"><a href="'.$link.'"><img width="32" height="32" src="'.$file_picture.'" align="left" border="0" alt="Link">'
		.substr($file,0,-4).'</a>'.$xtrastring.'</td>'; 
		}
	elseif($frame_file == true) { 
    echo '<td width="50%" valign="middle" align="left"><a href="'.get_url($id, 'subject='.urlencode($subject)."&view=".urlencode($file)).'"><img hspace="5" width="16" height="16" src="'.$file_picture.'" align="left" border="0">'
		.$file.'</a>'.$xtrastring.'</td>';
		}
	else {	
    echo '<td width="50%" valign="middle" align="left"><a href="redirect.php?url='.str_replace(' ', '%20',$value.$subject.$file).'"><img alt="'.floor(filesize($value.$subject.$file) / 1024).' KB" width="32" height="32" src="'.$file_picture.'" align="left" border="0">'
		.$file.'</a>'.$xtrastring.'</td>';
		}
	}

      ?>
      <?php
			//* if even, output and end of row
			if ($file_counter % 2 == 0) { 
			   echo '</tr>';
				 if ($colorswitch == 1) {
				    $colorswitch = 0;
    			  echo '<tr bgcolor="'.$design['table_row_dark'].'">';
				    } 
				 else {
				    $colorswitch = 1;
    			   echo '<tr bgcolor="'.$design['table_row_bright'].'">';
				    } 
				 }

     }
    } 
}
closedir($handle);
//* if uneven output blank cell
if ($file_counter % 2 == 1) { 
  echo '<td></td>'; 
  }

?>
</tr></table><br clear="all" />



