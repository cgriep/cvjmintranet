PHP RDF Parser is yet another PHP class that parses and outputs netscape's
rdf/rss (whichever you would like to call them) files. PHP RDF Parser can
easily be integrated into existing web content with an easy output function,
or you can use the variables within the class to arrange the data however you
want.
Copyright(c) Jason Williams (jason@nerdzine.net)
Licensed under the GNU/GPL

---------------------------------------------------------------------
Installation:
----------
There is no installation needed. Just copy the rdf_class.php file to a
directory accessible to your php scripts.

---------------------------------------------------------------------
Quick & Easy Use:
----------
The basic steps for using PHP RDF Parser are, declare the use of the class,
declare a new class, parse the file, and output the results.
Here's some sample code to get you started:

require ("/home/httpd/html/rdf_class.php"); //notice the full file system path
$channel = new rdfFile("/home/httpd/html/news.rdf"); //again, full system path
$channel->parse(True);
$channel->ReturnTable(True, "#00FF00", "#FFFFFF", "200");

The sample code above outputs a table that is 200 pixels wide with the title
cell (at the top) with a background color of green and a main body text
background color of white. The comments for each item will be listed along
with the links/titles for each item.
Also, the True parameter value for the parse function tells the parser to
check for encoding mistakes. This is handy in case the author of the rdf
file forgot to strickly use Latin-1 encoding. In essence, making this value
True will get rid of all rogue ampersands in the rdf/rss file.

More Complex Use:
----------
The basic steps are still the same. You must require the class file, create a
new rdfFile class object and parse the channel. You do not, however, have to
use the built-in function ReturnTable. You can, instead, use the storage
variables within the class itself.
For complete documentation and a list of the internal class variables you may
use, visit:
http://www.nerdzine.net/php_rdf/

---------------------------------------------------------------------
If you have questions, comments, or code revisions, please email
jason@nerdzine.net. Your feedback is always appreciated.
