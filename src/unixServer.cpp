/**
 * \file unixServer.h
 * \brief Source file for UnixServer class
 * \author Luca Di Mauro
 */

#include <unix.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <csignal>
#include <unistd.h>
#include <fcntl.h>




ipc::UnixServer::UnixServer (std::string path) {
	struct sockaddr_un socketAddr;

	socketPath	= path;
	validity	= false;

	bzero (socketAddr.sun_path, sizeof(socketAddr.sun_path));
	strncpy (socketAddr.sun_path, socketPath.c_str(), socketPath.length());
	socketAddr.sun_family	= AF_UNIX;

	socketFD	= socket (AF_UNIX, SOCK_STREAM, 0);
	if (socketFD < 0) {
		THROW_ERROR ("Error creating socket");
	}

	if ((bind (socketFD, (struct sockaddr *) &socketAddr, sizeof (socketAddr)))==-1) {
		THROW_ERROR ("Error binding socket with given path");
	}

	if ((listen (socketFD, SOMAXCONN))==-1) {
		THROW_ERROR ("Error listening for new connections");
	}

	// Ignoring SIGPIPE signal due to a socket error (disconnection)
	signal (SIGPIPE, SIG_IGN);

	validity	= true;
}




ipc::UnixServer::~UnixServer () {
	shutdown (socketFD, SHUT_RDWR);
	close (socketFD);
	remove (socketPath.c_str());
}




ipc::Socket *ipc::UnixServer::accept () {
	int tmpFD	= -1;

	if ((tmpFD = ::accept (socketFD, nullptr, 0)) < 0) {
		if (errno==EAGAIN || errno==EWOULDBLOCK) {
			return nullptr;
		}
		else {
			validity	= false;
			THROW_ERROR ("Error accepting new connection");
		}
	}

	Socket *clientSocket	= new Socket (tmpFD);

	return clientSocket;
}




void ipc::UnixServer::setBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		THROW_ERROR ("Error getting socket information");
	}

	opts	= (opts | ~O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		THROW_ERROR ("Error setting blocking socket");
	}
}




void ipc::UnixServer::setNonBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		THROW_ERROR ("Error getting socket information");
	}

	opts	= (opts | O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		THROW_ERROR ("Error setting non-blocking socket");
	}
}




bool ipc::UnixServer::isValid () {
	return validity;
}
