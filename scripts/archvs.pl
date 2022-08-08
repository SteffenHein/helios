#!/usr/bin/perl
#------------------------------------------------------------------------------#
#                                                                              #
#  new-arch.pl                                                                 #
#  elfe archive creation perl script                                           #
#                                                                              #
#  (C) SHEIN; Munich, December 2021                          Steffen Hein      #
#  [ Update: December 18, 2021 ]                          <contact@sfenx.de>   #
#                                                                              #
#------------------------------------------------------------------------------#
use warnings 'all';
BEGIN { push @INC, './'}
use Env;
use strict;
$ENV{OSTYPE}="linux";

#------------------------------------------------------------------------------#
my ( @dirtree,  @path,    @files,    @args );
my ( @srclist, @ziplist,  @tgzlist, @modlist, @text );
my ( $portname, $release, $distname, $directory, $infile, $outfile, $cksmd5 );
my ( $srcarch, $tararch, $tgzarch, $modarch, $gzarch, $bz2arch, $i, $j, $cks );

#------------------------------------------------------------------------------#
$portname = "helios";
$release  = "v1.0r1";
$distname = $portname . "-" . $release;
$srcarch  = $distname . ".src.tar";
$tararch  = $distname . ".tar";
$tgzarch  = $distname . ".tgz";
$gzarch   = $srcarch . ".gz";
$bz2arch  = $srcarch . ".bz2";
$outfile  = "CHECKSUM.MD5";

# $cks=[0] 1: [ don't ] compute checksums
$cks=1;

#------------------------------------------------------------------------------#
# the directory tree of $distname 
#
$i = 0;
$dirtree[ $i++ ] = $distname;
$dirtree[ $i++ ] = $distname . "/bin";
$dirtree[ $i++ ] = $distname . "/default";
$dirtree[ $i++ ] = $distname . "/doc";
$dirtree[ $i++ ] = $distname . "/math";
$dirtree[ $i++ ] = $distname . "/objects";
$dirtree[ $i++ ] = $distname . "/ports";
$dirtree[ $i++ ] = $distname . "/prv";
$dirtree[ $i++ ] = $distname . "/samples";
$dirtree[ $i++ ] = $distname . "/scripts";
$dirtree[ $i++ ] = $distname . "/src";
$dirtree[ $i++ ] = $distname . "/tools";
$dirtree[ $i++ ] = $distname . "/work";
#------------------------------------------------------------------------------#
# the list of files stored in $tgzarch
#
$i = 0;
$tgzlist[ $i++ ] = $distname . "/INSTALL";
$tgzlist[ $i++ ] = $distname . "/README";
$tgzlist[ $i++ ] = $distname . "/CONFIG.H";
$tgzlist[ $i++ ] = $distname . "/bin";
$tgzlist[ $i++ ] = $distname . "/default";
$tgzlist[ $i++ ] = $distname . "/doc";
$tgzlist[ $i++ ] = $distname . "/math";
$tgzlist[ $i++ ] = $distname . "/objects/.directory";
$tgzlist[ $i++ ] = $distname . "/ports";
$tgzlist[ $i++ ] = $distname . "/prv";
$tgzlist[ $i++ ] = $distname . "/samples";
$tgzlist[ $i++ ] = $distname . "/scripts";
$tgzlist[ $i++ ] = $distname . "/src";
$tgzlist[ $i++ ] = $distname . "/tools";
$tgzlist[ $i++ ] = $distname . "/work";
$tgzlist[ $i++ ] = $distname . "/Makefile";
$tgzlist[ $i++ ] = $distname . "/makefile.gnu";
$tgzlist[ $i++ ] = $distname . "/makefile.win";
$tgzlist[ $i++ ] = $distname . "/mk.gnu";
$tgzlist[ $i++ ] = $distname . "/mk.win";
$tgzlist[ $i++ ] = $distname . "/SETUP";
#------------------------------------------------------------------------------#
# The Z-compressed archive of the following [ essential ] sources; $tararch:
#
$i = 0;
$ziplist[ $i++ ] = $distname . "/INSTALL";
$ziplist[ $i++ ] = $distname . "/README";
$ziplist[ $i++ ] = $distname . "/CONFIG.H";
$ziplist[ $i++ ] = $distname . "/bin";
$ziplist[ $i++ ] = $distname . "/default";
$ziplist[ $i++ ] = $distname . "/doc";
$ziplist[ $i++ ] = $distname . "/math";
$ziplist[ $i++ ] = $distname . "/objects/.directory";
$ziplist[ $i++ ] = $distname . "/ports";
$ziplist[ $i++ ] = $distname . "/prv";
$ziplist[ $i++ ] = $distname . "/samples";
$ziplist[ $i++ ] = $distname . "/scripts";
$ziplist[ $i++ ] = $distname . "/src";
$ziplist[ $i++ ] = $distname . "/tools";
$ziplist[ $i++ ] = $distname . "/work";
$ziplist[ $i++ ] = $distname . "/Makefile";
$ziplist[ $i++ ] = $distname . "/makefile.gnu";
$ziplist[ $i++ ] = $distname . "/makefile.win";
$ziplist[ $i++ ] = $distname . "/mk.gnu";
$ziplist[ $i++ ] = $distname . "/mk.win";
$ziplist[ $i++ ] = $distname . "/SETUP";

#------------------------------------------------------------------------------#
# Create a new tar archive for FreeBSD port building system:
#
$i = 0;
$srclist[ $i++ ] = $distname;
$srclist[ $i++ ] = "LICENSE";
$srclist[ $i++ ] = "README";
$srclist[ $i++ ] = "INSTALL";

#------------------------------------------------------------------------------#
# Writing special directory checksums:
#
print "\n\nwriting directory checksums";

if ( $cks ) {
    $outfile = ".sha256";
    @args = ("./sha256sum.sh $outfile");
    system(@args);

    $outfile = ".md5";
    @args = ("./md5sum.sh $outfile");
    system(@args);
} # end if

# switch back to package directory, then write directory [ tree ] checksums:
#
chdir "../../";
#------------------------------------------------------------------------------#
# Creating achive for selected files: 
#
# switch to script directory then create archive:
#
print "\ncreating archives:";
chdir $distname . "/scripts";
print "\ntar -cvf spcf-" . $release . ".tgz\n";
@args = ("./new-spcf.sh");
system(@args);

#------------------------------------------------------------------------------#
# Create a new tgz archive [ program package ], $tgzarch:
#
# switch back to package directory, then create archives:
#
chdir "../../";

#print "\ncreating tgz-archive:";
print "\ntar -czf " . $tgzarch . "\n";
@args = ("tar -czf $tgzarch @tgzlist");
system(@args);

#print "\narchive ".$tgzarch." ready !";
#------------------------------------------------------------------------------#
# Create a new Z-compressed archive of essential sources [ $tararch.".Z" ]:
#
# switch into package directory, then create archive:
#print "\ncreating archive ".$tararch.".Z:";
print "\ntar -cf " . $tararch . "\n";
@args = ("tar -cf $tararch @ziplist");
system(@args);
print "\ncompressing to ~.7z";
@args = ("p7zip $tararch");
system(@args);

#print "\narchive ".$tararch.".7z ready !";
#------------------------------------------------------------------------------#
# Create a new tar archive for FreeBSD port building system:
#
#print "\ncreating tar archives ".$srcarch;
#print "\ncreating src.tar archives:";
print "\ntar -cf " . $srcarch . "\n";
@args = ("tar -cf $srcarch @srclist");
system(@args);
print "\ngzipping into ~.gz file";
@args = ("gzip -cf $srcarch > $gzarch");
system(@args);

#print "\narchive ".$gzarch." ready !";
print "\nbzipping into ~.bz2 file";
@args = ("bzip2 -f $srcarch");
system(@args);

#print "\narchive ".$srcarch.".bz2 ready !";
#------------------------------------------------------------------------------#
# Writing new final package checksum:
#
$directory = "../../";
print "\n\nwriting final package checksum";

if ( $cks )
{
    $outfile = "SHA256SUMS";
    @args = ("rm -f $outfile");
    system(@args);
    @args = ("openssl sha256 * > $outfile");
    system(@args);

    $outfile = "MD5SUMS";
    @args = ("rm -f $outfile");
    system(@args);
    @args = ("md5sum * > $outfile");
    system(@args);
}
#------------------------------------------------------------------------------#
print "\nterminated !\n";
