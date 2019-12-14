#pragma once

#include "Definitions.h"

#ifdef _WIN32
#	include <winsock2.h>
#	include <Ws2tcpip.h>
#
#	define EALREADY			WSAEALREADY
#	define EADDRNOTAVAIL	WSAEADDRNOTAVAIL
#	define ETIMEDOUT        WSAETIMEDOUT
#	define ESHUTDOWN        WSAESHUTDOWN
#	define EISCONN          WSAEISCONN
#	define ENOTCONN         WSAENOTCONN
#
#	define seterrno			WSASetLastError

#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

#include <lmerr.h>


#else

#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <cerrno>
#include <unistd.h>

#

#	define closesocket    close

#endif


namespace socketlib {
    namespace prvt {

        struct addrinfo : ::addrinfo {
        };

    }
}
