/*
	Java MultIV server query function by King_Hual
	
	
	Arguments:
	
	String address - The IP address
	int port [optional] - The port
	
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
    getUnknownByte() - Returns the unknown byte

*/

import java.lang.reflect.Array;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Arrays;

public class MultIVQuery
{
	private String
		address,
		hostname,
		identifier,
		mode;
	private int
		version,
		port,
		expansion;
	private short
		players,
		maxPlayers;
	private boolean
		error,
		password;
	private byte
		unknown;
	
	public MultIVQuery(String address, int port)
	{
		this.initialize(address, port);
	}
	
	public MultIVQuery(String address)
	{
		this.initialize(address, 5123);
	}
	
	public void initialize(String address, int port)
	{
		this.address = address;
		this.port = port;
		this.update();
	}
	
	public boolean update()
	{
		DatagramSocket socket = null;
		boolean error = false;
		try
		{
			socket = new DatagramSocket();
			final byte[]
				header = new byte[]
				{
					(byte)0x4d, (byte)0x49,
					(byte)0x56, (byte)0x01,
					(byte)0x00, (byte)0x00,
					(byte)0x00, (byte)0x00
				},
				buf = new byte[512];
			
			DatagramPacket packet = new DatagramPacket(header, header.length,  InetAddress.getByName(this.address), this.port);
			socket.send(packet);
			packet = new DatagramPacket(buf, buf.length);
			socket.receive(packet);
			this.identifier = new String(Arrays.copyOfRange(buf, 0, 3));
			this.version = bytesToInt(buf, 3);
			int lastByte; // IT crowd reference
			this.hostname = new String(Arrays.copyOfRange(buf, 12, (lastByte = 12+bytesToInt(buf, 8))));	// length will hopefully be 1 byte in the future
			this.mode = new String(Arrays.copyOfRange(buf, lastByte+4, (lastByte += 4+Array.getInt(buf, lastByte))));	// length will hopefully be 1 byte in the future
			this.password = !(buf[lastByte] == 0);
			this.unknown = buf[lastByte+1];
			this.players = bytesToShort(buf, lastByte+2);
			this.maxPlayers = bytesToShort(buf, lastByte+4);
			this.expansion = bytesToInt(buf, lastByte+6);	// hopefully 1 byte in the future
		}
		catch(Exception e)
		{
			error = true;
		}
		if(socket != null)
			socket.close();
		return (this.error = error);
	}
	
	private static int bytesToInt(byte[] array, int index)
	{
        int integer = 0;
        integer = (integer<<8)+array[index+3];
        integer = (integer<<8)+array[index+2];
        integer = (integer<<8)+array[index+1];
        integer = (integer<<8)+array[index];
        return integer;
	}
	
	private static short bytesToShort(byte[] array, int index)
	{
        int shrt = 0;
        shrt = (shrt<<8)+array[index+1];
        shrt = (shrt<<8)+array[index];
        return (short)shrt;
	}
	
	public boolean isErroneous()
	{
		return this.error;
	}
	
	public String getHostName()
	{
		return this.hostname;
	}
	
	public int getPlayers()
	{
		return this.players;
	}
	
	public int getMaxPlayers()
	{
		return this.maxPlayers;
	}
	
	public int getNetVersion()
	{
		return this.version;
	}
	
	public String getGameMode()
	{
		return this.mode;
	}
	
	public boolean isPassworded()
	{
		return this.password;
	}
	
	public int getExpansion()
	{
		return this.expansion;
	}
	
	public String getExpansionName()
	{
		return this.getExpansion() == 1 ? "The Ballad of Gay Tony" : "The Lost and Damned";
	}
	
	public String getIdentifier()
	{
		return this.identifier;
	}
	
	public byte getUnknownByte()
	{
		return unknown;
	}
}
