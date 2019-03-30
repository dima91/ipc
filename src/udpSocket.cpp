/**
* \file udpSocket.h
 * \brief Source file for UdpSocket class
 * \author Luca Di Mauro
 */

#include <udp.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <unistd.h>
#include <fcntl.h>




ipc::UdpSocket::UdpSocket (int port) {
	struct sockaddr_in serverSockAddr;

	bzero (&serverSockAddr, sizeof(serverSockAddr));
	bzero (&tmpClient, sizeof(tmpClient));
	tmpClientLen	= sizeof(tmpClient);

	socketFD		= socket (AF_INET, SOCK_DGRAM, 0);
	validity		= false;

	serverSockAddr.sin_family		= AF_INET;
	serverSockAddr.sin_addr.s_addr	= INADDR_ANY;
	serverSockAddr.sin_port			= htons(port);

	if ((bind (socketFD, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr))) < 0) {
		THROW_ERROR ("Error binding UDP socket");
	}

	// Ignoring SIGPIPE signal due to a socket error (disconnection)
	signal (SIGPIPE, SIG_IGN);

	validity	= true;
}




ipc::UdpSocket::~UdpSocket () {
	shutdown (socketFD, SHUT_RDWR);
	close (socketFD);
}




void ipc::UdpSocket::setBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		THROW_ERROR ("Error getting socket information");
	}

	opts	= opts & (~O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		THROW_ERROR ("Error setting blocking socket");
	}
}




void ipc::UdpSocket::setNonBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		THROW_ERROR ("Error getting socket information");
	}

	opts	= (opts | O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		THROW_ERROR ("Error setting non-blocking socket");
	}
}
