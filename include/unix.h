/**
 * \file unix.h
 * \brief Header file for AF_UNIX sockets related classes
 * \author Luca Di Mauro
 */

#ifndef UNIX_H
#define UNIX_H

#include <socket.h>
#include <serverInterface.h>

#include <string>



namespace ipc {

	/**
	 * \class UnixServer
	 * \brief
	 */
	class UnixServer : public ServerInterface {
	private :
		int socketFD;
		std::string socketPath;
		bool validity;

	public :
		UnixServer (std::string socketPath);
		~UnixServer ();

		Socket *accept ();
		void setNonBlocking ();
		void setBlocking ();
		bool isValid ();
	};




	/**
	 * \class UnixClient
	 * \brief
	 */
	class UnixClient : public Socket {
	private :
		int socketFD;

	public :
		UnixClient (std::string socketPath);
		~UnixClient ();
	};
}


#endif // UNIX_H
