/**
 * \file templatedServerApi.h
 * \brief Header file for TemplatedServerApi class
 * \author Luca Di Mauro
 */


#ifndef TEMPLATED_SERVER_API_H
#define TEMPLATED_SERVER_API_H

#define SOCKET_SLEEP_DELAY 50				// Number of milliseconds to sleep after reading from socket


#include <ipc/serverInterface.h>

#include <functional>
#include <map>
#include <thread>
#include <csignal>
#include <vector>

#include <unistd.h>
#include <iostream> //TODO REMOVE ME!


namespace ipc {

	template <typename OP_T>	// This type (mostly an enum) defines the type of operation exchanged between client and server
	class TemplatedServerApi {
	private :
		typedef std::function <void (ipc::Socket &)> ReqResFetcher;

		ipc::ServerInterface &serverInterface;
		std::map<OP_T, ReqResFetcher> reqResHandlersMap;

		volatile sig_atomic_t haltMe;
		std::thread serverThread;
		std::vector<std::thread> clients;


		void serverManager () {
			ipc::Socket *clientSocket;

			haltMe	= 0;
			while (!haltMe) {
				clientSocket	= serverInterface.accept ();
				if (clientSocket != nullptr) {
					clients.push_back (std::thread ([this, clientSocket]() {clientHandler(clientSocket);}));
				}

				usleep (SOCKET_SLEEP_DELAY * 1000);
			}
		}


		void clientHandler (ipc::Socket *clientSocket) {
			bool receiveOp	= true;
			//clientSocket.setNonBlocking (); // FIXME

			while (clientSocket && receiveOp && !haltMe) {
				OP_T *reqOp	= clientSocket->receive<OP_T> ();

				if (reqOp == nullptr) {
					receiveOp	= false;
					std::cerr << "[clientHandler]  Error on receiving! (is client disconnected?)\n";
				}

				else {
					auto element	= reqResHandlersMap.find(*reqOp);
					if (element != reqResHandlersMap.end()) {
						std::cout << "[clientHandler]  Found operation:  " << *reqOp << std::endl;
						element->second (*clientSocket);
					}
					else {
						std::cerr << "[clientHandler]  Wrong requested operation! (" << *reqOp << ")\n";
					}
				}

				usleep (SOCKET_SLEEP_DELAY * 1000);
			}

			if (clientSocket)
				delete (clientSocket);

			//DBG ("clientHandler", "Oh sheep! rO:" << receiveOp << "  hM:" << haltMe);
		}




	public :
		TemplatedServerApi	(ipc::ServerInterface &servIf) : serverInterface (servIf) {
			haltMe	= 0;
			serverInterface.setNonBlocking ();
		}


		~TemplatedServerApi	() {
			if (haltMe == 0) {
				haltMe	= 1;
			}

			if (serverThread.joinable()) {
				serverThread.join();
			}

			// Stopping clients
			for (std::thread &c : clients) {
				if (c.joinable()) {
					c.join();
				}
			}
		}




		template <typename REQ_T, typename RES_T>
		void addOperation (OP_T operationType, std::function<void (REQ_T &, RES_T &)> callback) {

			reqResHandlersMap.emplace (operationType, [this, callback] (ipc::Socket &client) {
				using LocalCB		= std::function<void (REQ_T &, RES_T &)>;
				REQ_T *req;
				RES_T res;
				LocalCB *localCB	= new LocalCB (callback);

				req	= client.receive<REQ_T> ();
				try {
					(*localCB) (*req, res);
				} catch (std::runtime_error &e) {
					std::cerr << "ERROR -->  " << e.what() << std::endl;
				} catch (std::exception &e) {
					std::cerr << "EXCEP -->  " << e.what() << std::endl;
				}
				client.send<RES_T> (&res);
			});
		}




		template <typename REQ_RES_T>
		void addOperation (OP_T operationType, std::function<void (REQ_RES_T &)> callback) {

			reqResHandlersMap.emplace (operationType, [this, callback] (ipc::Socket &client) {
				using LocalCB		= std::function<void (REQ_RES_T &)>;
				REQ_RES_T *reqRes;
				LocalCB *localCB	= new LocalCB (callback);

				reqRes	= client.receive<REQ_RES_T> ();
				try {
					(*localCB) (*reqRes);
				} catch (std::runtime_error &e) {
					std::cerr << "ERROR -->  " << e.what() << std::endl;
				} catch (std::exception &e) {
					std::cerr << "EXCEP -->  " << e.what() << std::endl;
				}
				client.send<REQ_RES_T> (&reqRes);
			});
		}




		void startMe (bool blocking=false) {
			serverThread	= std::thread ([this] {serverManager();});

			if (blocking)
				serverThread.join();
		}


		void stopMe () {
			haltMe	= 1;
		}
	};

} // namespace ipc


#endif
