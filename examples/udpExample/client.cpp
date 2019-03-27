/**
 * \file client.cpp
 * \brief
 * \author Luca Di Mauro
 */


#include <udp.h>
#include <iostream>

#define SERVER_ADDR	"127.0.0.1"
#define SERVER_PORT	8090


using namespace std;
using namespace ipc;



int main (int argn, char **argv) {
	UdpSocket client = UdpSocket (8899);

	string sendMe = "Hello server!";
	std::string endp = "127.0.0.1";

	UdpSocket::ClientMessage<char> message;

	client.send<const char> (endp, 8800, sendMe.c_str(), (int) sendMe.length());
	cout << "Server says:\t" << get<2> (client.receive<char> (1024)) << endl;


	return 0;
}