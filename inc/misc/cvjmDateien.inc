<?php

function Dateihochladen($Verzeichnis, $Parameter)
{
  global $_FILES;
  if ( isset($_FILES[$Parameter]["tmp_name"]) &&
         $_FILES[$Parameter]["tmp_name"] != "" &&
         $_FILES[$Parameter]['tmp_name'] != 'none')
         {
         mkdirs ($Verzeichnis, 0777);
     if(function_exists('move_uploaded_file')) {
       if (file_exists($Verzeichnis.basename($_FILES[$Parameter]['name'])))
         rename ($Verzeichnis.basename($_FILES[$Parameter]['name']),
            $Verzeichnis."Version vom ".date("d.m.Y")."-".basename($_FILES[$Parameter]['name']));
       move_uploaded_file($_FILES[$Parameter]['tmp_name'],
            $Verzeichnis.basename($_FILES[$Parameter]['name']));
       chmod($Verzeichnis.basename($_FILES[$Parameter]['name']), CONFIG_CREATE_MASK);
     }
         }
}

function Dateiloeschen($Verzeichnis, $Parameter)
{
  if ( isset($_REQUEST[$Parameter]))
  {
    $_REQUEST[$Parameter] = str_replace("..","",str_replace("/","",$_REQUEST[$Parameter]));
      unlink($Verzeichnis.$_REQUEST[$Parameter]);
  }
}

?>