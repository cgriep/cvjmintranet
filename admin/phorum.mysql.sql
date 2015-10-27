# MySQL dump 8.13
#
# Host: localhost    Database: awf
#--------------------------------------------------------
# Server version	3.23.36

#
# Table structure for table 'awf_forum'
#

DROP TABLE IF EXISTS awf_forum;
CREATE TABLE awf_forum (
  id int(10) unsigned NOT NULL default '0',
  datestamp datetime NOT NULL default '0000-00-00 00:00:00',
  thread int(10) unsigned NOT NULL default '0',
  parent int(10) unsigned NOT NULL default '0',
  author char(37) NOT NULL default '',
  subject char(255) NOT NULL default '',
  email char(200) NOT NULL default '',
  host char(50) NOT NULL default '',
  email_reply char(1) NOT NULL default 'N',
  approved char(1) NOT NULL default 'N',
  msgid char(100) NOT NULL default '',
  modifystamp int(10) unsigned NOT NULL default '0',
  userid int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY author (author),
  KEY userid (userid),
  KEY datestamp (datestamp),
  KEY subject (subject),
  KEY thread (thread),
  KEY parent (parent),
  KEY approved (approved),
  KEY msgid (msgid),
  KEY modifystamp (modifystamp)
) TYPE=MyISAM;

#
# Table structure for table 'awf_forum_bodies'
#

DROP TABLE IF EXISTS awf_forum_bodies;
CREATE TABLE awf_forum_bodies (
  id int(10) unsigned NOT NULL auto_increment,
  body text NOT NULL,
  thread int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (id),
  KEY thread (thread)
) TYPE=MyISAM;

#
# Table structure for table 'forums'
#

DROP TABLE IF EXISTS forums;
CREATE TABLE forums (
  id int(10) unsigned NOT NULL auto_increment,
  name char(50) NOT NULL default '',
  active smallint(6) NOT NULL default '0',
  description char(255) NOT NULL default '',
  config_suffix char(50) NOT NULL default '',
  folder char(1) NOT NULL default '0',
  parent int(10) unsigned NOT NULL default '0',
  display int(10) unsigned NOT NULL default '0',
  table_name char(50) NOT NULL default '',
  moderation char(1) NOT NULL default 'n',
  email_list char(50) NOT NULL default '',
  email_return char(50) NOT NULL default '',
  email_tag char(50) NOT NULL default '',
  check_dup smallint(5) unsigned NOT NULL default '0',
  multi_level smallint(5) unsigned NOT NULL default '0',
  collapse smallint(5) unsigned NOT NULL default '0',
  flat smallint(5) unsigned NOT NULL default '0',
  lang char(50) NOT NULL default '',
  html char(40) NOT NULL default 'N',
  table_width char(4) NOT NULL default '',
  table_header_color char(7) NOT NULL default '',
  table_header_font_color char(7) NOT NULL default '',
  table_body_color_1 char(7) NOT NULL default '',
  table_body_color_2 char(7) NOT NULL default '',
  table_body_font_color_1 char(7) NOT NULL default '',
  table_body_font_color_2 char(7) NOT NULL default '',
  nav_color char(7) NOT NULL default '',
  nav_font_color char(7) NOT NULL default '',
  allow_uploads char(1) NOT NULL default 'N',
  upload_types char(100) NOT NULL default '',
  upload_size int(10) unsigned NOT NULL default '0',
  max_uploads int(10) unsigned NOT NULL default '0',
  security int(10) unsigned NOT NULL default '0',
  showip smallint(5) unsigned NOT NULL default '1',
  emailnotification smallint(5) unsigned NOT NULL default '1',
  body_color char(7) NOT NULL default '',
  body_link_color char(7) NOT NULL default '',
  body_alink_color char(7) NOT NULL default '',
  body_vlink_color char(7) NOT NULL default '',
  PRIMARY KEY  (id),
  KEY name (name),
  KEY active (active),
  KEY parent (parent)
) TYPE=MyISAM;

#
# Dumping data for table 'forums'
#

INSERT INTO forums VALUES (1,'Default',1,'','awf_','0',0,30,'awf_forum','n','','','',1,1,0,1,'lang/english.php','html','','','','','','','','','','N','',0,0,0,1,1,'','','','');

#
# Table structure for table 'forums_auth'
#

DROP TABLE IF EXISTS forums_auth;
CREATE TABLE forums_auth (
  id int(10) unsigned NOT NULL auto_increment,
  sess_id varchar(32) NOT NULL default '',
  name varchar(50) NOT NULL default '',
  username varchar(50) NOT NULL default '',
  password varchar(50) NOT NULL default '',
  email varchar(200) NOT NULL default '',
  webpage varchar(200) NOT NULL default '',
  image varchar(200) NOT NULL default '',
  icq varchar(50) NOT NULL default '',
  aol varchar(50) NOT NULL default '',
  yahoo varchar(50) NOT NULL default '',
  msn varchar(50) NOT NULL default '',
  jabber varchar(50) NOT NULL default '',
  signature varchar(255) NOT NULL default '',
  PRIMARY KEY  (id),
  KEY name (name),
  KEY username (username)
) TYPE=MyISAM;

#
# Dumping data for table 'forums_auth'
#

INSERT INTO forums_auth VALUES (1,'eb3f35e17b0c3b0f20765eb8b21c1fc3','Admin','awf','1a0e464ff4f13755869adae40d07e1f3','webmaster@localhost','','','','','','','','');

#
# Table structure for table 'forums_moderators'
#

DROP TABLE IF EXISTS forums_moderators;
CREATE TABLE forums_moderators (
  user_id int(10) unsigned NOT NULL default '0',
  forum_id int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (user_id,forum_id),
  KEY forum_id (forum_id)
) TYPE=MyISAM;

#
# Dumping data for table 'forums_moderators'
#

INSERT INTO forums_moderators VALUES (1,0);

