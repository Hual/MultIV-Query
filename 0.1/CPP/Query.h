/*
       	ANSI C/++ MultIV server query class by King_Hual
        

        Arguments:

        string address - The IP address
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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#include <Ws2tcpip.h>
	#if defined _MSC_VER
		#pragma comment (lib, "Ws2_32.lib")
	#endif
#else
	#include <sys/socket.h>
	#include <netdb.h>
	#include <sys/stat.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <errno.h>
	#include <arpa/inet.h>
#endif

class MultIVQuery
{
	public:
		MultIVQuery(std::string, int);
		~MultIVQuery();
		bool update();
		bool isErroneous();
		bool isPassworded();
		int getMaxPlayers();
		int getPlayers();
		int getNetVersion();
		int getExpansion();
		std::string getHostName();
		std::string getGameMode();
		std::string getExpansionName();
		std::string getIdentifier();
		unsigned char getUnknownByte();
	private:
		bool error;
		bool password;
		int version;
		int expansion;
		int port;
		std::string hostname;
		std::string address;
		std::string identifier;
		std::string mode;
		short players;
		short max_players;
		unsigned char unknown;
		void closeSock(int);
};
