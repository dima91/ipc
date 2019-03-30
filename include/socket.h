/**
 * \file socket.h
 * \brief
 * \author Luca Di Mauro
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>


#define THROW_ERROR(prevMessage) {\
	int localErrno				= errno;\
	std::string errorMessage	= prevMessage;\
	errorMessage += ":    ";\
	errorMessage += strerror(localErrno);\
	throw std::runtime_error (errorMessage);\
}


namespace ipc {

	/**
	 * \class Socket
	 * \brief Class which represent an entity on which can be sent messages or bytes
	 */
	class Socket {
	private:
		int socketFD;


	protected:
		void setSocketDescriptor (int descriptor);


	public:
		Socket ();
		Socket (int fileDescriptor);
		~Socket ();

		void setBlocking ();
		void setNonBlocking ();


		template <typename T>
		void send (T *message, int bufLen=-1) {
			if (bufLen == -1) {
				bufLen	= sizeof (T);
			}
			if (::send (socketFD, message, bufLen, 0) < 0 )
				THROW_ERROR ("Error sending message")
		}



		template <typename T>
		void send (T *message, int *wroteBytes, int bufLen=-1) {
			if (bufLen == -1) {
				bufLen	= sizeof (T);
			}
			if (*wroteBytes	= ::send (socketFD, message, bufLen, 0) < 0 )
				THROW_ERROR ("Error sending message")
		}



		template <typename T>
		T *receive (int bufLen=-1) {
			if (bufLen == -1)
				bufLen	= sizeof (T);

			void *recvBuf	= (void *) malloc (bufLen);
			bzero (recvBuf, bufLen);

			if (::recv (socketFD, recvBuf, bufLen, 0) <= 0)
				return nullptr;
			else
				return static_cast<T*> (recvBuf);
		}

		template <typename T>
		T *receive (int *readBytes, int bufLen=-1) {
			if (bufLen == -1)
				bufLen	= sizeof (T);

			void *recvBuf	= (void *) malloc (bufLen);
			bzero (recvBuf, bufLen);

			*readBytes	= recv (socketFD, recvBuf, bufLen, 0);

			return static_cast<T*> (recvBuf);
		}
	};
}


#endif // SOCKET_H
