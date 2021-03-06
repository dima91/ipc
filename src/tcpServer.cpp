/**
 * \file tcpServer.h
 * \brief Source file for TcpServer class
 * \author Luca Di Mauro
 */

#include <tcp.h>

#include <csignal>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>




ipc::TcpServer::TcpServer (int port) {
	bzero (&serverSockAddr, sizeof(serverSockAddr));

	socketFD	= socket (AF_INET, SOCK_STREAM, 0);
	validity	= false;

	serverSockAddr.sin_family		= AF_INET;
	serverSockAddr.sin_addr.s_addr	= INADDR_ANY;
	serverSockAddr.sin_port			= htons(port);

	if ((bind (socketFD, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr))) < 0) {
		THROW_ERROR ("Cannot bind server socket");
	}

	if ((listen (socketFD, SOMAXCONN)) < 0) {
		THROW_ERROR ("Cannot listen for new connections");
	}

	// Ignoring SIGPIPE signal due to a socket error (disconnection)
	signal (SIGPIPE, SIG_IGN);

	validity	= true;
}




ipc::TcpServer::~TcpServer () {
	shutdown (socketFD, SHUT_RDWR);
	close (socketFD);
}




ipc::Socket *ipc::TcpServer::accept () {
	struct sockaddr_in clientSockAddr;
	int cliLen			= sizeof (clientSockAddr);
	int tmpFD			= -1;

	if ((tmpFD = ::accept (socketFD, (struct sockaddr *) &clientSockAddr, (socklen_t *) &cliLen)) < 0) {
		if (errno==EAGAIN || errno==EWOULDBLOCK) {
			return nullptr;
		}
		else {
			validity	= false;
			THROW_ERROR ("Error accepting new connection");
		}
	}

	return new Socket (tmpFD);;
}




void ipc::TcpServer::setBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		THROW_ERROR ("Error getting socket information");
	}

	opts	= opts & (~O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		THROW_ERROR ("Error setting blocking socket");
	}
}




void ipc::TcpServer::setNonBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		THROW_ERROR ("Error getting socket information");
	}

	opts	= (opts | O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		THROW_ERROR ("Error setting non-blocking socket");
	}
}




bool ipc::TcpServer::isValid () {
	return validity;
}



