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
        isSecure() - Returns true if the server is AC-protected, false if it isn't

*/

#include "Query.h"

MultIVQuery::MultIVQuery(std::string address, int port = 5000)
{
	this->address = address;
	this->port = port+123;
	this->update();
}

MultIVQuery::~MultIVQuery()
{
	this->players = 0;
	this->max_players = 0;
	this->hostname = "";
	this->address = "";
	this->port = 0;
	this->error = 0;
	this->version = 0;
	this->expansion = 0;
	this->identifier = "";
	this->mode = "";
	this->password = false;
	this->unknown = 0;
	this->secure = false;
}

bool MultIVQuery::isErroneous()
{
	return this->error;
}

int MultIVQuery::getMaxPlayers()
{
	return this->max_players;
}

int MultIVQuery::getPlayers()
{
	return this->players;
}

std::string MultIVQuery::getHostName()
{
	return this->hostname;
}

int MultIVQuery::getNetVersion()
{
	return this->version;
}

std::string MultIVQuery::getGameMode()
{
	return this->mode;
}

bool MultIVQuery::isPassworded()
{
	return this->password;
}

int MultIVQuery::getExpansion()
{
	return this->expansion;
}

std::string MultIVQuery::getExpansionName()
{
	return this->getExpansion() == 1 ? "The Ballad of Gay Tony" : "The Lost and Damned";
}

std::string MultIVQuery::getIdentifier()
{
	return this->identifier;
}

bool MultIVQuery::isSecure()
{
	return this->secure;
}

void MultIVQuery::closeSock(int socket)
{
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
	closesocket(socket);
	WSACleanup();
#else
	close(socket);
#endif
}

bool MultIVQuery::update()
{
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0)
		return (this->error = true);
#endif

	struct addrinfo hints, *servinfo, *p;
	char tmpPort[11];
	int sock, rv;
	memset(&hints, 0, sizeof(hints));
	sprintf(tmpPort, "%d", this->port);

	hostent *record = gethostbyname(this->address.c_str());
	if(record == NULL) return (this->error = true);
	in_addr *address = (in_addr *)record->h_addr;
	if ((rv = getaddrinfo(inet_ntoa(*address), tmpPort, &hints, &servinfo)) != 0)
		return (this->error = true);

	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	for(p=servinfo; p!=NULL; p=p->ai_next)
	{
		if ((sock = socket(p->ai_family, SOCK_DGRAM,0)) == -1)
			continue;
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
		if (connect(sock, p->ai_addr, p->ai_addrlen) == -1)
		{
			this->closeSock(sock);
			continue;
		}
		break;
	}
	if (p == NULL)
		return (this->error = true);

	freeaddrinfo(servinfo);
	const char sdata[] =
	{
		0x4d,0x49,
		0x56,0x01,
		0x00,0x00,
		0x00,0x00
	};
	if(send(sock, sdata, sizeof(sdata), 0) == -1)
		return (this->error = true);

	unsigned char buf[512], last_byte;
	size_t recv_s;
	if((recv_s = recv(sock, buf, sizeof(buf), 0)) < 23)
		return (this->error = true);

	this->identifier = std::string(reinterpret_cast<char*>(buf), 0, 3);
	if(this->identifier.compare("MIV") != 0)
		return (this->error = true);

	memcpy(&this->version, &buf[3], 4);
	int str_length; // The IT crowd reference here, length will hopefully be 1 byte in the future
	memcpy(&str_length, &buf[8], 4);
	this->hostname = std::string(reinterpret_cast<char*>(&buf[12]), 0, str_length);
	last_byte = 12+str_length;
	memcpy(&str_length, &buf[last_byte], 4);
	this->mode = std::string(reinterpret_cast<char*>(&buf[last_byte+4]), 0, str_length);
	last_byte += 4+str_length;	// length will hopefully be 1 byte in the future
	this->password = buf[last_byte];
	this->unknown = buf[last_byte+1];
	memcpy(&this->players, &buf[last_byte+2], 2);
	memcpy(&this->max_players, &buf[last_byte+4], 2);
	memcpy(&this->expansion, &buf[last_byte+6], 4);	// hopefully 1 byte in the future
	this->closeSock(sock);
	return (this->error = false);
}
