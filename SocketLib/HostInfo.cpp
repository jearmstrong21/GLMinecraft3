#include "HostInfo.h"
#include "Private.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#include <sstream>

namespace socketlib {


#ifdef _WIN32
#	ifndef BYTE
#		define BYTE	unsigned char
#	endif

	union IP4AddressConverter {
		struct { BYTE A, B, C, D; };
		unsigned long integer;
	};
#endif

	void AddressInfo::init(prvt::addrinfo *raw) {
		void *addr;
		this->raw=raw;

		if (raw->ai_family == AF_INET) { // IPv4
			sockaddr_in *ipv4 = (struct sockaddr_in *)raw->ai_addr;
			addr = &(ipv4->sin_addr);
			family = networking::Family::INET;

#ifdef _WIN32
			IP4AddressConverter c;
			c.integer=ipv4->sin_addr.S_un.S_addr;

			stringstream ss;
			ss<<(int)c.A<<"."<<(int)c.B<<"."<<(int)c.C<<"."<<(int)c.D;
			
			ipaddress=ss.str();
#endif

		} else { // IPv6
			sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)raw->ai_addr;
			addr = &(ipv6->sin6_addr);
			family = networking::Family::INET6;

			ipaddress="";
		}

#ifndef _WIN32
		char addressbuffer[INET6_ADDRSTRLEN];
		inet_ntop(raw->ai_family, addr, addressbuffer, sizeof addressbuffer);
		ipaddress=addressbuffer;
#endif
	}

	bool AddressInfo::operator ==(AddressInfo &addr) { return ipaddress==addr.ipaddress; }
	bool AddressInfo::operator !=(AddressInfo &addr) { return ipaddress!=addr.ipaddress; }


	HostInfo::HostInfo() : raw(NULL), ResolveComplete("ResolveComplete", this) {}
	HostInfo::HostInfo(const std::string &Address, networking::FamilyType Family, networking::ProtocolType protocol, std::string port) : 
	raw(NULL), ResolveComplete("ResolveComplete", this) {
		Resolve(Address,Family,protocol,port);
	}
	HostInfo::~HostInfo() { freeaddrinfo(raw); }


	void HostInfo::Resolve(const std::string &Address, networking::FamilyType Family, networking::ProtocolType protocol, std::string port) {
		if(raw) {
			freeaddrinfo(raw);
			raw=NULL;
		}

		Destroy();
		
		addrinfo hints, *p;

		memset(&hints, 0, sizeof hints);
		hints.ai_family = Family; // AF_INET or AF_INET6 to force version
		hints.ai_socktype = protocol;


		if(Address[0]!=':')
			Address.c_str();
		int status=getaddrinfo(Address.c_str(), port.c_str(), (::addrinfo*)&hints, (::addrinfo**)&raw);
		if (status != 0) {
			if(status==HOST_NOT_FOUND)
				return;
			else {
				throw SocketException(gai_strerror(status),status);
			}
		}

		if(raw==NULL) return;

		for(p = raw;p != NULL; p = p->ai_next) {
			Add(new AddressInfo(raw));
		}
	}

	struct resolverdata {
		std::string Address;
		networking::FamilyType Family;
		networking::ProtocolType protocol;
		std::string port;
		HostInfo *host;
	};

	void *host_resolve(void *params) {
		resolverdata *r = static_cast<resolverdata*>(params);

		r->host->Resolve(r->Address, r->Family, r->protocol, r->port);

		r->host->ResolveComplete();

		return 0;
	}

	void HostInfo::StartResolve(const std::string &Address, networking::FamilyType Family, networking::ProtocolType protocol, std::string port) {
		pthread_t resolver;
		resolverdata *r=new resolverdata;
		r->Address=Address;
		r->Family=Family;
		r->protocol=protocol;
		r->port=port;
		r->host=this;
		pthread_create( &resolver, NULL, &host_resolve, r );
	}

	HostInfo::operator bool() { return getCount()>0; }



	namespace {
		class SocketInit {
		public:
			SocketInit() {
#ifdef _WIN32
				WSADATA wsaData;   // if this doesn't work
				//WSAData wsaData; // then try this instead

				// MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0:

				if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
					SocketException se;
					throw se;
				}
#endif
			}
		};


		SocketInit s;
	}

}
