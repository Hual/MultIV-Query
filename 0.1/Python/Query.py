"""

        Python MultIV server query class by King_Hual
        
        
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

"""

import socket
import struct

class MultIVQuery:

	def __init__(self, address, port = 5000):
		self.address = address
		self.port = port+123
		self.update()

	def update(self):
		sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		error = False
		try:
			sock.settimeout(2)
			sdata = b'\x4d\x49\x56\x01\0\0\0\0'
			sock.sendto(sdata, (self.address, self.port))
			rdata = sock.recv(512)
			self.identifier = rdata[0:3]
			if self.identifier != "MIV":
				raise Exception("Invalid identifier")

			self.version = struct.unpack("<L", rdata[3:7])[0]
			str_len = struct.unpack("<L", rdata[8:12])[0]
			last_byte = 12+str_len
			self.hostname = rdata[12:last_byte]
			str_len = struct.unpack("<L", rdata[last_byte:last_byte+4])[0]
			self.mode = rdata[last_byte+4:last_byte+4+str_len]
			last_byte += 4+str_len
			self.password = (rdata[last_byte] == b"\1")
			self.secure =(rdata[last_byte+1] == b"\1")
			self.players = struct.unpack("<H", rdata[last_byte+2:last_byte+4])[0]
			self.max_players = struct.unpack("<H", rdata[last_byte+4:last_byte+6])[0]
			self.expansion = struct.unpack("<L", rdata[last_byte+6:last_byte+10])[0]

		except Exception:
			error = True

		sock.close()
		self.error = error
		return error

	def isErroneous(self):
		return self.error

	def getHostName(self):
		return self.hostname

	def getPlayers(self):
		return self.players

	def getMaxPlayers(self):
		return self.max_players

	def getNetVersion(self):
		return self.version

	def getGameMode(self):
		return self.mode

	def isPassworded(self):
		return self.password

	def getExpansion(self):
		return self.expansion

	def getExpansionName(self):
		return "The Ballad of Gay Tony" if self.expansion == 1 else "The Lost and Damned"

	def getIdentifier(self):
		return self.identifier

	def isSecure(self):
		return self.secure
