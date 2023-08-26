<?php
/*
This is a thumbnail content module for the AWF.
maintained by Christoph Stickel <mixer2@web.de>
last update 27.3.2002 by Christoph S.
--------------
v. 1.0
update: 20040129 by jesse@ilk.com
+ allow directories to be created on the fly
+ don't show directory drop down if no dirs...
+ cosmetics => spelling, grammar
+ generalized thumbnail basedir
+ auto create missing thumbnails w/ gd
+ added fields:
	* Icon  (default = none)
	* Thumbnail Width (default = 100)
	* Thumbnail Height (default = 100)



Possible ToDos:
- allow configurable base gallery path to allow for multiple picture galleries
	* need to set base path in a parent Overview if none exists
- allow comments (exif?) for each image
- language variables?

--------------
v. 1.0.a
update: 20040225 by wolfgang@schaefernet.de
+ use language variable (only one ...)
+ add "description" (variable 'descr') to editor form
  this field is shown in the overview (not 'body')

*/
$gallery_path = $image_path."gallery/";  //used to create dirs, etc.
$gallery_rpath = $image_rpath."gallery/";  //used for comparisons 
$resettn = $_POST['resettn'];
if ($document['tn_width'] == "") { $document['tn_width'] = 100; }
if ($document['tn_height'] == "") { $document['tn_height'] = 100; }

load_lang('imagegallery');
if($profile['editor']!='1')
	{
	?><center><table width="100%" border="0" cellpadding="1" cellspacing="1"><?php
	if(isset($document['title']) && $document['title'] != "") { ?><tr><td><h3><?=transform($document['title'], 'clean')?></h3></td></tr><?php }
	if(isset($document['body']) && $document['body'] != "") { ?><tr><td><?=transform($document['body'], $document['datatype']['body'])?></td></tr><?php }
	?></table></center><?php

	//first let's make sure we have a 'thumbs' dir
	if (! file_exists($document['directory']."/thumbs") ){
		mkdir($document['directory']."/thumbs");
	}

	//now check the GD version in case we need to create thumbnails...
	global $gd2;
	$gd2 = checkgd();
        $handle = opendir($document['directory']);
	while ($file = readdir($handle)) { $existingth[] = $file;}
	?><p align="left"><?php
	$handle = opendir($document['directory']);
        while ($file = readdir ($handle))  { 
        	if (in_array($file, $existingth) && (ereg(".jpeg",substr($file,strlen($file)-5)) || ereg(".jpg",substr($file,strlen($file)-4)) || ereg(".png",substr($file,strlen($file)-4)))) {
		     	if (! file_exists($document['directory']."thumbs/".$file) ) {
				createthumb($document['directory'], $document['directory']."thumbs/", $file, $document['tn_width'], $document['tn_height']);
                	}

			?>
			<a href="<?=$document['directory'].$file?>" target="_blank">
			<img alt="<?=substr($file,0,-4)?>" src="<?=$document['directory']?>thumbs/<?=$file?>" border=0>
			</a>
			<?php 
			}
		}
	?></p><?php
	}
//DO EDITOR STUFF BELOW HERE
else {
	//DELETE ALL THE THUMBS IF REQUESTED
	if ($resettn == '1'){
	        $handle = opendir($document['directory']."thumbs/");
		$delok = true;
		$delcnt = 0;
		while ($file = readdir ($handle))  {
	                if ( (ereg(".jpeg",substr($file,strlen($file)-5)) || ereg(".jpg",substr($file,strlen($file)-4)) || ereg(".png",substr($file,strlen($file)-4)))) {
				if (!unlink($document['directory']."thumbs/".$file)){
					$delok = false;
				} else {
					$delcnt++;
				}
	                }
		}
		if ($delok) {
			echo "<font color=green>Deleted ".$delcnt." thumbnail image files</font><br>";
		} else {
			echo "<font color=red>Problems while deleting. ".$delcnt." thumbnail image files were deleted</font><br>";
		}
	}//end delete...

        create_editor_input ('Page body (optional)', 'body', 'area_input');
	echo '<br><br>';
        echo 'Description (optional, used for overview)<br><textarea name="docdata[descr]" cols="60" rows="5">'.$document['descr'].'</textarea><br><br>'?>
	<table><tr><td>Thumbnail Width</td><td>Thumbnail Height</td></tr>
		<tr><td>
		
                <input type="text" size="5" name="docdata[tn_width]" value="<?=$document['tn_width']?>">
		</td><td>
                <input type="text" size="5" name="docdata[tn_height]" value="<?=$document['tn_height']?>">
	</td></tr></table>
	<?	
        create_editor_input('Icon','icon','icon_input');
	echo "<i>Note... icon will be shown on this Gallery's parent Overview page, if it has one</i>";

	//first, make sure the gallery path exists....
        if (! file_exists($gallery_path)){
                if (! mkdir( $gallery_path  )){
                        echo "<font color=red>Warning, <b>$gallery_rpath<b> does not exist and could not be created!</font><br>";
                }
        }

	//next setup vars to create the new gallery directory if requested
	if (isset($document['directory_new']) && $document['directory_new'] != ""){
		$document['directory'] = $document['directory_new'];
		$document['directory_new'] = "";
	}

	//now check to make sure the Gallery directory is set correctly (won't be on creation) and exists...
	$orig_dd = $document['directory'];
	if (substr($document['directory'],0,strlen($gallery_rpath)) != $gallery_rpath){
		$document['directory'] = $gallery_rpath.$document['directory'];
	}

	//make sure this gallery's directory exists.
	if (! file_exists( $document['directory'] )){
		if (! mkdir( $document['directory'] )){
			echo "<font color=red>Warning, <b>$gallery_rpath<b> is not writable!</font><br>";
		}
	} 

	//also go ahead and make sure the thumbs subdir is there...
        if (! file_exists($document['directory']."/thumbs") ){
                if (! mkdir($document['directory']."/thumbs") ){
                        echo '<font color=red>Warning, <b>'.$document['directory'].'<b> is not writable!</font><br>';
		}
        }
	//Get a list of available directories this new gallery represents
	$handle = opendir($gallery_rpath);
        $gooddirs=0;
	while ($dir = readdir ($handle)) { 
        	if ($dir != "." && $dir != ".." && $dir != "")  {
			if(is_dir($gallery_rpath.$dir)) {
				$done = "true";
				$gooddirs++;
				if($gallery_rpath.$dir."/" != $document['directory']) {
					$sellist .= '<option value="'.$gallery_rpath.$dir.'/">'.$gallery_rpath.$dir.'</option>\n';
					}
				else {
					$sellist .= '<option selected value="'.$gallery_rpath.$dir.'/">'.$gallery_rpath.$dir.'</option>\n';
					}
                 		}
			}
		}
        $sellist_beg = '<select name="docdata[directory]" size=1>\n';
	$sellist_end = '</select><br>';
	//if we have dirs available show them....
	if ($gooddirs > 0){
		echo '<br><br>Select a Directory:<br>';
		echo $sellist_beg.$sellist.$sellist_end;
	} else {
		echo '<br>Sorry there are no directories in '.$gallery_rpath.'. Create one below.  <br>'; 
	} 
	if ($orig_dd == "" || $gooddirs == 0){
		create_editor_input('Or Create Directory Named: ', 'directory_new', 'text_input');
	} else {
		echo '<br><b>Gallery is configured.</b><br>'; 
		echo 'Now you should add images to the directory selected above.<br>';
		// here we see if a file browser document exists. hopefully it is the "./" one...
		// I should probably make sure that we can get there from here, but... oh well, not yet.
		$fb = get_cmodule_id('browser');
		if ($fb >= 0){
			$link = "index.php?id=".$fb."&subject=".$document['directory'];
			echo '<a target=_blank href="'.$link.'">Click Here to Open the File Browser</a><br>';
		} else {
			echo 'An easy way to manage the files is using a File Browser document. Unfortunately, I was unable to find one.<br>';
		}
	}
	echo '<br />';
	echo '<input type=checkbox name=resettn value=1>Check this box to Recreate all Thumbnails for this Gallery';
	echo '<br /><br />';
	create_editor_input('User comments at bottom', 'comments', 'onoff_input');
	echo '<input type="hidden" name="docdataflavour[comments]" value="0" />';
	}

//Setup A Link Back to the Parent gallery, if it exists...
$isgallery = get_node($document['parent_id'], $flavour);
if(($isgallery['type_id'] == $doctypes['gallery_overview']) && $isgallery != '') {
	?><center><br><br>[ <a href="<?=$PHP_SELF?>?id=<?=$document['parent_id']?>"><?=LANG_GALLERY_BACK?></a> ]</center><?php
	}
/*
	Function createthumb($name,$filename,$new_w,$new_h)
	creates a resized image
	variables:
	$name		Original filename
	$filename	Filename of the resized image
	$new_w		width of resized image
	$new_h		height of resized image
*/	
//function createthumb($name,$filename,$new_w,$new_h){
function createthumb($imgPath, $thumbPath, $filename,$new_w,$new_h){
	global $gd2;
	$system=explode(".",$filename);
	if (preg_match("/jpg|jpeg/",$system[1])){$src_img=imagecreatefromjpeg($imgPath.$filename);}
	if (preg_match("/png/",$system[1])){$src_img=imagecreatefrompng($imgPath.$filename);}
	$old_x=imageSX($src_img);
	$old_y=imageSY($src_img);
	if ($old_x > $old_y) {
		$thumb_w=$new_w;
		$thumb_h=$old_y*($new_h/$old_x);
	}
	if ($old_x < $old_y) {
		$thumb_w=$old_x*($new_w/$old_y);
		$thumb_h=$new_h;
	}
	if ($old_x == $old_y) {
		$thumb_w=$new_w;
		$thumb_h=$new_h;
	}
	if ($gd2==""){
			$dst_img=ImageCreate($thumb_w,$thumb_h);
			imagecopyresized($dst_img,$src_img,0,0,0,0,$thumb_w,$thumb_h,$old_x,$old_y); 
	}else{
		$dst_img=ImageCreateTrueColor($thumb_w,$thumb_h);
		imagecopyresampled($dst_img,$src_img,0,0,0,0,$thumb_w,$thumb_h,$old_x,$old_y); 
	}
	if (preg_match("/png/",$system[1])){
		imagepng($dst_img,$thumbPath.$filename); 
	} else {
		imagejpeg($dst_img,$thumbPath.$filename); 
	}
	imagedestroy($dst_img); 
	imagedestroy($src_img); 
}
/*
        Function checkgd()
        checks the version of gd, and returns "yes" when it's higher than 2
*/
function checkgd(){
        $gd2="";
        ob_start();
        phpinfo(8);
        $phpinfo=ob_get_contents();
        ob_end_clean();
        $phpinfo=strip_tags($phpinfo);
        $phpinfo=stristr($phpinfo,"gd version");
        $phpinfo=stristr($phpinfo,"version");
        preg_match('/\d/',$phpinfo,$gd);
        if ($gd[0]=='2'){$gd2="yes";}
        return $gd2;
}

?>
