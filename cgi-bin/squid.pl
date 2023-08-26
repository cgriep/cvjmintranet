#!/usr/bin/perl
#
# Internetcafe-Skript
# Voraussetzungen:
# Die abzuschaltenden Rechner sind als acl in /etc/squid/squid.conf
# eingetragen, in der Form:
# acl PlatzNN src IP
# Es gibt ein Skript /srv/www/cgi-bin/denyPlatzNN.sh, in dem 
# der Rechner abgeschaltet wird:
# #!/bin/sh
# /srv/www/cgi-bin/squid.pl Rechner=192.168.110.7
# NN ist die Platznummer, die der IP zugeordnet ist.
# Durch Aenderung der Bezeichnungen klappen die Ersetungen nicht mehr, also 
# Vorsicht.
# Aufbau der Codedate /srv/www/intenetcodes.txt:
# Codenr=Minutenanzahl
# Ist ein Code benutzt, so wird das Wort BENUTZT gefolgt von der Uhrzeit
# der Freischaltung eingefü
use strict;
use CGI qw(:standard);
use CGI::Carp qw/fatalsToBrowser/;
print header(-pragma=>'no-cache',
             -charset=>'iso-8859-1');
my $codefile = "/srv/www/internetcodes.txt";
my $Rechner = param("Rechner"); 
if ( $Rechner eq "" )
{ 
  $Rechner = $ENV{'REMOTE_ADDR'}; "192.168.110.7"; # "Platz7";
}
$Rechner =~ /^(.*)$/;
$Rechner = $1;

my $Was;
my $Neu;
my $Minuten = 0;
my $timestamp = localtime();

my $Aktion = param("Aktion");
if ( $Aktion eq "allow" )
{
  # Freischaltungsanforderung
  # Code
  my $Code = param("Code");
  # Get the old Codes
  open(OLD,"<$codefile");
  my $inhalt;
  $Minuten = 0;
  while(<OLD>) {
    $inhalt .= $_;
  }
  close(OLD);
  if ( $inhalt =~ /^$Code=/m )
  {
    # $' - Zeichenkette nach Fundort
    my $Rest;
    my $AllesRest = $';
    ($Minuten,$Rest) =  explode(/\n/,$');
    print "$Minuten Minuten wurden gekauft";
    $Minuten =~ /^(.*)$/;
    $Minuten = $1;

    # Code aus Datei entfernen
    $inhalt =~ s/^$Code=$AllesRest/BENUTZT $timestamp:$Code-$AllesRest/m;
    unlink("$codefile");
    open(NEW,">$codefile");
    print NEW "$inhalt";
    close(NEW);
    $Was = "deny";
    $Neu = "allow";
  }
  else
  {
    print "Falscher Code!";
    $Was = "allow";
    $Neu = "deny";
  }

}
else
{
  $Was = "allow";
  $Neu = "deny";
}

my $datafile = "/etc/squid/squid.conf";
my $newfile = $datafile;

# Get the old stats
open(OLD,"<$datafile");
my $inhalt;
while(<OLD>) {
# chomp($line)i
  $inhalt .= $_;        
}
close(OLD);
if ( $inhalt =~ /acl Platz([0-9]*) src $Rechner/i )
{
  my $PlatzNr = $1;
  if ( $PlatzNr eq "" )
  { 
    $PlatzNr = 7;
  } 
  $inhalt =~ s/$Was Platz$PlatzNr/$Neu Platz$PlatzNr/;
  open(NEW,">$newfile");
  print NEW "$inhalt";
  close(NEW);
  print "$Rechner (Platz$PlatzNr) umgestellt auf $Neu\n";
  open(LOG, ">>/var/log/internetcafe.log");
  print LOG "$timestamp $Rechner (Platz$PlatzNr) auf $Neu umgestellt\n";
  close(LOG);
# Squid rekonfigurieren um Aenderungen wirksam zu machen
  $ENV{'PATH'} = '/bin:/usr/sbin:/usr/bin:/srv/www/cgi-bin:';
  delete @ENV{'IFS', 'CDPATH', 'ENV', 'BASH_ENV'};
  system "/usr/sbin/squid -k reconfigure";
  if ( $Neu eq "allow" )
  {
    print $timestamp;
    print " - Abschalten initiieren in $Minuten Minuten: ";
    my $s = "/usr/bin/at -f /srv/www/cgi-bin/denyPlatz$PlatzNr.sh +$Minuten minutes";
    system "$s";
  }
}
else
{
  print "IP $Rechner nicht gefunden!\n";
}
#open (MOVIRT, "| /usr/sbin/squid -k reconfigure");
#close(MOVIRT);


