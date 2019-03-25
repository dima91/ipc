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
		// TODO Throw an error
		return ;
	}

	if (connect (socketFD, (struct sockaddr *) &serverAddr, sizeof (serverAddr))==-1) {
		// TODO Throw an error
		return ;
	}

	setSocketDescriptor (socketFD);
}




ipc::UnixClient::~UnixClient () {
	// TODO
}