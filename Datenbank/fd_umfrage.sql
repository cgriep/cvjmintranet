CREATE TABLE `fd_Umfrage` (
  `id` char(36) NOT NULL,
  `Buchungsnummer` int(11) NOT NULL,
  `Name` varchar(100) NOT NULL,
  `Von` int(11) NOT NULL,
  `Bis` int(11) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
