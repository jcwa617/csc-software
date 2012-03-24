#!/usr/bin/perl

use strict;
use warnings;
use File::Path;
use Test::More 'no_plan';
use compiler_exclusion 'SUCCESS';
use compiler_exclusion 'FAIL';
my @subs = qw (process);
use_ok( 'compiler_exclusion',@subs) or exit;

# Deletion Tests
# --------------
my $testDir = 'test';
unless (-d $testDir)
{
   mkdir $testDir or die $!;
}
my $actual  = compiler_exclusion::clean_up_unused_files ();
ok(!defined $actual, 'clean_up_unused_files: No files specified');

my $expected = 0;
$actual  = compiler_exclusion::clean_up_unused_files ($testDir);
ok($actual == $expected , 'clean_up_unused_files: Directories are not deleted');

my $testFile = "$testDir/moo.txt";
open (my $fh, '>', "$testFile");
print $fh "Test File\n";
close $fh;

ok(-e "$testFile", 'clean_up_unused_files: Create Test File for deletion');

$expected = 1;
$actual  = compiler_exclusion::clean_up_unused_files ($testFile);
ok($actual == $expected , 'clean_up_unused_files: Files are deleted');

# Includes File Creation
# ----------------------
$expected = FAIL;
$actual = compiler_exclusion::generate_includes_list();
ok ($actual == $expected,'generate_includes_list: No Input');
$actual = compiler_exclusion::generate_includes_list("");
ok ($actual == $expected,'generate_includes_list: Empty file name');

$expected = SUCCESS;
$actual =  compiler_exclusion::generate_includes_list($testFile);
ok ($actual == $expected,'generate_includes_list: Generate a file with no list items');
$expected =<<MOO_SQUID;
/*
   --------------
   DO NOT MODIFY!
   --------------
   This file is automatically generated and contains all the header
   files required to build the system. This file is used as a step 
   in the conditional build system.
*/


MOO_SQUID

$actual  = read_in_file($testFile); 
ok ($actual eq $expected,'generate_includes_list: Check Generated file') or print ("Expected:\n$expected|\nActual:\n$actual|");



$expected = SUCCESS;
my @files = qw(aaa.h bbb.h ccc.h);
$actual =  compiler_exclusion::generate_includes_list($testFile, @files);
ok ($actual == $expected,'generate_includes_list: Generate a file with list items');
$expected =<<MOO_SQUID;
/*
   --------------
   DO NOT MODIFY!
   --------------
   This file is automatically generated and contains all the header
   files required to build the system. This file is used as a step 
   in the conditional build system.
*/
#include "aaa.h"
#include "bbb.h"
#include "ccc.h"

MOO_SQUID

$actual  = read_in_file($testFile); 
ok ($actual eq $expected,'generate_includes_list: Check Generated file with list') or print ("Expected:\n$expected|\nActual:\n$actual|");

# Retrieve i files
# ----------------

my @expected = qw(test/aaa.i test/bbb.i test/ccc.i);
# Create files 
for my $file (@expected)
{
   create_file ($file);
}

my @actual = compiler_exclusion::get_i_files($testDir);
is_deeply(\@actual,\@expected,'get_i_files: Verify that only i files are picked up');


# Retrive Rejection List
# ----------------------

my $input =<<MOO_SQUID;
<?xml version="1.0"?>
<!--
   ----------------------------------
   BLUESAT Application Isolation List
   ----------------------------------
   
-->
<applist>
   <DemoApp>
      <name>Demo Application</name>
      <desc>Demo application to test out the uart</desc>
      <disable>NO</disable>
      <files>Demo_Application.c</files>
   </DemoApp>
   <TestApp>
      <name>Test Application</name>
      <desc>Test application to test out the uart</desc>
      <disable>YES</disable>
      <files>Test_Application.c Test_Support.c</files>
   </TestApp>
</applist>
MOO_SQUID
my $testXML = "$testDir/moo.xml";
create_file($testXML, $input);
@expected = qw(Test_Application.i Test_Support.i);
@actual = compiler_exclusion::get_exclusion_list($testXML);
is_deeply(\@actual,\@expected,'get_exclusion_list: Check that only excluded files are added to the return list');


my @L1 = qw(a/b/d/aaa.i s/d/f/bbb.i q/w/e/ccc.i);
my @L2 = qw(aaa.i bbb.i);
my @expKeepRef = qw (q/w/e/ccc.i);
my @expDelRef  = qw (a/b/d/aaa.i s/d/f/bbb.i);
my ($actKeepRef, $actDelRef) = compiler_exclusion::get_exc_keep_lists(\@L1, \@L2);
is_deeply($actKeepRef,\@expKeepRef,'remove_excluded_files: Check keep list');
is_deeply($actDelRef,\@expDelRef,'remove_excluded_files: Check deletion list');


#Helper Functions
#----------------
sub read_in_file
{
   my $file = shift;
   open (my $fh,'<', $file);
   my @readFile = <$fh>;
   close $fh;
   return join ('', @readFile);
}
sub create_file
{
   my $file = shift;
   my $text = shift;
   open (my $fh,'>', $file);
   print $fh "$file Test File" if (!defined $text);
   print $fh $text if (defined $text);
   close $fh;
}






END
{
  # rmtree($testDir);
}