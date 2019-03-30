#define CATCH_CONFIG_MAIN
#include <catch2.h>
#include <interface.h>

#include <future>
#include <iostream>

#include <socket.h>
#include <tcp.h>
#include <udp.h>
#include <unix.h>
#include <templatedServerApi.h>
#include <templatedClientApi.h>

using namespace std;
using namespace ipc;



/* =================================================================== */
/* ==============================  TCP  ============================== */

future<string> tcpServerAsync () {
	return async (launch::async, []{
		TcpServer server (8090);
		Socket *client	= server.accept ();
		char *rcvC		= client->receive<char> (1024);
		string rcvS		= string (rcvC);
		free (rcvC);
		delete (client);

		return rcvS;
	});
}

future<string> tcpClientAsync () {
	return async (launch::async, []{
		string sendMe	= "hello server!";
		TcpClient client ("localhost", 8090);
		client.send<const char> (sendMe.c_str(), 100);

		return sendMe;
	});
}

TEST_CASE ("Testing TCP classes") {
	auto serverHandle	= tcpServerAsync ();
	auto clientHandle	= tcpClientAsync ();

	REQUIRE (serverHandle.get() == clientHandle.get());
}




/* =================================================================== */
/* ==============================  UDP  ============================== */

future<string> udpServerAsync () {
	return async (launch::async, []{
		UdpSocket socket (8090);
		UdpSocket::ClientMessage<char> message;
		message		= socket.receive<char> (100) ;
		string rcvS	= string (get<2>(message));
		free (get<2>(message));

		return rcvS;
	});
}

future<string> udpClientAsync () {
	return async (launch::async, []{
		string sendMe	= "hello entity!";
		string endpoint	= "localhost";
		UdpSocket socket (8091);
		socket.send<const char> (endpoint, 8090, sendMe.c_str(), 100);

		return sendMe;
	});
}

TEST_CASE ("Testing UDP classes") {
	auto serverHandle	= udpServerAsync ();
	auto clientHandle	= udpClientAsync ();

	REQUIRE (serverHandle.get() == clientHandle.get());
}




/* ====================================?=============================== */
/* ==============================  UNIX  ============================== */

future<string> apiServerAsync () {
	return async (launch::async, []{
		using ServerType		= TemplatedServerApi<SupportedOperations>;
		ServerInterface *server	= new TcpServer (9089);
		ServerType *serverApi	= new ServerType (*server);

		std::string finalResult	= "";
		std::string *resultPtr	= &finalResult;


		serverApi->addOperation<ReqResOp1, ReqResOp1> (OP_1, [resultPtr](ReqResOp1 &req, ReqResOp1 &res){
			res.y		= 140319;
			*resultPtr += to_string (res.y);
		});

		serverApi->addOperation<ReqResOp2, ReqResOp2> (OP_2, [resultPtr](ReqResOp2 &req, ReqResOp2 &res){
			res.b		= 'b';
			*resultPtr += res.b;
		});

		serverApi->addOperation<ReqResOp3, ReqResOp3> (OP_3, [resultPtr, serverApi](ReqResOp3 &req, ReqResOp3 &res){
			res.v	= 'v';
			*resultPtr += res.v;
			serverApi->stopMe ();
		});


		serverApi->startMe (true);

		delete (serverApi);
		delete (server);

		return finalResult;
	});
}

future<string> apiClientAsync () {
	return async (launch::async, []{
		string finalResult	= "";
		SupportedOperations reqOp;
		TemplatedClientApi<SupportedOperations> api ("127.0.0.1", 9089);

		// OP 1
		ReqResOp1 req1;
		req1.x			= 100;
		reqOp			= OP_1;
		ReqResOp1 *res1	= api.invokeOperation<ReqResOp1, ReqResOp1> (reqOp, req1);
		finalResult += to_string (res1->y);
		free (res1);


		// OP 2.1
		ReqResOp2 req21;
		req21.a				= 'a';
		reqOp				= OP_2;
		ReqResOp2 *res21	= api.invokeOperation<ReqResOp2, ReqResOp2> (reqOp, req21);
		finalResult += res21->b;
		free (res21);

		// OP 2.2
		ReqResOp2 req22;
		req22.a				= 'a';
		reqOp				= OP_2;
		ReqResOp2 *res22	= api.invokeOperation<ReqResOp2, ReqResOp2> (reqOp, req22);
		finalResult += res22->b;
		free (res22);


		// OP 3
		ReqResOp3 req3;
		req3.u			= 'u';
		reqOp			= OP_3;
		ReqResOp3 *res3	= api.invokeOperation<ReqResOp3, ReqResOp3> (reqOp, req3);
		finalResult += res3->v;
		free (res3);

		return finalResult;
	});
}

TEST_CASE ("Testing UNIX classes") {
	auto serverHandle	= apiServerAsync ();
	auto clientHandle	= apiClientAsync ();

	REQUIRE (serverHandle.get() == clientHandle.get());
}