<?php

function erstelleCode($laenge)
{
  $tmpfname = tempnam ("/tmp", "");
  unlink($tmpfname);
  return basename($tmpfname);
}

$datei = fopen("/webdata/internetcodes.txt","r");
$Codes = array();
while ( $zeile = fgets($datei) )
{
  echo nl2br($zeile);
  list($Code,$Laenge) = explode("=",$zeile);
  if ( substr($Code,0,7) != "BENUTZT" )
    $Codes[] = $Code;
}
fclose($datei);
if ( isset($_REQUEST["Neu"]) && is_numeric($_REQUEST["Neu"]) )
{
  $datei = fopen("/webdata/internetcodes.txt","a");
  $Laenge = 15;
  if ( isset($_REQUEST["Laenge"]) && is_numeric($_REQUEST["Laenge"]) )
    $Laenge = $_REQUEST["Laenge"];
  for ( $i = 0; $i < $_REQUEST["Neu"]; $i++ )
  {
    do
    {
      // Code ausdenken
      $Code = erstelleCode(8);
    }
    while ( in_array($Code, $Codes) );
    echo "Neuer Code: $Code<br />";
    fwrite($datei, $Code."=".$Laenge."\n");
  }
  fclose($datei);

}

?>