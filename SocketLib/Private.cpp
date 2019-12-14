#include "Private.h"
#include <error.h>
#include <cstring>

namespace socketlib { namespace prvt {
	std::string
	GetErrorText(
		int lasterror
		)
	{
		std::string ret="";

#ifdef _WIN32
		HMODULE hModule = NULL; // default to system source
		LPSTR MessageBuffer;
		DWORD dwBufferLength;

		DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_FROM_SYSTEM ;

		//
		// If dwLastError is in the network range, 
		//  load the message source.
		//

		if(lasterror >= NERR_BASE && lasterror <= MAX_NERR) {
			hModule = LoadLibraryEx(
				TEXT("netmsg.dll"),
				NULL,
				LOAD_LIBRARY_AS_DATAFILE
				);

			if(hModule != NULL)
				dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
		}

		//
		// Call FormatMessage() to allow for message 
		//  text to be acquired from the system 
		//  or from the supplied module handle.
		//

		if(dwBufferLength = FormatMessageA(
			dwFormatFlags,
			hModule, // module to get message from (NULL == system)
			lasterror,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
			(LPSTR) &MessageBuffer,
			0,
			NULL
			))
		{
			
			ret=std::string(MessageBuffer,dwBufferLength);

			//
			// Free the buffer allocated by the system.
			//
			LocalFree(MessageBuffer);
		}

		//
		// If we loaded a message source, unload it.
		//
		if(hModule != NULL)
			FreeLibrary(hModule);
#else
		ret=strerror(lasterror);
#endif
		return ret;
	}

	int GetError() {
#ifdef _WIN32
			return WSAGetLastError();
#else
			return errno;
#endif
	}

	void SetError(int err) {
#ifdef _WIN32
		seterrno(err);
#else
		errno=err;
#endif

	}

} }
