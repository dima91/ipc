/**
 * \file tcpClient.h
 * \brief Source file for TcpClient class
 * \author Luca Di Mauro
 */

#include <tcp.h>

#include <netdb.h>




ipc::TcpClient::TcpClient (std::string endpoint, int port) {
	struct sockaddr_in serverAddr;
	struct hostent *serverName;
	int socketFD	= 0;

	bzero (&serverAddr, sizeof(serverAddr));

	socketFD	= socket (AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0) {
		// TODO Throw exception
		return ;
	}

	serverName	= gethostbyname (endpoint.c_str());
	if (serverName == nullptr) {
		// TODO Throw an exception
		return ;
	}

	serverAddr.sin_family	= AF_INET;
	serverAddr.sin_port		= htons (port);
	memcpy(&serverAddr.sin_addr, serverName->h_addr_list[0], serverName->h_length);

	if (::connect (socketFD, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
		// TODO Throw exception
		return ;
	}

	setSocketDescriptor (socketFD);
}




ipc::TcpClient::~TcpClient () {
	// TODO
}