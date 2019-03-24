/**
 * \file templatedClientApi.h
 * \brief Header file for TemplatedClientApi class
 * \author Luca Di Mauro
 */


#ifndef TEMPLATED_CLIENT_API_H
#define TEMPLATED_CLIENT_API_H

#define SOCKET_SLEEP_DELAY 50				// Number of milliseconds to sleep after reading from socket


#include <unix.h>
#include <tcp.h>


namespace ipc {

	template <typename OP_T>	// This type (mostly an enum) defines the type of operation exchanged between client and server
	class TemplatedClientApi {
	private :
		ipc::Socket *serverSocket;




	public :
		// Constructor to connect to an UNIX server
		TemplatedClientApi	(std::string socketPath) {
			serverSocket	= new ipc::UnixClient (socketPath);
		}


		// Constructor to connect to a TCP server
		TemplatedClientApi  (std::string endpoint, int port) {
			serverSocket	= new ipc::TcpClient (endpoint, port);
		}


		~TemplatedClientApi	() {
			delete (serverSocket);
		}




		template <typename REQ_T, typename RES_T>
		RES_T *invokeOperation (OP_T &operationType, REQ_T &request) {
			// Sending request
			serverSocket->send<OP_T> (&operationType);
			serverSocket->send<REQ_T> (&request);

			// Receiving response
			RES_T *response	= serverSocket->receive<RES_T> ();

			return response;
		}
	};

} // namespace ipc


#endif
