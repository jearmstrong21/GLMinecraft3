#include "TCPSocketStreamBuffer.h"
#include "Private.h"

using namespace std;

namespace socketlib {

	void *socketstreambuffer_recvd(void *param) {
		TCPSocketStreamBuffer *sock=static_cast<TCPSocketStreamBuffer *>(param);

recall:
		bool shouldrecall=false;
		sock->Received(TCPSocketStreamBuffer::received_params(sock->gleft, shouldrecall));
		if(shouldrecall && sock->gleft>0)
			goto recall;

		sock->eventisrunning=false;

		return 0;
	}

	void *socketstreambuffer_read(void *param) {
		
		int recvd, maxlen;

		TCPSocketStreamBuffer *sock=static_cast<TCPSocketStreamBuffer *>(param);

		do {
			if(sock->sock==0)
				break;

#ifdef _MSC_VER
#	define _P_READ_PTR	greadnxt
#else
#	define _P_READ_PTR	_M_in_cur
#endif

			if(sock->_P_READ_PTR<=sock->gwritenxt) { //read pointer is behind us
				if(sock->gend-sock->gwritenxt>0)
					maxlen=sock->gend-sock->gwritenxt;
				else {
					if(sock->_P_READ_PTR!=sock->gbuffer.GetBuffer()) {
						sock->gwritenxt=sock->gbuffer.GetBuffer();
						continue;
					}
					else {
						pthread_mutex_lock  ( &sock->greadsignal );
						pthread_mutex_lock  ( &sock->greadsignal );
						pthread_mutex_unlock( &sock->greadsignal );
						continue;
					}
				}
					
			}
			else {
				if((sock->_P_READ_PTR-sock->gwritenxt)>1)
					maxlen=sock->_P_READ_PTR-sock->gwritenxt-1;
				else {
					pthread_mutex_lock  ( &sock->greadsignal );
					pthread_mutex_lock  ( &sock->greadsignal );
					pthread_mutex_unlock( &sock->greadsignal );
					continue;
				}
			}

#undef _P_READ_PTR

			fd_set readfds;
			timeval timeout;
			timeout.tv_sec=0;
			timeout.tv_usec=10000;

			FD_ZERO(&readfds);
			FD_SET(sock->sock, &readfds);

			select(sock->sock+1, &readfds, NULL, NULL, &timeout);

			if(!FD_ISSET(sock->sock, &readfds)) {
				recvd=1;
				continue;
			}
			else
				recvd=recv(sock->sock,sock->gwritenxt,maxlen,0);

			if(recvd<=0 || sock->sock==0)
				break;

			pthread_mutex_lock( &sock->gbufferlock );
			{
		 		sock->gwritenxt+=recvd;
				sock->gleft+=recvd;
#ifndef _MSC_VER
				sock->_M_in_end=sock->_M_in_cur+sock->gleft;
#endif

				if(sock->readrequester == TCPSocketStreamBuffer::norequester && !sock->eventisrunning) {
					sock->eventisrunning=true;
					pthread_create(&sock->eventthread, NULL, socketstreambuffer_recvd, sock);
				} else {
					sock->readrequester = TCPSocketStreamBuffer::norequester;
				}

				pthread_mutex_unlock( &sock->gwritesignal );
			} pthread_mutex_unlock( &sock->gbufferlock );
		} while(recvd>0);

		sock->closed();
		sock->readerisrunning=false;

		return 0;
	}

	TCPSocketStreamBuffer::TCPSocketStreamBuffer() : 
	sock(0), Disconnected("Disconnected", this),
	Received("Received", this)
	{
		init();
	}

	TCPSocketStreamBuffer::TCPSocketStreamBuffer(socketconnection sock) : 
	sock(sock), Disconnected("Disconnected", this),
	Received("Received", this)
	{
		init();

		sockaddr_storage addr; //!Check on win32
		socklen_t addrlen=sizeof(addr);
		char *ipstr;


		if(getpeername(sock, (struct sockaddr*)&addr, &addrlen)!=-1) {
#ifdef _WIN32
			ipstr=inet_ntoa( ((sockaddr_in*)&addr)->sin_addr );
#else
			ipstr=new char[INET6_ADDRSTRLEN];

			// deal with both IPv4 and IPv6:
			if (addr.ss_family == AF_INET) {
			    struct sockaddr_in *s = (struct sockaddr_in *)&addr;
			    inet_ntop(AF_INET, &s->sin_addr, ipstr, INET6_ADDRSTRLEN*sizeof(char));
			} else { // AF_INET6
			    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
			    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, INET6_ADDRSTRLEN*sizeof(char));
			}
#endif

			HostInfo h(ipstr);
			if(h)
				their=*h[0];

			h.Destroy();

		}

		readerisrunning=true;
		pthread_create( &reader, NULL, socketstreambuffer_read, this);
	}

	void TCPSocketStreamBuffer::init() {
		pthread_mutex_t empty=PTHREAD_MUTEX_INITIALIZER;
		gwritesignal=empty;
		greadsignal=empty;
		gbufferlock=empty;

		pbuffer.Resize(OutputBufferSize);
		pnxt=pbuffer.GetBuffer();
		pleft=OutputBufferSize;
		pend=pnxt+OutputBufferSize;

		gbuffer.Resize(InputBufferSize);
		greadnxt=gbuffer.GetBuffer();
		gwritenxt=gbuffer.GetBuffer();
		gend=greadnxt+InputBufferSize;
		gleft=0;

		readrequester=norequester;

		eventisrunning=false;
		readerisrunning=false;


#ifdef _MSC_VER
		_Init(&greadnxt,&greadnxt,&gleft, &pnxt,&pnxt,&pleft);
#else
		_M_in_beg=_M_in_cur=greadnxt;
		_M_in_end=gend;

		_M_out_beg=_M_out_cur=pnxt;
		_M_out_end=pend;
#endif

	}

	int TCPSocketStreamBuffer::overflow(int c) {
		if(!sock) {
			prvt::SetError(ENOTCONN);
			throw SocketException();
		}

		if(pleft==0) {
			sync();

			*pnxt=char_traits<char>::to_char_type(c);
			pnxt++;

			return char_traits<char>::not_eof(c);
		} else
			return streambuf::overflow(c);
	}

	int TCPSocketStreamBuffer::sync() {

#ifdef _MSC_VER
#	define _P_P_PTR	pnxt
#else
#	define _P_P_PTR	_M_out_cur
#endif

		if(_P_P_PTR==pbuffer.GetBuffer()) return 0;

		if(!sock) {
			return char_traits<char>::eof();
		}

		char *sending=pbuffer.GetBuffer();
		int total=_P_P_PTR-sending;
		while(total) {
			int length=send(sock, sending, total, 0);

			if(length==-1)
				throw SocketException();


			sending+=length;
			total=_P_P_PTR-sending;
		}

		_P_P_PTR = pbuffer.GetBuffer();
		pleft	 = pbuffer.GetSize();

		return pleft;
	}


	TCPSocketStreamBuffer::int_type TCPSocketStreamBuffer::underflow() {

#ifndef _MSC_VER
		greadnxt=_M_in_cur;
		gleft=_M_in_end-_M_in_cur;
#endif

		if(gleft)
			return char_traits<char>::to_int_type(*greadnxt);
		else {
			pthread_mutex_lock( &gbufferlock );
			{
				readrequester=syncedrequester;
				pthread_mutex_lock  ( &gwritesignal );
			} pthread_mutex_unlock( &gbufferlock );

			pthread_mutex_lock  ( &gwritesignal );
			pthread_mutex_unlock( &gwritesignal );

			if(gleft)
				return *greadnxt;
		}

		return char_traits<char>::eof();
	}

	bool TCPSocketStreamBuffer::Connect(std::string host, std::string port) {
		HostInfo h(host, networking::Family::Any, networking::Protocol::TCP, port);
		if(h)
			return Connect(*h[0]);
		else
			return false;
	}
	bool TCPSocketStreamBuffer::Connect(AddressInfo address) {
		if(address.GetRaw()==NULL) {
			throw SocketException("Invalid address definition");
		}
		sock=socket(address.GetRaw()->ai_family, address.GetRaw()->ai_socktype, address.GetRaw()->ai_protocol);
		if(sock==-1) {
			sock=0;

			throw SocketException();
		}


		int result=connect(sock, address.GetRaw()->ai_addr, address.GetRaw()->ai_addrlen);
		if(result==-1) {
			sock=0;

			throw SocketException();
		}


		sockaddr_storage addr; //!Check on win32
		char *ipstr;


#ifdef _WIN32
		ipstr=inet_ntoa( ((sockaddr_in*)&addr)->sin_addr );
#else
		ipstr=new char[INET6_ADDRSTRLEN];

		// deal with both IPv4 and IPv6:
		if (addr.ss_family == AF_INET) {
			struct sockaddr_in *s = (struct sockaddr_in *)&addr;
			inet_ntop(AF_INET, &s->sin_addr, ipstr, INET6_ADDRSTRLEN*sizeof(char));
		} else { // AF_INET6
			struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
			inet_ntop(AF_INET6, &s->sin6_addr, ipstr, INET6_ADDRSTRLEN*sizeof(char));
		}
#endif


		HostInfo h(ipstr);
		if(h)
			their=*h[0];

		h.Destroy();


		readerisrunning=true;
		pthread_create( &reader, NULL, socketstreambuffer_read, this);

		return true;
	}
	void TCPSocketStreamBuffer::Close() {
		if(!sock) {
			//seterrno(ENOTCONN);
			//throw SocketException();
			return;
		}


		if(pnxt!=pbuffer.GetBuffer())
			sync();

		closesocket(sock);
		sock=0;

		if(readerisrunning)
			pthread_join( reader, NULL );
	}

	void TCPSocketStreamBuffer::closed() {
		closesocket(sock);
		sock=0;

		Disconnected();
	}

	TCPSocketStreamBuffer::~TCPSocketStreamBuffer() {
		Close();

		if(readerisrunning)
			pthread_join( reader, NULL );

		pthread_mutex_unlock( &gwritesignal );
	}


	int TCPSocketStreamBuffer::InputBufferSize=4096;
	int TCPSocketStreamBuffer::OutputBufferSize=2048;

}
