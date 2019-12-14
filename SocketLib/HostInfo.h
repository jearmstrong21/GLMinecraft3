#pragma once

#include "../GGE/Utils/Property.h"
#include "../GGE/Utils/Collection.h"
#include "../GGE/Utils/Any.h"
#include "../GGE/Utils/EventChain.h"
#include <string>
#include <sstream>
#include "Definitions.h"
#include "Networking.h"
#include "SocketException.h"


#ifdef _WIN32
#	include "pThreads/pthread.h"
#else
#	include <pthread.h>
#endif


#define HOSTINFO_EXIST


namespace socketlib {

	class AddressInfo {
		friend class HostInfo;
		friend class TCPSocketStreamBuffer;

	public:
		bool operator ==(AddressInfo &addr);
		bool operator !=(AddressInfo &addr);
		operator std::string() const { return ipaddress; }
		operator networking::FamilyType() const { return family; }

		const std::string &IPAddress() const { return ipaddress; }

		const networking::FamilyType &Family() const { return family; }

		prvt::addrinfo *GetRaw() { return raw; }

		AddressInfo(const AddressInfo &inf) {
			raw=inf.raw; ipaddress=inf.ipaddress; family=inf.family;
		}

		AddressInfo &operator =(AddressInfo &inf) { 
			raw=inf.raw; ipaddress=inf.ipaddress; family=inf.family;

			return *this;
		}
		AddressInfo(prvt::addrinfo *raw) { init(raw); }
		AddressInfo() : raw(NULL) { }
		~AddressInfo() { /*if(raw) delete(raw);*/ }

	protected:

		void init(prvt::addrinfo *raw);

		std::string ipaddress;
		networking::FamilyType family;

		prvt::addrinfo *raw;
	};

	class HostInfo : protected Collection<AddressInfo> {
	public:
		HostInfo();
		HostInfo(const std::string &Address, networking::FamilyType Family=networking::Family::Any, networking::ProtocolType protocol=networking::Protocol::TCP, std::string port="");
		virtual ~HostInfo();


		void Resolve(const std::string &Address, networking::FamilyType Family=networking::Family::Any, networking::ProtocolType Protocol=networking::Protocol::TCP, std::string Port="");
		void StartResolve(const std::string &Address, networking::FamilyType Family=networking::Family::Any, networking::ProtocolType Protocol=networking::Protocol::TCP, std::string Port="");

		void Resolve(const std::string &Address, networking::FamilyType Family, networking::ProtocolType Protocol, networking::PortNumber Port) {
			std::stringstream ss;
			ss<<Port;
			Resolve(Address, Family, Protocol, ss.str());
		}
		void StartResolve(const std::string &Address, networking::FamilyType Family, networking::ProtocolType Protocol, networking::PortNumber Port) {
			std::stringstream ss;
			ss<<Port;
			StartResolve(Address, Family, Protocol, ss.str());
		}

		operator bool();

		typedef IIterator<AddressInfo> myiterator;

		using Collection<AddressInfo>::getCount;
		using Collection<AddressInfo>::Search;
		using Collection<AddressInfo>::ReverseSearch;
		using Collection<AddressInfo>::operator [];
		using Collection<AddressInfo>::operator ();
		using Collection<AddressInfo>::operator myiterator*;
		using Collection<AddressInfo>::Destroy;

		gge::EventChain<HostInfo> ResolveComplete;
	protected:
		bool isresolving;
		prvt::addrinfo *raw;
	};

}
