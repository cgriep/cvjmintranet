DROP SEQUENCE "awf_setup_id_seq";
CREATE SEQUENCE "awf_setup_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
DROP SEQUENCE "awf_modules_id_seq";
CREATE SEQUENCE "awf_modules_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
DROP SEQUENCE "awf_flavours_id_seq";
CREATE SEQUENCE "awf_flavours_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
DROP SEQUENCE "awf_nodedata_id_seq";
CREATE SEQUENCE "awf_nodedata_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
DROP SEQUENCE "awf_nodes_id_seq";
CREATE SEQUENCE "awf_nodes_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
SELECT nextval ('"awf_nodes_id_seq"');
DROP SEQUENCE "awf_polls_id_seq";
CREATE SEQUENCE "awf_polls_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
DROP SEQUENCE "awf_polls_items_id_seq";
CREATE SEQUENCE "awf_polls_items_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
DROP SEQUENCE "1_redirect_id_seq";
CREATE SEQUENCE "1_redirect_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
DROP SEQUENCE "awf_typedata_id_seq";
CREATE SEQUENCE "awf_typedata_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
DROP SEQUENCE "awf_types_id_seq";
CREATE SEQUENCE "awf_types_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
DROP SEQUENCE "awf_userdata_id_seq";
CREATE SEQUENCE "awf_userdata_id_seq" start 22 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
SELECT nextval ('"awf_userdata_id_seq"');
DROP SEQUENCE "awf_users_id_seq";
CREATE SEQUENCE "awf_users_id_seq" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;
SELECT nextval ('"awf_users_id_seq"');
DROP TABLE "awf_setup";
CREATE TABLE "awf_setup" (
	"id" int4 DEFAULT nextval('awf_setup_id_seq'::text) NOT NULL,
	"name" character varying(20) NOT NULL,
	"value" character varying(128) NOT NULL
);
DROP TABLE "awf_modules";
CREATE TABLE "awf_modules" (
	"id" int4 DEFAULT nextval('awf_modules_id_seq'::text) NOT NULL,
	"sort_order" int8 NOT NULL,
	"name" character varying(64) NOT NULL,
	"placement" character varying(20) NOT NULL,
	"flavour_id" int8 DEFAULT -1 NOT NULL,
	"section_id" int8 DEFAULT -1 NOT NULL,
	"document_id" int8 DEFAULT 1 NOT NULL,
	"visible" int2 DEFAULT 1 NOT NULL,
	"removeable" int2 NOT NULL,
	"target" int2 NOT NULL,
	"caption" text NOT NULL,
	PRIMARY KEY ("id")
);
DROP TABLE "awf_flavours";
CREATE TABLE "awf_flavours" (
	"id" int4 DEFAULT nextval('awf_flavours_id_seq'::text) NOT NULL,
	"name" character varying(30) NOT NULL,
	"published" int2 NOT NULL,
	PRIMARY KEY ("id")
);
DROP TABLE "awf_nodedata";
CREATE TABLE "awf_nodedata" (
	"id" int4 DEFAULT nextval('awf_nodedata_id_seq'::text) NOT NULL,
	"value" text NOT NULL,
	"flavour_id" int8 NOT NULL,
	"version" character varying(8) NOT NULL,
	"node_id" int8 NOT NULL,
	"name" character varying(60) NOT NULL,
	PRIMARY KEY ("id")
);
DROP TABLE "awf_nodes";
CREATE TABLE "awf_nodes" (
	"id" int4 DEFAULT nextval('awf_nodes_id_seq'::text) NOT NULL,
	"sort_order" int8 NOT NULL,
	"parent_id" int8 NOT NULL,
	"enabled" int2 NOT NULL,
	"searchable" int2 NOT NULL,
	"cacheable" int2 NOT NULL,
	"type_id" int8 NOT NULL,
	"published" int2 NOT NULL,
	"description" character varying(50) NOT NULL,
	"key_id" int8 NOT NULL,
	"views" float8 NOT NULL,
	PRIMARY KEY ("id")
);
DROP TABLE "awf_polls";
CREATE TABLE "awf_polls" (
	"id" int4 DEFAULT nextval('awf_polls_id_seq'::text) NOT NULL,
	"question" text NOT NULL,
	"active" int2 NOT NULL,
	"sortorder" int8 NOT NULL,
	"last_access" timestamp NOT NULL,
	PRIMARY KEY ("id")
);
DROP TABLE "awf_polls_items";
CREATE TABLE "awf_polls_items" (
	"id" int4 DEFAULT nextval('awf_polls_items_id_seq'::text) NOT NULL,
	"poll_id" int8 NOT NULL,
	"text" text NOT NULL,
	"counter" int8 NOT NULL,
	"last_access" timestamp NOT NULL
);
DROP TABLE "awf_redirect";
CREATE TABLE "awf_redirect" (
	"id" int4 DEFAULT nextval('1_redirect_id_seq'::text) NOT NULL,
	"url" character varying(256) NOT NULL,
	"referer" character varying(256) NOT NULL,
	"views" int8 NOT NULL
);
DROP TABLE "awf_typedata";
CREATE TABLE "awf_typedata" (
	"id" int4 DEFAULT nextval('awf_typedata_id_seq'::text) NOT NULL,
	"platform" character varying(32) NOT NULL,
	"template" character varying(64) NOT NULL,
	"visible" int2 NOT NULL,
	"type_id" int8 NOT NULL,
	PRIMARY KEY ("id")
);
DROP TABLE "awf_types";
CREATE TABLE "awf_types" (
	"id" int4 DEFAULT nextval('awf_types_id_seq'::text) NOT NULL,
	"parent_id" int8 NOT NULL,
	"name" character varying(32) NOT NULL,
	"description" character varying(128) NOT NULL,
	PRIMARY KEY ("id")
);
DROP TABLE "awf_userdata";
CREATE TABLE "awf_userdata" (
	"id" int4 DEFAULT nextval('awf_userdata_id_seq'::text) NOT NULL,
	"user_id" int8 NOT NULL,
	"name" text NOT NULL,
	"value" text NOT NULL,
	PRIMARY KEY ("id")
);
DROP TABLE "awf_users";
CREATE TABLE "awf_users" (
	"id" int4 DEFAULT nextval('awf_users_id_seq'::text) NOT NULL,
	"email" character varying(128) NOT NULL,
	"password" character varying(32) NOT NULL,
	"valid" int2 NOT NULL,
	"views" int8 NOT NULL,
	"last_login" timestamp NOT NULL
);
INSERT INTO "awf_setup" VALUES (4,'image_rpath','img/');
INSERT INTO "awf_setup" VALUES (5,'inc_path','inc/');
INSERT INTO "awf_setup" VALUES (7,'design_inc','design.inc');
INSERT INTO "awf_setup" VALUES (8,'init_inc','init.inc');
INSERT INTO "awf_setup" VALUES (9,'header_inc','header.inc');
INSERT INTO "awf_setup" VALUES (10,'content_inc','content.inc');
INSERT INTO "awf_setup" VALUES (11,'footer_inc','footer.inc');
INSERT INTO "awf_setup" VALUES (12,'shutdown_inc','shutdown.inc');
INSERT INTO "awf_setup" VALUES (13,'module_path','modules/');
INSERT INTO "awf_setup" VALUES (14,'design_path','design/');
INSERT INTO "awf_setup" VALUES (15,'cmodules_path','cmodules/');
INSERT INTO "awf_setup" VALUES (16,'default_background','#ffffff');
INSERT INTO "awf_setup" VALUES (17,'default_linkcolor','#ff0000');
INSERT INTO "awf_setup" VALUES (18,'default_textcolor','#000000');
INSERT INTO "awf_setup" VALUES (19,'default_vlinkcolor','#aa0000');
INSERT INTO "awf_setup" VALUES (21,'default_header','default');
INSERT INTO "awf_setup" VALUES (23,'data_path','data/');
INSERT INTO "awf_setup" VALUES (24,'header_path','header/');
INSERT INTO "awf_setup" VALUES (25,'footer_path','footer/');
INSERT INTO "awf_setup" VALUES (6,'default_document','80');
INSERT INTO "awf_setup" VALUES (22,'default_flavour','1');
INSERT INTO "awf_setup" VALUES (28,'show_warnings','0');
INSERT INTO "awf_setup" VALUES (30,'count_views','0');
INSERT INTO "awf_setup" VALUES (29,'cache_time','0');
INSERT INTO "awf_setup" VALUES (1,'base_path','/var/www/html/cdrom/awf/');
INSERT INTO "awf_setup" VALUES (2,'webmaster_mail','webmaster@postgres.de');
INSERT INTO "awf_setup" VALUES (3,'site_title','PostgreSQL');
INSERT INTO "awf_setup" VALUES (20,'default_design','Default');
INSERT INTO "awf_setup" VALUES (27,'um_store_method','sql');
INSERT INTO "awf_modules" VALUES (1,'4','navigation.inc','left','-1','-1','-1',1,0,0,'Navigation');
INSERT INTO "awf_modules" VALUES (2,'2','login.inc','right','-1','-1','-1',1,0,0,'Login');
INSERT INTO "awf_modules" VALUES (4,'9','design.inc','left','-1','-1','-1',1,1,0,'Design');
INSERT INTO "awf_modules" VALUES (6,'15','search.inc','left','-1','-1','-1',1,1,0,'Search');
INSERT INTO "awf_modules" VALUES (5,'10','links.inc','right','1','2','-1',1,1,0,'Links');
INSERT INTO "awf_modules" VALUES (7,'1','polls.inc','right','-1','-1','-1',1,1,2,'Polls');
INSERT INTO "awf_modules" VALUES (8,'1','newdoc.inc','right','-1','-1','-1',1,0,3,'Create new document');
INSERT INTO "awf_modules" VALUES (9,'5','flavours.inc','left','-1','-1','-1',1,1,0,'Flavours');
INSERT INTO "awf_modules" VALUES (3,'8','didyouknow.inc','right','-1','-1','-1',1,1,1,'Did you know?');
INSERT INTO "awf_flavours" VALUES (1,'Deutsch',1);
INSERT INTO "awf_flavours" VALUES (2,'English',1);
INSERT INTO "awf_nodedata" VALUES (4,'News','1','0','2','title');
INSERT INTO "awf_nodedata" VALUES (833,'','1','0','154','keywords');
INSERT INTO "awf_nodedata" VALUES (300,'Welcome to your new AWF installation','1','0','83','title');
INSERT INTO "awf_nodedata" VALUES (351,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (22,'News section','2','0','2','title');
INSERT INTO "awf_nodedata" VALUES (482,'','1','0','59','keywords');
INSERT INTO "awf_nodedata" VALUES (340,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (832,'996673265','1','0','153','timestamp');
INSERT INTO "awf_nodedata" VALUES (346,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (189,'','1','0','2','keywords');
INSERT INTO "awf_nodedata" VALUES (190,'','1','0','59','keywords');
INSERT INTO "awf_nodedata" VALUES (831,'a','1','0','153','author');
INSERT INTO "awf_nodedata" VALUES (188,'991512643','1','0','2','timestamp');
INSERT INTO "awf_nodedata" VALUES (187,'a','1','0','2','author');
INSERT INTO "awf_nodedata" VALUES (186,'','1','0','2','keywords');
INSERT INTO "awf_nodedata" VALUES (301,'If you can read this, AWF seems to be installed correctly. This is the demo content that was written to your database while setup. The German documentation can be found <a href=\\"docs/german.sdw\\">here</a>. An English version is not available yet, sorry.','1','0','83','body');
INSERT INTO "awf_nodedata" VALUES (711,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (355,'','1','0','2','keywords');
INSERT INTO "awf_nodedata" VALUES (628,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (138,'Suchen','1','0','59','title');
INSERT INTO "awf_nodedata" VALUES (830,'996673254','2','0','153','timestamp');
INSERT INTO "awf_nodedata" VALUES (299,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (338,'','1','0','59','keywords');
INSERT INTO "awf_nodedata" VALUES (829,'a','2','0','153','author');
INSERT INTO "awf_nodedata" VALUES (710,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (828,'Liquid Bytes is a small source forge in Tuebingen, Germany. If you want more information, please have a look at liquidbytes.net.\015\012\015\012','2','0','153','body');
INSERT INTO "awf_nodedata" VALUES (141,'a','1','0','59','author');
INSERT INTO "awf_nodedata" VALUES (142,'991566184','1','0','59','timestamp');
INSERT INTO "awf_nodedata" VALUES (827,'What\\''s Liquid Bytes?','2','0','153','title');
INSERT INTO "awf_nodedata" VALUES (292,'992687384','1','0','80','timestamp');
INSERT INTO "awf_nodedata" VALUES (291,'a','1','0','80','author');
INSERT INTO "awf_nodedata" VALUES (290,'1','1','0','80','comments');
INSERT INTO "awf_nodedata" VALUES (826,'','2','0','153','keywords');
INSERT INTO "awf_nodedata" VALUES (289,'Home','1','0','80','title');
INSERT INTO "awf_nodedata" VALUES (288,'','1','0','80','keywords');
INSERT INTO "awf_nodedata" VALUES (715,'b','1','0','143','author');
INSERT INTO "awf_nodedata" VALUES (716,'992688137','1','0','143','timestamp');
INSERT INTO "awf_nodedata" VALUES (713,'Test','1','0','143','title');
INSERT INTO "awf_nodedata" VALUES (714,'This is simple test comment.','1','0','143','body');
INSERT INTO "awf_nodedata" VALUES (761,'1','2','0','146','comments');
INSERT INTO "awf_nodedata" VALUES (762,'An images\\'' bombing. The sequence of Carlo Giuliani\\''s death, keeps looping, over and over. Together with the riots, police charges, rain of tear-gas and stones, devastations and fires, faces covered with blood, shot from one thousand different angles. Images of a not so far war, not any more in Palestine, Afghanistan, Ireland. A war exploded in Genova, Western Europe, under the distracted eyes of the Great Eight, and then suddenly vanished from the streets, like a flower of fire. \015\012\015\012\015\012','2','0','146','body');
INSERT INTO "awf_nodedata" VALUES (763,'a','2','0','146','author');
INSERT INTO "awf_nodedata" VALUES (764,'996670524','2','0','146','timestamp');
INSERT INTO "awf_nodedata" VALUES (731,'','1','0','142','keywords');
INSERT INTO "awf_nodedata" VALUES (703,'','1','0','142','keywords');
INSERT INTO "awf_nodedata" VALUES (707,'','1','0','142','keywords');
INSERT INTO "awf_nodedata" VALUES (708,'','1','0','142','keywords');
INSERT INTO "awf_nodedata" VALUES (729,'','1','0','142','keywords');
INSERT INTO "awf_nodedata" VALUES (724,'','1','0','142','keywords');
INSERT INTO "awf_nodedata" VALUES (727,'','1','0','142','keywords');
INSERT INTO "awf_nodedata" VALUES (704,'Public Bytes','1','0','142','title');
INSERT INTO "awf_nodedata" VALUES (725,'1','1','0','142','comments');
INSERT INTO "awf_nodedata" VALUES (759,'','2','0','146','keywords');
INSERT INTO "awf_nodedata" VALUES (760,'Justice in the cross-fire of institutions','2','0','146','title');
INSERT INTO "awf_nodedata" VALUES (302,'a','1','0','83','author');
INSERT INTO "awf_nodedata" VALUES (303,'996572789','1','0','83','timestamp');
INSERT INTO "awf_nodedata" VALUES (304,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (712,'','1','0','80','keywords');
INSERT INTO "awf_nodedata" VALUES (322,'','1','0','80','keywords');
INSERT INTO "awf_nodedata" VALUES (746,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (747,'Welcome to your new AWF installation','1','0','83','title');
INSERT INTO "awf_nodedata" VALUES (754,'a','1','0','83','author');
INSERT INTO "awf_nodedata" VALUES (755,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (748,'a','1','0','83','author');
INSERT INTO "awf_nodedata" VALUES (749,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (750,'Welcome to your new AWF installation','1','0','83','title');
INSERT INTO "awf_nodedata" VALUES (751,'a','1','0','83','author');
INSERT INTO "awf_nodedata" VALUES (752,'','1','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (757,'If you can read this, AWF seems to be installed correctly. This is the demo content that was written to your database while setup. The German documentation can be found <a href=\\"docs/german.sdw\\">here</a>. An English version is not available yet, sorry.','1','0','83','body');
INSERT INTO "awf_nodedata" VALUES (753,'Welcome to your new AWF installation','1','0','83','title');
INSERT INTO "awf_nodedata" VALUES (756,'Welcome to your new AWF installation','1','0','83','title');
INSERT INTO "awf_nodedata" VALUES (758,'a','1','0','83','author');
INSERT INTO "awf_nodedata" VALUES (781,'','2','0','146','keywords');
INSERT INTO "awf_nodedata" VALUES (782,'1','2','0','146','comments');
INSERT INTO "awf_nodedata" VALUES (783,'a','2','0','146','author');
INSERT INTO "awf_nodedata" VALUES (817,'a','2','0','83','author');
INSERT INTO "awf_nodedata" VALUES (816,'Hi there, just a stupid test :o)','2','0','83','body');
INSERT INTO "awf_nodedata" VALUES (815,'This is the English front page.','2','0','83','title');
INSERT INTO "awf_nodedata" VALUES (814,'','2','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (813,'996673182','2','0','80','timestamp');
INSERT INTO "awf_nodedata" VALUES (812,'a','2','0','80','author');
INSERT INTO "awf_nodedata" VALUES (811,'1','2','0','80','comments');
INSERT INTO "awf_nodedata" VALUES (806,'fsdfsd','1','0','151','body');
INSERT INTO "awf_nodedata" VALUES (805,'sdfsd','1','0','151','title');
INSERT INTO "awf_nodedata" VALUES (810,'Home','2','0','80','title');
INSERT INTO "awf_nodedata" VALUES (809,'','2','0','80','keywords');
INSERT INTO "awf_nodedata" VALUES (807,'b','1','0','151','author');
INSERT INTO "awf_nodedata" VALUES (808,'996671888','1','0','151','timestamp');
INSERT INTO "awf_nodedata" VALUES (818,'996674218','2','0','83','timestamp');
INSERT INTO "awf_nodedata" VALUES (819,'','2','0','80','keywords');
INSERT INTO "awf_nodedata" VALUES (820,'Home','2','0','80','title');
INSERT INTO "awf_nodedata" VALUES (821,'a','2','0','80','author');
INSERT INTO "awf_nodedata" VALUES (822,'eng','2','0','152','title');
INSERT INTO "awf_nodedata" VALUES (823,'asd','2','0','152','body');
INSERT INTO "awf_nodedata" VALUES (824,'b','2','0','152','author');
INSERT INTO "awf_nodedata" VALUES (825,'996673202','2','0','152','timestamp');
INSERT INTO "awf_nodedata" VALUES (834,'What\\''s Liquid Bytes?','1','0','154','title');
INSERT INTO "awf_nodedata" VALUES (835,'Liquid Bytes is a small source forge in Tuebingen, Germany. If you want more information, please have a look at liquidbytes.net.','1','0','154','body');
INSERT INTO "awf_nodedata" VALUES (836,'a','1','0','154','author');
INSERT INTO "awf_nodedata" VALUES (837,'996673308','1','0','154','timestamp');
INSERT INTO "awf_nodedata" VALUES (838,'','2','0','83','keywords');
INSERT INTO "awf_nodedata" VALUES (839,'a','2','0','83','author');
INSERT INTO "awf_nodedata" VALUES (732,'Blafhgouhfo','1','0','142','header');
INSERT INTO "awf_nodedata" VALUES (730,'Blafhgouhfo','1','0','142','header');
INSERT INTO "awf_nodedata" VALUES (728,'Blafhgouhfo','1','0','142','header');
INSERT INTO "awf_nodedata" VALUES (726,'Blafhgouhfo','1','0','142','header');
INSERT INTO "awf_nodedata" VALUES (705,'a','1','0','142','author');
INSERT INTO "awf_nodedata" VALUES (706,'997100106','1','0','142','timestamp');
INSERT INTO "awf_nodedata" VALUES (775,'a','1','0','146','author');
INSERT INTO "awf_nodedata" VALUES (780,'a','1','0','146','author');
INSERT INTO "awf_nodedata" VALUES (733,'','1','0','145','keywords');
INSERT INTO "awf_nodedata" VALUES (737,'','1','0','145','keywords');
INSERT INTO "awf_nodedata" VALUES (740,'','1','0','145','keywords');
INSERT INTO "awf_nodedata" VALUES (741,'','1','0','145','keywords');
INSERT INTO "awf_nodedata" VALUES (742,'','1','0','145','keywords');
INSERT INTO "awf_nodedata" VALUES (744,'','1','0','145','keywords');
INSERT INTO "awf_nodedata" VALUES (734,'Register / Change Settings','1','0','145','title');
INSERT INTO "awf_nodedata" VALUES (738,'<b>Create new user...</b>','1','0','145','header_new');
INSERT INTO "awf_nodedata" VALUES (743,'Now you may add comments or take part at the polls.','1','0','145','reg_success');
INSERT INTO "awf_nodedata" VALUES (739,'<b>Change your settings...</b>','1','0','145','header_change');
INSERT INTO "awf_nodedata" VALUES (735,'a','1','0','145','author');
INSERT INTO "awf_nodedata" VALUES (736,'997100285','1','0','145','timestamp');
INSERT INTO "awf_nodedata" VALUES (771,'','1','0','146','keywords');
INSERT INTO "awf_nodedata" VALUES (765,'','1','0','146','keywords');
INSERT INTO "awf_nodedata" VALUES (776,'','1','0','146','keywords');
INSERT INTO "awf_nodedata" VALUES (772,'Code Red: Das Internet lebt noch WEITER','1','0','146','title');
INSERT INTO "awf_nodedata" VALUES (777,'Code Red: Das Internet lebt noch WEITER','1','0','146','title');
INSERT INTO "awf_nodedata" VALUES (773,'1','1','0','146','comments');
INSERT INTO "awf_nodedata" VALUES (766,'Code Red: Das Internet lebt noch WEITER','1','0','146','title');
INSERT INTO "awf_nodedata" VALUES (778,'1','1','0','146','comments');
INSERT INTO "awf_nodedata" VALUES (769,'a','1','0','146','author');
INSERT INTO "awf_nodedata" VALUES (767,'1','1','0','146','comments');
INSERT INTO "awf_nodedata" VALUES (768,'Die vom FBI bef\374rchtete \334berflutung des Internet mit dem Code-Red-Wurm hat nicht stattgefunden \226 bislang sind kaum Aktivit\344ten des Sch\344dlings festzustellen. Dabei h\344tte es laut dem NIPC (National Infrastructure Protection Center) heute nacht um zwei Uhr deutscher Zeit zu einer weiteren gro\337en Verbreitungswelle kommen sollen.\015\012\015\012Viele Sicherheits- und Virusexperten haben dem NIPC jedoch bereits Panikmache unterstellt. David Perry von Trend-Micro erkl\344rte gegen\374ber dem Nachrichtensender CNN, dass es durch den Wurm sicher einigen Betrieb im Internet geben w\374rde, aber er rechnete nicht mit weit reichenden Folgen. Das NIPC zeigt sich von all dem unbeeindruckt und verbucht das Ausbleiben der Katastrophe als eigenen Erfolg \226 schlie\337lich h\344tten die (sogar weltweit im Fernsehen ausgestrahlten) Warnungen die Administratoren sensibilisiert und daf\374r gesorgt, dass betroffene Rechner gepatcht wurden. Marc Maiffret, von eEye Digital Security, den Entdeckern der zugrunde liegenden Sicherheitsl\374cke, wirft Microsoft und dem NIPC vor, viel zu sp\344t reagiert zu haben. \\"Die haben wirklich bis zur letzten Minute gewartet. Die h\344tten viel mehr in der Woche zuvor unternehmen k\366nnen\\", sagte er.\015\012\015\012Dabei d\374rfte das Ausbleiben der Code-Red-Lawine einen viel einfacheren Grund haben: Bereits infizierte Rechner beteiligen sich nicht mehr an der Verbreitung. Das ergaben unter anderem Analysen bei Network Associates (NAI). \\"Offenbar wacht Code Red aus seiner Schlafroutine nicht mehr auf, wenn er sich im Vormonat bereits verbreitet hat\\", sagte Dirk Kollberg, Virusexperte bei NAI gegen\374ber heise online. Man wei\337 noch nicht, ob es sich dabei um einen Bug oder ein Feature handelt. Eine vollst\344ndige Entwarnung m\366chte man aber auch bei NAI noch nicht geben. Zum einen wurden laut Ger\374chten schon sieben verschiedene Varianten von Code Red gesehen, die sich m\366glicherweise in ihren Verbreitungsroutinen unterscheiden. Zum anderen spielen andere Faktoren wie Systemdatum und -zeit eine Rolle. \\"Die Analysen werden erst in ein paar Tagen zeigen, inwieweit Code Red sich noch verbreitet. Auch wenn die gro\337e Welle ausgeblieben ist, sollten Administratoren sicherstellen, dass sie den von Microsoft bereitgestellten Patch auf betroffene IIS-Rechner eingespielt haben\\", sagte Kollberg.\015\012\015\012Die momentane Entwicklung zeigt, dass man k\374nftig noch mehr auf Sicherheitsl\374cken achten muss \226 besonders bei Rechnern, die st\344ndig mit dem Internet verbunden sind. Wenn W\374rmer wie Code Red vollautomatisch Sicherheitsl\374cken ausnutzen, um sich weiterzuverbreiten, ist das eine ganz andere Dimension als vereinzelte \\"Hacker\\"-Angriffe. Und Code Red war mit seiner au\337er Gefecht gesetzten Schadroutine noch harmlos: Sp\344testens wenn solche Sch\344dlinge die Dateien auf den befallenen Rechnern kompromittieren, m\374ssen die Administratoren reagieren. (pab/c\\''t) \015\012\015\012','1','0','146','body');
INSERT INTO "awf_nodedata" VALUES (774,'Die vom FBI bef\374rchtete \334berflutung des Internet mit dem Code-Red-Wurm hat nicht stattgefunden \226 bislang sind kaum Aktivit\344ten des Sch\344dlings festzustellen. Dabei h\344tte es laut dem NIPC (National Infrastructure Protection Center) heute nacht um zwei Uhr deutscher Zeit zu einer weiteren gro\337en Verbreitungswelle kommen sollen.\015\012\015\012Viele Sicherheits- und Virusexperten haben dem NIPC jedoch bereits Panikmache unterstellt. David Perry von Trend-Micro erkl\344rte gegen\374ber dem Nachrichtensender CNN, dass es durch den Wurm sicher einigen Betrieb im Internet geben w\374rde, aber er rechnete nicht mit weit reichenden Folgen. Das NIPC zeigt sich von all dem unbeeindruckt und verbucht das Ausbleiben der Katastrophe als eigenen Erfolg \226 schlie\337lich h\344tten die (sogar weltweit im Fernsehen ausgestrahlten) Warnungen die Administratoren sensibilisiert und daf\374r gesorgt, dass betroffene Rechner gepatcht wurden. Marc Maiffret, von eEye Digital Security, den Entdeckern der zugrunde liegenden Sicherheitsl\374cke, wirft Microsoft und dem NIPC vor, viel zu sp\344t reagiert zu haben. \\"Die haben wirklich bis zur letzten Minute gewartet. Die h\344tten viel mehr in der Woche zuvor unternehmen k\366nnen\\", sagte er.\015\012\015\012Dabei d\374rfte das Ausbleiben der Code-Red-Lawine einen viel einfacheren Grund haben: Bereits infizierte Rechner beteiligen sich nicht mehr an der Verbreitung. Das ergaben unter anderem Analysen bei Network Associates (NAI). \\"Offenbar wacht Code Red aus seiner Schlafroutine nicht mehr auf, wenn er sich im Vormonat bereits verbreitet hat\\", sagte Dirk Kollberg, Virusexperte bei NAI gegen\374ber heise online. Man wei\337 noch nicht, ob es sich dabei um einen Bug oder ein Feature handelt. Eine vollst\344ndige Entwarnung m\366chte man aber auch bei NAI noch nicht geben. Zum einen wurden laut Ger\374chten schon sieben verschiedene Varianten von Code Red gesehen, die sich m\366glicherweise in ihren Verbreitungsroutinen unterscheiden. Zum anderen spielen andere Faktoren wie Systemdatum und -zeit eine Rolle. \\"Die Analysen werden erst in ein paar Tagen zeigen, inwieweit Code Red sich noch verbreitet. Auch wenn die gro\337e Welle ausgeblieben ist, sollten Administratoren sicherstellen, dass sie den von Microsoft bereitgestellten Patch auf betroffene IIS-Rechner eingespielt haben\\", sagte Kollberg.\015\012\015\012Die momentane Entwicklung zeigt, dass man k\374nftig noch mehr auf Sicherheitsl\374cken achten muss \226 besonders bei Rechnern, die st\344ndig mit dem Internet verbunden sind. Wenn W\374rmer wie Code Red vollautomatisch Sicherheitsl\374cken ausnutzen, um sich weiterzuverbreiten, ist das eine ganz andere Dimension als vereinzelte \\"Hacker\\"-Angriffe. Und Code Red war mit seiner au\337er Gefecht gesetzten Schadroutine noch harmlos: Sp\344testens wenn solche Sch\344dlinge die Dateien auf den befallenen Rechnern kompromittieren, m\374ssen die Administratoren reagieren. (pab/c\\''t) \015\012\015\012','1','0','146','body');
INSERT INTO "awf_nodedata" VALUES (779,'Die vom FBI bef\374rchtete \334berflutung des Internet mit dem Code-Red-Wurm hat nicht stattgefunden \226 bislang sind kaum Aktivit\344ten des Sch\344dlings festzustellen. Dabei h\344tte es laut dem NIPC (National Infrastructure Protection Center) heute nacht um zwei Uhr deutscher Zeit zu einer weiteren gro\337en Verbreitungswelle kommen sollen.\015\012\015\012Viele Sicherheits- und Virusexperten haben dem NIPC jedoch bereits Panikmache unterstellt. David Perry von Trend-Micro erkl\344rte gegen\374ber dem Nachrichtensender CNN, dass es durch den Wurm sicher einigen Betrieb im Internet geben w\374rde, aber er rechnete nicht mit weit reichenden Folgen. Das NIPC zeigt sich von all dem unbeeindruckt und verbucht das Ausbleiben der Katastrophe als eigenen Erfolg \226 schlie\337lich h\344tten die (sogar weltweit im Fernsehen ausgestrahlten) Warnungen die Administratoren sensibilisiert und daf\374r gesorgt, dass betroffene Rechner gepatcht wurden. Marc Maiffret, von eEye Digital Security, den Entdeckern der zugrunde liegenden Sicherheitsl\374cke, wirft Microsoft und dem NIPC vor, viel zu sp\344t reagiert zu haben. \\"Die haben wirklich bis zur letzten Minute gewartet. Die h\344tten viel mehr in der Woche zuvor unternehmen k\366nnen\\", sagte er.\015\012\015\012Dabei d\374rfte das Ausbleiben der Code-Red-Lawine einen viel einfacheren Grund haben: Bereits infizierte Rechner beteiligen sich nicht mehr an der Verbreitung. Das ergaben unter anderem Analysen bei Network Associates (NAI). \\"Offenbar wacht Code Red aus seiner Schlafroutine nicht mehr auf, wenn er sich im Vormonat bereits verbreitet hat\\", sagte Dirk Kollberg, Virusexperte bei NAI gegen\374ber heise online. Man wei\337 noch nicht, ob es sich dabei um einen Bug oder ein Feature handelt. Eine vollst\344ndige Entwarnung m\366chte man aber auch bei NAI noch nicht geben. Zum einen wurden laut Ger\374chten schon sieben verschiedene Varianten von Code Red gesehen, die sich m\366glicherweise in ihren Verbreitungsroutinen unterscheiden. Zum anderen spielen andere Faktoren wie Systemdatum und -zeit eine Rolle. \\"Die Analysen werden erst in ein paar Tagen zeigen, inwieweit Code Red sich noch verbreitet. Auch wenn die gro\337e Welle ausgeblieben ist, sollten Administratoren sicherstellen, dass sie den von Microsoft bereitgestellten Patch auf betroffene IIS-Rechner eingespielt haben\\", sagte Kollberg.\015\012\015\012Die momentane Entwicklung zeigt, dass man k\374nftig noch mehr auf Sicherheitsl\374cken achten muss \226 besonders bei Rechnern, die st\344ndig mit dem Internet verbunden sind. Wenn W\374rmer wie Code Red vollautomatisch Sicherheitsl\374cken ausnutzen, um sich weiterzuverbreiten, ist das eine ganz andere Dimension als vereinzelte \\"Hacker\\"-Angriffe. Und Code Red war mit seiner au\337er Gefecht gesetzten Schadroutine noch harmlos: Sp\344testens wenn solche Sch\344dlinge die Dateien auf den befallenen Rechnern kompromittieren, m\374ssen die Administratoren reagieren. (pab/c\\''t) \015\012\015\012','1','0','146','body');
INSERT INTO "awf_nodedata" VALUES (770,'997100790','1','0','146','timestamp');
INSERT INTO "awf_nodes" VALUES (78,'1','1',1,1,1,'2',1,'Unknown','0',0);
INSERT INTO "awf_nodes" VALUES (2,'15','0',1,1,1,'5',1,'start','0',1050);
INSERT INTO "awf_nodes" VALUES (151,'2','146',1,1,1,'13',0,'','0',0);
INSERT INTO "awf_nodes" VALUES (30,'5','0',1,1,1,'2',1,'Career','0',5);
INSERT INTO "awf_nodes" VALUES (31,'1','0',1,1,1,'2',1,'Unknown','0',2);
INSERT INTO "awf_nodes" VALUES (81,'30','0',1,1,1,'2',1,'Unknown','0',0);
INSERT INTO "awf_nodes" VALUES (58,'1','2',1,1,1,'10',1,'Unknown','0',1);
INSERT INTO "awf_nodes" VALUES (59,'1','0',1,0,1,'10',0,'Unknown','0',142);
INSERT INTO "awf_nodes" VALUES (143,'1','80',1,1,1,'13',0,'','0',0);
INSERT INTO "awf_nodes" VALUES (80,'30','0',1,1,1,'2',1,'Unknown','0',0);
INSERT INTO "awf_nodes" VALUES (153,'0','0',0,0,0,'0',0,'0','0',0);
INSERT INTO "awf_nodes" VALUES (83,'21','80',1,1,1,'3',0,'Unknown','0',0);
INSERT INTO "awf_nodes" VALUES (152,'2','80',1,1,1,'13',0,'','0',0);
INSERT INTO "awf_nodes" VALUES (154,'4','80',1,1,1,'3',0,'Unknown','0',0);
INSERT INTO "awf_nodes" VALUES (142,'0','0',0,0,0,'0',0,'0','0',0);
INSERT INTO "awf_nodes" VALUES (145,'4','0',1,1,1,'15',1,'Unknown','0',0);
INSERT INTO "awf_nodes" VALUES (146,'2','2',1,1,1,'6',0,'Unknown','0',0);
INSERT INTO "awf_redirect" VALUES (1,'http://freshmeat.net/projects/awf/','','2');
INSERT INTO "awf_typedata" VALUES (1,'phpweb','cmscontent',1,'2');
INSERT INTO "awf_typedata" VALUES (2,'phpweb','viewparagraph',1,'3');
INSERT INTO "awf_typedata" VALUES (3,'phpweb','cmscontent',0,'1');
INSERT INTO "awf_typedata" VALUES (4,'phpweb','htmlsource',1,'4');
INSERT INTO "awf_typedata" VALUES (5,'phpweb','news',1,'5');
INSERT INTO "awf_typedata" VALUES (6,'phpweb','newsitem',1,'6');
INSERT INTO "awf_typedata" VALUES (7,'phpweb','customers_area',1,'7');
INSERT INTO "awf_typedata" VALUES (8,'phpweb','project',1,'8');
INSERT INTO "awf_typedata" VALUES (9,'phpweb','task',1,'9');
INSERT INTO "awf_typedata" VALUES (10,'phpweb','search',1,'10');
INSERT INTO "awf_typedata" VALUES (11,'phpweb','forum',0,'11');
INSERT INTO "awf_typedata" VALUES (12,'phpweb','message',1,'12');
INSERT INTO "awf_typedata" VALUES (13,'phpweb','comment',1,'13');
INSERT INTO "awf_typedata" VALUES (14,'phpweb','browser',1,'14');
INSERT INTO "awf_typedata" VALUES (15,'phpweb','user',1,'15');
INSERT INTO "awf_typedata" VALUES (16,'phpweb','user',0,'16');
INSERT INTO "awf_types" VALUES (1,'0','root','none');
INSERT INTO "awf_types" VALUES (2,'1','topic','none');
INSERT INTO "awf_types" VALUES (3,'2','paragraph','none');
INSERT INTO "awf_types" VALUES (4,'1','html-source','none');
INSERT INTO "awf_types" VALUES (5,'1','news','none');
INSERT INTO "awf_types" VALUES (6,'5','newsitem','none');
INSERT INTO "awf_types" VALUES (7,'1','customers_area','none');
INSERT INTO "awf_types" VALUES (8,'7','project','none');
INSERT INTO "awf_types" VALUES (9,'8','task','none');
INSERT INTO "awf_types" VALUES (11,'1','forum','');
INSERT INTO "awf_types" VALUES (10,'1','search_form','A form for searching');
INSERT INTO "awf_types" VALUES (12,'11','message','');
INSERT INTO "awf_types" VALUES (13,'1','comment','');
INSERT INTO "awf_types" VALUES (14,'1','browser','Tool for browsing and downloading dirs/files');
INSERT INTO "awf_types" VALUES (15,'1','user','Add User/Change User Settings');
INSERT INTO "awf_types" VALUES (16,'1','denied','What to if a user has no right to access');
INSERT INTO "awf_userdata" VALUES (1,'1','editor','1');
INSERT INTO "awf_userdata" VALUES (3,'1','edithost_all','1');
INSERT INTO "awf_userdata" VALUES (5,'2','editor','0');
INSERT INTO "awf_userdata" VALUES (7,'2','Poll','1');
INSERT INTO "awf_userdata" VALUES (8,'2','editor','0');
INSERT INTO "awf_userdata" VALUES (9,'2','module_size_9','1');
INSERT INTO "awf_userdata" VALUES (10,'2','module_size_1','1');
INSERT INTO "awf_userdata" VALUES (11,'2','module_size_4','1');
INSERT INTO "awf_userdata" VALUES (12,'2','module_size_6','1');
INSERT INTO "awf_userdata" VALUES (13,'2','module_size_2','1');
INSERT INTO "awf_userdata" VALUES (14,'2','module_size_7','1');
INSERT INTO "awf_userdata" VALUES (15,'2','module_size_3','1');
INSERT INTO "awf_userdata" VALUES (16,'2','module_size_7','1');
INSERT INTO "awf_userdata" VALUES (17,'2','module_size_7','1');
INSERT INTO "awf_userdata" VALUES (2,'1','editor','1');
INSERT INTO "awf_userdata" VALUES (4,'1','module_size_9','1');
INSERT INTO "awf_userdata" VALUES (6,'1','module_size_9','1');
INSERT INTO "awf_userdata" VALUES (18,'1','module_size_9','1');
INSERT INTO "awf_userdata" VALUES (19,'1','module_size_9','1');
INSERT INTO "awf_userdata" VALUES (22,'1','module_size_8','1');
INSERT INTO "awf_userdata" VALUES (20,'1','module_size_2','1');
INSERT INTO "awf_userdata" VALUES (21,'1','module_size_3','1');
INSERT INTO "awf_users" VALUES (1,'a','a',1,'0','2001-08-06 12:19:00+02');
DROP INDEX "awf_setup_id_key";
CREATE UNIQUE INDEX "awf_setup_id_key" on "awf_setup" using btree ( "id" "int4_ops" );
DROP INDEX "awf_polls_items_id_key";
CREATE UNIQUE INDEX "awf_polls_items_id_key" on "awf_polls_items" using btree ( "id" "int4_ops" );
DROP INDEX "1_redirect_id_key";
CREATE UNIQUE INDEX "1_redirect_id_key" on "awf_redirect" using btree ( "id" "int4_ops" );
DROP INDEX "awf_users_id_key";
CREATE UNIQUE INDEX "awf_users_id_key" on "awf_users" using btree ( "id" "int4_ops" );
CREATE CONSTRAINT TRIGGER "<unnamed>" AFTER DELETE ON "awf_flavours"  NOT DEFERRABLE INITIALLY IMMEDIATE FOR EACH ROW EXECUTE PROCEDURE "RI_FKey_noaction_del" ('<unnamed>', 'awf_nodedata', 'awf_flavours', 'UNSPECIFIED', 'flavour_id', 'id');
CREATE CONSTRAINT TRIGGER "<unnamed>" AFTER UPDATE ON "awf_flavours"  NOT DEFERRABLE INITIALLY IMMEDIATE FOR EACH ROW EXECUTE PROCEDURE "RI_FKey_noaction_upd" ('<unnamed>', 'awf_nodedata', 'awf_flavours', 'UNSPECIFIED', 'flavour_id', 'id');
CREATE CONSTRAINT TRIGGER "<unnamed>" AFTER INSERT OR UPDATE ON "awf_nodedata"  NOT DEFERRABLE INITIALLY IMMEDIATE FOR EACH ROW EXECUTE PROCEDURE "RI_FKey_check_ins" ('<unnamed>', 'awf_nodedata', 'awf_flavours', 'UNSPECIFIED', 'flavour_id', 'id');
CREATE CONSTRAINT TRIGGER "<unnamed>" AFTER DELETE ON "awf_polls"  NOT DEFERRABLE INITIALLY IMMEDIATE FOR EACH ROW EXECUTE PROCEDURE "RI_FKey_noaction_del" ('<unnamed>', 'awf_polls_items', 'awf_polls', 'UNSPECIFIED', 'poll_id', 'id');
CREATE CONSTRAINT TRIGGER "<unnamed>" AFTER UPDATE ON "awf_polls"  NOT DEFERRABLE INITIALLY IMMEDIATE FOR EACH ROW EXECUTE PROCEDURE "RI_FKey_noaction_upd" ('<unnamed>', 'awf_polls_items', 'awf_polls', 'UNSPECIFIED', 'poll_id', 'id');
CREATE CONSTRAINT TRIGGER "<unnamed>" AFTER INSERT OR UPDATE ON "awf_polls_items"  NOT DEFERRABLE INITIALLY IMMEDIATE FOR EACH ROW EXECUTE PROCEDURE "RI_FKey_check_ins" ('<unnamed>', 'awf_polls_items', 'awf_polls', 'UNSPECIFIED', 'poll_id', 'id');
