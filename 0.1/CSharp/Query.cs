/*
	C# MultIV server query class by King_Hual
	
	
	Arguments:
	
	    String address - The IP address
	    Int32 port [optional] - The port
	
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

*/

using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

class MultIVQuery
{
    private String
        address,
        hostname,
        identifier,
        mode;
    private UInt16
        players,
        max_players;
    private Int32
        port;
    private UInt32
        expansion,
        version;
    private Boolean
        password,
        secure,
        error;

    public MultIVQuery(String address, Int32 port = 5000)
    {
        this.address = address;
        this.port = port+123;
        this.update();
    }

    public bool update()
    {
        UdpClient con = null;
        Boolean error = false;
        try
        {
            con = new UdpClient(this.address, this.port);
            IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse(this.address), this.port);
            Byte[] header = new Byte[]
            {
				0x4d,0x49,
				0x56,0x01,
				0x00,0x00,
				0x00,0x00
            };
            if (con.Send(header, header.Length) != header.Length)
                throw new Exception();
            byte[] rdata = con.Receive(ref endpoint);
            this.identifier = System.Text.Encoding.UTF8.GetString(rdata, 0, 3);
            if (this.identifier.CompareTo("MIV") != 0)
                throw new Exception();
            this.version = BitConverter.ToUInt32(rdata, 3);
            Int32 str_len = BitConverter.ToInt32(rdata, 8);
            this.hostname = System.Text.Encoding.UTF8.GetString(rdata, 12, str_len);
            Int32 last_byte = 12 + str_len;
            str_len = BitConverter.ToInt32(rdata, last_byte);
            this.mode = System.Text.Encoding.UTF8.GetString(rdata, last_byte+4, str_len);
            last_byte += 4 + str_len;
            this.password = rdata[last_byte] == 1;
            this.secure = rdata[last_byte+1] == 1;
            this.players = BitConverter.ToUInt16(rdata, last_byte + 2);
            this.max_players = BitConverter.ToUInt16(rdata, last_byte + 4);
            this.expansion = BitConverter.ToUInt32(rdata, last_byte + 6);
        }
        catch (Exception)
        {
            error = true;
        }
        if (con != null)
            con.Close();
        return (this.error = error);
    }

    public Boolean isErroneous()
    {
        return this.error;
    }

    public String getHostName()
    {
        return this.hostname;
    }

    public UInt16 getPlayers()
    {
        return this.players;
    }

    public UInt16 getMaxPlayers()
    {
        return this.max_players;
    }

    public UInt32 getNetVersion()
    {
        return this.version;
    }

    public String getGameMode()
    {
        return this.mode;
    }

    public Boolean isPassworded()
    {
        return this.password;
    }

    public UInt32 getExpansion()
    {
        return this.expansion;
    }

    public String getExpansionName()
    {
        return this.expansion == 1 ? "The Ballad of Gay Tony" : "The Lost and Damned";
    }

    public String getIdentifier()
    {
        return this.identifier;
    }

    public Boolean isSecure()
    {
        return this.secure;
    }
}
