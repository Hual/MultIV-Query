=begin 

	Ruby MultIV server query class by King_Hual
	
	
	Arguments:
	
		address - The IP address
		port [optional] - The port
	
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

=end

require 'socket'

class MultIVQuery
	def initialize(address, port = 5000)
		@address = address
		@port = port+123
		update()
	end

	def update()
		sock = UDPSocket.new
		@error = false
		begin 
			sdata = [
				0x4d,0x49,
				0x56,0x01,
				0x00,0x00,
				0x00,0x00
			].pack("C*")
			sock.connect(@address, @port)
			sock.send(sdata, 0)
			rdata = sock.recvfrom(512)[0]
			@identifier = rdata[0, 3]
			if @identifier != "MIV" then
				raise "Packet doesn't match required identifier"
			end
			@version = rdata[3, 4].unpack("V").first
			str_length = rdata[8, 4].unpack("V").first
			@hostname = rdata[12, str_length]
			last_byte = 12+str_length
			str_length = rdata[last_byte, 4].unpack("V").first
			@mode = rdata[last_byte+4, str_length]
			last_byte += 4+str_length
			@password = rdata[last_byte] == 1
			@secure = rdata[last_byte+1] == 1
			@players = rdata[last_byte+2, 2].unpack("v").first
			@max_players = rdata[last_byte+4, 2].unpack("v").first
			@expansion = rdata[last_byte+6, 4].unpack("V").first
		rescue Exception
			@error = true
		end
		sock.close()
		return @error
	end

	def isErroneous()
		return @error
	end

	def getHostName()
		return @hostname
	end

	def getPlayers()
		return @players
	end

	def getMaxPlayers()
		return @max_players
	end

	def getNetVersion()
		return @version
	end

	def getGameMode()
		return @mode
	end

	def isPassworded()
		return @password
	end

	def getExpansion()
		return @expansion
	end

	def getExpansionName()
		return @expansion == 1 ? "The Ballad of Gay Tony" : "The Lost and Damned"
	end

	def getIdentifier()
		return @identifier
	end

	def isSecure()
		return @secure
	end

end
