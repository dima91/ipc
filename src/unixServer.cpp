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
#include <iostream> // TODO Remove me




ipc::UnixServer::UnixServer (std::string path) {
	struct sockaddr_un socketAddr;

	socketPath	= path;
	validity	= false;

	bzero (socketAddr.sun_path, sizeof(socketAddr.sun_path));
	strncpy (socketAddr.sun_path, socketPath.c_str(), socketPath.length());
	socketAddr.sun_family	= AF_UNIX;

	socketFD	= socket (AF_UNIX, SOCK_STREAM, 0);
	if (socketFD < 0) {
		//std::cerr << "sockDesc :  " << socketDescriptor << std::endl;
		// TODO Throw an exception
		return ;
	}

	if ((bind (socketFD, (struct sockaddr *) &socketAddr, sizeof (socketAddr)))==-1) {
		//std::cerr << "cannot bind!" << std::endl;
		// TODO Throw an exception
		return ;
	}

	if ((listen (socketFD, SOMAXCONN))==-1) {
		//std::cerr << "Cannot listen!!!" << std::endl;
		// TODO Throw an exception
		return ;
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
	int clientDescriptor	= ::accept (socketFD, nullptr, 0);

	if (clientDescriptor == -1)
		return nullptr;	// FIXME Do I throw an exception..?

	Socket *clientSocket	= new Socket (clientDescriptor);

	return clientSocket;
}




void ipc::UnixServer::setBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		//std::cerr << "setNonBlocking: Cannot get sock information (LinuxServer::setNonBlocking)\n";
		// TODO Throw an exception
		return;
	}

	opts	= (opts | ~O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		//std::cerr << "setNonBlocking: Cannot set non-blocking socket (LinuxServer::setNonBlocking)\n";
		// TODO Throw an exception
		return ;
	}
}




void ipc::UnixServer::setNonBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		//std::cerr << "setNonBlocking: Cannot get sock information (LinuxServer::setNonBlocking)\n";
		// TODO Throw an exception
		return;
	}

	opts	= (opts | O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		//std::cerr << "setNonBlocking: Cannot set non-blocking socket (LinuxServer::setNonBlocking)\n";
		// TODO Throw an exception
		return ;
	}
}




bool ipc::UnixServer::isValid () {
	return validity;
}
