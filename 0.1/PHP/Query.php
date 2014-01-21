<?php
/*
	PHP MultIV server query class by King_Hual
	

	Arguments:

	$address - The IP address
	$port [optional] - The port
	

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
	getData() - Returns an array containing all of the data
	getIdentifier() - Returns the packet identifier as a string (should be "MIV")

*/
class MultIVQuery
{	
	private $address, $port, $data;

	function __construct($address, $port = 5123)
	{
		$this->address = $address;
		$this->port = $port;
		$this->update();
	}

	public function update()
	{
		$handle = pfsockopen("udp://".$this->address, $this->port, $errno, $errstr);
		stream_set_timeout($handle, 2);
		if (!$handle) return ($this->data['error'] = true);
		$data = pack("C*",
			0x4d,0x49,
			0x56,0x01,
			0x00,0x00,
			0x00,0x00
		);
		if(!fwrite($handle, $data)) return ($this->data['error'] = true);
		$this->data['identifier'] = fread($handle, 3);
		$this->data['version'] = unpack("L", fread($handle, 5))[1];
		$this->data['name'] = fread($handle, unpack("L", fread($handle, 4))[1]); // length will hopefully be 1 byte in the future
		$this->data['mode'] = fread($handle, unpack("L", fread($handle, 4))[1]); // length will hopefully be 1 byte in the future
		$this->data['password'] = (int)fread($handle, 1);
		$this->data['?'] = (int)fread($handle, 1);
		$this->data['players'] = unpack("S", fread($handle, 2))[1];
		$this->data['max-players'] = unpack("S", fread($handle, 2))[1];
		$this->data['expansion'] = unpack("L", fread($handle, 4))[1]; // hopefully 1 byte in the future
		if(!fclose($handle)) return ($this->data['error'] = true);
		return ($this->data['error'] = false);
	}

	public function isErroneous()
	{
		return $this->data['error'];
	}

	public function getHostName()
	{
		return $this->data['name'];
	}

	public function getPlayers()
	{
		return $this->data['players'];
	}

	public function getMaxPlayers()
	{
		return $this->data['max-players'];
	}

	public function getNetVersion()
	{
		return $this->data['version'];
	}

	public function getGameMode()
	{
		return $this->data['mode'];
	}

	public function isPassworded()
	{
		return $this->data['password'] == 1;
	}

	public function getExpansion()
	{
		return $this->data['expansion'];
	}

	public function getExpansionName()
	{
		return $this->getExpansion() == 1 ? "The Ballad of Gay Tony" : "The Lost and Damned";
	}

	public function getData()
	{
		return $this->data;
	}

	public function getIdentifier()
	{
		return $this->data['identifier'];
	}
}
