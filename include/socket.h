/**
 * \file socket.h
 * \brief
 * \author Luca Di Mauro
 */

#ifndef SOCKET_H
#define SOCKET_H

#define DEFAULT_BUF_LENGTH 2048

#include <serializable.h>

#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>


namespace ipc {

	/**
	 * \class Socket
	 * \brief Class which represent an entity on which can be sent messages or bytes
	 */
	class Socket {
	private:
		int socketFD;
		int bufLen;


	protected:
		void setSocketDescriptor (int descriptor);


	public:
		Socket ();
		Socket (int fileDescriptor);
		~Socket ();

		void setBlocking ();
		void setNonBlocking ();
		void setBufferLength (int length);


		template <typename T>
		void send (T *message, int bufLen=-1) {
			if (bufLen == -1) {
				bufLen	= sizeof (T);
			}
			::send (socketFD, message, bufLen, 0);
		}



		template <typename T>
		void send (T *message, int *wroteBytes, int bufLen=-1) {
			if (bufLen == -1) {
				bufLen	= sizeof (T);
			}
			*wroteBytes	= ::send (socketFD, message, bufLen, 0);
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
				return (T*) recvBuf;
		}

		template <typename T>
		T *receive (int *readBytes, int bufLen=-1) {
			if (bufLen == -1)
				bufLen	= sizeof (T);

			void *recvBuf	= (void *) malloc (bufLen);
			bzero (recvBuf, bufLen);

			*readBytes	= recv (socketFD, recvBuf, bufLen, 0);

			return (T*) recvBuf;
		}
	};
}


#endif // SOCKET_H
