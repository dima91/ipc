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
	bufLen		= DEFAULT_BUF_LENGTH;
}




ipc::Socket::Socket (int fileDescriptor) {
	if (socketFD != -1) {
		// TODO Throw an exception
	}

	socketFD	= fileDescriptor;
	bufLen		= DEFAULT_BUF_LENGTH;
}




ipc::Socket::~Socket () {
	shutdown (socketFD, SHUT_RDWR);
	close (socketFD);
}




void ipc::Socket::setBlocking () {
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




void ipc::Socket::setNonBlocking () {
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




void ipc::Socket::setBufferLength (int length) {
	if (length<0)
		//std::cerr << "Wrong length value!\n";
		// TODO Throw an exception
		return ;
	else
		bufLen	= length;
}




void ipc::Socket::setSocketDescriptor (int fileDescriptor) {
	if (socketFD != -1 || fileDescriptor < 0) {
		// TODO Throw an exception
		return ;
	}

	socketFD	= fileDescriptor;
}