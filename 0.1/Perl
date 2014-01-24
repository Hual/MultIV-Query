=begin documentation

        	Perl MultIV server query class by King_Hual
        

        Arguments:

	        string address - The IP address
	        int port - The port

        Methods:

		    update() - Updates the information
		    isErroneous() - Returns true if any errors occured, false otherwise
		    getHostName() - Returns the hostname of a server
		    getPlayers() - Returns the amount of players currently connected to the server
		    getMaxPlayers() - Returns the maximum amount of players that can be connected to the server
		    getNetVersion() - Returns the net version of the server
		    getGameMode() - Returns the name of the server's current gamemode
		    isPassworded() - Returns true if the server is passworded, false if it isn't
		    getExpansion() - Returns an integer representing the expansion the server is available on
		    getExpansionName() - Returns the name of the expansion the server is available on
		    getIdentifier() - Returns the packet identifier as a string (should be "MIV")
		    isSecure() - Returns true if the server is AC-protected, false if it isn't

=cut

package MultIVQuery;

use strict;
use IO::Socket;

sub new
{
	my $class = shift;
	my $self =
	{
		address => shift,
		port => shift,
		hostname => "",
		players => 0,
		max_players => 0,
		error => 1,
		identifier => "",
		version => 0,
		mode => "",
		password => 0,
		secure => 0,
		expansion => 0
	};
	$self->{port} += 123;
	bless $self, $class;
	$self->update();
	return $self;
}

sub update
{
	my($self) = @_;
	my $sock = IO::Socket::INET->new(Proto => 'udp', PeerPort => $self->{port}, PeerAddr => $self->{address}, Timeout => 2) or return($self->{error} = 1);
	my $sdata = pack("C*", 
			0x4d,0x49,
			0x56,0x01,
			0x00,0x00,
			0x00,0x00
	);
	my $rawdata;
	$sock->send($sdata) or return($self->{error} = 1);
	$sock->recv($rawdata, 512) or return($self->{error} = 1);
	$self->{identifier} = substr($rawdata, 0, 3);
	if($self->{identifier} != "MIV")
	{
		return($self->{error} = 0);
	}
	$self->{version} = unpack("V", substr($rawdata, 3, 4));
	my $str_len = unpack("V", substr($rawdata, 8, 4));
	$self->{hostname} = substr($rawdata, 12, $str_len);
	my $last_byte = 12+$str_len;
	$str_len = unpack("V", substr($rawdata, $last_byte, 4));
	$self->{mode} = substr($rawdata, $last_byte+4, $str_len);
	$last_byte += 4+$str_len;
	$self->{password} = unpack("c", substr($rawdata, $last_byte, 1));
	$self->{secure} = unpack("c", substr($rawdata, $last_byte+1, 1));
	$self->{players} = unpack("v", substr($rawdata, $last_byte+2, 2));
	$self->{max_players} = unpack("v", substr($rawdata, $last_byte+4, 2));
	$self->{expansion} = unpack("V", substr($rawdata, $last_byte+6, 4));
	return($self->{error} = 0);
}

sub isErroneous()
{
	my($self) = @_;
	return $self->{error};
}

sub getHostName()
{
	my($self) = @_;
	return $self->{hostname};
}

sub getPlayers()
{
	my($self) = @_;
	return $self->{players};
}

sub getMaxPlayers()
{
	my($self) = @_;
	return $self->{max_players};
}

sub getNetVersion()
{
	my($self) = @_;
	return $self->{version};
}

sub getGameMode()
{
	my($self) = @_;
	return $self->{mode};
}

sub isPassworded()
{
	my($self) = @_;
	return $self->{password};
}

sub getExpansion()
{
	my($self) = @_;
	return $self->{expansion};
}

sub getExpansionName()
{
	my($self) = @_;
	return $self->{expansion} == 1 ? "The Ballad of Gay Tony" : "The Lost and Damned";
}

sub getIdentifier()
{
	my($self) = @_;
	return $self->{identifier};
}

sub isSecure()
{
	my($self) = @_;
	return $self->{secure};
}
