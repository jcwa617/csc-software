package compiler_exclusion;

use strict;
use Exporter;
use XML::Simple;
use Data::Dumper;
use vars qw($VERSION @ISA @EXPORT @EXPORT_OK %EXPORT_TAGS);
use constant {
        SUCCESS   => 1,
        FAIL      => 0
    };   
$VERSION     = 1.00;
@ISA         = qw(Exporter);
@EXPORT      = ();
@EXPORT_OK   = qw(SUCCESS FAIL);





my $iFiles = {};


sub fatal_error
{
   my $msg = shift;
   
   print <<MOO_SQUID;
Fatal Error
-----------
$msg
MOO_SQUID
   exit 1;
}


sub process
{
   my ($excFile, $iDir, $outFile) = @_;
   # Read in exclude files
   my @ignoreList = get_exclusion_list($excFile);   
   # read in i files
   my @iFiles = get_i_files($iDir); 
   my ($keepList, $removeList) = get_exc_keep_lists(\@iFiles, \@ignoreList );
   # delete unnecessare i files
   clean_up_unused_files(@$removeList);
   # process remaining i files and pull .h out into hash
   my @hFiles = get_unique_h_files(@$keepList);  
   #produce output list
   generate_includes_list($outFile, @hFiles);
}

sub get_unique_h_files
{
   my @files = @_;
   my @hFiles=();
   for my $tempFile (@files)
   {
      push (@hFiles, extract_h_files($tempFile));
   }
   my %uniqH = map { $_ => $_ } @hFiles;
   return keys(%uniqH);
}

sub extract_h_files
{
   my $file = shift;
   return unless defined $file;
   return unless -e $file;
   my @result = ();
   open (my $fh, '<', $file);
   my @lines = <$fh>;
   close $fh;
   for my $temp_line (@lines)
   {
      if ($temp_line =~ /(\w+\.h)/)
      {
         push (@result, $1);
      }
   }
   return @result;
}

sub get_exc_keep_lists
{
   my($iFilesListRef, $delFilesRef)=@_;
   my $excList = join ('|',@$delFilesRef);
   my @keepList = ();
   my @delList =();
   for my $temp (@$iFilesListRef)
   {
      my $file;
      if ($temp =~/(\w\.i)/)
      {
         $file = $1;
      }
      else{
         next;
      }
      if ($excList =~ /$file/i)
      {
         push (@delList,$temp);
         next;
      }
      push (@keepList, $temp);
   }
   return (\@keepList, \@delList);
}

sub get_exclusion_list
{
   my $inputFile = shift;
   return unless (defined $inputFile);
   return unless ($inputFile=~/\w\.xml/);
   # create object
   my $xh = new XML::Simple;
   my $data = $xh->XMLin($inputFile);
   my @ignoreFiles = ();
   foreach my $key ( keys %$data )
   {
      my $tempApp = $data->{$key};
      next unless ($tempApp->{disable}=~/yes/i);
      push (@ignoreFiles, split(/\s/,$tempApp->{files}));
   }
   # CHange all the .c files to .i
   map { $_ =~s/(\w\.)c/$1i/ } @ignoreFiles;
   return @ignoreFiles
}

sub get_i_files
{
   my $dir = shift;
   
   return unless (defined $dir);
   return unless (-d $dir);
   opendir(my $dh, $dir) or die $!;
   my @result = ();
   while (my $file = readdir($dh)) {
      # Ignore all non .i files
      next unless ($file =~ m/\w+\.i/);
      push ( @result, $dir.'/'.$file);
   }
   closedir($dh);  
   return @result;
}

sub generate_includes_list
{
   my ($outputFile, @pathlessIncFiles) = @_;
   
   return FAIL unless (defined $outputFile );
   return FAIL unless ($outputFile =~ /\w+/);
   @pathlessIncFiles = () unless (defined  @pathlessIncFiles);
   my @includes = map {'#include '.$_} @pathlessIncFiles;
   my $list  = join ("\n",@includes);
   my $output =<<MOO_SQUID;
/*
   --------------
   DO NOT MODIFY!
   --------------
   This file is automatically generated and contains all the header
   files required to build the system. This file is used as a step 
   in the conditional build system.
*/
$list

MOO_SQUID
   open (my $fh, '>', $outputFile);
   print $fh $output;
   close $fh;
   return SUCCESS;
}

sub clean_up_unused_files
{
   my @unwantedFiles = @_;
   my $summary_deleted = 0;
   return unless (defined @unwantedFiles);
   foreach my $tempFile (@unwantedFiles)
   {
      # Do not delete directories
      next if (-d $tempFile);
      # Delete file if it exists
      unlink $tempFile if (-e $tempFile);
      ++$summary_deleted;
   }
   return $summary_deleted;
}


1;