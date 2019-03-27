/**
 * \file server.cpp
 * \brief
 * \author Luca Di Mauro
 */


#include <unix.h>
#include <iostream>

#define TCP_PORT	8090


using namespace std;
using namespace ipc;



int main (int argn, char **argv) {
	string sendMe	= "Hello client!";
	UnixServer server ("/home/luca/tester.socket");

	Socket *client	= server.accept ();

	client->send<const char> (sendMe.c_str(), (int) sendMe.length());
	cout << "Client says:\t" << client->receive<char> (1024) << endl;

	delete (client);


	return 0;
}