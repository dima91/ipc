/**
 * \file socket.cpp
 * \brief Implementation file of Socket class
 * \author Luca Di Mauro
 */

#include <socket.h>

#include <unistd.h>
#include <fcntl.h>




ipc::Socket::Socket () {
	socketFD	= -1;
}




ipc::Socket::Socket (int fileDescriptor) {
	if (socketFD == -1) {
		throw std::runtime_error ("Bad file descriptor");
	}

	socketFD	= fileDescriptor;
}




ipc::Socket::~Socket () {
	shutdown (socketFD, SHUT_RDWR);
	close (socketFD);
}




void ipc::Socket::setBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		THROW_ERROR ("Error getting socket information");
	}

	opts	= opts & (~O_NONBLOCK);

	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		THROW_ERROR ("Error setting blocking socket");
	}
}




void ipc::Socket::setNonBlocking () {
	int opts	= 0;

	if ((opts = fcntl(socketFD, F_GETFL)) < 0) {
		THROW_ERROR ("Error getting socket information");
	}

	opts	= (opts | O_NONBLOCK);
	
	if ((fcntl(socketFD, F_SETFL, opts)) < 0) {
		THROW_ERROR ("Error setting non-blocking socket");
	}
}




void ipc::Socket::setSocketDescriptor (int fileDescriptor) {
	if (socketFD != -1 || fileDescriptor < 0) {
		throw std::runtime_error ("Bad file descriptor");
	}

	socketFD	= fileDescriptor;
}