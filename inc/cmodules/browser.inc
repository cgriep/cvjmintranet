<?php
/*
        Copyright (C) 2000-2002 Liquid Bytes (R), Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the liquidbytes.net Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 23.06.2002
*/

if(isset($basedir)) { unset($basedir); }

if(isset($document['basedir']) && $document['basedir'] != '') {
        // we can use this directory (default is public/)...
        $basedir = $document['basedir'];
        }
else {
        // ...or we create our own one ;)
        if(!file_exists(get_dir_name('files',$id).'/published')) {
          mkdirs(get_dir_name('files',$id).'/published', CONFIG_CREATE_MASK);
        }
        $basedir = get_dir_name('files',$id).'/published';
        }

$subject    = $_REQUEST['subject'];
$view       = $_REQUEST['view'];

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
// Delete file or directory
if((isset($docinput['deletefile']) || isset($deletefile)) && ($profile['allow_delete'] || file_exists(BASE_PATH.$basedir.$subject.'.allow_delete') || $profile['editor'] == '1')) {
        if (isset($docinput['deletefile']))
          $deletefile = urldecode($docinput['deletefile']);
        else
          $deletefile = urldecode($deletefile);
        chmod(BASE_PATH.$basedir.$subject.basename($deletefile), CONFIG_CREATE_MASK);
        if(is_dir(BASE_PATH.$basedir.$subject.basename($deletefile))) {
          $handle=opendir(BASE_PATH.$basedir.$subject.basename($deletefile));
          while (false != ($file = readdir($handle))) {
            if(filetype(BASE_PATH.$basedir.$subject.basename($deletefile).'/'.$file) == 'file') {
              unlink(BASE_PATH.$basedir.$subject.basename($deletefile).'/'.$file);
            }
          }
          if ( !rmdir(BASE_PATH.$basedir.$subject.basename($deletefile)) ) echo "ERROR";
        }
        else {
          unlink(BASE_PATH.$basedir.$subject.basename($deletefile));
        }
        unset($deletefile);
}

// Create directory
if(($profile['allow_createdir'] == '1' || file_exists(BASE_PATH.$basedir.$subject.'.allow_createdir') || $profile['editor'] == '1')
        && isset($docinput['newdir']) && $docinput['newdir'] != '') {
        if(file_exists(BASE_PATH.$basedir.$subject.basename($docinput['newdir']))) {
                echo('<p><b>'.LANG_DIRECTORY_EXISTS.'</b></p>');
                }
        elseif(strstr($subject, '.') || strstr(basename($docinput['newdir']), '.')) {
                echo('<p><b>'.LANG_DIRECTORY_NO_DOTS.'</b></p>');
                }
        else {
                mkdirs(BASE_PATH.$basedir.$subject.basename($docinput['newdir']), CONFIG_CREATE_MASK);
                }
        }

// File upload
if($profile['allow_upload'] == '1' || file_exists(BASE_PATH.$basedir.$subject.'.allow_upload') || $profile['editor'] == '1') {
if ( isset($_FILES['userfile']['name']))
  $_FILES['userfile']['name'] = str_replace("+", "", $_FILES['userfile']['name']);
if(isset($_FILES['userfile']['tmp_name']) && $_FILES['userfile']['tmp_name'] != '' && $_FILES['userfile']['tmp_name'] != 'none' &&  !strstr($subject, '.')) {
if(function_exists('move_uploaded_file')) {
  if (file_exists(BASE_PATH.$basedir.$subject.basename($_FILES['userfile']['name'])))
    rename (BASE_PATH.$basedir.$subject.basename($_FILES['userfile']['name']),
      BASE_PATH.$basedir.$subject."Version vom ".date("d.m.Y")."-".basename($_FILES['userfile']['name']));
        move_uploaded_file($_FILES['userfile']['tmp_name'], BASE_PATH.$basedir.$subject.basename($_FILES['userfile']['name']));
        chmod(BASE_PATH.$basedir.$subject.basename($_FILES['userfile']['name']), CONFIG_CREATE_MASK);
        }
   }
 }



if($profile['editor'] == 1) {
    echo'<p>';
    create_editor_input(LANG_DIRECTORY, 'basedir', 'text_input');
    if($docnames[$document['type_id']] == 'browser') {
        echo '<br /><br />';
        create_editor_input('User comments at bottom', 'comments', 'onoff_input');
        }
    echo '<br /><br />';
    create_editor_input('Header', 'header', 'area_input');
    echo '<br /><br />';
    }
else {
    if(isset($document['header']) && $document['header'] != '') echo '<p>'.transform($document['header'],$document['datatype']['header']).'</p>';
    }

  if(isset($view)) { $view = urldecode($view); }

  if(isset($view) && ($view != '') && (!strstr($view, '..'))) {
        echo $design['table_begin'];
        ?>
        <tr><td bgcolor="<?=$design['table_row_bright']?>" align="left">
        <?=LANG_DOCUMENT?>:
        <?
        $buffer = split('/', $subject);
        while(list($nr, $dir) = each($buffer)) {
          $real_path .= $dir.'/';
          if($nr==0) echo('<a href="'.get_url($id,'subject=%2f').'">'.$document['title'].'</a> > ');
          if($dir != '') {
                echo('<a href="'.get_url($id,'subject='.urlencode($real_path)).'">'.$dir.'</a> > ');
                }
          }
        echo $view;
        ?>
        </td>
        <td bgcolor="<?=$design['table_row_bright']?>" align="right">
        <a href="redirect.php?url=<? echo str_replace(' ', '%20',$basedir.$subject.$view); ?>"><?=LANG_DOWNLOAD?></a>
        </td></tr>
        <?=$design['table_end']?>
        <br />
        <?

        $file_extention = strtolower(substr($view,strlen($view)-4,4));
        switch ($file_extention) {
                case '.gif':
                case '.png':
                case '.jpeg':
                case '.jpg':
                case '.gif': echo '<p style="text-align: center"><img border="0" src="'.str_replace(' ','%20',$basedir.$subject.$view).'" /></p>'; break;
                case '.txt': echo('<p><pre>'); highlight_file($basedir.$subject.$view); echo('</pre></p>'); break;
                case '.php';
                case 'php3':
                case 'php4': echo('<p>'); highlight_file($basedir.$subject.$view); echo('</p>'); break;
                case '.pdf':
                        //passthru('pdftohtml -F -i -o - '.BASE_PATH.''.$basedir.$subject.$view);
                        passthru('pdftotext '.BASE_PATH.$basedir.$subject.$view.' -');
                        break;
                case '.htm':
                case 'html':
                        $htmlfile = file($basedir.$subject.$view);
                        $htmlcontent = join('',$htmlfile);
                        if(strpos($htmlcontent,'</body>') > 0 || strpos($htmlcontent,'</BODY>') > 0) {
                                $htmlcontent = substr(strstr(stristr($htmlcontent,'<body'),'>'),1);
                                $htmlcontent = strrev(substr(stristr(strrev($htmlcontent),'>ydob/<'),7));
                                }
                        echo('<p>'.$htmlcontent.'</p>');
                        break;
                default: htmlentities(readfile($basedir.$subject.$view));
                }
        }
  else {

if($docnames[$document['type_id']] == 'browser') {
        transform($basedir, 'dir_list');
        }
else {
        transform($basedir, 'dir_icons');
        }

if($profile['allow_upload'] == '1' || file_exists(BASE_PATH.$basedir.$subject.'.allow_upload') || $profile['editor'] == '1') {

if($profile['editor'] == '0') { ?>
<form enctype="multipart/form-data" action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="id" value="<?=$id?>">
<? } else { echo '<p>'; } ?>
<input type="hidden" name="subject" value="<?=$subject?>">
<?=LANG_BROWSE_UPLOAD?><br /><input name="userfile" type="file">
<input type="submit" value="<?=LANG_BROWSE_SENDFILE?>">
<? if($profile['editor'] == '0') { ?>
</form>
<? }

}

if($profile['allow_createdir'] == '1' || file_exists(BASE_PATH.$basedir.$subject.'.allow_createdir') || $profile['editor'] == '1') {

if($profile['editor'] == '0') { ?>
<form enctype="multipart/form-data" action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="hidden" name="id" value="<?=$id?>">
<? } else { echo '<p>'; } ?>
<input type="hidden" name="subject" value="<?=$subject?>">
<?=LANG_BROWSE_CREATEDIR?><br /><input name="docinput[newdir]" type="text">
<input type="submit" value="<?=LANG_BROWSE_CREATE?>">
<? if($profile['editor'] == '0') { ?>
</form>
<? }

}
}

unset($subject);
unset($view);

?>
</p>
