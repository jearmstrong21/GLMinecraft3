#pragma once

#include "Definitions.h"
#include <string>

#define SOCKET_NETWORKING_EXIST

namespace socketlib { namespace networking {

	typedef int ProtocolType;

	class Protocol {
	public:
		static const ProtocolType TCP;
		static const ProtocolType UDP;

		static ProtocolType byName(std::string name);

	};

	typedef int PortNumber;

	class Port {
	public:
		static const PortNumber ftp =21;
		static const PortNumber ssh =22;
		static const PortNumber pop3=23;
		static const PortNumber http=80;
		///!Include IANA port listing
	};

	typedef int FamilyType;

	class Family {
	public:
			static const FamilyType Any;
			static const FamilyType INET;
			static const FamilyType INET6;
	};

} }
