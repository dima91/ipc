/**
 * \file tcpServer.h
 * \brief Source file for TcpServer class
 * \author Luca Di Mauro
 */

#include <tcp.h>

#include <csignal>
#include <unistd.h>
#include <fcntl.h>




ipc::TcpServer::TcpServer (int port) {
	bzero (&serverSockAddr, sizeof(serverSockAddr));

	socketFD	= socket (AF_INET, SOCK_STREAM, 0);
	validity	= false;

	serverSockAddr.sin_family		= AF_INET;
	serverSockAddr.sin_addr.s_addr	= INADDR_ANY;
	serverSockAddr.sin_port			= htons(port);

	if ((bind (socketFD, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr))) < 0) {
		// TODO Throw exception
		validity	= false;
		return ;
	}

	if ((listen (socketFD, SOMAXCONN)) < 0) {
		//OUTPUT ("TcpServer", "Cannot listen (ServerSocket::listen)");
		// TODO Throw exception
		validity	= false;
		return ;
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
	Socket *newSock		= nullptr;
	int tmpFD			= -1;

	if ((tmpFD = ::accept (socketFD, (struct sockaddr *) &clientSockAddr, (socklen_t *) &cliLen)) < 0) {
		if (errno==EAGAIN || errno==EWOULDBLOCK) {
			// TODO Throw an exception..?
			return nullptr;
		}
		else {
			validity	= false;
			// TODO Throw exception
			//throw exceptions::SocketAcceptException ("Cannot accept connection");
			return nullptr;
		}
	}

	newSock	= new Socket (tmpFD);

	return newSock;
}




void ipc::TcpServer::setBlocking () {
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




void ipc::TcpServer::setNonBlocking () {
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




bool ipc::TcpServer::isValid () {
	// TODO Write me!
	return true;
}



