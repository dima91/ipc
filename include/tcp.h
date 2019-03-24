/**
 * \file tcp.h
 * \brief Header file for TCP protocol related classes
 * \author Luca Di Mauro
 */

#ifndef TCP_H
#define TCP_H

#include <socket.h>
#include <serverInterface.h>

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>


namespace ipc {

	/**
	 * \class TcpServer
	 * \brief Class which implements a TCP base server
	 */
	class TcpServer : public ServerInterface {
	private :
		int socketFD;
		struct sockaddr_in serverSockAddr;
		bool validity;

	public :
		TcpServer	(int port);
		~TcpServer	();

		Socket *accept		();
		bool isValid		();
		void setBlocking	();
		void setNonBlocking	();
	};




	/**
	 * \class TcpClient
	 * \brief Class which implements a TCP base client
	 */
	class TcpClient : public Socket {
	private :

	public :
		TcpClient	(std::string endpointIp, int port);
		~TcpClient	();
	};
}


#endif // TCP_H
