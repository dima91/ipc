/**
 * \file server.cpp
 * \brief
 * \author Luca Di Mauro
 */


#include <udp.h>
#include <iostream>

#define TCP_PORT	8090


using namespace std;
using namespace ipc;



int main (int argn, char **argv) {

	UdpSocket server	= UdpSocket (8800);

	
	char chars[20] = "Hello client!";
	std::string endp = "127.0.0.1";

	UdpSocket::ClientMessage<char> message;

	cout << "Client says:  " << get<2> (server.receive<char> (1024)) << endl;
	server.send<char> (endp, 8899, chars, strlen (chars));


	return 0;
}