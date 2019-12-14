#pragma once

#include <string>
#include <stdexcept>

namespace socketlib { 
	

	typedef int socketconnection;

	
	
	namespace prvt {

		std::string GetErrorText(int);
		int GetError();
		void SetError(int);

		struct addrinfo;


	} 

}
