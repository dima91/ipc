/**
 * \file udp.h
 * \brief Header file for UDP protocol related classes
 * \author Luca Di Mauro
 */

#ifndef UDP_H
#define UDP_H

#include <socket.h>

#include <string>
#include <tuple>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <cerrno>


namespace ipc {

	/**
	 * \class UdpSocket
	 * \brief TODO
	 */
	class UdpSocket {
	public :
		template <typename T>
		using ClientMessage	= std::tuple<std::string, int, T*>;		// TODO Create a class!


	private :
		int socketFD;
		bool validity;
		struct sockaddr_in tmpClient;
		unsigned int tmpClientLen;


	public :
		UdpSocket (int port);
		~UdpSocket ();

		void setBlocking ();
		void setNonBlocking ();


		/* ============================== */
		/* ============================== */




		template <typename T>
		void send (std::string &endpoint, int port, T *message, int bufLen=-1) {
			bzero (&tmpClient, tmpClientLen);
			struct hostent *clientName	= gethostbyname (endpoint.c_str());

			bufLen					= (bufLen == -1) ? sizeof(T) : bufLen;
			tmpClient.sin_family	= AF_INET;
			tmpClient.sin_port		= htons (port);
			memcpy (&tmpClient.sin_addr, clientName->h_addr_list[0], clientName->h_length);

			sendto (socketFD, message, bufLen, 0, (struct sockaddr *) &tmpClient, tmpClientLen);
		}




		template <typename T>
		ClientMessage<T> receive (int bufLen=-1) {
			bzero (&tmpClient, tmpClientLen);

			bufLen			= (bufLen == -1) ? sizeof(T) : bufLen;
			void *recvBuf	= (void *) malloc (bufLen);

			recvfrom (socketFD, recvBuf, bufLen, 0, (struct sockaddr *)&tmpClient, &tmpClientLen);

			std::string clientIp	= std::string (inet_ntoa (tmpClient.sin_addr));
			int clientPort			= ntohs (tmpClient.sin_port);

			return std::make_tuple (clientIp, clientPort, (T*) recvBuf);
		}
	};
}


#endif // UDP_H
