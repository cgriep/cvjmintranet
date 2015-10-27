# MySQL dump 8.13
#
# Host: localhost    Database: awf
#--------------------------------------------------------
# Server version        3.23.36
 
#
# Table structure for table '1_ads'
#
 
DROP TABLE IF EXISTS 1_ads;
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

#
# Table structure for table '1_flavours'
#

ALTER TABLE 1_flavours ADD lang CHAR(2) DEFAULT 'en' NOT NULL AFTER published;
ALTER TABLE 1_flavours CHANGE id id INT (11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_flavours CHANGE published published TINYINT (1) UNSIGNED NOT NULL DEFAULT '0';

#
# Table structure for table '1_groupdata'
#
 
DROP TABLE IF EXISTS 1_groupdata;
CREATE TABLE 1_groupdata (
  id int(11) unsigned NOT NULL auto_increment,
  group_id int(11) unsigned NOT NULL default '0',
  name text NOT NULL,
  value text NOT NULL,
  PRIMARY KEY  (id)
) TYPE=MyISAM;
 
#
# Dumping data for table '1_groupdata'
#
 
INSERT INTO 1_groupdata VALUES (7,1,'editor','1');
INSERT INTO 1_groupdata VALUES (2,1,'edithost_all','1');
INSERT INTO 1_groupdata VALUES (8,2,'allow_upload','1');
INSERT INTO 1_groupdata VALUES (9,2,'allow_delete','1');
INSERT INTO 1_groupdata VALUES (10,2,'allow_createdir','1');
 
#
# Table structure for table '1_groups'
#
 
DROP TABLE IF EXISTS 1_groups;
CREATE TABLE 1_groups (
  id int(11) unsigned NOT NULL auto_increment,
  group_name varchar(32) NOT NULL default '',
  PRIMARY KEY  (id),
  UNIQUE KEY name (group_name)
) TYPE=MyISAM;

#
# Dumping data for table '1_groups'
#
 
INSERT INTO 1_groups VALUES (1,'Administrators');
INSERT INTO 1_groups VALUES (2,'File Management');

#
# Table structure for table '1_messages'
#
 
DROP TABLE IF EXISTS 1_messages;
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

#
# Table structure for table '1_modules'
#

ALTER TABLE 1_modules CHANGE id id INT (11) UNSIGNED NOT NULL AUTO_INCREMENT;
ALTER TABLE 1_modules CHANGE visible visible tinyint(1) unsigned NOT NULL default '1';
ALTER TABLE 1_modules CHANGE removeable removeable tinyint(1) unsigned NOT NULL default '1';

#
# Table structure for table '1_nodedata'
#

ALTER TABLE 1_nodedata CHANGE id id int(11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_nodedata CHANGE name name varchar(32) NOT NULL default '';
ALTER TABLE 1_nodedata ADD datatype varchar(16) NOT NULL default 'notrans';
ALTER TABLE 1_nodedata ADD INDEX(flavour_id);
ALTER TABLE 1_nodedata ADD INDEX(node_id);

#
# Table structure for table '1_nodes'
#

ALTER TABLE 1_nodes CHANGE id id int(11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_nodes ADD expiration_date int(11) unsigned NOT NULL default '0' AFTER release_date;
ALTER TABLE 1_nodes ADD ssl_only tinyint(1) unsigned NOT NULL default '0' AFTER members_only;
ALTER TABLE 1_nodes ADD INDEX(parent_id);

#
# Table structure for table '1_polls'
#

ALTER TABLE 1_polls CHANGE id id int(11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_polls CHANGE active active tinyint(1) unsigned NOT NULL default '1';
ALTER TABLE 1_polls ADD module_id int(11) NOT NULL default '0' AFTER sortorder;

#
# Table structure for table '1_polls_items'
#

ALTER TABLE 1_polls_items CHANGE id id int(11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_polls_items CHANGE poll_id poll_id int(11) unsigned NOT NULL default '0';
ALTER TABLE 1_polls_items CHANGE counter counter int(11) unsigned NOT NULL default '0';

#
# Table structure for table '1_redirect'
#

ALTER TABLE 1_redirect CHANGE id id int(11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_redirect CHANGE views views int(11) unsigned default '0';

#
# Table structure for table '1_setup'
#

ALTER TABLE 1_setup CHANGE id id int(11) unsigned NOT NULL auto_increment;
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
INSERT INTO 1_setup VALUES (49,'version','1.02');

#
# Table structure for table '1_typedata'
#

ALTER TABLE 1_typedata CHANGE id id int(11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_typedata CHANGE type_id type_id int(11) unsigned NOT NULL default '0';

UPDATE 1_typedata SET template = 'text_doc' WHERE id = 1;
UPDATE 1_typedata SET template = 'text_para' WHERE id = 2;

#
# Table structure for table '1_types'
#

ALTER TABLE 1_types CHANGE id id int(11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_types CHANGE parent_id parent_id int(11) unsigned NOT NULL default '0';

#
# Table structure for table '1_userdata'
#

ALTER TABLE 1_userdata CHANGE id id int(11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_userdata CHANGE user_id user_id int(11) unsigned NOT NULL default '0';
ALTER TABLE 1_userdata ADD INDEX(user_id);

#
# Table structure for table '1_users'
#

ALTER TABLE 1_users CHANGE id id int(11) unsigned NOT NULL auto_increment;
ALTER TABLE 1_users CHANGE email email varchar(128) default NULL;
ALTER TABLE 1_users CHANGE password password varchar(16) default NULL;
ALTER TABLE 1_users CHANGE views views int(11) unsigned default '0';
ALTER TABLE 1_users ADD registered int(10) unsigned NOT NULL default '1010886588';
ALTER TABLE 1_users ADD UNIQUE(email);

