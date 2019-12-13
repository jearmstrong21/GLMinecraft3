#pragma once

#include "Definitions.h"
#include <iostream>

#define SOCKET_EXCEPTION_EXIST


namespace socketlib {

	class SocketException : public std::runtime_error {
	public:
		SocketException(const std::string &msg, int socket_errno=-1) : runtime_error(msg), str(msg) {
			this->socket_errno=socket_errno;
		}

		SocketException() : runtime_error("") {
			socket_errno=prvt::GetError();

			str=prvt::GetErrorText(socket_errno);
		}

		virtual const char *what() const throw() {
			return (str.c_str());
		}

		int errorno() { return socket_errno; }

		~SocketException() throw() { }

	protected:
		int socket_errno;
		std::string str;

	};

}
