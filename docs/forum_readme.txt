 _  _   _
|_ | | |_| | | |\/|
|  |_| | \ |_| |  |
===================

Version 1.07 (testing)
last update 06.01.2002

About
-----
This forum is a content module for the AWF.

File list
---------
docs/forum_readme.txt
inc/data/smilies.conf
inc/data/forum_buttonlist.conf
inc/lang/forum.de
inc/lang/forum.en
inc/cmodules/forum_overview.inc
inc/cmodules/forum_forum.inc
inc/cmodules/forum_posting.inc
inc/cmodules/forum_mailto.inc
inc/misc/forum_funcitons.inc
img/buttons/forum/icon_quote_topic2.gif
img/buttons/forum/icon_reply_topic2.gif
img/buttons/forum/icon_quote_topic3.gif
img/buttons/forum/icon_reply_topic3.gif
img/smilies/biggrin.gif
img/smilies/cool.gif
img/smilies/frown.gif
img/smilies/redface.gif
img/smilies/smile.gif
img/smilies/wink.gif
img/smilies/confused.gif
img/smilies/eek.gif
img/smilies/mad.gif
img/smilies/rolleyes.gif
img/smilies/tongue.gif
img/smilies/crazy.gif 
img/smilies/vader.gif 
img/smilies/money.gif 
img/smilies/alien.gif 
img/smilies/alienoh.gif 
img/smilies/alienlaugh.gif 
img/smilies/alienangry.gif

Installation instructions
-------------------------
1.Extract the archive directly into your awf root directory.
2.Use the types configuration tool (admin/types.php) and create the 4 new types:
  2.1 Name: forum_overview
      Description: Forum Overview
      Module: forum_overview
      Parent type: root
      Visible: YES

  2.2 Name: forum_forum
      Description: Forum
      Module: forum_forum
      Parent type: forum_overview
      Visible: Yes

  2.3 Name: forum_posting
      Description: Forum Posting
      Module: forum_posting
      Parent type: forum_forum
      Visible: NO

  2.4 Name: forum_mailto
      Description: Forum Mailto
      Module: Forum Mailto
      Parent type: forum_posting
      Visible: NO
3. Now log in as editor and add the forum with the "Create new document" module.

Uninstall instructions
----------------------
1. Login as editor and remove the forum from your website with the "Remove document" option.
2. Use the types configuration tool (admin/types.php) and delete the 4 forum types (forum_overview, forum_forum, forum_posting, forum_mailto).
3. Delete all files you've extracted at the installation. (look at the file list)

Changelog
---------
-Version 1.07 (testing)  06.01.02 changed by Christoph Stickel <mixer2@web.de>
	*removed files:
		img/buttons/forum/icon_quote_topic2.gif
		img/buttons/forum/icon_reply_topic2.gif
	*moved file:
		inc/cmodules/forum_funcitons.inc
		to
		inc/misc/forum_funcitons.inc
	*function transform() is used everywhere now
	*to long nicknames/titles will be cutted from now on
	*fixed some language bugs	

-Version 1.06 (untested) 20.12.01 changed by Christoph Stickel <mixer2@web.de>
	*the awf cache function is enabled again at the forum documents by default.
	*icons moved to img/buttons/forum
	*added file inc/data/forum_buttonlist.conf
	*icons are now chooseable at the editor screen
	*added some funny smilies

-Version 1.05 (untested) 17.12.01 changed by Christoph Stickel <mixer2@web.de>
        *adapted the mail layout to awf default
        *use the docinput array for most variables
        *security improved (strip_tags and htmlentities are used everywhere)

-Version 1.04 (untested) 09.12.01 changed by Christoph Stickel <mixer2@web.de>
	*fixed the "Page:" bug in the single posting view
	*fixed the bug, which prevented the forum options from changing.

-Version 1.03 (untested) 08.12.01 changed by Christoph Stickel <mixer2@web.de>
	*the smilies are working again
	*fixed the bug with nickname and instant message in forum_posting

-Version 1.02 (untested) 07.12.01 changed by Christoph Stickel <mixer2@web.de>
	*the awf cache function is complete disabled at the forum documents by default, because of some problems.
	 (searching for a better solution)
	*fixed the forum_mailto document creating bug, from the forum_forum module.
	*the child will get the members only option from it's parent at creation.
	 (attention! if you change the members only option the existing children will not adapt themself)

-Version 1.01 (untested) 01.12.01 changed by Christoph Stickel <mixer2@web.de>
	*performance improved
	*readme updated
	*added language files
	*moved all functions to forum_functions.inc
	*reduced source code in length

-Version 1.00 (untested) 25.11.01 changed by Christoph Stickel <mixer2@web.de>
	*performance improved
	*readme updated
	*count of threads field added at the forum_overview
	*fixed some design&logic bugs
	*"new"-sign added
