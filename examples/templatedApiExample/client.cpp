#include <interface.h>
#include <templatedClientApi.h>
#include <iostream>


using namespace ipc;
using namespace std;


int main (int argn, char **argv) {
	cout << "Hello user!!\n";

	SupportedOperations reqOp;

	TemplatedClientApi<SupportedOperations> api ("127.0.0.1", 9089);

	// OP 1
	ReqResOp1 req1;
	req1.x			= 100;
	reqOp			= OP_1;
	ReqResOp1 *res1	= api.invokeOperation<ReqResOp1, ReqResOp1> (reqOp, req1);
	cout << "res1->y:  " << res1->y << endl;
	free (res1);


	// OP 2.1
	ReqResOp2 req21;
	req21.a				= 'a';
	reqOp				= OP_2;
	ReqResOp2 *res21	= api.invokeOperation<ReqResOp2, ReqResOp2> (reqOp, req21);
	cout << "res2->b:  " << res21->b << endl;
	free (res21);

	// OP 2.2
	ReqResOp2 req22;
	req22.a				= 'a';
	reqOp				= OP_2;
	ReqResOp2 *res22	= api.invokeOperation<ReqResOp2, ReqResOp2> (reqOp, req22);
	cout << "res2->b:  " << res22->b << endl;
	free (res22);


	// OP 3
	ReqResOp3 req3;
	req3.u			= 'u';
	reqOp			= OP_3;
	ReqResOp3 *res3	= api.invokeOperation<ReqResOp3, ReqResOp3> (reqOp, req3);
	cout << "res3->v:  " << res3->v << endl;
	free (res3);


	cout << "Bye bye..\n";
}