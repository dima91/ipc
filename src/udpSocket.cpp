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
		// TODO Throw exception
		validity	= false;
		return ;
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
		//std::cerr << "setNonBlocking: Cannot get sock information (LinuxSocket::setBlocking)\n";
		// TODO Throw an exception
		return;
	}

	opts	= opts & (~O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		//std::cerr << "setNonBlocking: Cannot set non-blocking socket (LinuxSocket::setBlocking)\n";
		// TODO Throw an exception
		return ;
	}
}




void ipc::UdpSocket::setNonBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		//std::cerr << "setNonBlocking: Cannot get sock information (LinuxSocket::setNonBlocking)\n";
		// TODO Throw an exception
		return;
	}

	opts	= (opts | O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		//std::cerr << "setNonBlocking: Cannot set non-blocking socket (LinuxSocket::setNonBlocking)\n";
		// TODO Throw an exception
		return ;
	}
}
