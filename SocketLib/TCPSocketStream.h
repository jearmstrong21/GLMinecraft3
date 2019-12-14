#pragma once

#include <iostream>
#include "Definitions.h"
#include "TCPSocketStreamBuffer.h"
#include "SocketException.h"
#include "../GGE/Utils/Any.h"
#include "../GGE/Utils/EventChain.h"

#define SOCKET_STREAM_DEFINED

namespace socketlib {

	
	class TCPSocketStream : public std::basic_iostream<char,std::char_traits<char> > {
		TCPSocketStreamBuffer buffer;
	public:

		typedef TCPSocketStreamBuffer::received_params received_params;

		TCPSocketStream() : 
		  std::basic_iostream<char,std::char_traits<char> >(&buffer),
		  Disconnected("Disconnected", this),
		  Received("Received", this)
		{
			init();
		}

		TCPSocketStream(socketconnection sock) : 
		  buffer(sock),
		  std::basic_iostream<char,std::char_traits<char> >(&buffer),
		  Disconnected("Disconnected", this),
		  Received("Received", this)
		{
			init();
		}

		~TCPSocketStream() {
		}

		AddressInfo Destination() { return buffer.Destination(); }
		AddressInfo RemoteAddress() { return buffer.Destination(); }

		template<class T_>
		TCPSocketStream &WriteBinary(T_ data) {
			write(reinterpret_cast<char*>(&data), sizeof(T_));

			return *this;
		}

		template<class T_>
		TCPSocketStream &WriteBinary(T_ *data) {
			write(reinterpret_cast<char*>(data), sizeof(T_));

			return *this;
		}


		template<class T_>
		TCPSocketStream &ReadBinary(T_ &data) {
			read(reinterpret_cast<char*>(&data), sizeof(T_));

			return *this;
		}

		template<class T_>
		TCPSocketStream &ReadBinary(T_ *data) {
			read(reinterpret_cast<char*>(data), sizeof(T_));

			return *this;
		}

		void Close() {
			buffer.Close();
		}

		bool Connect(std::string host, std::string port) {
			return buffer.Connect(host, port);
		}

		bool Connect(std::string host, networking::PortNumber port) {
			std::stringstream ss;
			ss<<port;
			return buffer.Connect(host, ss.str());
		}
		bool Connect(AddressInfo address) {
			return buffer.Connect(address);
		}

		bool isConnected() {
			return buffer.isConnected(); 
		}

		int Available() {
			return buffer.Available();
		}

		gge::EventChain<TCPSocketStream, gge::empty_event_params> Disconnected;
		gge::EventChain<TCPSocketStream, received_params> Received;


	private:
		void init() {
			buffer.Received.LinkTo(Received);
			buffer.Disconnected.LinkTo(Disconnected);
		}
	};

	typedef TCPSocketStream TCPClient;
}
