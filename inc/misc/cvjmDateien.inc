<?php

function Dateihochladen($Verzeichnis, $Parameter)
{
  global $_FILES;
  if ( isset($_FILES[$Parameter]["tmp_name"]) &&
         $_FILES[$Parameter]["tmp_name"] != "" &&
         $_FILES[$Parameter]['tmp_name'] != 'none')
         {
         mkdirs ($Verzeichnis, 0750);
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

function restructureFiles(array $files): array {
    $result = [];

    foreach ($files as $fieldName => $fileData) {

        // Einzeldatei (kein multiple)
        if (!is_array($fileData['name'])) {
            $result[] = [
                'field'    => $fieldName,
                'name'     => $fileData['name'],
                'type'     => $fileData['type'],
                'tmp_name' => $fileData['tmp_name'],
                'error'    => $fileData['error'],
                'size'     => $fileData['size']
            ];
            continue;
        }

        // Mehrere Dateien
        foreach ($fileData['name'] as $i => $name) {
            $result[] = [
                'field'    => $fieldName,
                'name'     => $name,
                'type'     => $fileData['type'][$i],
                'tmp_name' => $fileData['tmp_name'][$i],
                'error'    => $fileData['error'][$i],
                'size'     => $fileData['size'][$i]
            ];
        }
    }

    return $result;
}

?>