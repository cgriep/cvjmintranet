<?php
// +----------------------------------------------------------------------+
// | phpOpenOffice - Solution for modifying OpenOffice documents with PHP |
// | v0.1b - 18/12/2003                                                   |
// |                                                                      |
// | This software is published under BSD licence.                        |
// | http://www.opensource.org/licenses/bsd-license.php                   |
// +----------------------------------------------------------------------+
// | Written by Bjoern Kahle, Hamburg 2003 (phpopenoffice at pinasoft.de) |
// | http://www.pinasoft.de/projects/phpOpenOffice/                       |
// +----------------------------------------------------------------------+

DEFINE("ZEILENENDE","</table:table-row>");
DEFINE("ZEILENANFANG","<table:table-row");
DEFINE("ZELLENENDE","</table:table-cell>");
DEFINE("ZELLENANFANG","<table:table-cell");
DEFINE("OFFICEAUTOSTYLES","<office:automatic-styles");
DEFINE("OOCOLUMN", '<table:table-column');
DEFINE("OOCOLUMNREPEATED", 'table:number-columns-repeated="');

// Where is phpOpenOffice going to store the documents temporarly
if (!defined('POO_TMP_PATH')) {
  define('POO_TMP_PATH', '/tmp/');
}


// Where are the OpenOffice templates
if (!defined('POO_TEMPLATE_PATH')) {
  define('POO_TEMPLATE_PATH', "");
}

// PhpConcept Library - Zip Module 2.0
// http://www.phpconcept.net
if (!defined('PCLZIP_INCLUDE_PATH')) {
  define('PCLZIP_INCLUDE_PATH',"./pclzip/");
}
define( 'PCLZIP_TEMPORARY_DIR', POO_TMP_PATH );
require PCLZIP_INCLUDE_PATH . 'pclzip.lib.php';


// Use zlib from PHPMyAdmin for writing zipped files,
// because documents created with PclZip cannot be opened with OpenOffice
// Needs to be fixed in later version.
/* removed 18.03.25
if (!defined('ZIPLIB_INCLUDE_PATH')) {
  define('ZIPLIB_INCLUDE_PATH', "./");
}
require ZIPLIB_INCLUDE_PATH . 'zip.lib.php';
*/

// How are variables defined within the document
if (!defined('POO_VAR_PREFIX')) {
  define('POO_VAR_PREFIX', '%');
}

if (!defined('POO_VAR_SUFFIX')) {
  define('POO_VAR_SUFFIX', '%');
}

// Setzt Texten fett- bzw. kursiv-Attribute vor
function ooMakeBold($string)
{
  return '<text:span text:style-name="ooAutoBold">'.$string."</text:span>";
}
function ooMakeItalic($string)
{
  return '<text:span text:style-name="ooAutoItalic">'.$string."</text:span>";
}

function ooInsertLineBreak()
{
  return '<text:line-break/>';
}

// Callback function for pclzip
$archiveFiles = array();
function ooPreAdd($p_event, &$p_header)
{
	global $archiveFiles;
	if($p_header['folder'] == 0)
		$archiveFiles[] = $p_header["stored_filename"];
	return 0;
}


class phpOpenOffice
{
	var $tmpDirName = "";
	var $parserFiles = "";
	var $parsedDocuments = array();
	var $mimetypeFile = "";
	var $mimetype = "";
	var $zipFile = "";


	// Load document from filesystem
	function loadDocument($filename)
	{
		if(!file_exists($filename))
		{
			$this->handleError("File not found: ".$filename, E_USER_ERROR);
		}
		else
		{
			$this->zipFile = $filename;
		}


		// Find a random folder name for PCLZIP_OPT_ADD_PATH
		$this->tmpDirName = $this->getRandomString(16);
		$this->mimetypeFile = POO_TMP_PATH."/".$this->tmpDirName."/mimetype";
		$this->parserFiles = array();
		$this->parserFiles["content.xml"] = POO_TMP_PATH."/".$this->tmpDirName."/content.xml";
		$this->parserFiles["styles.xml"] = POO_TMP_PATH."/".$this->tmpDirName."/styles.xml";


		// Open archive and extract content.xml
		$archive = new PclZip($filename);
		$list = $archive->extract(PCLZIP_OPT_PATH, POO_TMP_PATH, PCLZIP_OPT_ADD_PATH, $this->tmpDirName);
	}

         // setzt die Styles für Fett- und Kursivdruck ein
         // muss nach loaddocument aufgerufen werden
    function insertStyles()
    {
           $erfolg = true;
           if($this->tmpDirName == "")
           {
	    $this->handleError("No document loaded. Use loadDocument function first.", E_USER_ERROR);
           }
           // Is dir still there
	  if(!is_dir(POO_TMP_PATH."/".$this->tmpDirName))
	  {
	    $this->handleError("Directory not found: ".$this->tmpDirName, E_USER_ERROR);
	  }
	  // Open files and start parsing
	  //  $this->parsedDocuments = array();
      $file = "content.xml";
      $fp = fopen($this->parserFiles[$file], "r");	  
	  $this->parsedDocuments[$file] = fread($fp, filesize($this->parserFiles[$file]));
	  fclose($fp);
           // Style einfügen
           if ( ($pos = strpos($this->parsedDocuments[$file], OFFICEAUTOSTYLES)) !== false )
           {
              if ( substr($this->parsedDocuments[$file],$pos+strlen(OFFICEAUTOSTYLES),2) == "/>" )
              {
                // bisher keine vorhanden
                $zeile = substr($this->parsedDocuments[$file],0,$pos+strlen(OFFICEAUTOSTYLES));
                $zeile .= '><style:style style:name="ooAutoBold" style:family="text">'.
                          '<style:properties fo:font-weight="bold" style:font-weight-'.
                          'asian="bold" style:font-weight-complex="bold"/></style:style>'.
                          '<style:style style:name="ooAutoItalic" style:family="text">'.
                          '<style:properties fo:font-style="italic" style:font-style-'.
                          'asian="italic" style:font-style-complex="italic"/>'.
                          '</style:style>';
                $zeile .= substr($this->parsedDocuments[$file],$pos+strlen(OFFICEAUTOSTYLES)+2);
                $this->parsedDocuments[$file] = $zeile;
              }
              else
              {
                $zeile = substr($this->parsedDocuments[$file],0,$pos+strlen(OFFICEAUTOSTYLES)+1);
                $zeile .= '<style:style style:name="ooAutoBold" style:family="text">'.
                          '<style:properties fo:font-weight="bold" style:font-weight-'.
                          'asian="bold" style:font-weight-complex="bold"/></style:style>'.
                          '<style:style style:name="ooAutoItalic" style:family="text">'.
                          '<style:properties fo:font-style="italic" style:font-style-'.
                          'asian="italic" style:font-style-complex="italic"/>'.
                          '</style:style>';
                          ;
                $zeile .= substr($this->parsedDocuments[$file],$pos+strlen(OFFICEAUTOSTYLES)+1);
                $this->parsedDocuments[$file] = $zeile;
              }
           }
           else
             $erfolg = false;
           $fp = fopen($this->parserFiles[$file], "w+");
           fputs($fp, $this->parsedDocuments[$file]);
           fclose($fp);
           return $erfolg;
         }

	// Put variables into extracted content file
	function parse($variables, $attribute = array())
	{
		// Has file been extracted ?
		if($this->tmpDirName == "")
		{
			$this->handleError("No document loaded. Use loadDocument function first.", E_USER_ERROR);
		}


		// Is dir still there
		if(!is_dir(POO_TMP_PATH."/".$this->tmpDirName))
		{
			$this->handleError("Directory not found: ".$this->tmpDirName, E_USER_ERROR);
		}


		// Is argument valid ?
		if(!is_array($variables))
		{
			$this->handleError("First parameter need to been an array.", E_USER_ERROR);
		}


		// Read mimetype
		$fp = fopen($this->mimetypeFile, "r");
		$this->mimetype = fread($fp, filesize($this->mimetypeFile));
		fclose($fp);


		// Open files and start parsing
		$parsedDocuments = array();
                 $Spalteneingefuegt = false;
		foreach (array_keys($this->parserFiles) as $file)
		{
		  $fp = fopen($this->parserFiles[$file], "r");
		  $this->parsedDocuments[$file] = fread($fp, filesize($this->parserFiles[$file]));
		  fclose($fp);
                   $Zeilen = array();
                   $Zeile = "";
                   foreach(array_keys($variables) as $key )
		  {
                     $value = $variables[$key];
                     if ( is_array($value) )
                     {
                       $anz = substr_count($this->parsedDocuments[$file], POO_VAR_PREFIX.$key.POO_VAR_SUFFIX);
                       if ( $anz > 0 )
                       {
                         // Erster Durchlauf
                         // Suchen des Zeilenanfangs
                         $Pos = strpos($this->parsedDocuments[$file], POO_VAR_PREFIX.$key.POO_VAR_SUFFIX);
                         $such = strrev(substr($this->parsedDocuments[$file],0,$Pos));
                         $Zeilenanfang = strpos($such,strrev(ZEILENANFANG));
                         $Zeilenanfang = strlen($such)-$Zeilenanfang-strlen(ZEILENANFANG);
                         $Zeilenende = strpos(substr($this->parsedDocuments[$file],$Zeilenanfang),ZEILENENDE);
                         $Zeile = substr($this->parsedDocuments[$file],$Zeilenanfang,
                                   $Zeilenende+strlen(ZEILENENDE));
                         if ( Count($value) == 0 )
                           $this->parsedDocuments[$file] = str_replace(POO_VAR_PREFIX.$key.POO_VAR_SUFFIX,
                                   "", $this->parsedDocuments[$file]);
                         else
                           foreach ( $value as $vkey=>$vvalue )
                           {
                             if ( ! isset($Zeilen[$Zeile][$vkey]) ) $Zeilen[$Zeile][$vkey] = $Zeile;
                             if ( is_array($vvalue) )
                             {
                               // Neue Zellen einbauen
                               $Pos = strpos($Zeilen[$Zeile][$vkey], POO_VAR_PREFIX.$key.POO_VAR_SUFFIX);
                               $such = strrev(substr($Zeilen[$Zeile][$vkey],0,$Pos));
                               $Zellenanfang = strpos($such,strrev(ZELLENANFANG));
                               $Zellenanfang = strlen($such)-$Zellenanfang-strlen(ZELLENANFANG);
                               $Zellenende = strpos(substr($Zeilen[$Zeile][$vkey],$Zellenanfang),ZELLENENDE);
                               $Zelle = substr($Zeilen[$Zeile][$vkey],
                                 $Zellenanfang,$Zellenende+strlen(ZELLENENDE));
                               $Zellen = "";
                               foreach ( $vvalue as $vvkey => $vvvalue )
                               {
                                 $vvvalue = $this->xmlencode( $vvvalue );
                                 $vvvalue = str_replace("\n",ooInsertLineBreak(), $vvvalue);
                                 if ( isset($attribute[$key][$vkey][$vvkey]) )
                                 {
                                   if ( strpos($attribute[$key][$vkey][$vvkey],"b") !== false )
                                           $vvvalue = ooMakeBold($vvvalue);
                                   if ( strpos($attribute[$key][$vkey][$vvkey],"i") !== false )
                                           $vvvalue = ooMakeItalic($vvvalue);
                                 }
                                 $Zellen .= str_replace(POO_VAR_PREFIX.$key.POO_VAR_SUFFIX, $vvvalue,
                                   $Zelle);
                               }
                               $Zeilen[$Zeile][$vkey] = str_replace($Zelle, $Zellen, $Zeilen[$Zeile][$vkey]);
                               // Spaltenzähler erneuern
                               if ( ! $Spalteneingefuegt )
                               {
                                 $Pos = strpos($this->parsedDocuments[$file], POO_VAR_PREFIX.$key.POO_VAR_SUFFIX);
                                 $such = strrev(substr($this->parsedDocuments[$file],0,$Pos));
                                 $Zellenanfang = strpos($such,strrev(OOCOLUMN));
                                 $Zellenanfang = strlen($such)-$Zellenanfang; //-strlen(OOCOLUMN);
                                 $this->parsedDocuments[$file] =
                                   substr($this->parsedDocuments[$file],0,$Zellenanfang).
                                   " ".OOCOLUMNREPEATED.Count($vvalue).'" '.
                                   substr($this->parsedDocuments[$file],$Zellenanfang+1);
                                 $Spalteneingefuegt = true;
                               }
                             }
                             else
                             {
                               $vvalue = $this->xmlencode( $vvalue );
                               $vvalue = str_replace("\n",ooInsertLineBreak(), $vvalue);
                               if ( isset($attribute[$key][$vkey]) )
                               {
                                 if ( strpos($attribute[$key][$vkey],"b") !== false )
                                         $vvalue = ooMakeBold($vvalue);
                                 if ( strpos($attribute[$key][$vkey],"i") !== false )
                                         $vvalue = ooMakeItalic($vvalue);
                               }
                               $Zeilen[$Zeile][$vkey] = str_replace(POO_VAR_PREFIX.$key.POO_VAR_SUFFIX,
                                     $vvalue, $Zeilen[$Zeile][$vkey]);
                             }
                           }
                       }
                     }
                     else
                     {
                       $value = $this->xmlencode( $value );
                       $value = str_replace("\n",ooInsertLineBreak(), $value);
                       if ( isset($attribute[$key]) )
                       {
                         if ( strpos($attribute[$key],"b") !== false )
                                 $value = ooMakeBold($value);
                         if ( strpos($attribute[$key],"i") !== false )
                                 $value = ooMakeItalic($value);
                       }
                       $this->parsedDocuments[$file] = str_replace(POO_VAR_PREFIX.$key.POO_VAR_SUFFIX,
                                 $value, $this->parsedDocuments[$file]);
                     }
		  }
                   // ersetze Array-Zeilen ersetzen
                   if ( Count($Zeilen) > 0 )
                   {
                     foreach ( $Zeilen as $Zeile=>$Zeilenfeld)
                     {
                       $ersZeile = implode("",$Zeilenfeld);
                       $this->parsedDocuments[$file] = str_replace($Zeile,
                                     $ersZeile, $this->parsedDocuments[$file]);
                     }
                   }
		}
	}


	// encode string xml compatible
	function xmlencode($param)
	{
		$xml = $param;

		$xml = str_replace("&", "&amp;", $xml);
		$xml = str_replace(">", "&gt;", $xml);
		$xml = str_replace("<", "&lt;", $xml);
		$xml = str_replace("'", "&apos;", $xml);
		$xml = str_replace("\"", "&quot;", $xml);

		//$xml = utf8_encode($xml);  03.09. entfernt
		return $xml;
	}


	 function folderToZip($folder, &$zipFile, $exclusiveLength) {

    $handle = opendir($folder);

    while (false !== $f = readdir($handle)) {
      if ($f != '.' && $f != '..') {
        $filePath = "$folder/$f";
        // Remove prefix from file path before add to zip.
	$localPath = substr($filePath, $exclusiveLength);
        if (is_file($filePath)) {
          $zipFile->addFile($filePath, $localPath);
        } elseif (is_dir($filePath)) {
          // Add sub-directory.
          $zipFile->addEmptyDir($localPath);
          $this->folderToZip($filePath, $zipFile, $exclusiveLength);
        }
      }
    }
    closedir($handle);
  }

	// Save parsed document
	function savefile($filename)
	{
		global $archiveFiles;


		// Has file been extracted ?
		if($this->tmpDirName == "")
		{
			$this->handleError("No document loaded. Use loadDocument function first.", E_USER_ERROR);
		}


		// Is dir still there
		if(!is_dir(POO_TMP_PATH."/".$this->tmpDirName))
		{
			$this->handleError("Directory not found: ".$this->tmpDirName, E_USER_ERROR);
		}

		// Overwrite parsed documents
		foreach (array_keys($this->parserFiles) as $file)
		{
			$fp = fopen($this->parserFiles[$file], "w+");
			fputs($fp, $this->parsedDocuments[$file]);
			fclose($fp);
		}

		 // Added new library for ZIP 18.03.25
                $zip = new ZipArchive;
		$zip->open($filename, ZipArchive::CREATE);
		$this->folderToZip( POO_TMP_PATH."/".$this->tmpDirName, $zip, strlen(POO_TMP_PATH."/".$this->tmpDirName)+1);
		$zip->close();


/* removed 18.03.25
		// Create new (zip-)file - Add all files and subdirectories from temporary directory
		$archive = new PclZip($filename);
		$v_list = $archive->create(POO_TMP_PATH."/".$this->tmpDirName, PCLZIP_OPT_REMOVE_PATH, POO_TMP_PATH."/".$this->tmpDirName."/", PCLZIP_CB_PRE_ADD, "ooPreAdd");

		// zip.lib dirty hack
		$zip = new zipfile();
		
		// Add specials files without compression
		for($i = 0; $i < count($archiveFiles); $i++)
		{
			$file = $archiveFiles[$i];
			/*if( $file == "mimetype" || $file == "meta.xml" || substr( $file, 0, 9) == "Pictures/" )
			{
				$v_list = $archive->add(POO_TMP_PATH."/".$this->tmpDirName."/".$file, PCLZIP_OPT_REMOVE_PATH, POO_TMP_PATH."/".$this->tmpDirName."/", PCLZIP_OPT_NO_COMPRESSION);
			}
			else
			{
				$v_list = $archive->add(POO_TMP_PATH."/".$this->tmpDirName."/".$file, PCLZIP_OPT_REMOVE_PATH, POO_TMP_PATH."/".$this->tmpDirName."/");
			}
			*/

/* removed 18.03.25
			// zip.lib dirty hack
			$fp = fopen(POO_TMP_PATH."/".$this->tmpDirName."/".$file, "r");
			if ( filesize(POO_TMP_PATH."/".$this->tmpDirName."/".$file) > 0 )
			{ 
			  $content = fread($fp, filesize(POO_TMP_PATH."/".$this->tmpDirName."/".$file));
			}
			else
			  $content = '';
			fclose($fp);
			$zip->addFile($content, $file);

		}
		// Finally write file to disk => zip.lib dirty hack
		$fp = fopen($filename, "w+");
		fputs($fp, $zip->file());
		fclose($fp);
		*/
	}



	function download($filename)
	{
		// Build filename and save file temporarly to harddisk
		if($filename == "") $filename = $this->getRandomString(16);
		$info = pathinfo($this->zipFile);
		$fullfile = $filename.".".$info["extension"];
		$downloadFile = POO_TMP_PATH."/".$fullfile;
		$this->savefile($downloadFile);


		// Read temp file
		$fp = fopen($downloadFile, "r");
		$content = fread($fp, filesize($downloadFile));
		fclose($fp);


		// Build HTTP header and send file
		header("Expires: ".date("D, d M Y H:i:s", time() - 24 * 60 * 60)." GMT");	// expires in the past
		header("Last-Modified: ".gmdate("D, d M Y H:i:s")." GMT");			// Last modified, right now
		header("Cache-Control: no-cache, must-revalidate");				// Prevent caching, HTTP/1.1
		header("Pragma: no-cache");
		header("Content-Type: ".$this->mimetype);
		header('Content-Length: '.filesize($downloadFile));
		header('Content-Transfer-Encoding: binary');


		// (Browser specific)
		$browser = $_SERVER["HTTP_USER_AGENT"];
		if( preg_match('/MSIE 5.5/', $browser) || preg_match('/MSIE 6.0/', $browser) )
		{
			header('Content-Disposition: filename="'.$fullfile.'"');
		}
		else
		{
			header('Content-Disposition: attachment; filename="'.$fullfile.'"');
		}


		// Data
		echo $content;


		// Delete temp file
		unlink($downloadFile);
	}


	// Cleans up filesystem after job is done
	function clean()
	{
		if($this->tmpDirName == "")
			return;
		$tmpPath = POO_TMP_PATH."/".$this->tmpDirName;
		$this->deldir($tmpPath);
	}


	// Returns random string..easy, eh ?
	function getRandomString($length)
	{
		srand(date("s"));
		$possible_charactors = "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		$string = "";
		while(strlen($string)<$length)
		{
			$string .= substr($possible_charactors, rand()%(strlen($possible_charactors)), 1);
		}
		return($string);
	}


	// Default error handler
	function handleError($errorMessage, $errorType = E_USER_WARNING)
	{
		$prefix = 'phpOpenOffice ' . (($errorType == E_USER_ERROR) ? 'Error' : 'Warning') . ': ';
		echo $prefix . $errorMessage;

		if($errorType == E_USER_ERROR) die;
    	}


	// Borrowed from marcelognunez at hotmail dot com
	function deldir($dir)
	{
		$current_dir = opendir($dir);
		while($entryname = readdir($current_dir))
		{
			if(is_dir("$dir/$entryname") and ($entryname != "." and $entryname!=".."))
			{
				$this->deldir("$dir/$entryname");
			}
			elseif($entryname != "." and $entryname!="..")
			{
				unlink("$dir/$entryname");
		}
		}
		closedir($current_dir);
		rmdir($dir);
	}

}

?>
