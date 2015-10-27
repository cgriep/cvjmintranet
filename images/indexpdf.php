<?

// Globals...
global $HTTP_GET_VARS;
global $HTTP_HOST;
global $HTTP_REFERER;
global $PHP_SELF;


//
// 'topdf()' - Convert the named file/URL to PDF.
//

function topdf($filename, $options = "") {
    # Write the content type to the client...
    header("Content-Type: application/pdf");
    header("Content-Disposition: inline; filename=\"pdf-o-matic.pdf\"");
    flush();

    # Run HTMLDOC to provide the PDF file to the user...
    # Use the --no-localfiles option for enhanced security!
    $ss = "htmldoc --no-localfiles --no-compression -t pdf14 --quiet ";
    $ss .= "--jpeg --webpage "
            ."$options '$filename'";
    passthru($ss);
}

$url = "";
while ( list($key, $value) = each ($_REQUEST) )
{
  if ( is_array($value)) {
    while (list($k, $v) = each($value))
      $url .= $key."[".$k."]=$v&";
  }
    else $url .= "$key=$value&";
}
$content = file_get_contents("http://192.168.100.102/index.php?$url");
// Inhalt schreiben
$datnam = tempnam("PDF", "forpdf");
echo $datname;
if ($file = fopen($datname.".html", 'w')) {
  $n = fwrite($file, $content);
  fclose($file);
}
topdf($datname.".html",
      "--header t.D --footer ./. --size letter --left 0.5in");
//unlink($datname.".html");
?>


