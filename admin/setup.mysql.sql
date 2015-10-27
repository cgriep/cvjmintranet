-- MySQL dump 8.22
--
-- Host: localhost    Database: awf
---------------------------------------------------------
-- Server version	3.23.51

--
-- Table structure for table '1_ads'
--

CREATE TABLE 1_ads (
  id int(11) unsigned NOT NULL auto_increment,
  destination varchar(255) NOT NULL default '',
  counter int(11) unsigned NOT NULL default '0',
  host varchar(255) NOT NULL default '',
  views int(11) unsigned NOT NULL default '0',
  url varchar(255) NOT NULL default '',
  banner varchar(64) NOT NULL default '',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_ads'
--



--
-- Table structure for table '1_blobs'
--

CREATE TABLE 1_blobs (
  id int(11) NOT NULL auto_increment,
  sid varchar(128) NOT NULL default '',
  data blob NOT NULL,
  mime varchar(128) NOT NULL default '',
  target varchar(128) NOT NULL default '',
  target_id int(11) NOT NULL default '0',
  filename varchar(128) NOT NULL default '',
  created int(11) NOT NULL default '0',
  last_change int(11) NOT NULL default '0',
  PRIMARY KEY  (id),
  UNIQUE KEY sid (sid)
) TYPE=MyISAM;

--
-- Dumping data for table '1_blobs'
--



--
-- Table structure for table '1_code'
--

CREATE TABLE 1_code (
  id int(11) NOT NULL auto_increment,
  serial int(10) unsigned NOT NULL default '0',
  active int(1) unsigned NOT NULL default '1',
  category varchar(16) NOT NULL default 'unknown',
  name varchar(32) NOT NULL default '',
  type varchar(8) NOT NULL default '',
  code text NOT NULL,
  filename text NOT NULL,
  description text NOT NULL,
  created int(10) unsigned NOT NULL default '0',
  last_change int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_code'
--


INSERT INTO 1_code VALUES (1,0,1,'adf_inputs','code','db','return db_create_docinput($attributes[\'name\'], $attributes[\'format\'], TABLE_CODE, \'id\', $target_id, $autosave, true);','','',0,1036726655);
INSERT INTO 1_code VALUES (2,0,1,'adf_strings','code','db','return db_get_string($attributes[\'name\'], $attributes[\'type\'], TABLE_CODE, \'id\', $target_id);','','',0,1038534418);
INSERT INTO 1_code VALUES (3,0,1,'adf_strings','items','db','return items_get_string($attributes[\'name\'], $target, $target_id);','','',0,1038534111);
INSERT INTO 1_code VALUES (4,0,1,'adf_inputs','items','db','return items_create_docinput($attributes[\'name\'], $target, $target_id, $attributes[\'format\'], $autosave);','','',0,1038534304);
INSERT INTO 1_code VALUES (8,0,1,'adf_strings','userlogin','db','return db_get_string($attributes[\'name\'], $attributes[\'format\'], TABLE_USERS, \'id\', $target_id);','','',0,1038534142);
INSERT INTO 1_code VALUES (6,0,1,'adf_inputs','userlogin','db','return db_create_docinput($attributes[\'name\'], $attributes[\'format\'], TABLE_USERS, \'id\', $target_id, $autosave, false);','','',0,1038534171);
INSERT INTO 1_code VALUES (7,0,1,'adf_inputs','userprofile','db','return profile_create_docinput($attributes[\'name\'], $attributes[\'format\'], $target_id, $autosave);','','',0,1038534336);
INSERT INTO 1_code VALUES (9,0,1,'adf_strings','userprofile','db','return profile_get_string($attributes[\'name\'], $attributes[\'type\'], $target_id);','','',0,1038534383);
INSERT INTO 1_code VALUES (10,0,1,'adf_tags','input','db','adf_create_docinput($attributes, $target, $target_id, $save, $tables);','','',0,1036960781);
INSERT INTO 1_code VALUES (11,0,1,'adf_tags','string','db','echo \'<span class=\"text\">\'.transform(adf_get_string($attributes, $target, $target_id),$attributes[\'format\']).\'</span>\';','','',0,1038534211);
INSERT INTO 1_code VALUES (12,0,1,'adf_tags','space','db','echo \' \';','','',0,0);
INSERT INTO 1_code VALUES (13,0,1,'adf_tags','text','db','$text = $node->child_nodes();\r\necho \'<span class=\"text\">\'.nl2br(utf8_decode($text[0]->content)).\'</span>\';','','',0,0);
INSERT INTO 1_code VALUES (14,0,1,'adf_tags','title','db','$text = $node->child_nodes();\r\necho \'<span class=\"title\">\'.utf8_decode($text[0]->content).\'</span>\';','','',0,1036730815);
INSERT INTO 1_code VALUES (15,0,1,'adf_tags','link','db','$text = $node->child_nodes();\r\necho \'<a href=\"\'.$node->get_attribute(\'url\').\'\">\'.utf8_decode($text[0]->content).\'</a>\';','','',0,0);
INSERT INTO 1_code VALUES (16,0,1,'adf_tags','br','db','echo \'<br />\';','','',0,0);
INSERT INTO 1_code VALUES (19,0,1,'adf_inputs','db_table','db','return db_create_docinput ($attributes[\'name\'], $attributes[\'format\'], $attributes[\'table\'], $tables[$attributes[\'table\']][\'key\'], $target_id, $autosave, $tables[$attributes[\'table\']][\'set_timestamp\']);','','DB Table Input',0,1037843158);
INSERT INTO 1_code VALUES (20,0,1,'adf_strings','db_table','db','return db_get_string($attributes[\'name\'], $attributes[\'format\'], $attribute[\'table\'], $tables[$attribute[\'table\']][\'key\'], $target_id);','','DB Table String Output',0,0);
INSERT INTO 1_code VALUES (21,0,1,'adf_functions','default','file','','inc/misc/adf_functions.inc','ADF Function LIB',0,0);

--
-- Table structure for table '1_companies'
--

CREATE TABLE 1_companies (
  id int(11) NOT NULL auto_increment,
  company varchar(255) NOT NULL default '',
  address varchar(128) NOT NULL default '',
  zip varchar(128) NOT NULL default '',
  city varchar(128) NOT NULL default '',
  country char(2) NOT NULL default 'DE',
  phone varchar(128) NOT NULL default '',
  fax varchar(128) NOT NULL default '',
  web varchar(128) NOT NULL default '',
  description text NOT NULL,
  created int(10) unsigned NOT NULL default '0',
  last_change int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (id),
  UNIQUE KEY company (company)
) TYPE=MyISAM;

--
-- Dumping data for table '1_companies'
--



--
-- Table structure for table '1_flavours'
--

CREATE TABLE 1_flavours (
  id int(11) unsigned NOT NULL auto_increment,
  name varchar(30) NOT NULL default '',
  published tinyint(1) unsigned NOT NULL default '0',
  lang char(2) NOT NULL default 'en',
  PRIMARY KEY  (id),
  UNIQUE KEY name (name)
) TYPE=MyISAM;

--
-- Dumping data for table '1_flavours'
--


INSERT INTO 1_flavours VALUES (1,'English',1,'en');
INSERT INTO 1_flavours VALUES (2,'German',1,'de');
INSERT INTO 1_flavours VALUES (3,'French',0,'en');

--
-- Table structure for table '1_groupdata'
--

CREATE TABLE 1_groupdata (
  id int(11) unsigned NOT NULL auto_increment,
  group_id int(11) unsigned NOT NULL default '0',
  name text NOT NULL,
  value text NOT NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_groupdata'
--


INSERT INTO 1_groupdata VALUES (7,1,'editor','1');
INSERT INTO 1_groupdata VALUES (2,1,'edithost_all','1');
INSERT INTO 1_groupdata VALUES (8,6,'allow_upload','1');
INSERT INTO 1_groupdata VALUES (9,6,'allow_delete','1');
INSERT INTO 1_groupdata VALUES (10,6,'allow_createdir','1');

--
-- Table structure for table '1_groups'
--

CREATE TABLE 1_groups (
  id int(11) unsigned NOT NULL auto_increment,
  group_name varchar(32) NOT NULL default '',
  PRIMARY KEY  (id),
  UNIQUE KEY name (group_name)
) TYPE=MyISAM;

--
-- Dumping data for table '1_groups'
--


INSERT INTO 1_groups VALUES (1,'Administrators');

--
-- Table structure for table '1_itemmap'
--

CREATE TABLE 1_itemmap (
  id int(11) NOT NULL auto_increment,
  target varchar(128) NOT NULL default '1_users',
  target_id int(11) NOT NULL default '0',
  item_id int(11) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY target (target)
) TYPE=MyISAM;

--
-- Dumping data for table '1_itemmap'
--



--
-- Table structure for table '1_items'
--

CREATE TABLE 1_items (
  id int(11) NOT NULL auto_increment,
  category varchar(64) NOT NULL default '',
  caption varchar(255) NOT NULL default '',
  sort_order int(11) NOT NULL default '0',
  active tinyint(4) NOT NULL default '1',
  PRIMARY KEY  (id),
  KEY category (category)
) TYPE=MyISAM;

--
-- Dumping data for table '1_items'
--


INSERT INTO 1_items VALUES (1,'title','Herr',1,1);
INSERT INTO 1_items VALUES (2,'title','Frau',2,1);

--
-- Table structure for table '1_messages'
--

CREATE TABLE 1_messages (
  id int(11) unsigned NOT NULL auto_increment,
  user_id int(11) unsigned NOT NULL default '0',
  subject text NOT NULL,
  message text NOT NULL,
  attachment blob,
  sender text NOT NULL,
  sender_id int(11) NOT NULL default '0',
  type varchar(8) NOT NULL default 'default',
  status tinyint(1) unsigned NOT NULL default '0',
  created int(11) unsigned NOT NULL default '0',
  delivered int(11) unsigned NOT NULL default '0',
  release_date int(11) unsigned NOT NULL default '0',
  expiration_date int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY user_id (user_id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_messages'
--



--
-- Table structure for table '1_modules'
--

CREATE TABLE 1_modules (
  id int(11) unsigned NOT NULL auto_increment,
  sort_order int(11) NOT NULL default '0',
  name varchar(64) NOT NULL default 'unknown',
  placement varchar(20) NOT NULL default 'right',
  flavour_id int(11) NOT NULL default '-1',
  section_id int(11) NOT NULL default '-1',
  document_id int(11) NOT NULL default '-1',
  visible tinyint(1) unsigned NOT NULL default '1',
  removeable tinyint(1) unsigned NOT NULL default '1',
  target tinyint(1) NOT NULL default '0',
  caption text NOT NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_modules'
--


INSERT INTO 1_modules VALUES (1,4,'navigation.inc','left',-1,-1,-1,1,0,0,'Navigation');
INSERT INTO 1_modules VALUES (2,2,'login.inc','right',-1,-1,-1,1,0,0,'Login');
INSERT INTO 1_modules VALUES (3,9,'design.inc','left',-1,-1,-1,1,1,0,'Design');
INSERT INTO 1_modules VALUES (4,15,'search.inc','left',-1,-1,-1,1,1,0,'Search');
INSERT INTO 1_modules VALUES (5,10,'links.inc','right',-1,-1,3,1,1,0,'Links');
INSERT INTO 1_modules VALUES (6,1,'polls.inc','right',-1,-1,-1,1,1,2,'Polls');
INSERT INTO 1_modules VALUES (7,1,'newdoc.inc','right',-1,-1,-1,1,0,3,'Create new document');
INSERT INTO 1_modules VALUES (8,5,'flavours.inc','right',-1,-1,-1,1,1,0,'Flavours');
INSERT INTO 1_modules VALUES (90,1,'docoptions.inc','right',-1,-1,-1,1,1,3,'Options');
INSERT INTO 1_modules VALUES (10,1,'polls.inc','left',-1,-1,-1,0,1,0,'Umfrage');
INSERT INTO 1_modules VALUES (11,1,'news.inc','right',-1,-1,-1,1,1,0,'Latest News');

--
-- Table structure for table '1_nodedata'
--

CREATE TABLE 1_nodedata (
  id int(11) unsigned NOT NULL auto_increment,
  value text NOT NULL,
  flavour_id int(11) NOT NULL default '0',
  version varchar(8) NOT NULL default '0',
  node_id int(11) NOT NULL default '0',
  name varchar(32) NOT NULL default '',
  datatype varchar(16) NOT NULL default 'notrans',
  PRIMARY KEY  (id),
  KEY flavour_id (flavour_id),
  KEY node_id (node_id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_nodedata'
--


INSERT INTO 1_nodedata VALUES (1,'',2,'0',1,'keywords','notrans');
INSERT INTO 1_nodedata VALUES (2,'Home',2,'0',1,'title','clean');
INSERT INTO 1_nodedata VALUES (3,'1',2,'0',1,'comments','notrans');
INSERT INTO 1_nodedata VALUES (4,'a',2,'0',1,'author','email');
INSERT INTO 1_nodedata VALUES (5,'1010316459',2,'0',1,'timestamp','datetime');
INSERT INTO 1_nodedata VALUES (6,'1',2,'0',1,'author_id','user_id');
INSERT INTO 1_nodedata VALUES (7,'0',0,'0',1,'hide_right_col','notrans');
INSERT INTO 1_nodedata VALUES (8,'files/0/1/public/image.jpg',1,'0',1,'image','image_hide');
INSERT INTO 1_nodedata VALUES (9,'a',1,'0',1,'author','email');
INSERT INTO 1_nodedata VALUES (10,'1',1,'0',1,'author_id','user_id');
INSERT INTO 1_nodedata VALUES (11,'1010701447',1,'0',1,'timestamp','datetime');
INSERT INTO 1_nodedata VALUES (12,'Home',1,'0',1,'title','clean');
INSERT INTO 1_nodedata VALUES (13,'',1,'0',2,'keywords','notrans');
INSERT INTO 1_nodedata VALUES (14,'XML Demo',1,'0',2,'title','clean');
INSERT INTO 1_nodedata VALUES (15,'<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<adf>\r\n<context name=\"functions\" label=\"Code Overview\" mode=\"list\" wrap=\"50\">\r\n<db_table name=\"1_code\" type=\"primary\" key=\"id\" />\r\n<col source=\"db_table\" name=\"id\" format=\"number\" label=\"ID\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"category\" label=\"Category\" format=\"text\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"name\" label=\"Name\" link=\"2.html?page=document_edit&amp;target_id=%%id%%\" format=\"text\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"type\" label=\"Type\" format=\"text\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"description\" label=\"Description\" format=\"text\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"created\" label=\"Created\" format=\"date\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"last_change\" label=\"Change\" format=\"date\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"active\" changeable=\"true\" label=\"Active\" format=\"bool\" table=\"1_code\"/>\r\n<col type=\"edit\" name=\"edit\" label=\"L&#246;schen\" function=\"delete\"/>\r\n</context>\r\n\r\n<context name=\"add_code\" label=\"Add code\" mode=\"insert\">\r\n<db_table name=\"1_code\" type=\"primary\" key=\"id\" set_timestamp=\"true\"/>\r\n<col source=\"db_table\" name=\"category\" values=\"adf_inputs,adf_strings,adf_tags,adf_functions,transform\" label=\"Category\" format=\"text\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"name\" label=\"Name\" format=\"text\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"type\" values=\"db,file,remote\" label=\"Type\" format=\"text\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"description\" label=\"Description\" format=\"text\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"code\" label=\"Code\" format=\"textarea\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"filename\" label=\"Filename (optional)\" format=\"text\" table=\"1_code\"/>\r\n<col source=\"db_table\" name=\"active\" label=\"Active\" format=\"bool\" table=\"1_code\"/>\r\n</context>\r\n\r\n\r\n<context name=\"document_edit\" label=\"Edit Code\" mode=\"edit\" target=\"code\">\r\n<text>Category</text><br/>\r\n<input name=\"category\" source=\"code\" format=\"text\"/>\r\n<br/><br/>\r\n<text>Name</text><br/>\r\n<input name=\"name\" source=\"code\" format=\"text\"/>\r\n<br/><br/>\r\n<text>Type</text><br/>\r\n<input name=\"type\" source=\"code\" format=\"text\"/>\r\n<br/><br/>\r\n<text>Description</text><br/>\r\n<input name=\"description\" source=\"code\" format=\"text\"/>\r\n<br/><br/>\r\n<text>Code</text><br/>\r\n<input name=\"code\" source=\"code\" format=\"textarea\"/>\r\n<br/><br/>\r\n<text>Filename (optional)</text><br/>\r\n<input name=\"filename\" source=\"code\" format=\"text\"/>\r\n<br/><br/>\r\n<text>Serial</text><br/>\r\n<input name=\"serial\" source=\"code\" format=\"number\"/>\r\n<br/><br/>\r\n<text>Active</text><br/>\r\n<input name=\"active\" source=\"code\" format=\"bool\"/>\r\n</context>\r\n\r\n<context name=\"example\" label=\"Example\" mode=\"edit\" target=\"userlogin\" target_id=\'\".SESSION_DBID.\"\'>\r\n<db_table name=\"1_users\" key=\"id\" set_timestamp=\"false\"/>\r\n<db_table name=\"1_code\" key=\"id\" set_timestamp=\"true\"/>\r\n<text>db_table eMail</text><br/>\r\n<input name=\"email\" source=\"db_table\" table=\"1_users\" format=\"text\"/><br />\r\n<input name=\"code\" source=\"db_table\" table=\"1_code\" format=\"text\"/>\r\n<br/><br/>\r\n<text>eMail</text><br/>\r\n<input name=\"email\" source=\"userlogin\" format=\"text\"/>\r\n<br/><br/>\r\n<text>Registered</text><br/>\r\n<string name=\"registered\" source=\"userlogin\" format=\"timestamp\"/>\r\n<br/><br/>\r\n<text>Nickname</text><br/>\r\n<input name=\"nickname\" source=\"userprofile\" format=\"text\"/>\r\n<br/><br/>\r\n</context>\r\n\r\n\r\n<context name=\"export\" label=\"Code export\" mode=\"export\">\r\n<db_table name=\"1_code\" type=\"primary\" key=\"id\" />\r\n<col type=\"db\" name=\"id\" format=\"number\" label=\"ID\" table=\"1_code\"/>\r\n<col type=\"db\" name=\"category\" label=\"Category\" format=\"text\" table=\"1_code\"/>\r\n<col type=\"db\" name=\"name\" label=\"Name\" link=\"2.html?page=document_edit&amp;target_id=%%id%%\" format=\"text\" table=\"1_code\"/>\r\n<col type=\"db\" name=\"type\" label=\"Type\" format=\"text\" table=\"1_code\"/>\r\n<col type=\"db\" name=\"description\" label=\"Description\" format=\"text\" table=\"1_code\"/>\r\n<col type=\"db\" name=\"active\" changeable=\"true\" label=\"Active\" format=\"bool\" table=\"1_code\"/>\r\n</context>\r\n\r\n\r\n</adf>\r\n',1,'0',2,'body','notrans');
INSERT INTO 1_nodedata VALUES (16,'a',1,'0',2,'author','email');
INSERT INTO 1_nodedata VALUES (17,'1',1,'0',2,'author_id','user_id');
INSERT INTO 1_nodedata VALUES (18,'1038542684',1,'0',2,'timestamp','datetime');

--
-- Table structure for table '1_nodes'
--

CREATE TABLE 1_nodes (
  id int(11) unsigned NOT NULL auto_increment,
  sort_order int(11) NOT NULL default '0',
  parent_id int(11) unsigned NOT NULL default '0',
  enabled tinyint(1) unsigned NOT NULL default '1',
  searchable tinyint(1) unsigned NOT NULL default '1',
  cacheable tinyint(1) unsigned NOT NULL default '1',
  type_id int(11) unsigned NOT NULL default '0',
  published tinyint(1) unsigned NOT NULL default '1',
  description varchar(128) NOT NULL default '',
  key_id int(11) unsigned NOT NULL default '0',
  release_date int(11) unsigned NOT NULL default '0',
  expiration_date int(11) unsigned NOT NULL default '0',
  members_only tinyint(1) unsigned NOT NULL default '0',
  ssl_only tinyint(1) unsigned NOT NULL default '0',
  views int(11) unsigned NOT NULL default '0',
  created int(11) NOT NULL default '0',
  last_change int(11) NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY parent_id (parent_id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_nodes'
--


INSERT INTO 1_nodes VALUES (1,20,0,1,1,1,2,1,'',0,0,0,0,0,470,0,0);
INSERT INTO 1_nodes VALUES (2,1,1,1,1,1,40,1,'',0,0,0,0,0,1308,1031076574,1038542684);

--
-- Table structure for table '1_polls'
--

CREATE TABLE 1_polls (
  id int(11) unsigned NOT NULL auto_increment,
  question varchar(128) NOT NULL default '',
  active tinyint(1) unsigned NOT NULL default '1',
  sortorder int(11) NOT NULL default '0',
  module_id int(11) NOT NULL default '0',
  last_access timestamp(14) NOT NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_polls'
--


INSERT INTO 1_polls VALUES (1,'This web site is...',0,1,0,20021107042435);
INSERT INTO 1_polls VALUES (2,'What OS are you using',1,2,13,20011223180121);

--
-- Table structure for table '1_polls_items'
--

CREATE TABLE 1_polls_items (
  id int(11) unsigned NOT NULL auto_increment,
  poll_id int(11) unsigned NOT NULL default '0',
  text varchar(128) NOT NULL default '',
  counter int(11) unsigned NOT NULL default '0',
  last_access timestamp(14) NOT NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_polls_items'
--


INSERT INTO 1_polls_items VALUES (4,1,'quite ok',6,20011218003406);
INSERT INTO 1_polls_items VALUES (1,1,'nice',3,20011224090150);
INSERT INTO 1_polls_items VALUES (3,1,'useless',1,20011223110005);
INSERT INTO 1_polls_items VALUES (5,2,'Windows 9X/ME',0,20011223100433);
INSERT INTO 1_polls_items VALUES (6,2,'Windows NT/2000',1,20011223113028);
INSERT INTO 1_polls_items VALUES (7,2,'Windows XP',1,20020105214457);
INSERT INTO 1_polls_items VALUES (8,2,'Linux',2,20020106142510);
INSERT INTO 1_polls_items VALUES (9,2,'OS/2',0,20011223100506);
INSERT INTO 1_polls_items VALUES (10,2,'MacOS',0,20011223100524);
INSERT INTO 1_polls_items VALUES (11,2,'DOS',0,20011223100537);
INSERT INTO 1_polls_items VALUES (12,2,'Other',0,20011223100552);

--
-- Table structure for table '1_redirect'
--

CREATE TABLE 1_redirect (
  id int(11) unsigned NOT NULL auto_increment,
  url text NOT NULL,
  referer text NOT NULL,
  views int(11) unsigned default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_redirect'
--



--
-- Table structure for table '1_setup'
--

CREATE TABLE 1_setup (
  id int(11) unsigned NOT NULL auto_increment,
  name varchar(20) NOT NULL default '',
  value varchar(128) default NULL,
  PRIMARY KEY  (id),
  UNIQUE KEY name (name)
) TYPE=MyISAM;

--
-- Dumping data for table '1_setup'
--


INSERT INTO 1_setup VALUES (1,'base_path','/usr1/public/documents/net/awf/');
INSERT INTO 1_setup VALUES (2,'webmaster_mail','michael@localhost');
INSERT INTO 1_setup VALUES (3,'site_title','Demo');
INSERT INTO 1_setup VALUES (4,'image_rpath','img/');
INSERT INTO 1_setup VALUES (5,'inc_path','inc/');
INSERT INTO 1_setup VALUES (6,'default_document','1');
INSERT INTO 1_setup VALUES (7,'design_inc','design.inc');
INSERT INTO 1_setup VALUES (8,'init_inc','init.inc');
INSERT INTO 1_setup VALUES (9,'header_inc','header.inc');
INSERT INTO 1_setup VALUES (10,'content_inc','content.inc');
INSERT INTO 1_setup VALUES (11,'footer_inc','footer.inc');
INSERT INTO 1_setup VALUES (13,'module_path','modules/');
INSERT INTO 1_setup VALUES (14,'design_path','design/');
INSERT INTO 1_setup VALUES (15,'cmodules_path','cmodules/');
INSERT INTO 1_setup VALUES (16,'default_background','#ffffff');
INSERT INTO 1_setup VALUES (17,'default_linkcolor','#ff0000');
INSERT INTO 1_setup VALUES (18,'default_textcolor','#000000');
INSERT INTO 1_setup VALUES (19,'default_vlinkcolor','#aa0000');
INSERT INTO 1_setup VALUES (20,'default_design','CreativeThings');
INSERT INTO 1_setup VALUES (21,'default_header','default');
INSERT INTO 1_setup VALUES (22,'default_flavour','1');
INSERT INTO 1_setup VALUES (23,'data_path','data/');
INSERT INTO 1_setup VALUES (24,'header_path','header/');
INSERT INTO 1_setup VALUES (25,'footer_path','footer/');
INSERT INTO 1_setup VALUES (30,'count_views','1');
INSERT INTO 1_setup VALUES (27,'um_store_method','sql');
INSERT INTO 1_setup VALUES (28,'show_warnings','1');
INSERT INTO 1_setup VALUES (29,'cache_time','0');
INSERT INTO 1_setup VALUES (32,'denied_action','1');
INSERT INTO 1_setup VALUES (31,'denied_document','1');
INSERT INTO 1_setup VALUES (33,'shutdown_inc','shutdown.inc');
INSERT INTO 1_setup VALUES (34,'disable_url_to_link','1');
INSERT INTO 1_setup VALUES (35,'disable_wildcards','0');
INSERT INTO 1_setup VALUES (36,'disable_email_check','1');
INSERT INTO 1_setup VALUES (37,'default_lang','en');
INSERT INTO 1_setup VALUES (38,'lang_path','inc/lang/');
INSERT INTO 1_setup VALUES (39,'datetime_format','j M Y H:i');
INSERT INTO 1_setup VALUES (40,'time_format','H:i');
INSERT INTO 1_setup VALUES (41,'date_format','j M Y');
INSERT INTO 1_setup VALUES (42,'dec_point',',');
INSERT INTO 1_setup VALUES (43,'decimals','2');
INSERT INTO 1_setup VALUES (44,'currency_sym','EUR');
INSERT INTO 1_setup VALUES (45,'charset','iso-8859-1');
INSERT INTO 1_setup VALUES (46,'thousands_sep','.');
INSERT INTO 1_setup VALUES (47,'force_ssl','0');
INSERT INTO 1_setup VALUES (48,'unique_nicknames','0');
INSERT INTO 1_setup VALUES (49,'version','1.10');
INSERT INTO 1_setup VALUES (50,'disable_wysiwyg','1');

--
-- Table structure for table '1_typedata'
--

CREATE TABLE 1_typedata (
  id int(11) unsigned NOT NULL auto_increment,
  platform varchar(30) NOT NULL default 'phpweb',
  template varchar(60) NOT NULL default 'root',
  visible tinyint(1) unsigned NOT NULL default '1',
  type_id int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_typedata'
--


INSERT INTO 1_typedata VALUES (1,'phpweb','text_doc',1,2);
INSERT INTO 1_typedata VALUES (2,'phpweb','text_para',1,3);
INSERT INTO 1_typedata VALUES (3,'phpweb','overview',0,1);
INSERT INTO 1_typedata VALUES (5,'phpweb','news',1,5);
INSERT INTO 1_typedata VALUES (6,'phpweb','newsitem',1,6);
INSERT INTO 1_typedata VALUES (10,'phpweb','search',0,10);
INSERT INTO 1_typedata VALUES (32,'phpweb','phorum',1,32);
INSERT INTO 1_typedata VALUES (13,'phpweb','comment',0,13);
INSERT INTO 1_typedata VALUES (14,'phpweb','browser',1,14);
INSERT INTO 1_typedata VALUES (15,'phpweb','user',1,15);
INSERT INTO 1_typedata VALUES (16,'phpweb','user',0,16);
INSERT INTO 1_typedata VALUES (17,'phpweb','module_prefs',1,17);
INSERT INTO 1_typedata VALUES (18,'phpweb','guestbook_guestbook',1,18);
INSERT INTO 1_typedata VALUES (19,'phpweb','guestbook_posting',0,19);
INSERT INTO 1_typedata VALUES (24,'phpweb','db_browser',1,24);
INSERT INTO 1_typedata VALUES (25,'phpweb','forum_overview',1,25);
INSERT INTO 1_typedata VALUES (26,'phpweb','forum_forum',1,26);
INSERT INTO 1_typedata VALUES (27,'phpweb','forum_posting',0,27);
INSERT INTO 1_typedata VALUES (28,'phpweb','forum_mailto',0,28);
INSERT INTO 1_typedata VALUES (29,'phpweb','redirect',1,29);
INSERT INTO 1_typedata VALUES (30,'phpweb','external',1,30);
INSERT INTO 1_typedata VALUES (31,'phpweb','overview',1,31);
INSERT INTO 1_typedata VALUES (35,'phpweb','online',1,35);
INSERT INTO 1_typedata VALUES (36,'phpweb','friends',1,36);
INSERT INTO 1_typedata VALUES (37,'phpweb','faqs',1,37);
INSERT INTO 1_typedata VALUES (38,'phpweb','preferences',1,38);
INSERT INTO 1_typedata VALUES (39,'phpweb','messages',1,39);
INSERT INTO 1_typedata VALUES (40,'phpweb','adf',1,40);

--
-- Table structure for table '1_types'
--

CREATE TABLE 1_types (
  id int(11) unsigned NOT NULL auto_increment,
  parent_id int(11) unsigned NOT NULL default '0',
  name varchar(30) NOT NULL default 'unknown',
  description varchar(128) NOT NULL default 'none',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_types'
--


INSERT INTO 1_types VALUES (1,1,'root','Root Type');
INSERT INTO 1_types VALUES (2,1,'topic','Text Document');
INSERT INTO 1_types VALUES (3,2,'paragraph','Text Paragraph');
INSERT INTO 1_types VALUES (5,1,'news','News Overview');
INSERT INTO 1_types VALUES (6,5,'newsitem','News Item');
INSERT INTO 1_types VALUES (10,1,'search_form','Search Form');
INSERT INTO 1_types VALUES (32,1,'phorum','Phorum');
INSERT INTO 1_types VALUES (13,1,'comment','Comment');
INSERT INTO 1_types VALUES (14,1,'browser','File Browser');
INSERT INTO 1_types VALUES (15,1,'user','Add/Modify Account');
INSERT INTO 1_types VALUES (16,1,'denied','Access Denied');
INSERT INTO 1_types VALUES (17,1,'module_prefs','Module Preferences');
INSERT INTO 1_types VALUES (18,1,'guestbook_guestbook','Guestbook');
INSERT INTO 1_types VALUES (19,18,'guestbook_posting','Posting (Guestbook)');
INSERT INTO 1_types VALUES (24,1,'db_browser','DB Browser');
INSERT INTO 1_types VALUES (25,1,'forum_overview','Forum Overview');
INSERT INTO 1_types VALUES (26,25,'forum_forum','Forum');
INSERT INTO 1_types VALUES (27,26,'forum_posting','Forum Posting');
INSERT INTO 1_types VALUES (28,27,'forum_mailto','Forum Mailto');
INSERT INTO 1_types VALUES (29,1,'redirect','Redirection URL');
INSERT INTO 1_types VALUES (30,1,'external','Ext. Document');
INSERT INTO 1_types VALUES (31,1,'overview','Overview');
INSERT INTO 1_types VALUES (35,1,'online','Online List');
INSERT INTO 1_types VALUES (36,1,'friends','Friends List');
INSERT INTO 1_types VALUES (37,31,'faqs','FAQs');
INSERT INTO 1_types VALUES (38,1,'preferences','User Preferences');
INSERT INTO 1_types VALUES (39,1,'messages','Message List');
INSERT INTO 1_types VALUES (40,1,'adf_main','ADF');

--
-- Table structure for table '1_userdata'
--

CREATE TABLE 1_userdata (
  id int(11) unsigned NOT NULL auto_increment,
  user_id int(11) unsigned NOT NULL default '0',
  name text NOT NULL,
  value text NOT NULL,
  PRIMARY KEY  (id),
  KEY user_id (user_id)
) TYPE=MyISAM;

--
-- Dumping data for table '1_userdata'
--


INSERT INTO 1_userdata VALUES (1,1,'editor','1');
INSERT INTO 1_userdata VALUES (2,1,'edithost_all','1');
INSERT INTO 1_userdata VALUES (3,1,'key_master','1');
INSERT INTO 1_userdata VALUES (4,1,'nickname','Default Admin');
INSERT INTO 1_userdata VALUES (5,2,'nickname','Demo User');
INSERT INTO 1_userdata VALUES (6,1,'editmode','0');
INSERT INTO 1_userdata VALUES (7,1,'module_size_1','1');
INSERT INTO 1_userdata VALUES (8,1,'module_size_6','2');
INSERT INTO 1_userdata VALUES (9,1,'','');
INSERT INTO 1_userdata VALUES (10,1,'','');
INSERT INTO 1_userdata VALUES (11,1,'','');
INSERT INTO 1_userdata VALUES (12,1,'module_size_11','2');
INSERT INTO 1_userdata VALUES (13,1,'module_size_2','1');
INSERT INTO 1_userdata VALUES (14,1,'module_size_8','1');
INSERT INTO 1_userdata VALUES (15,1,'module_size_3','1');
INSERT INTO 1_userdata VALUES (16,1,'module_size_4','1');

--
-- Table structure for table '1_users'
--

CREATE TABLE 1_users (
  id int(11) unsigned NOT NULL auto_increment,
  email varchar(128) default NULL,
  password varchar(16) default NULL,
  valid tinyint(1) NOT NULL default '1',
  expires int(10) unsigned NOT NULL default '0',
  views int(11) unsigned default '0',
  last_login timestamp(14) NOT NULL,
  registered int(10) unsigned NOT NULL default '1009185013',
  PRIMARY KEY  (id),
  UNIQUE KEY email (email)
) TYPE=MyISAM;

--
-- Dumping data for table '1_users'
--


INSERT INTO 1_users VALUES (2,'b','b',1,0,344,20021110215434,1009185013);
INSERT INTO 1_users VALUES (1,'a','a',1,0,2143,20021129050640,1009185013);

