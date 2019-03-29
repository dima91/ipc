#include <interface.h>
#include <templatedServerApi.h>
#include <iostream>
#include <tcp.h>


using namespace ipc;
using namespace std;


void signalHandler (int sigType);
TemplatedServerApi<SupportedOperations>	*serverApi	= nullptr;


int main (int argn, char **argv) {
	cout << "Hello user!!\n";

	//ServerInterface *server	= new UnixServer ("/home/luca/tester.socket");
	ServerInterface *server	= new TcpServer (9089);
	serverApi				= new TemplatedServerApi<SupportedOperations> (*server);


	serverApi->addOperation<ReqResOp1, ReqResOp1> (OP_1, [](ReqResOp1 &req, ReqResOp1 &res){
		res.y	= 140319;
	});

	serverApi->addOperation<ReqResOp2, ReqResOp2> (OP_2, [](ReqResOp2 &req, ReqResOp2 &res){
		res.b	= 'b';
	});

	serverApi->addOperation<ReqResOp3, ReqResOp3> (OP_3, [](ReqResOp3 &req, ReqResOp3 &res){
		res.v	= 'v';
	});


	serverApi->startMe (true);

	delete (serverApi);
	delete (server);

	cout << "Bye bye..\n";
}




void signalHandler (int sigType)  {
	serverApi->stopMe ();
}