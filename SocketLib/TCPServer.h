#pragma once

#include "../GGE/Utils/Any.h"
#include "../GGE/Utils/EventChain.h"
#include "../GGE/Utils/Property.h"

#include "TCPSocketStream.h"
#include "Definitions.h"
#include "Networking.h"
#include "SocketException.h"

#ifdef _WIN32
#	include "pThreads/pthread.h"
#else
#	include <pthread.h>
#endif

#define SOCKET_SERVER_EXIST


namespace socketlib {

	
	class TCPServer {
		friend void *server_thread_start(void *arg);
	public:

		struct accept_params {
			accept_params(TCPSocketStream &socket, AddressInfo addrinfo) : 
				socket(socket), addrinfo(addrinfo)	
			{ }
			TCPSocketStream &socket;
			AddressInfo addrinfo;
		};

		struct connlost_params {
			connlost_params(TCPSocketStream &socket) : socket(socket) { }
			TCPSocketStream &socket;
		};

		enum Status {
			Idle,
			Listening,
			Accepting,
			BlockingAccept
		};

		 TCPServer();
		~TCPServer();

		void Listen(std::string port);
		void Listen(networking::PortNumber Port) {
			std::stringstream ss;
			ss<<Port;
			Listen(ss.str());
		}
		void StopListening();

		void StartAccept();
		TCPSocketStream &Accept(int Timeout=-1);

		Status getStatus() { return state; }

		gge::EventChain<TCPServer, accept_params> ConnectionReceived;
		gge::EventChain<TCPServer, connlost_params> ConnectionLost;

		Property<networking::FamilyType, TCPServer>	Family;
		Property<int, TCPServer>					MaxConnQueue;

		//Call ConnectionReceived Event in a new thread
		bool CallConnRcvedEvtInNThrd;

		void CloseAll();

		int LiveConnections();

		void setFamily(networking::FamilyType protocol);
		networking::FamilyType getFamily() const;

		void setMaxConnQueue(int protocol);
		int getMaxConnQueue() const;

	protected:
		Status state;
		socketconnection sock;

		//<Family>
		networking::FamilyType family;

		void child_disconnected(TCPSocketStream &sock);

		//<MaxConnQueue>
		int maxconnqueue;

		pthread_mutex_t isacceptthreadrunning;

		TCPSocketStream *accept(int Timeout);



		Collection<TCPSocketStream> Connections;

		pthread_t acceptthread;
	};

}
