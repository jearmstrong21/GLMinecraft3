#include "Networking.h"
#include "Private.h"

namespace socketlib { namespace networking {

	const ProtocolType Protocol::TCP=SOCK_STREAM;
	const ProtocolType Protocol::UDP=SOCK_DGRAM;


	ProtocolType Protocol::byName(std::string name) {
		protoent* pe=getprotobyname(name.c_str());
		if(pe)
			return (ProtocolType)pe->p_proto;
		else
			return (ProtocolType)0;
	}


	const FamilyType Family::Any		= AF_UNSPEC;
	const FamilyType Family::INET		= AF_INET;
	const FamilyType Family::INET6		= AF_INET6;



} }