#pragma once

#include <iostream>
#include "../GGE/Utils/Any.h"
#include "../GGE/Utils/EventChain.h"
#include "../GGE/Utils/ManagedBuffer.h"
#include "Definitions.h"
#include "HostInfo.h"
#include "SocketException.h"


#ifdef _WIN32
#	include "pThreads/pthread.h"
#else
#	include <pthread.h>
#endif


#define SOCKET_STREAM_BUFFER_DEFINED


namespace socketlib {
	class TCPSocketStreamBuffer : public std::basic_streambuf<char, std::char_traits<char> > {
		friend void *socketstreambuffer_read(void*);
		friend void *socketstreambuffer_recvd(void *param);
	public:

		struct received_params {
			received_params(int available,bool &shouldrecall) : available(available),shouldrecall(shouldrecall)
			{ }

			int available;
			bool &shouldrecall;
		};
	

		enum Status {
			Idle,
			Connected,
			Sending,
			Receiving
		};

		TCPSocketStreamBuffer();
		TCPSocketStreamBuffer(socketconnection sock);
		~TCPSocketStreamBuffer();

		void Close();

		bool Connect(std::string host, std::string port);
		bool Connect(AddressInfo address);
		
		gge::EventChain<TCPSocketStreamBuffer, gge::empty_event_params> Disconnected;
		gge::EventChain<TCPSocketStreamBuffer, received_params> Received;

		static int InputBufferSize;
		static int OutputBufferSize;

		bool isConnected() {
			return sock!=0;
		}

		int Available() {
			return gleft;
		}

		AddressInfo Destination() { return their; }

	protected:
		virtual int overflow(int);

		virtual int sync();

		virtual int_type underflow();


	protected:
		gge::ManagedBuffer<char> pbuffer;
		gge::ManagedBuffer<char> gbuffer;

		AddressInfo their;

		char *pnxt, *pend;
		int  pleft;
		char *greadnxt, *gwritenxt, *gend;
		int gleft;

		enum {
			norequester,
			syncedrequester
		} readrequester;

		socketconnection sock;

		pthread_t reader;
		pthread_t eventthread;

		pthread_mutex_t gwritesignal;
		pthread_mutex_t greadsignal;
		pthread_mutex_t gbufferlock;

		bool eventisrunning;
		bool readerisrunning;

		void closed();

	private:
		void init();

	};

}
