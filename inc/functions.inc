<?php
/*
        Copyright (C) 2000-2003 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 08.10.2003
*/

function getmicrotime () {
        // returns the current time
        list($usec, $sec) = explode(" ", microtime());
        return ((float)$usec + (float)$sec);
        }

// uncomment this, to log page creation time
define('START', getmicrotime ());

function htmlchars ($string) {
        if(CHARSET == 'iso-8859-1') {
                return htmlentities($string);
                }
        return htmlspecialchars($string, ENT_QUOTES, CHARSET);
        }

function random_id () {
        return md5(uniqid(rand(), 1));
        }

function cross_sum ($number) {
        $sum = 0;
        $number = (string) $number;
        for($i = 0; $i < strlen($number); $i++) {
                $sum += (integer) $number[$i];
                }

        if($sum > 9) {
                $sum = cross_sum($sum);
                }

        return $sum;
        }

function insert_char ($input, $char, $pos) {
        // insert a char into a sting at a given position
        $input = (string) $input;
        return substr($input, 0, $pos).$char.substr($input, $pos);
        }

function replace_wildcards ($text, $array) {
        // replaces wildcards in the form %%wildcard%%
        if(!is_array($array)) return $text;
        global $profile;
        if(DISABLE_WILDCARDS == 1 || $profile['editor'] == 1) return $text;
        foreach ( $array as $key => $value) {
                $text = str_replace('%%'.$key.'%%', $value, $text);
                }
        return $text;
        }

function replace_pattern ($text, $array, $override = false, $mode = 'fast') {
        // replaces a given pattern (similar to replace_wildcards() but more powerful)
        if(!is_array($array)) return $text;
        global $profile;
        if((DISABLE_WILDCARDS == 1 || $profile['editor'] == 1) && $override = false) return $text;
        if($mode == 'fast') {
                $text = strtr($text, $array);
                }
        else {
                foreach ( $array as $key => $value)  {
                        // variable?
                        if($value[0] == '$') {
                                if(strstr($value, ']')) {
                                        $reg = substr(strstr(substr($value, 1), '['), 1, -1);
                                        $value = $GLOBALS[substr($value, 1, -1 * (strlen($reg) + 2))][$reg];
                                        }
                                else {
                                        $value = $GLOBALS[substr($value, 1)];
                                        }
                                }
                        // replace!
                        $text = ereg_replace($key, $value, $text);
                        }
                }
        return $text;
        }

function email_to_string ($email) {
        // michael@liquidbytes.net -> michael at liquidbytes dot net
        if($email == '') return '';
        $search  = array ("." => ' '.LANG_DOT.' ', "@" => " at ");
        return strtr($email, $search);
        }

function timestamp_to_string ($timestamp = -1, $mode = 'datetime') {
        // converts a given timestamp to a human readable string
        if($timestamp == -1) $timestamp = time();
        if($mode == 'datetime') return date(DATETIME_FORMAT, $timestamp);
        if($mode == 'time') return date(TIME_FORMAT, $timestamp);
        if($mode == 'date') return date(DATE_FORMAT, $timestamp);
        }

function isemail ($email, $override = false) {
        // returns true if the given email address is valid
        if(DISABLE_EMAIL_CHECK == 1 && !$override) { return true; }
        if(strpos ($email, '@') == false) { return false; }
        if(getmxrr (substr(strstr($email, '@'),1), $mxhosts) == false) { return false; }
        return true;
        }

function url_to_link ($in, $override = false) {
        // makes URLs and email addresses "clickable"
        if(DISABLE_URL_TO_LINK == 1 && !$override) return $in;
        $out = preg_replace( "([[:alnum:]]+)://([^[:space:]]*)([[:alnum:]#?/&=])/i",
                "<a href=\"\\1://\\2\\3\" target=\"_blank\">\\1://\\2\\3</a>", $in);
        $out = preg_replace( "(([a-z0-9_]|\\-|\\.)+@([^[:space:]]*)([[:alnum:]-]))/i",
                "<a href=\"mailto:\\1\">\\1</a>", $out);
        return $out;
        }

function add_user ($email, $password, $valid=1) {
        // creates a new user account

        // enforce secure db queries
        if(!is_numeric($valid)) return false;
        $email = addslashes($email);
        $password = addslashes($password);

        $qres = sql_query ("SELECT email FROM ".TABLE_USERS." WHERE email='$email'");
        if(sql_num_rows($qres) == 0) {
          sql_free_result($qres);
          sql_query ("INSERT INTO ".TABLE_USERS." (email, password, valid, views, last_login, registered)".
                        " VALUES ('$email','$password', $valid, 0, ".time().", ".time().")");
          $insert_id = sql_insert_id();
          set_profile("im_default", "email", $insert_id);
          set_profile("newsletter", "1", $insert_id );
          return $insert_id;
          } else { sql_free_result($qres); return false; }
        }

function update_user ($email, $password='', $old_email=SESSION_EMAIL, $valid=1) {
        // updates an existing user account

        // enforce secure db queries
        if(!is_numeric($valid)) return false;
        $email = addslashes($email);
        $old_email = addslashes($old_email);
        $password = addslashes($password);

        if($password == '') {
                $qres = sql_query ("UPDATE ".TABLE_USERS." SET email='$email', valid=$valid WHERE email='$old_email'");
                }
        else {
                $qres = sql_query ("UPDATE ".TABLE_USERS." SET email='$email', password='$password', valid=$valid WHERE email='$old_email'");
                }
        sql_free_result($qres);
        if(sql_affected_rows() > 0) {
                  return true;
                }
        else {
                $res = sql_query("SELECT id FROM ".TABLE_USERS." WHERE email='$old_email'");
                if(sql_num_rows($res) > 0) { return true; }
                else { return false; }
                }
        }

function get_user_email ($user_id = -1) {
        // returns the email address corresponding to the given user_id

        // enforce secure db queries
        if(!is_numeric($user_id)) return false;

        if($user_id == -1) {
                if(!defined("SESSION_DBID")) { return false; }
                $user_id = SESSION_DBID;
                }
        $email = sql_query ("SELECT email FROM ".TABLE_USERS." WHERE id=".$user_id);
                if(sql_num_rows($email) > 0) {
                        $prow = sql_fetch_row($email);
                        sql_free_result($email);
                        return stripslashes($prow[0]);
                        }
        return "";
        }

function get_user_id ($email) {
        // returns the user_id corresponding to the given email address
        if(defined(SESSION_EMAIL) && SESSION_EMAIL == $email) return SESSION_DBID;
        $user_id = sql_query ("SELECT id FROM ".TABLE_USERS." WHERE email='".addslashes($email)."'");
                if(sql_num_rows($user_id) > 0) {
                        $prow = sql_fetch_row($user_id);
                        sql_free_result($user_id);
                        return $prow[0];
                        }
        return -1;
        }

function get_user_nickname ($user_id = -1) {
        // returns the nickname corresponding to the given user_id

        // enforce secure db queries
        if(!is_numeric($user_id)) return false;

        if($user_id == -1 || (defined(SESSION_DBID) && $user_id == SESSION_DBID)) {
                global $profile;
                if(!isset($profile['nickname'])) $profile['nickname'] = LANG_STRANGER;
                return htmlchars($profile['nickname']);
                }
        $nickname = sql_query ("SELECT value FROM ".TABLE_USERDATA." WHERE user_id=".$user_id." AND name='nickname'");
                if(sql_num_rows($nickname) > 0) {
                        $prow = sql_fetch_row($nickname);
                               sql_free_result($nickname);
                        return stripslashes($prow[0]);
                        }
        return email_to_string(get_user_email($user_id));
        }

function is_unique_nickname ($name, $user_id = -1) {
        // returns true if the given nickname is unique

        // enforce secure db queries
        if(!is_numeric($user_id) || $user_id == -1) $user_id = 0;
        $name = addslashes($name);

        $nickname = sql_query ("SELECT count(*) FROM ".TABLE_USERDATA." WHERE name='nickname' AND value='$name' AND user_id != $user_id");
        $row = sql_fetch_row($nickname);
        sql_free_result($nickname);
        if(is_numeric($row[0]) && $row[0] > 0) { return false; }
        else { return true; }
        }

function remove_user ($user_id) {
        // removes user account

        // enforce secure db queries
        if(!is_numeric($user_id)) return false;

        // Delete all messages
        sql_query ("DELETE FROM ".TABLE_MESSAGES." WHERE user_id=$user_id");
        // Delete profile
        sql_query ("DELETE FROM ".TABLE_USERDATA." WHERE user_id=$user_id");
        // Delete user
        sql_query ("DELETE FROM ".TABLE_USERS." WHERE id=$user_id");
        remove_file(get_dir_name('users/public', $user_id, false));
        remove_file(get_dir_name('users/private', $user_id, false));
        // ok?
        if(sql_affected_rows() == 0) { return false; } else { return true; }
        }

function set_profile ($name, $value, $user_id = -1) {
        // saves a key/value-pair in a user profile

        // enforce secure db queries
        if (($name == '' && $value == '') || !is_numeric($user_id)) return false;

        if (SESSION_STATUS != 'ok' && $user_id == -1) {
                return false;
                }

        if($user_id == -1 || $user_id == SESSION_DBID) {
                $user_id = SESSION_DBID;
                $GLOBALS['profile'][$name] = $value;
                }

        $value = addslashes($value);
        $name = addslashes($name);
        if ($value == '' && $name != '') {
                sql_query("DELETE FROM ".TABLE_USERDATA." WHERE name='".$name."' AND user_id=".$user_id);
                return true;
                }
        $qresult = sql_query ("UPDATE ".TABLE_USERDATA." SET value='".$value."' WHERE name='".$name."' AND user_id=".$user_id);
        if(sql_affected_rows() == 0) {
                $res = sql_query("SELECT id FROM ".TABLE_USERDATA." WHERE name='".$name."' AND user_id=".$user_id);
                if(sql_num_rows($res) > 0) {
                        return true; }
                else {
                        sql_query ("INSERT INTO ".TABLE_USERDATA." (name, value, user_id) VALUES ('".$name."', '".$value."', ".$user_id.")");
                        }
                }
        return true;
        }

function remove_profile ($name, $user_id = -1) {
        // removes a key/value-pair from a user profile

        // enforce secure db queries
        if (!is_numeric($user_id)) return false;
        $name = addslashes($name);

        if (SESSION_STATUS != 'ok' && $user_id == -1) {
                return false;
                }
        if($user_id == -1) {
                $user_id = SESSION_DBID;
                global $profile;
                unset ($profile[$name]);
                }

        $qresult = sql_query ("DELETE FROM ".TABLE_USERDATA." WHERE name='".$name."' AND user_id=".$user_id);

        return true;
        }

function update_nodedata ($node_id, $name, $value, $flavour_id, $datatype = 'notrans') {
        // updates a document property

        // enforce secure db queries
        if(!is_numeric($node_id) || !is_numeric($flavour_id)) return false;

        global $cache_refresh;
        sql_query("UPDATE ".TABLE_NODEDATA." SET value='".addslashes($value)."', datatype='".addslashes($datatype)."' WHERE
                name='".addslashes($name)."' AND node_id=$node_id AND flavour_id=$flavour_id");
        $aff_rows = sql_affected_rows();
        switch($aff_rows) {
                case -1: return false;
                case 0 : $res = sql_query("SELECT id FROM ".TABLE_NODEDATA." WHERE name='".addslashes($name)."' AND node_id=$node_id AND
                        (flavour_id=$flavour_id OR flavour_id=0)");
                        if(sql_num_rows($res) > 0) { return true; }
                        else { return false; }
                default: $cache_refresh[$node_id] = true; return true;
                }
        }

function add_nodedata ($node_id, $name, $value, $flavour_id, $datatype = 'notrans') {
        // adds new document property

        // enforce secure db queries
        if(!is_numeric($node_id) || !is_numeric($flavour_id)) return false;

        if(update_nodedata($node_id, $name, $value, $flavour_id, $datatype) == false) {
                sql_query("INSERT INTO ".TABLE_NODEDATA." (flavour_id,value,name,node_id,datatype) VALUES
                ($flavour_id, '".addslashes($value)."', '".addslashes($name)."', $node_id, '".addslashes($datatype)."')");
                global $cache_refresh;
                $cache_refresh[$node_id] = true;
                return sql_insert_id();
                }
        }

function delete_nodedata ($node_id, $name, $value, $flavour_id, $datatype = 'notrans') {
        // removes a document property

        // enforce secure db queries
        if(!is_numeric($node_id) || !is_numeric($flavour_id)) return false;

        sql_query("DELETE FROM ".TABLE_NODEDATA." WHERE name='".addslashes($name)."' AND node_id=$node_id AND flavour_id=$flavour_id");

        global $cache_refresh;
        $cache_refresh[$node_id] = true;

        return true;
        }

function create_node ($parent_id, $type_id, $sort_order, $enabled, $published, $description, $key_id=-1, $searchable=1, $cacheable=1, $release_date=0, $members_only=0, $expiration_date=0) {
        // creates new document

        // enforce secure db queries
        if(!is_numeric($parent_id) || !is_numeric($type_id) || !is_numeric($sort_order)
        || !is_numeric($key_id) || !is_numeric($release_date) || !is_numeric($expiration_date)) return false;

        if($key_id == -1 && isset($GLOBALS['default_key_id'])) {
                $key_id = $GLOBALS['default_key_id'];
                }

        if($searchable != 1)          $searchable         = 0;
        if($cacheable  != 1)          $cacheable          = 0;
        if($published  != 1)          $published          = 0;
        if($enabled    != 1)         $enabled            = 0;
        if($members_only != 1)          $members_only         = 0;
        if($release_date < 0)          $release_date         = 0;
        if($expiration_date < 0) $expiration_date = 0;
        sql_query("INSERT INTO ".TABLE_NODES." (created, last_change, sort_order, parent_id, enabled, published,
        type_id, description, key_id, searchable, cacheable, release_date, expiration_date, members_only) VALUES
        (".time().", ".time().", $sort_order, $parent_id, $enabled, $published, $type_id, '".addslashes($description)."', $key_id,
        $searchable, $cacheable, $release_date, $expiration_date, $members_only)");
        return sql_insert_id();
        }

function destroy_node ($id) {
        // removes a document

        // enforce secure db queries
        if(!is_numeric($id)) return false;

        $qresult = sql_query("SELECT id FROM ".TABLE_NODES." WHERE parent_id=$id");
        if(sql_num_rows($qresult) > 0) {
                while($row = sql_fetch_row($qresult)) {
                        destroy_node ($row[0]);
                        }
                sql_free_result($qresult);
                }
        sql_query("DELETE FROM ".TABLE_NODES." WHERE id=$id");
        sql_query("DELETE FROM ".TABLE_NODEDATA." WHERE node_id=$id");
        remove_file(get_dir_name('files', $id, false));
        remove_file(get_dir_name(BASE_PATH.INC_PATH.DATA_PATH.'nodes', $id, false));
        global $cache_refresh;
        $cache_refresh[$id] = true;

        return true;
        }

function update_node ($node_id, $parent_id, $type_id, $sort_order, $enabled, $published, $description, $key_id=0, $searchable=1, $cacheable=1, $release_date=-1, $members_only=-1, $expiration_date=-1) {
        // updates an existing document

        // enforce secure db queries
        if(!is_numeric($parent_id) || !is_numeric($type_id) || !is_numeric($sort_order) || !is_numeric($node_id)
        || !is_numeric($key_id) || !is_numeric($release_date) || !is_numeric($expiration_date) || !is_numeric($members_only)
        || !is_numeric($release_date) || !is_numeric($expiration_date)) return false;

        if($searchable != 1) $searchable   = 0;
        if($cacheable  != 1) $cacheable    = 0;
        if($published  != 1) $published    = 0;
        if($enabled    != 1) $enabled           = 0;
        $optional = '';
        if($members_only != -1) { $optional = ", members_only=$members_only"; }
        if($release_date > -1) { $optional = ", release_date=$release_date"; }
        if($expiration_date > -1) { $optional = ", expiration_date=$expiration_date"; }
        sql_query("UPDATE ".TABLE_NODES." SET last_change=".time().", sort_order=$sort_order, parent_id=$parent_id, enabled=$enabled,
                        published=$published, type_id=$type_id, description='".addslashes($description)."',
                        key_id=$key_id, searchable=$searchable, cacheable=$cacheable $optional WHERE id=$node_id");
        if(sql_affected_rows() < 1) {
                $res = sql_query("SELECT id FROM ".TABLE_NODES." WHERE id=$node_id");
                if(sql_num_rows($res) > 0) { return true; }
                else { return false; }
                }
        else {
                global $cache_refresh;
                $cache_refresh[$node_id] = true;
                return true; }
        }

function get_nodedata ($id, $flavour_id) {
        // returns all properties of a document

        // enforce secure db queries
        if(!is_numeric($id) || !is_numeric($flavour_id)) return false;

        $qresult = sql_query("SELECT name, value, datatype FROM ".TABLE_NODEDATA." WHERE node_id=$id AND (flavour_id=$flavour_id OR flavour_id=0)");
        $document = array();
        if(sql_num_rows($qresult) > 0) {
                while($row = sql_fetch_row($qresult)) {
                        $document[stripslashes($row[0])] = stripslashes($row[1]);
                        $document['datatype'][stripslashes($row[0])] = $row[2];
                        }
                sql_free_result($qresult);
                }

        return $document;
        }

function get_node ($id, $flavour_id, $counter_inc = true) {
        // returns a complete document
        global $enabled_filter;
        global $profile;

        // enforce secure db queries
        if($counter_inc == true) { global $count_views; }
        if(!isset($count_views) || !is_numeric($count_views)) $count_views = 0;
        if(!is_numeric($id) || !is_numeric($flavour_id)) return false;
        $qresult = sql_query("SELECT parent_id, type_id, published, description, sort_order, enabled, searchable, cacheable, key_id,
                                release_date, expiration_date, members_only, ssl_only, views, created, last_change FROM ".TABLE_NODES."
                                WHERE id=$id $enabled_filter");
        if(sql_num_rows($qresult) > 0) {
                $row = sql_fetch_array($qresult);
                sql_free_result($qresult);

                foreach ( $row as $key => $value )  {
                        $document[$key] = stripslashes($value);
                        }

                       static $disable_counting = array();

                if($count_views == 1 && $id == $GLOBALS['id'] && $profile['editor'] == 0 &&
                        !isset($disable_counting[$id])) {

                        sql_query("UPDATE ".TABLE_NODES." SET views=views+1 WHERE id=$id");

                        $disable_counting[$id] = true;
                        }

                $docdata = get_nodedata ($id, $flavour_id);
				if ( is_array($docdata)) 
                  return array_merge($document, $docdata);
                else
                  return $document;
                }
        else {
                return false;
                }
        }

function count_children ($type_id = -1, $flavour_id = -1, $item_id = -1) {
        // counts all documents having the given parent id
        global $enabled_filter;

        // enforce secure db queries
        if(!is_numeric($type_id) || !is_numeric($flavour_id) || !is_numeric($item_id)) return false;

        if($item_id > 0) { $id = $item_id; } else { $id = $GLOBALS['id']; }

        if($type_id >= 0) { $filter = "AND type_id = $type_id"; } else { $filter = ''; }

        if($flavour_id != -1) {
                $qresult = sql_query("SELECT count(*) FROM ".TABLE_NODES.", ".TABLE_NODEDATA." WHERE ".TABLE_NODEDATA.".node_id=".TABLE_NODES.".id ".
                "AND parent_id = $id AND (flavour_id = $flavour_id OR flavour_id = 0) AND name='title' $filter $enabled_filter");
                }
        else {
                $qresult = sql_query("SELECT count(*) FROM ".TABLE_NODES." WHERE parent_id = $id $filter $enabled_filter");
                }

        if(sql_num_rows($qresult) > 0) {
                $row = sql_fetch_row($qresult);
                sql_free_result($qresult);
                return $row[0];
                }
        else {
                return 0;
                }
        }

function get_children ($offset, $count, $type_id = -1, $key_only = false, $sort_order = 'DESC', $published = false, $item_id = -1) {
        // returns all documents with the given parent id
        global $flavour;
        global $enabled_filter;

        if(!isset($flavour)) $flavour = DEFAULT_FLAVOUR;

        // enforce secure db queries
        if(!is_numeric($type_id) || !is_numeric($flavour) || !is_numeric($offset) || !is_numeric($count) || !is_numeric($item_id)
        || !is_bool($published)) return false;
        if($sort_order != 'DESC') $sort_order = 'ASC';

        $filter = '';
        if($type_id >= 0) $filter .= "AND type_id = $type_id";
        if($published) $filter .= "AND published = 1 ";
        if($item_id >= 0) { $id = $item_id; } else { $id = $GLOBALS['id']; }

        // just return one specific key/value pair of the document properties?
        if($key_only === true) {
                // hack to make it compative with older code
                if(is_bool($key_only)) $key_only = 'title';
                $qresult = sql_query("SELECT DISTINCT node_id, value, type_id, published, key_id FROM ".TABLE_NODES.
                ", ".TABLE_NODEDATA." WHERE parent_id=$id AND name='".addslashes($key_only)."' $filter AND node_id=".TABLE_NODES.".id AND
                (flavour_id=$flavour OR flavour_id=0) $enabled_filter ORDER BY sort_order $sort_order ".sql_limit($count,$offset));
                if(sql_num_rows($qresult) > 0) {
                while($row = sql_fetch_row($qresult)) {
                        $children[$row[0]]['type_id'] = $row[2];
                        $children[$row[0]]['published'] = $row[3];
                        $children[$row[0]][$key_only] = stripslashes($row[1]);
                        $children[$row[0]]['key_id'] = $row[4];
                        }
                sql_free_result($qresult);
                return $children;
                }
                }
        else {
                $qresult = sql_query("SELECT parent_id, type_id, published, description, sort_order, enabled, searchable,
                                        cacheable, key_id, id, release_date, expiration_date, members_only, ssl_only,
                                        views, created, last_change FROM ".TABLE_NODES." WHERE parent_id=$id $enabled_filter $filter
                                        ORDER BY sort_order $sort_order ".sql_limit($count,$offset));
                $document = array();
                if(sql_num_rows($qresult) > 0) {
                        while($row = sql_fetch_array($qresult)) {

                                foreach ( $row as $key => $value )  {
                                        $document[$key] = stripslashes($value);
                                        }
                        $children[$row['id']] = array_merge(get_nodedata($row['id'], $flavour), $document);
                        }
                sql_free_result($qresult);
                return $children;
                }
                }
        }

function get_sorted_children ($offset, $count, $name, $type_id = -1, $item_id = -1, $sort_order = 'DESC', $published = false) {
        // like get_children(), but documents will be sorted

        global $flavour;
        global $search_time;

        if(!isset($flavour)) $flavour = DEFAULT_FLAVOUR;

        // enforce secure db queries
        if(!is_numeric($type_id) || !is_numeric($flavour) || !is_numeric($offset) || !is_numeric($count) || !is_numeric($item_id)
        || !is_bool($published)) return false;
        if($sort_order != 'DESC') $sort_order = 'ASC';

        $where = "name='$name'";
        if($type_id != -1) { $where .= " AND type_id=".$type_id; }
        if($published == true) { $where .= " AND published=1"; }
        $where .= " ORDER BY value ".$sort_order;
        $start = getmicrotime();
        if($item_id != -1) { $id = $item_id; } else { $id = $GLOBALS['id']; }

         $qresult=sql_query("SELECT DISTINCT node_id FROM ".TABLE_NODEDATA.", ".TABLE_NODES." WHERE
        node_id = ".TABLE_NODES.".id AND (flavour_id = $flavour OR flavour_id=0) AND parent_id = $id AND $where
        ".sql_limit($count,$offset));
        while($row = sql_fetch_row($qresult)) {
                $result = get_node($row[0], $flavour, 0);
                       if($result != false) $results[$row[0]] = $result;
                }
        sql_free_result($qresult);
         $search_time = number_format(getmicrotime() - $start, 4, ',', '');

        if(isset($results)) {
                return $results;
                }
        else {
                return false;
                }
        }

function get_siblings ($offset, $count, $type = -1, $title_only = false, $published = false, $parent_id = -1, $no_filter = false) {
        // returns all siblings of the given document
        global $document;
        global $flavour;
        global $enabled_filter;
        global $id;

        if(!isset($flavour)) $flavour = DEFAULT_FLAVOUR;

        if($parent_id < 0) { $parent_id = $document['parent_id']; }

        // enforce secure db queries
        if(is_numeric($id) || !is_numeric($type_id) || !is_numeric($flavour) || !is_numeric($offset) || !is_numeric($count) || !is_numeric($parent_id)
        || !is_bool($published) || !is_bool($title_only)) return false;
        if($sort_order != 'DESC') $sort_order = 'ASC';

        $filter = '';
        if($type_id >= 0) $filter .= "AND type_id = $type_id ";
        if($published) $filter .= "AND published = 1 ";

        if($title_only) {
                $qresult = sql_query("SELECT DISTINCT node_id, value, type_id, published, key_id FROM ".TABLE_NODES.",
                ".TABLE_NODEDATA." WHERE parent_id=$parent_id AND name='title' $filter AND
                node_id=".TABLE_NODES.".id AND (flavour_id=$flavour OR flavour_id=0) $enabled_filter ORDER BY sort_order DESC ".sql_limit($count,$offset));
                if(sql_num_rows($qresult) > 0) {
                while($row = sql_fetch_array($qresult)) {
                        if($row['id'] != $id || $no_filter) {
                                foreach ( $row as $key => $value )  {
                                        $related[$key] = stripslashes($value);
                                        }
                                }
                        }
                sql_free_result($qresult);
                return $related;
                }
                }
        else {
                $qresult = sql_query("SELECT parent_id, type_id, published, description, sort_order, enabled, searchable, ".
                                        "cacheable, key_id, id, release_date, expiration_date, members_only, ssl_only, views, created, last_change ".
                                        "FROM ".TABLE_NODES." WHERE parent_id=$parent_id $enabled_filter $filter AND ".
                                        "NOT (id = $id) ORDER BY sort_order DESC        ".sql_limit($count,$offset));

                if(sql_num_rows($qresult) > 0) {
                        while($row = sql_fetch_array($qresult)) {
                                foreach ( $row as $key => $value )  {
                                        $doc[$key] = stripslashes($value);
                                        }
                                $related[$row['id']] = array_merge(get_nodedata($row['id'], $flavour), $doc);
                                }
                        sql_free_result($qresult);
                        return $related;
                        }
                }
        }

function init_published_children () {
        // initializes $published_children array
        if($done) return true;
        static $done = true;
        global $id;
        global $flavour;
        global $enabled_filter;
        global $published_children;

        $published_children = array();

        // enforce secure db queries
        if(is_numeric($id) || !is_numeric($flavour)) return false;

        $qresult = sql_query("SELECT node_id, name, value, type_id, published, datatype FROM ".TABLE_NODES.",
        ".TABLE_NODEDATA." WHERE parent_id=$id AND name='title' AND node_id=".TABLE_NODES.".id AND
        (flavour_id=$flavour OR flavour_id=0) AND published=1 $enabled_filter ORDER BY sort_order DESC");
        if(sql_num_rows($qresult) > 0) {
        while($row = sql_fetch_array($qresult)) {
                foreach ( $row as $key => $value )  {
                        $published_children[$row['node_id']][$key] = stripslashes($value);
                        }
                }
        sql_free_result($qresult);
        }

        return true;
        }

function init_published_roots () {
        // initializes $published_roots array
        if($done) return true;
        static $done = true;
        global $enabled_filter;
        global $flavour;
        global $published_roots;

        $published_roots = array();

        // enforce secure db queries
        if(!is_numeric($flavour)) return false;

        $qresult = sql_query("SELECT ".TABLE_NODES.".id, name, value, type_id, datatype FROM ".TABLE_NODES.",
        ".TABLE_NODEDATA." WHERE parent_id=0 AND node_id=".TABLE_NODES.".id AND published=1
        AND (flavour_id=$flavour OR flavour_id=0) AND name='title' $enabled_filter ORDER BY sort_order DESC");

        if(sql_num_rows($qresult) > 0) {
                while($row = sql_fetch_row($qresult)) {
                        $published_roots[$row[0]]['title'] = stripslashes($row[2]);
                        $published_roots[$row[0]]['datatype']['title'] = stripslashes($row[4]);
                        }
                sql_free_result($qresult);
                  }

        return true;
        }

function init_parents () {
        // initializes $parents array
        if($done) return true;
        static $done = true;
        global $document;
        global $id;
        global $root_parent_id;
        global $parents;
        global $flavour;
        $parents = array();

        if($document['parent_id'] == 0) {
                $root_parent_id = $id;
                return true;
                }

        $parent_id = $document['parent_id'];

        // enforce secure db queries
        if(!is_numeric($flavour) || !is_numeric($parent_id)) return false;

        while($parent_id != 0) {
                $qresult = sql_query("SELECT ".TABLE_NODES.".id, name, value, parent_id, type_id, datatype FROM
                        ".TABLE_NODES.", ".TABLE_NODEDATA." WHERE ".TABLE_NODES.".id=$parent_id
                        AND node_id=$parent_id AND (flavour_id=$flavour OR flavour_id=0) AND name='title'");
                if(sql_num_rows($qresult) > 0) {
                        while($row = sql_fetch_row($qresult)) {
                                $parents[$row[0]]['title'] = stripslashes($row[2]);
                                $parents[$row[0]]['datatype']['title'] = stripslashes($row[5]);
                                $root_parent_id = $row[0];
                                $parents[$row[0]]['parent_id'] = $row[3];
                                if($parent_id != $row[3]) { $parent_id = $row[3]; }
                                else { $parent_id = 0; }
                                }
                        sql_free_result($qresult);
                        }
                else {
                        $parent_id = 0;
                        }
        }

        return true;
        }

function init_published_related () {
        // initializes $published_related array
        if($done) return true;
        static $done = true;
        global $document;
        global $published_related;
        global $flavour;
        global $enabled_filter;

        $published_related = array();
        $parent_id = $document['parent_id'];

        // enforce secure db queries
        if(!is_numeric($flavour) || !is_numeric($parent_id)) return false;

        if($document['parent_id'] > 0) {
        $qresult = sql_query("SELECT node_id, value, type_id, published, datatype  FROM ".TABLE_NODES.",
                                ".TABLE_NODEDATA." WHERE parent_id=$parent_id AND node_id=".TABLE_NODES.".id AND
                                (flavour_id=$flavour OR flavour_id=0) AND name='title' $enabled_filter ORDER BY sort_order DESC");

        if(sql_num_rows($qresult) > 0) {
                while($row = sql_fetch_row($qresult)) {
                        $published_related[$row[0]]['type_id'] = $row[2];
                        $published_related[$row[0]]['published'] = $row[3];
                        $published_related[$row[0]]['title'] = stripslashes($row[1]);
                        $published_related[$row[0]]['datatype']['title'] = stripslashes($row[4]);
                        }
                sql_free_result($qresult);
                return true;
                }
        }
        }

function init_doctypes () {
        // initializes the arrays $doctypes, $docnames, $doctemplates, $docparents, $docvisible and $docdescriptions
        if($done) return true;
        static $done = true;
        global $doctypes;
        global $docnames;
        global $doctemplates;
        global $docparents;
        global $docvisible;

        $doctypes = array();
        $docnames = array();
        $doctemplates = array();
        $docparents = array();
        $docvisible = array();

        global $profile;

        if($profile['editor'] == 1) {
                $editor = ', description';
                global $docdescriptions;
                $docdescriptions = array();
                }
        else { $editor = ''; }

        $qresult = sql_query("SELECT type_id, name, template, parent_id, visible $editor FROM ".TABLE_TYPEDATA.",
        ".TABLE_TYPES." WHERE ".TABLE_TYPES.".id=".TABLE_TYPEDATA.".type_id AND platform='phpweb' ORDER BY name");

        if(sql_num_rows($qresult) > 0) {
                while($row = sql_fetch_row($qresult)) {
                        $doctypes[stripslashes($row[1])] = $row[0];
                        $docnames[$row[0]] = stripslashes($row[1]);
                        $doctemplates[$row[0]] = stripslashes($row[2]);
                        $docvisible[$row[0]] = $row[4];
                        $docparents[$row[0]] = $row[3];
                        if($editor != '') { $docdescriptions[$row[0]] = stripslashes($row[5]); }
                        }
                sql_free_result($qresult);
                }

        }

function update_node_sortorder ($node_id, $sort_order) {
        // sets the sort order of an existing document

        // enforce secure db queries
        if(!is_numeric($node_id) || !is_numeric($sort_order)) return false;

        sql_query("UPDATE ".TABLE_NODES." SET sort_order=$sort_order WHERE id=$node_id");

        return true;
        }

function get_profile ($user_id = -1, $use_groups = true) {
        // returns a user profile
        // enforce secure db queries
        if(!is_numeric($user_id) || !is_bool($use_groups)) return false;

        if($user_id == -1) {
                if(!defined("SESSION_DBID")) { return false; }
                $user_id = SESSION_DBID;
                }
        $qprofile = sql_query ("SELECT name, value FROM ".TABLE_USERDATA." WHERE user_id=".$user_id);
               if(sql_num_rows($qprofile) > 0) {
                       while($prow = sql_fetch_row($qprofile)) {
                               $profile[stripslashes($prow[0])] = stripslashes($prow[1]);
                        if(substr($prow[0], 0, 6) == 'group_' && $prow[1] == '1' && $use_groups) {
                        $profile = array_merge(get_group_profile (substr($prow[0], 6)), $profile);
                     }
                               }
                       sql_free_result($qprofile);
                       }
        return $profile;
        }

function caching ($buffer) {
        // this is used by AWF to write out cached data
        if(!defined('CACHE_FILENAME')) return false;
        global $cache_refresh;
        global $id;
        if(CACHE_TIME == 0 || isset($cache_refresh[$id])) {
                return false;
                }
        $fp = fopen (CACHE_FILENAME, "w");
        fwrite($fp,$buffer);
        fclose($fp);
        return true;
        }

function init_modules () {
        // initializes $modules array
        if($done) return true;
        static $done = true;
        global $modules;
        $modules = array();
        global $flavour;
        global $root_parent_id;
        global $parents;
        global $profile;
        global $id;
        if(!isset($parents)) init_parents();

        if($profile['editor'] && SESSION_STATUS == 'ok') { $cond = 3; }
        elseif(SESSION_STATUS == 'ok') { $cond = '2'; }
        else { $cond = '1'; }

        if(!is_numeric($root_parent_id)) $root_parent_id = 0;

        // enforce correct data types
        if(!is_numeric($flavour) || !is_numeric($id)) return false;

        // add modules to array
        $qresult = sql_query("SELECT name, placement, id, visible, removeable, flavour_id, section_id, document_id, caption FROM
                        ".TABLE_MODULES." WHERE (target = $cond) OR (target = 0) ORDER BY sort_order");

        if ($qresult) {
        while($row = sql_fetch_row($qresult)) {
                if($row[0] && $row[1] && ($row[5] == -1 || $row[5] == $flavour) && ($row[6] == -1
                || $row[6] == $root_parent_id) && ($row[7] == -1 || $row[7] == $id)) {
                        $modules[$row[2]]['id'] = $row[2];
                        $modules[$row[2]]['position'] = stripslashes($row[1]);
                        $modules[$row[2]]['visible'] = $row[3];
                        $modules[$row[2]]['removeable'] = $row[4];
                        $modules[$row[2]]['name'] = stripslashes($row[0]);
                        $modules[$row[2]]['flavour_id'] = $row[5];
                        $modules[$row[2]]['section_id'] = $row[6];
                        $modules[$row[2]]['document_id'] = $row[7];
                        $modules[$row[2]]['caption'] = stripslashes($row[8]);
                        }
                }
        sql_free_result($qresult);
        }

        return true;
        }

function search_nodes ($where, $limit = 100, $offset = 0) {
        // allows you to search nodes (for your own security: pay attention the $where parameter!)
        $start = getmicrotime();
        global $search_time;
        if(!$where) {
                return false;
                $search_time = 0;
                }
        global $flavour;

        // enforce secure db queries
        if(!is_numeric($flavour) || !is_numeric($limit)|| !is_numeric($offset)) return false;

         $qresult=sql_query("SELECT DISTINCT node_id FROM ".TABLE_NODEDATA." WHERE $where ".sql_limit($limit,$offset));
        while($row = sql_fetch_row($qresult)) {
                $result = get_node($row[0], $flavour);
                       if($result != false && $result['searchable'] == 1 && isset($result['title'])) $results[$row[0]] = $result;
                }
        sql_free_result($qresult);
         $search_time = Number_format(getmicrotime() - $start, 4, ',', '');

        if(isset($results)) {
                return $results;
                }
        else {
                return false;
                }
        }

function search_children ($where, $limit = 100, $offset = 0, $item_id = -1) {
        // similar to search_nodes() (for your own security: pay attention the $where parameter!)
        $start = getmicrotime();
        global $search_time;
        if(!$where) {
                return false;
                $search_time = 0;
                }
        if($item_id > 0) { $id = $item_id; } else { $id = $GLOBALS['id']; }
        global $flavour;

        if(!is_numeric($flavour) || !is_numeric($limit)|| !is_numeric($offset) || !is_numeric($item_id)) return false;

         $qresult=sql_query("SELECT DISTINCT node_id FROM ".TABLE_NODEDATA.", ".TABLE_NODES." WHERE
        node_id = ".TABLE_NODES.".id AND parent_id = $id AND $where ".sql_limit($limit,$offset));
        while($row = sql_fetch_row($qresult)) {
                $result = get_node($row[0], $flavour);
                       if($result != false && $result['searchable'] == 1 && isset($result['title'])) $results[$row[0]] = $result;
                }
        sql_free_result($qresult);
         $search_time = number_format(getmicrotime() - $start, 4, ',', '');

        if(isset($results)) {
                return $results;
                }
        else {
                return false ;
                }
        }

function cmp_nodes ($a, $b) {
        // AWF internal function to compare documents
        global $cmp_key_name;

        // enforce correct parameter type
        if(!is_array($a) || !is_array($b)) return false;

        if ($a[$cmp_key_name] == $b[$cmp_key_name]) return 0;
        return ($a[$cmp_key_name] < $b[$cmp_key_name]) ? -1 : 1;
        }

function sort_nodes ($nodes, $name, $reverse = false) {
        // sorts an array containing documents
        global $cmp_key_name;
           $cmp_key_name = $name;
        uasort($nodes, "cmp_nodes");
        if($reverse) { $nodes = array_reverse($nodes); }

        return $nodes;
        }

function remove_message ($message_id, $user_id = -1) {
        // removes a message from queue

        // enforce secure db queries
        if(!is_numeric($message_id)) return false;

        if($user_id > 0 && is_numeric($user_id)) { $where = 'AND user_id = '.$user_id; }
        else { $where = ''; }

        sql_query("DELETE FROM ".TABLE_MESSAGES." WHERE id = $message_id $where");

        return true;
        }

function mark_message ($message_id, $user_id = -1) {
        // marks a message as delivered

        // enforce secure db queries
        if(!is_numeric($message_id)) return false;

        if($user_id > 0 && is_numeric($user_id)) { $where = 'AND user_id = '.$user_id; }
        else { $where = ''; }

        sql_query("UPDATE ".TABLE_MESSAGES." SET delivered = ".time().", status = 1 WHERE id = $message_id $where");

        return true;
        }

function count_messages ($user_id = -1, $type = '', $status = -1, $release_date = 0, $sender_id = -1, $sender = '', $text = '', $expiration_date = 0) {
        // count message in queue

        // enforce secure db queries
        if(!is_numeric($user_id) || !is_numeric($status) || !is_numeric($release_date) || !is_numeric($sender_id) || !is_numeric($expiration_date)) return false;

        $where = '';
        $pre = '';
        if($user_id != -1) { $where .= "user_id = $user_id"; $pre = ' AND '; }
        if($type != '') { $where .= $pre."type = '".addslashes($type)."'"; $pre = ' AND '; }
        if($status != -1) { $where .= $pre."status = $status"; $pre = ' AND '; }
        if($release_date > 0) { $where .= $pre."release_date < $release_date"; $pre = ' AND '; }
        if($expiration_date > 0) { $where .= $pre."(expiration_date > $expiration_date OR expiration_date = 0)"; $pre = ' AND '; }
        if($sender_id != -1) { $where .= $pre."sender_id = $sender_id"; $pre = ' AND '; }
        if($sender != '') { $where .= $pre."sender = '".addslashes($sender)."'"; $pre = ' AND '; }
        if($text != '') { $where .= $pre."text LIKE '%".addslashes($text)."%' OR subject LIKE '%".addslashes($text)."'%)"; $pre = ' AND '; }
        if($where != '') { $where = 'WHERE '.$where; }
        $message_query = sql_query("SELECT count(*) FROM ".TABLE_MESSAGES." $where");
        $row = sql_fetch_row($message_query);
        sql_free_result($message_query);
        return $row[0];
        }

function search_messages ($user_id = -1, $type = '', $status = -1, $release_date = 0, $sender_id = -1, $sender = '', $text = '', $mark = false, $sort_dir = 'ASC', $expiration_date = 0) {
        // search for messages

        // enforce secure db queries
        if(!is_numeric($user_id) || !is_numeric($status) || !is_numeric($release_date) || !is_numeric($sender_id) || !is_numeric($expiration_date)
                || !is_bool($mark)) return false;

        $where = '';
        $pre = '';
        if($sort_dir != 'ASC') $sort_dir = 'DESC';
        if($user_id != -1) { $where .= "user_id = $user_id"; $pre = ' AND '; }
        if($type != '') { $where .= $pre."type = '".addslashes($type)."'"; $pre = ' AND '; }
        if($status != -1) { $where .= $pre."status = $status"; $pre = ' AND '; }
        if($release_date > 0) { $where .= $pre."release_date < $release_date"; $pre = ' AND '; }
        if($expiration_date > 0) { $where .= $pre."(expiration_date > $expiration_date OR expiration_date = 0)"; $pre = ' AND '; }
        if($sender_id != -1) { $where .= $pre."sender_id = $sender_id"; $pre = ' AND '; }
        if($sender != '') { $where .= $pre."sender = '".addslashes($sender)."'"; $pre = ' AND '; }
        if($text != '') { $where .= $pre."(message LIKE '%".addslashes($text)."%' OR subject LIKE '%".addslashes($text)."'%)"; $pre = ' AND '; }
        if($where != '') { $where = 'WHERE '.$where; }
        $message_query = sql_query("SELECT id, user_id, subject, message, sender, sender_id, type, status, created,
                delivered, release_date, expiration_date FROM ".TABLE_MESSAGES." $where ORDER BY created $sort_dir");
        while($row = sql_fetch_array($message_query)) {
                foreach ( $row as $key => $value )  {
                        $messages[$row['id']][$key] = stripslashes($value);
                        }
                if($mark) mark_message($row['id']);
                }
        sql_free_result($message_query);
        return $messages;
        }

function get_message ($message_id, $mark = true) {
        // returns the requested message

        // enforce secure db queries
        if(!is_numeric($message_id) || !is_bool($mark)) return false;

        $message_query = sql_query("SELECT id, user_id, subject, message, sender, sender_id, type, status, created,
                delivered, release_date, expiration_date FROM ".TABLE_MESSAGES." WHERE id = $message_id ORDER BY created");
        $row = sql_fetch_array($message_query);
        foreach ( $row as $key => $value )  {
                $message[$key] = stripslashes($value);
                }
        sql_free_result($message_query);
        if($mark) mark_message($message_id);
        return $message;
        }

function send_message ($user_id, $message, $sender_id = -1, $sender = '', $subject = '', $type = 'default', $release_date = 0, $expiration_date = 0) {
        // send a message

        // enforce correct parameter type
        if(!is_numeric($user_id) || !is_numeric($sender_id) || !is_numeric($release_date) || !is_numeric($expiration_date)) return false;

        $profile = get_profile($user_id);
        if($type == 'default') {
                if(!isset($profile['im_default']) || $profile['im_default'] == '') {
                        $type = 'instant';
                        }
                else {
                        $type = $profile['im_default'];
                        }
                }
        switch ($type) {
                case 'sms' : break;
                case 'icq' :
                        if($sender_id != -1) {
                                $from = 'From: "'.get_user_nickname($sender_id).'" <'.get_user_email($sender_id).'>';
                                }
                        else {
                                $from = 'From: Unknown User <unknown_user@'.$_SERVER['SERVER_NAME'].'>';
                                }
                        if(isset($profile['icq']) && is_numeric($profile['icq'])) {
                                mail($profile['icq'].'@pager.icq.com', $subject, $message, $from);
                                break;
                                }
                case 'instant' :
                        // secure db query enforced by using addslashes()
                        sql_query("INSERT INTO ".TABLE_MESSAGES." (user_id, subject, message, sender, sender_id, type,
                        status, created, release_date, expiration_date) VALUES ($user_id, '".addslashes($subject)."',
                        '".addslashes($message)."', '".addslashes($sender)."', $sender_id, 'instant', 0, ".time().",
                        $release_date, $expiration_date)");
                        if(!isset($profile['im_as_email']) || !$profile['im_as_email'] != '1') break;
                case 'email' :
                        if($sender_id != -1) {
                                $from = 'From: '.get_user_nickname($sender_id).' <'.get_user_email($sender_id).'>';
                                }
                        else {
                                $from = 'From: Unknown User <unknown_user@'.$_SERVER['SERVER_NAME'].'>';
                                }
                        mail(get_user_email($user_id), $subject, $message, $from);
                        break;
                }
        }

function receive_content () {
        // not yet finished
        // establish trust relationship

        // insert as XML
        // type can be SMTP and HTTP...maybe something else in the future


        // 25.05.2002: *smile* should start working on that [michael@liqudibytes.net]
        }

function send_content () {
        // not yet finished
        // establish trust relationship
        // maybe we can send all content for free!?

        // send as XML over SMTP or HTTP
        }

function init_groups () {
        // initializes $groups array
        if($done) return true;
        static $done = true;
        global $groups;
        $groups = array();
        global $groups_ids;
        $groups_ids = array();
        $qresult = sql_query("SELECT id, group_name FROM ".TABLE_GROUPS." ORDER BY group_name");
        if(sql_num_rows($qresult) == 0) {
                sql_free_result($qresult);
                return false;
                }
        while($row = sql_fetch_row($qresult)) {
                $groups[$row[0]] = stripslashes($row[1]);
                $groups_ids[stripslashes($row[1])] = $row[0];
                }
        sql_free_result($qresult);
        return true;
        }

function get_group_profile ($group_id, $recursive = true) {
        // returns the profile a user group
        // enforce secure db queries
        if(!is_numeric($group_id) || !is_bool($recursive)) return false;
        $profile = array();
        $qresult = sql_query("SELECT name, value FROM ".TABLE_GROUPDATA." WHERE group_id = $group_id");
        if(sql_num_rows($qresult) == 0) {
                sql_free_result($qresult);
                return $profile;
                }
        while($row = sql_fetch_row($qresult)) {
                $profile[stripslashes($row[0])] = stripslashes($row[1]);
                if(substr($row[0], 0, 6) == 'group_' && $row[1] == '1' && $recursive) {
                        $profile = array_merge(get_group_profile (substr($row[0], 6)), $profile);
                        }
                }
        sql_free_result($qresult);

        return $profile;
        }

function add_group ($group_name) {
        // creates a new user group

        // secure db query enforced by using addslashes()
        sql_query("INSERT INTO ".TABLE_GROUPS." (group_name) VALUES ('".addslashes($group_name)."')");

        return sql_insert_id();
        }

function remove_group ($group_id, $send_info = true) {
        // removes a user group
        global $groups;

        // enforce secure db queries
        if(!is_numeric($group_id) || !is_bool($send_info)) return false;

        $qresult = sql_query("SELECT user_id FROM ".TABLE_USERDATA." WHERE name='group_".$group_id."' AND value != '0'");
        if(sql_num_rows($qresult) > 0) {
                       while($row = sql_fetch_row($qresult)) {
                        // Remove user from group
                        remove_user_from_group ($row[0], $group_id);
                        if($send_info) {
                                // send message to all group members
                                send_message ($row[0], 'Group '.$groups[$group_id].' was removed.', 1);
                                }
                        }
                sql_free_result($qresult);
                }
        sql_query("DELETE FROM ".TABLE_GROUPS." WHERE id=$group_id");
        sql_query("DELETE FROM ".TABLE_GROUPDATA." WHERE group_id=$group_id");

        return true;
        }

function add_user_to_group ($user_id, $group_id, $send_info = true) {
        // add a new user to group
        global $groups;

        // enforce correct parameter type
        if(!is_numeric($user_id) || !is_numeric($group_id) || !is_bool($send_info)) return false;

        set_profile('group_'.$group_id, '1', $user_id);
        if($send_info) send_message ($user_id, 'You were added to group '.$groups[$group_id].'.', 1);

        return true;
        }

function remove_user_from_group ($user_id, $group_id, $send_info = true) {
        // removes a user from group
        global $groups;

        // enforce correct parameter type
        if(!is_numeric($user_id) || !is_numeric($group_id) || !is_bool($send_info)) return false;

        remove_profile('group_'.$group_id, $user_id);
        if($send_info) send_message ($user_id, 'You were removed from group '.$groups[$group_id].'.', 1);

        return true;
        }

function set_group_profile ($name, $value, $group_id) {
        // sets a key/value pair in a group profile

        // enforce secure db queries
        if(!is_numeric($group_id)) return false;

        if ($value == '' && $name != '') {
                sql_query("DELETE FROM ".TABLE_GROUPDATA." WHERE name='".addslashes($name)."' AND group_id=".$group_id);
                return true;
                }
        $qresult = sql_query ("UPDATE ".TABLE_GROUPDATA." SET value='".addslashes($value)."' WHERE name='".addslashes($name)."' AND group_id=".$group_id);
        if(sql_affected_rows($qresult) < 1) {
                $res = sql_query("SELECT id FROM ".TABLE_GROUPDATA." WHERE name='".addslashes($name)."' AND group_id=".$group_id);
                if(sql_num_rows($res) > 0) {
                        return true; }
                else {
                        sql_query("INSERT INTO ".TABLE_GROUPDATA." (name, value, group_id) VALUES ('".addslashes($name)."', '".addslashes($value)."', $group_id)");
                        }
                }

        return true;
        }

function remove_group_profile ($name, $group_id) {
        // removes key/value pair from a group profile

        // enforce secure db queries
        if(!is_numeric($group_id)) return false;

        sql_query("DELETE FROM ".TABLE_GROUPDATA." WHERE name='".addslashes($name)."' AND group_id = $group_id");

        return true;
        }

function load_lang ($name, $lang = '') {
        // load language specific constants

        // for security reasons, there shoudn't be more than one dot
        if(!(strpos($name.' '.$lang, '..') === false)) return false;

        if($lang == '') global $lang;
        if(file_exists(LANG_PATH.$lang.'/'.$name.'.'.$lang)) {
                $filename = LANG_PATH.$lang.'/'.$name.'.'.$lang;
                }
        elseif(file_exists(LANG_PATH.$lang.'/'.$name.'.'.DEFAULT_LANG)) {
                $filename = LANG_PATH.$lang.'/'.$name.'.'.DEFAULT_LANG;
                }
        else {
                return false;
                }
        $lines = file($filename);
        // parse file
        foreach ($lines as $$line_num => $line) {
                $buffer = explode("\t", $line);
                if(!strstr('#', $buffer[0])) {
                        $text   = trim(strstr($line, "\t"));
                        if(!defined('LANG_'.strtoupper($buffer[0]))) define('LANG_'.strtoupper($buffer[0]), $text);
                        }
                }

        return true;
        }

function html2txt ($content) {
        // converts a given html text to ASCII

        if(!is_string($content)) return false;

        $trans = array_flip(get_html_translation_table(HTML_ENTITIES));

        $search = array ("'<script[^>]*?>.*?</script>'si",  // Strip out javascript
                 "'<[\/\!]*?[^<>]*?>'si",           // Strip out html tags
                 "'([\r\n])[\s]+'",                 // Strip out white space
                 "'&(quot|#34);'i",                 // Replace html entities
                 "'&(amp|#38);'i",
                 "'&(lt|#60);'i",
                 "'&(gt|#62);'i",
                 "'&(nbsp|#160);'i",
                 "'&(iexcl|#161);'i",
                 "'&(cent|#162);'i",
                 "'&(pound|#163);'i",
                 "'&(copy|#169);'i");

        $replace = array ("",
                  "",
                  "\\1",
                  "\"",
                  "&",
                  "<",
                  ">",
                  " ",
                  chr(161),
                  chr(162),
                  chr(163),
                  chr(169),
                  "chr(\\1)");

        if(file_exists('inc/data/html_replace.conf')) {
                $lines = file('inc/data/html_replace.conf');
                foreach ( $lines as $line_num => $line)  {
                        $buffer = explode('        ', $line);
                        $wildcards[$buffer[0]] = trim($buffer[1]);
                        }
                }

        $content = str_replace("\n", "", $content);
        $content = strtr($content, $wildcards);
        $content = str_replace('\tab', "        ", $content);
        $content = preg_replace ($search, $replace, $content);
        $content = str_replace('\nl', "\n", $content);
        $content = strtr($content, $trans);
        $content = wordwrap($content);

        return $content;
        }

function write_log () {
        // writes out log line
        // format:
        // host timestamp id time user lang flavour design
        $fp = fopen('logs/'.date("Y_m").'.log', 'a');
        while(!$fp && is_writable('logs/'.date("Y_m").'.log')) {
                $fp = fopen('logs/'.date("Y_m").'.log', 'a');
                usleep(20);
                }
        if(defined('START')) {
                $time = substr((string) (getmicrotime() - START), 0, 6);
                }
        if(SESSION_DBID > 0) { $user = SESSION_DBID; }
        else { $user = '-'; }
        global $design;
        fwrite($fp, $_SERVER['REMOTE_ADDR']."\t".time()."\t".$GLOBALS['id']."\t$time\t$user".
                "\t".$GLOBALS['lang']."\t".$GLOBALS['flavour']."\t".$design['name']."\n");
        fclose($fp);

        return true;
        }

function awf_error_handler ($errno, $errstr, $errfile, $errline) {
        // handles errors
        $errorlevel = error_reporting(1);
        global $id;
        static $mail_sent = false;
        switch($errno) {
                case 1:
                        echo '<p><b>Fatal Error. Mail sent to webmaster...</b></p>';
                        error_log($_SERVER['REMOTE_ADDR']."\t".time()."\t".$id.
                        "\tFATAL\t$errno\t$errstr $errline\t$errfile\n", 3, 'log/'.date("Y_m").'_error.log');
                        if(!$mail_sent) {
                                // send email
                                $sent_mail = true;
                                error_log("FATAL ERROR:\nhttp://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI']."?id=$id\n\nDate:".date(DATETIME_FORMAT).
                                "\nError: $errstr\nFile: $errfile\nLine: $errline\n\nhave a nice day ;-)", 1, WEBMASTER_MAIL);
                                }
                        break;
                case 2:
                case 3:
                case 4:
                        error_log($_SERVER['REMOTE_ADDR']."\t".time()."\t".$id."\tERROR\t$errno\t$errstr $errline\t$errfile\n", 3, 'log/'.date("Y_m").'_error.log');
                        break;
                case 5:
                case 6:
                case 7:
                        error_log($_SERVER['REMOTE_ADDR']."\t".time()."\t".$id.
                        "\tWARNING\t$errno\t$errstr $errline\t$errfile\n", 3, 'log/'.date("Y_m").'_error.log');
                default: break;
                }
        error_reporting($errorlevel);

        return true;
        }

function transform ($value, $datatype) {
        // converts a value to a given type
        global $id;
        global $document;
        global $profile;
        global $wildcards;
        switch ($datatype) {
        case 'notrans': return $value;
        case 'image_hide': return '';
        case 'image':
                if(file_exists($value)) return get_image_tag($value);
                return '';
        case 'image_left':
                if(file_exists($value)) return get_image_tag($value, '','', 10, 1, 'left');
                return '';
        case 'image_right':
                if(file_exists($value)) return get_image_tag($value, '', '', '10', '1', 'right');
                return '';
        case 'image_popup':
                if(file_exists($value)) include(BASE_PATH.INC_PATH.'misc/show_image.inc'); return '';
                return '';
        case 'clean': return trim(strip_tags(replace_pattern(replace_wildcards($value, $profile), $wildcards)));
        case 'text': return nl2br(subst_document_refs(url_to_link(replace_pattern(replace_wildcards(htmlchars($value), $profile), $wildcards))));
        case 'usertext': return nl2br(url_to_link(replace_pattern(htmlchars($value), $wildcards)));
        case 'html': return subst_document_refs(replace_pattern(replace_wildcards($value, $profile), $wildcards));
        case 'extrans': return nl2br(subst_document_refs(replace_pattern(replace_wildcards($value, $profile), $wildcards)));
        case 'php': return replace_pattern(eval($value), $wildcards);
        case 'datetime': return timestamp_to_string ($value, $mode = 'datetime');
        case 'date': return timestamp_to_string ($value, $mode = 'date');
        case 'time': return timestamp_to_string ($value, $mode = 'time');
        case 'user_id': if($value == '-1') { return LANG_ANONYMOUS_USER; }
                else { return '<a href="javascript:sendmessage('.$value.')">'.get_user_nickname($value).'</a>'; }
        case 'external': readfile(trim($value)); return '';
        case 'html2txt': return html2txt($value);
        case 'country': return code_to_country($value);
        case 'currency': return number_format($value, DECIMALS, DEC_POINT, THOUSANDS_SEP).' '.CURRENCY_SYM;
        case 'xml': return ($value); // TODO!!!
        case 'dir_icons': include(BASE_PATH.INC_PATH.'misc/dir_icons.inc'); return '';
        case 'dir_list': include(BASE_PATH.INC_PATH.'misc/dir_list.inc'); return '';
        default:
                // this allows us to dynamically add new datatypes...
                $code = get_code('transform', $datatype);
                if($code != '') {
                        return eval($code);
                        }
        }

        return $value;
        }

function create_editor_input ($title, $value, $type) {
        // creates editor input field
        global $id;
        global $document;
        global $profile;
        global $wildcards;

        include(BASE_PATH.INC_PATH.'misc/'.$type.'.inc');

        return true;
        }

function remove_file ($filename) {
        // recursive remove function (hope you know, how to use it...)

        $name = str_replace(array('<','>',':',"\\",'/','|','*','?','"','..'), '_', $name);

        if(!file_exists(BASE_PATH.$filename)) return false;
        if(is_dir(BASE_PATH.$filename)) {
                $handle=opendir(BASE_PATH.$filename);
                while (false != ($file = readdir($handle))) {
                        if($file != '..' && $file != '.') remove_file ($filename.'/'.$file);
                        }
                rmdir(BASE_PATH.$filename); }
        else {
                unlink(BASE_PATH.$filename);
                }

        return true;
        }

function clear_cache ($id) {
        // clears the cache for the given document
        if(!is_numeric($id) || CACHE_TIME == '0') return false;

        remove_file(get_dir_name('cache', $id, false));

        return true;
        }

function get_age ($birthday) {
        // counts the years since a given timestamp
        if(!is_numeric($birthday)) return false;

        return floor((time() - $birthday) / 31536000);
        }

function get_online_users ($time = 10) {
        // returns a list of users currently online

        if(!is_numeric($time)) return false;

        $time = time() - ($time * 60);

        $qres = sql_query("SELECT AVG(views), AVG(registered) FROM ".TABLE_USERS);
        $row = sql_fetch_row($qres);
        sql_free_result($qres);

        $avg_views    = $row[0];
        $avg_reg_time = time() - $row[1];

        $avg_activity = ($avg_views / $avg_reg_time);

        $qres = sql_query("SELECT id, email, views, registered, last_login FROM ".TABLE_USERS." WHERE last_login >= ".date('YmdHis', $time)." AND online = 1 ORDER BY last_login DESC");

        while($row = sql_fetch_row($qres)) {

                $users[$row[0]]['email']         = stripslashes($row[1]);
                $users[$row[0]]['views']         = $row[2];
                $users[$row[0]]['registered']         = $row[3];
                $users[$row[0]]['online']         = true;
                $users[$row[0]]['activity']         = round((($row[2] / (time() - $row[3])) / (($row[2] / (time() - $row[3])) + $avg_activity)) * 100);

                $qres2 = sql_query("SELECT name, value FROM ".TABLE_USERDATA." WHERE user_id = $row[0] AND
                                        ( name = 'gender' OR name = 'nickname' OR name = 'birthday' OR name = 'icq')");
                while($row2 = sql_fetch_row($qres2)) {
                        $users[$row[0]][stripslashes($row2[0])] = stripslashes($row2[1]);
                        }

                sql_free_result($qres2);

                if(!isset($users[$row[0]]['nickname']) || $users[$row[0]]['nickname'] == '')
                        $users[$row[0]]['nickname'] = email_to_string($users[$row[0]]['email']);
                }

        sql_free_result($qres);

        return $users;
        }

function save_var ($name, $var, $user_id = -1) {
        // saves a variable

        if(!is_numeric($user_id)) return false;
        $name = str_replace(array('<','>',':',"\\",'/','|','*','?','"','..'), '_', $name);

        if($user_id != -1) {
                $dir = get_dir_name('users/private', $user_id);
                }
        else {
                $dir = BASE_PATH.INC_PATH.DATA_PATH.'vars';
                }
        $fp = fopen($dir.'/'.$name, 'w');
        if(!$fp) return false;
        fwrite($fp, serialize($var));
        fclose($fp);

        return true;
        }

function load_var ($name, $user_id = -1) {
        // load a variable

        if(!is_numeric($user_id)) return false;
        $name = str_replace(array('<','>',':',"\\",'/','|','*','?','"','..'), '_', $name);

        if($user_id != -1) {
                $dir = get_dir_name('users/private', $user_id);
                }
        else {
                $dir = BASE_PATH.INC_PATH.DATA_PATH.'vars';
                }
        if(!file_exists($dir.'/'.$name)) return NULL;
        $fp = fopen($dir.'/'.$name, 'r');
        if(!$fp) return false;
        $var = unserialize(fread($fp, filesize($dir.'/'.$name)));
        fclose($fp);

        return $var;
        }

function get_users ($userlist, $time = 10) {
        // returns a list of users contained in a given list

        if(!is_numeric($time)) return false;

        $time = time() - ($time * 60);

        $qres = sql_query("SELECT AVG(views), AVG(registered) FROM ".TABLE_USERS);
        $row = sql_fetch_row($qres);
        sql_free_result($qres);

        $avg_views    = $row[0];
        $avg_reg_time = time() - $row[1];

        $avg_activity = ($avg_views / $avg_reg_time);

        foreach ( $userlist as $key => $value) {
                $qres = sql_query("SELECT id, email, views, registered, last_login FROM ".TABLE_USERS." WHERE id = $key");

                $row = sql_fetch_row($qres);
                $users[$row[0]]['email']         = stripslashes($row[1]);
                $users[$row[0]]['views']         = $row[2];
                $users[$row[0]]['registered']         = $row[3];
                $users[$row[0]]['online']         = ($row[4] >= date(YmdHis, $time));
                $users[$row[0]]['activity']         = round((($row[2] / (time() - $row[3])) / (($row[2] / (time() - $row[3])) + $avg_activity)) * 100);

                $qres2 = sql_query("SELECT name, value FROM ".TABLE_USERDATA." WHERE user_id = $row[0] AND
                                        ( name = 'gender' OR name = 'nickname' OR name = 'birthday' OR name = 'icq')");
                while($row2 = sql_fetch_row($qres2)) {
                        $users[$row[0]][stripslashes($row2[0])] = stripslashes($row2[1]);
                        }

                sql_free_result($qres2);

                if(!isset($users[$row[0]]['nickname']) || $users[$row[0]]['nickname'] == '')
                        $users[$row[0]]['nickname'] = email_to_string($users[$row[0]]['email']);

                sql_free_result($qres);
                }

        return $users;
        }

function search_users ($search = '', $time = 10) {
        // search users

        if(!is_numeric($time)) return false;

        if($search != '') {
                $qres = sql_query("SELECT user_id FROM ".TABLE_USERDATA." WHERE (name='nickname' OR name='interests' OR name='country') AND value LIKE '%".addslashes($search)."%'");
                }
        else {
                $qres = sql_query("SELECT id FROM ".TABLE_USERS." WHERE valid = 1 ORDER BY last_login");
                }

        while($row = sql_fetch_row($qres)) {
                $userlist[$row[0]] = 1;
                }

        return get_users ($userlist, $time);
        }

function get_dir_name ($basedir, $id, $create_dir = true) {
        // returns a directory name needed for caching, file uploads,...

        if(!is_numeric($id) || !is_bool($create_dir)) return false;

        if(defined(CONFIG_CREATE_MASK)) { $CONFIG_CREATE_MASK = CONFIG_CREATE_MASK; }
        else { $CONFIG_CREATE_MASK = 0707; }
        if(!file_exists($basedir) && $create_dir) mkdir($basedir, $CONFIG_CREATE_MASK);
        if(!is_numeric($id) || $id < 0) {
                if(!file_exists($basedir.'/default') && $create_dir) mkdir($basedir.'/default', $CONFIG_CREATE_MASK);
                return ($basedir.'/default');
                }
        else {
                // A maximum of 2^15 files per directory should be allowed on 32 bit os, but we want to split up
                // after 3.000 for performance reasons (this means max. 96.000.000 files):
                $dir = $basedir.'/'.floor($id / 3000);
                if(!file_exists($dir) && $create_dir) mkdir($dir, $CONFIG_CREATE_MASK);
                $dir .= '/'.$id;
                if(!file_exists($dir) && $create_dir) mkdir($dir, $CONFIG_CREATE_MASK);
                return $dir;
                }

        }

function is_online ($user_id, $time = 10) {
        // returns true if the user is currentlt online

        if(!is_numeric($user_id) || !is_numeric($time)) return false;

        if(!is_numeric($user_id)) return false;
        $time = time() - ($time * 60);

        $qres = sql_query("SELECT last_login FROM ".TABLE_USERS." WHERE id = $user_id AND online = 1");

        $row = sql_fetch_row($qres);
        sql_free_result($qres);
        return ($row[0] >= date(YmdHis, $time));
        }

function user_since ($user_id) {
        // returns registration timestamp
        if(!is_numeric($user_id)) return false;

        $qres = sql_query("SELECT registered FROM ".TABLE_USERS." WHERE id = $user_id");

        $row = sql_fetch_row($qres);
        sql_free_result($qres);
        return $row[0];
        }

function include_modules ($position) {
        // output all modules at a given position
        global $modules;
        global $profile;
        if(isset($modules)){
                if(SESSION_STATUS != 'ok') {
                        foreach ($modules as $modid => $value)  {
                                include_module($value, 1, $position);
                                }
                        }
                else {
                        foreach ($modules as $modid => $value)  {
                                if($value['removeable'] == 1 && isset($profile['module_'.$modid])) {
                                        $value['visible'] = $profile['module_'.$modid];
                                        }
                                if(!isset($profile['module_size_'.$modid])) { $profile['module_size_'.$modid] = 1; }
                                include_module($value, $profile['module_size_'.$modid], $position);
                                }
                        }
                }
        }

function get_base_url () {
        if(SSL === true) { $result = 'https://'; }
        else { $result = 'http://'; }

        $result .= $_SERVER['SERVER_NAME'];
        if(dirname($_SERVER['SCRIPT_NAME']) != '/') {
                $result .= dirname($_SERVER['SCRIPT_NAME']);
                }
        $result .= '/';
        return $result;
        }

function get_url ($id, $params = '', $add_base_url = false) {
        // composes a url

        if(!is_numeric($id)) return false;

        if($add_base_url) {
                $result = get_base_url();
                }
        else {
                $result = '';
                }

        if(is_array($params)) {
                $param_str = '';
                $pre = '';
                foreach ( $params as $key => $value) {
                  if ( is_array($value) )
                         {
                           foreach ($value as $kkey => $vvalue) 
                           {
                             $param_str .= $pre.urlencode($key)."[".urlencode($kkey).
                               "]=".urlencode($vvalue);
                             $pre = "&";
                           }
                         }
                         else
                        $param_str .= $pre.urlencode($key).'='.urlencode($value);
                        $pre = '&';
                        }
                unset($params);
                $params = $param_str;
                }

        if(defined('URL_REWRITE')) {
                if($params != '') $params = '?'.$params;
                if($id == '') return $result."index.html$params";
                return $result."$id.html$params";
                }
        else {
                if($id == '') {
                        if($params != '') $params = '?'.$params;
                        return $result.'index.php'.$params;
                        }
                if($params != '') $params = '&'.$params;
                return $result.'index.php'.'?id='.$id.$params;
                }
        }

function get_cmodule_id ($doctype) {
        // returns the id of a cmodule with the given doctype
        global $doctypes;
        static $cmodules = array();
        if(count($cmodules) == 0) $cmodules = load_var('common_cmodules');
        if(isset($cmodules[$doctype])) {
                return $cmodules[$doctype];
                }
        else {
                $qres = sql_query("SELECT id FROM ".TABLE_NODES." WHERE type_id = ".$doctypes[$doctype]." ORDER BY sort_order LIMIT 1");
                if(sql_num_rows($qres) > 0) {
                        $row = sql_fetch_row($qres);
                        sql_free_result($qres);
                        $cmodules[$doctype] = $row[0];
                        save_var('common_cmodules', $cmodules);
                        return $row[0];
                        }
                else {
                        return -1;
                        }
                }
        }

function mkdirs ($path, $create_mask) {
        $tmp_path = '';
        $dirs = explode('/', $path);
        foreach ( $dirs as $key => $value) {
                $tmp_path .= $value.'/';
                if(!file_exists($tmp_path)) mkdir($tmp_path, $create_mask);
                @chmod ($tmp_path, $create_mask);
                }
        }

function get_document_id ($title, $flavour = -1) {
        // returns the id of the document with the given title

        if($flavour == -1) global $flavour;
        if(!is_numeric($flavour)) $flavour = 0;

        $qres = sql_query("SELECT node_id FROM ".TABLE_NODEDATA." WHERE name='title' AND value='".addslashes($title)."' AND (flavour_id = $flavour OR flavour_id = 0)");

        if(sql_num_rows($qres) > 0) {
                $row = sql_fetch_row($qres);
                sql_free_result($qres);
                return $row[0];
                }
        else {
                return false;
                }
        }

function get_document_title ($id, $flavour = -1) {
        // returns the title of the document with the given id

        if($flavour == -1) global $flavour;
        if(!is_numeric($flavour)) $flavour = 0;

        $qres = sql_query("SELECT value FROM ".TABLE_NODEDATA." WHERE name='title' AND node_id=".$id." AND (flavour_id = $flavour OR flavour_id = 0)");
        if(sql_num_rows($qres) > 0) {
                $row = sql_fetch_row($qres);
                sql_free_result($qres);
                return stripslashes($row[0]);
                }
        else {
                return false;
                }
        }

function subst_document_refs ($htmlcode) {
        // substitutes %%id%% and %%title%% with URL
        $htmlcodelist = array();
        $htmlcodelist = explode('%%', $htmlcode);
        $res = $htmlcodelist[0];
        for($i = 1; $i < count($htmlcodelist); $i = $i + 2) {
                $id = $htmlcodelist[$i];
                if(ereg("^[[:digit:]]+$", $id)) {
                        $ref_id                = $id;
                        $ref_name        = get_document_title($id);
                        }
                else {
                        $ref_id                = get_document_id($id);
                        $ref_name        = $id;
                        }
                $res = $res.'<a href="'.get_url($ref_id).'">'.$ref_name.'</a>'.$htmlcodelist[$i+1];
                }
        return $res;
        }

function get_image_tag ($filename, $alt_text = '', $link = '', $hspace = 0, $vspace = 0, $align = '', $width = -1, $height = -1) {
        // outputs XHTML comliant image tags

        // accept valid parameters only
        if($filename == '' || !is_numeric($hspace) || !is_numeric($vspace) || !is_numeric($height) || !is_numeric($width)) return false;

        // define image size
        $size = '';
        if($width  != -1) $size = 'width="'.$width.'"';
        if($height != -1) $size .= ' height="'.$height.'"';
        if($width == -1 && $height == -1) {
                $image_size = @getimagesize($filename);
                if($image_size !== NULL) $size = $image_size[3];
                }

        $filename = htmlentities($filename);
        if($alt_text != '') {
                $alt_text = htmlchars($alt_text);
                $alt_text = ' alt="'.$alt_text.'"';
                }

        if($align != '') {
                $align = ' align="'.htmlchars($align).'"';
                }

        $result = '<img src="'.$filename.'"'.$alt_text.$align.' hspace="'.$hspace.'" vspace="'.$vspace.'" '.$size.' border="0" />';
        if($link != '') $result = '<a href="'.$link.'" style="text-decoration: none">'.$result.'</a>';

        return $result;
        }

function get_code ($category, $name = '') {
        // returns sourcecode from database

        static $code;

        if($name != '') {

                // caching
                if(isset($code[$category][$name])) {
                        return $code[$category][$name];
                        }

                $where_name = "AND name = '".addslashes($name)."'";
                }

        $qres = sql_query("SELECT code, type, filename, serial FROM ".TABLE_CODE." WHERE category='".addslashes($category)."' $where_name AND active = 1");

        while($row = sql_fetch_array($qres)) {
                switch($row['type']) {
                        case 'db':
                                $result .= $row['code'];
                                break;
                        case 'file':
                                $result .= join('', file($row['filename']));
                                break;
                        case 'remote':
                                $result .= join('', file($row['filename']));
                                break;
                        }
                }
        sql_free_result($qres);

        if($name != '') {
                $code[$category][$name] = $result;
                }

        return $result;

        }

function load_code ($category, $name = '') {
        // loads and executes code

        $code = get_code($category, $name);

        if($code != '') {
                return eval($code);
                }
        }

function create_random_password ($num_chars) {
        // return a random password

        return substr(md5(uniqid(rand(),1)), 0, $num_chars);
        }

function code_to_country ($code) {
        // cobverts country code to full name
        $code = strtoupper($code);
        static $countries = array();
        if(count($countries) === 0) {
                $countries = unserialize(join('', file(BASE_PATH.INC_PATH.DATA_PATH.'countries.ser')));
                }
        return $countries[$code];
        }

function items_get_item_string ($item_id) {
        // return string representation of a item

        if(!is_numeric($item_id)) return false;

        $qres = sql_query("SELECT caption FROM ".TABLE_ITEMS." WHERE id = $item_id");

        if(sql_num_rows($qres) > 0) {
                $row = sql_fetch_row($qres);
                sql_free_result($qres);
                return stripslashes($row[0]);
                }
        else {
                return '';
                }

        }

function items_get_array ($category, $target, $target_id) {
        // todo

        if($category == '' || $target == '' || !is_numeric($target_id)) return false;

        $result = array();

        $qres = sql_query("SELECT caption FROM ".TABLE_ITEMS.",".TABLE_ITEMMAP." WHERE item_id=".TABLE_ITEMS.".id AND target='".addslashes($target)."' AND ".
                        "target_id=$target_id AND category='".addslashes($category)."' ORDER BY sort_order");

        while($row = sql_fetch_row($qres)) {
                $result[] = stripslashes($row[0]);
                }
        return $result;
        }

function items_get_id_array ($category, $target, $target_id) {
        // todo

        if($category == '' || $target == '' || !is_numeric($target_id)) return false;

        $result = array();

        $qres = sql_query("SELECT 1_items.id FROM ".TABLE_ITEMS.",".TABLE_ITEMMAP." WHERE item_id=".TABLE_ITEMS.".id AND target='".addslashes($target)."' AND ".
                        "target_id=$target_id AND category='".addslashes($category)."' ORDER BY sort_order");

        while($row = sql_fetch_row($qres)) {
                $result[] = $row[0];
                }
        return $result;
        }

function items_get_items ($category) {
        // get array of all items of a category

        if($category == '') return false;

        $result = array();

        $qres = sql_query("SELECT id, caption FROM ".TABLE_ITEMS." WHERE category='".addslashes($category)."' ORDER BY sort_order");

        while($row = sql_fetch_row($qres)) {
                $result[$row[0]] = stripslashes($row[1]);
                }
        return $result;
        }

function items_get_string ($category, $target, $target_id) {
        // returns string representation of associated items

        if($category == '' || $target == '' || !is_numeric($target_id)) return false;

        return join(', ', items_get_array($category, $target, $target_id));
        }

function items_save_docinput ($select, $target, $target_id, $category) {
        // todo

        if($target == '' || !is_numeric($target_id)) return false;
        if(!is_array($select)) $select = array();

        $query = '';
        $pre = '';

        $qres = sql_query("SELECT id FROM ".TABLE_ITEMS." WHERE category='".addslashes($category)."'");

        while($row = sql_fetch_row($qres)) {
                $query = $query.$pre.' item_id='.$row[0];
                $pre = ' OR ';
                }

        sql_query("DELETE FROM ".TABLE_ITEMMAP." WHERE target='".addslashes($target)."' AND target_id=$target_id AND (".$query.")");
        foreach ($select as $key => $value) {
                if(is_numeric($value)) {
                        sql_query("INSERT INTO ".TABLE_ITEMMAP." SET item_id=$value, target='".addslashes($target)."', target_id=$target_id");
                        }
                }
        return true;
        }

function items_create_docinput ($category, $target = '', $target_id = 0, $type = 'dropdown', $autosave = false) {
        // type can be dropdown, multi_select, radio and checkbox

        // secure db query
        if(!is_numeric($target_id)) return false;

        if($autosave == true) items_save_docinput($_REQUEST['docinput'][$category], $target, $target_id, $category);

        $qres = sql_query("SELECT item_id FROM ".TABLE_ITEMMAP." WHERE target='".addslashes($target)."' AND target_id=$target_id");
        while($row = sql_fetch_row($qres)) {
                $preselected[$row[0]] = true;
                }
        sql_free_result($qres);

        $qres = sql_query("SELECT id, caption FROM ".TABLE_ITEMS." WHERE category='".addslashes($category)."' AND active = 1 ORDER BY sort_order");

        if($type == 'dropdown') {
                echo '<select name="docinput['.$category.'][]">'."\n";
                while($row = sql_fetch_row($qres)) {
                        echo '  <option value="'.$row[0].'"';
                        if(isset($preselected[$row[0]])) echo ' selected="true"';
                        echo '>'.htmlchars($row[1])."</option>\n";
                        }
                echo "</select>\n";
                }

        elseif($type == 'multi_select') {
                echo '<select size="5" multiple="true" name="docinput['.$category.'][]">'."\n";
                while($row = sql_fetch_row($qres)) {
                        echo '  <option value="'.$row[0].'"';
                        if(isset($preselected[$row[0]])) echo ' selected="true"';
                        echo '>'.htmlchars($row[1])."</option>\n";
                        }
                echo "</select>\n";
                }

        elseif($type == 'radio') {
                while($row = sql_fetch_row($qres)) {
                        echo '  <input type="radio" name="docinput['.$category.'][]" value="'.$row[0].'"';
                        if(isset($preselected[$row[0]])) echo ' checked="true"';
                        echo ' /> '.htmlchars($row[1])."\n";
                        }
                }

        elseif($type == 'radio_br') {
                while($row = sql_fetch_row($qres)) {
                        echo '  <input type="radio" name="docinput['.$category.'][]" value="'.$row[0].'"';
                        if(isset($preselected[$row[0]])) echo ' checked="true"';
                        echo ' /> '.htmlchars($row[1])."<br />\n";
                        }
                }

        elseif($type == 'checkbox') {
                while($row = sql_fetch_row($qres)) {
                        echo '  <input type="checkbox" name="docinput['.$category.'][]" value="'.$row[0].'"';
                        if(isset($preselected[$row[0]])) echo ' checked="true"';
                        echo ' /> '.htmlchars($row[1])."\n";
                        }
                }

        elseif($type == 'checkbox_br') {
                while($row = sql_fetch_row($qres)) {
                        if($row[1] == '') {
                                echo '<br />';
                                }
                        else {
                                echo '  <input type="checkbox" name="docinput['.$category.'][]" value="'.$row[0].'"';
                                if(isset($preselected[$row[0]])) echo ' checked="true"';
                                echo ' /> '.htmlchars($row[1])."<br />\n";
                                }
                        }
                }

        sql_free_result($qres);

        return true;
        }

function add_blob ($file, $mime, $filename, $target, $target_id) {
        // add new blob

        if(isset($file) && file_exists($file) && is_numeric($target_id)) {
                $fp = fopen($file, "rb");
                $file_contents = fread($fp, filesize($file));
                fclose($fp);
                $result = md5(uniqid(rand(), 1));
                sql_query("INSERT INTO ".TABLE_BLOBS." SET sid = '".$result."', data = '".addslashes(base64_encode($file_contents))."', ".
                        "mime = '".addslashes($mime)."', filename = '".addslashes($filename)."', target = '".addslashes($target)."', ".
                        "target_id = $target_id, created = ".time().", last_change = ".time());
                return $result;
                }
        return false;
        }

function get_blob ($sid) {
        // returns blob

        $qres = sql_query("SELECT data FROM ".TABLE_BLOBS." WHERE sid = '".addslashes($sid)."'");
        if(sql_num_rows($qres) > 0) {
                $row = sql_fetch_row($qres);
                sql_free_result($qres);
                return base64_decode(stripslashes($row[0]));
                }
        return false;
        }

function output_blob ($sid) {
        // directly ouputs a blob

        $qres = sql_query("SELECT data, mime FROM ".TABLE_BLOBS." WHERE sid = '".addslashes($sid)."'");
        if(sql_num_rows($qres) > 0) {
                $row = sql_fetch_row($qres);
                sql_free_result($qres);
                if(!headers_sent()) header("Content-type: ".stripslashes($row[1]));
                echo base64_decode(stripslashes($row[0]));
                return true;
                }
        return false;
        }

function sql_queries ($query_string, $table_prefix_wildcard = 'table_prefix_') {
        global $db_table_prefix;
        $sql_queries = explode(";\n", $query_string);
        foreach ( $sql_queries as $line_nr => $sql_query) {
                $sql_query = trim($sql_query);
                if($sql_query != '') {
                        $sql_query = str_replace($table_prefix_wildcard, $db_table_prefix, $sql_query);
                        sql_query($sql_query);
                        }
                }
        }

if(file_exists('inc/custom/custom_functions.inc')) include('inc/custom/custom_functions.inc');

?>
