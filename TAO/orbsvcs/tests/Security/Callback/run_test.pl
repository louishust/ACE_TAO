eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib  '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$file = PerlACE::LocalFile ("server.ior");

unlink $file;

$status = 0;

# Set the SSL environment
# This doesn't work on Windows.  For some reason,
# environment variables aren't propagated to child processes.
#$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$SV = new PerlACE::Process ("server",
			    "-o $file -ORBSvcConf server$PerlACE::svcconf_ext");
$CL = new PerlACE::Process ("client",
			    "-ORBSvcConf client$PerlACE::svcconf_ext");

print STDERR "\n\n==== Running SSLIOP Callback test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($file, 15) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $file;

exit $status;
