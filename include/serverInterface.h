/**
 * \file serverInterface.h
 * \brief Header file for "ServerInterface" abstract class
 * \author Luca Di Mauro
 */

#ifndef SERVER_INTERFACE_H
#define SERVER_INTERFACE_H

#include <socket.h>


namespace ipc {

	/**
	 * \class ServerInterface
	 * \brief Interface to define supported operations by a server (e.g. tcp or unix server)
	 */
	class ServerInterface {
	public:
		virtual ~ServerInterface	() {};

		virtual Socket *accept		() = 0;
		virtual bool isValid		() = 0;
		virtual void setBlocking	() = 0;
		virtual void setNonBlocking () = 0;
	};
}


#endif // SERVER_INTERFACE_H
