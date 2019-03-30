/**
 * \file unixClient.h
 * \brief Source file for UnixClient class
 * \author Luca Di Mauro
 */

#include <unix.h>

#include <sys/un.h>
#include <csignal>
#include <unistd.h>
#include <fcntl.h>




ipc::UnixClient::UnixClient (std::string path) {
	struct sockaddr_un serverAddr;

	bzero (serverAddr.sun_path, sizeof(serverAddr.sun_path));
	strncpy (serverAddr.sun_path, path.c_str(), path.length());
	serverAddr.sun_family	= AF_UNIX;

	socketFD	= socket (AF_UNIX, SOCK_STREAM, 0);
	if (socketFD < 0) {
		THROW_ERROR ("Error creating socket");
	}

	if (connect (socketFD, (struct sockaddr *) &serverAddr, sizeof (serverAddr))==-1) {
		THROW_ERROR ("Error connecting to server socket");
	}

	setSocketDescriptor (socketFD);
}




ipc::UnixClient::~UnixClient () {
	// Do nothing
}