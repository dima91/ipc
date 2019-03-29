/**
 * \file client.cpp
 * \brief
 * \author Luca Di Mauro
 */


#include <unix.h>
#include <iostream>

#define SERVER_ADDR	"127.0.0.1"
#define SERVER_PORT	8090


using namespace std;
using namespace ipc;



int main (int argn, char **argv) {
	string sendMe	= "Hello server!";
	UnixClient client ("/home/luca/tester.socket");


	char *message	= client.receive<char> (1024);
	cout << "Server says:\t" << message << endl;
	client.send<const char> (sendMe.c_str(), sendMe.length());

	free (message);


	return 0;
}