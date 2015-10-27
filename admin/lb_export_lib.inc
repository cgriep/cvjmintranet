<?php
/*
        Copyright (C) 2000-2004 Liquid Bytes (R) Technologies, Germany. All rights reserved.
        http://www.liquidbytes.net/

        This file is part of the Liquid Bytes (R) Adaptive Website Framework (AWF)
        The author is Michael Mayer (michael@liquidbytes.net)
        Last update: 07.04.2004
*/

function lb_export_get_header ($format, $cols) {
	switch($format) {
		case 'html': 
			$result = "<html>\n<head>\n<title>Export</title>\n</head>\n<body bgcolor=\"#ffffff\">\n".
					"<table border=\"0\" cellspacing=\"1\" cellpadding=\"2\" ".
					"bgcolor=\"#000000\">\n<tr bgcolor=\"#dddddd\">\n";
			while(list($key,$value) = each($cols)) {
				$result .= "<td><b>$value</b></td>\n";
				}
			$result .= "</tr>\n";
			return $result;
		case 'xml': 
			$result = '<?xml version="1.0" encoding="UTF-8"?>'."\n<awf_export>\n";
			return $result;
		case 'text': 
			while(list($key, $value) = each($cols)) {
				$row[$key] = strtr(str_pad($value, 8), "\n", " ");
				}
			$result = join(" | ", $row)."\n";
			$result .= str_repeat('=', strlen($result) - 1)."\n";
			return $result;
		case 'csv': 
			while(list($key, $value) = each($cols)) {
				$row[$key] = strtr(strtr($value, '"', "'"), "\n", " ");
				}
			return '"'.join('","', $row).'"'."\n";
		}
	}

function lb_export_get_row ($format, $row) {
	switch($format) {
		case 'html': 
			$result = "<tr bgcolor=\"#ffffff\">\n";
			while(list($key,$value) = each($row)) {
				$result .= "<td>$value</td>\n";
				}
			$result .= "</tr>\n";
			return $result;
		case 'xml':
			$result = "<row>\n";
			while(list($key,$value) = each($row)) {
				$result .= "<$key>".utf8_encode($value)."</$key>\n";
				}
			$result .= "</row>\n";
			return $result;
		case 'text': 
			while(list($key, $value) = each($row)) {
				$row[$key] = strtr(str_pad($value, 8), "\n", " ");
				}
			return join(" | ", $row)."\n";
		case 'csv': 
			while(list($key, $value) = each($row)) {
				$row[$key] = strtr(strtr($value, '"', "'"), "\n", " ");
				}
			return '"'.join('","', $row).'"'."\n";
		}
	}

function lb_export_get_footer ($format, $cols) {
	switch($format) {
		case 'html':
			return "</tr>\n</table>\n</body></html>";
			break;
		case 'xml':
			return "</awf_export>";
		}
	}

?>
