#include "TCPServer.h"
#include "Private.h"

#pragma comment(lib, "SocketLib/pThreads/pthreadVC2.lib")

namespace socketlib {


    class server_connection_thread_params {
    public:
        server_connection_thread_params(TCPServer& server, TCPSocketStream& connection, AddressInfo addrinf) :
                server(server), connection(connection), addrinf(addrinf) {}

        TCPServer& server;
        TCPSocketStream& connection;
        AddressInfo addrinf;
    };

    void* server_connection_thread_start(void* arg) {

        server_connection_thread_params params = *static_cast<server_connection_thread_params*>(arg);

        params.server.ConnectionReceived(TCPServer::accept_params(params.connection, params.addrinf));

        delete &params;

        return NULL;
    }

    void* server_thread_start(void* arg) {
        TCPServer* server = static_cast<TCPServer*>(arg);
        TCPServer::Status st = server->state;
        server->state = TCPServer::Accepting;

        while (server->state == TCPServer::Accepting) {
            TCPSocketStream* ss = NULL;

            ss = server->accept(100);
            if (ss == NULL)
                continue;

            if (server->CallConnRcvedEvtInNThrd) {
                pthread_t evt;

                pthread_create(&evt, NULL, server_connection_thread_start,
                               new server_connection_thread_params(*server, *ss, ss->Destination()));
            } else
                server->ConnectionReceived(TCPServer::accept_params(*ss, ss->Destination()));
        }

        server->state = st;

        pthread_mutex_unlock(&server->isacceptthreadrunning);
        return NULL;
    }

    TCPServer::TCPServer() :
            ConnectionReceived("ConnectionReceived", this),
            ConnectionLost("ConnectionLost", this),

            INIT_PROPERTY(MaxConnQueue, TCPServer),
            INIT_PROPERTY(Family, TCPServer),

            family(networking::Family::Any),
            maxconnqueue(5),
            state(Idle), sock(0), CallConnRcvedEvtInNThrd(false) {
        pthread_mutex_t empty = PTHREAD_MUTEX_INITIALIZER;

        isacceptthreadrunning = empty;
    }

    TCPServer::~TCPServer() {
        StopListening();

        CloseAll();
    }

    void TCPServer::StopListening() {
        state = Idle;
        if (sock) {
            closesocket(sock);
            sock = 0;
        }

        pthread_mutex_lock(&isacceptthreadrunning);
        pthread_mutex_unlock(&isacceptthreadrunning);

        return;
    }

    void TCPServer::CloseAll() {
        foreach(TCPSocketStream, ss, Connections) {
            ss->Close();
        }
        Connections.Destroy();
    }

    TCPSocketStream& TCPServer::Accept(int Timeout) {
        TCPSocketStream* con = accept(Timeout);

        if (con == NULL && state == Idle) {
            prvt::SetError(ESHUTDOWN);
            throw SocketException();
        }

        if (con == NULL && Timeout > 0) {
            prvt::SetError(ETIMEDOUT);
            state = Idle;
            throw SocketException();
        }

        return *con;
    }

    TCPSocketStream* TCPServer::accept(int Timeout) {
        Status st = state;
        if (st != Accepting)
            state = BlockingAccept;

        if (Timeout != -1) {
            fd_set readfds;
            timeval timeout;
            timeout.tv_sec = Timeout / 1000;
            timeout.tv_usec = 1000 * (Timeout % 1000);

            FD_ZERO(&readfds);
            FD_SET(sock, &readfds);

            select(sock + 1, &readfds, NULL, NULL, &timeout);
            if (!FD_ISSET(sock, &readfds)) {
                return NULL;
            }
        }

        socketconnection newconn;
        sockaddr_storage their_addr;
        socklen_t addr_size;

        addr_size = sizeof their_addr;

        newconn = ::accept(sock, (sockaddr*) &their_addr, &addr_size);

        if (newconn == -1) {
            if (state == Idle) { //we are done
                return NULL;
            } else
                throw SocketException();
        }

        TCPSocketStream* ss = new TCPSocketStream(newconn);
        ss->Disconnected.Register(this, &TCPServer::child_disconnected);
        Connections.Add(ss);
        state = st;

        return ss;
    }

    void TCPServer::child_disconnected(TCPSocketStream& sock) {
        ConnectionLost(connlost_params(sock));
    }

    int TCPServer::LiveConnections() {
        int count = 0;
        foreach(TCPSocketStream, conn, Connections) {
            if (conn->isConnected())
                count++;
        }

        return count;
    }

    void TCPServer::StartAccept() {
        pthread_mutex_lock(&isacceptthreadrunning);
        pthread_create(&acceptthread, NULL, server_thread_start, this);
    }

    void TCPServer::Listen(std::string port) {
        if (state != Idle)
            throw SocketException("Server is already listening", EALREADY);

        ///*Obtain info
        addrinfo hints, * res;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = family;
        hints.ai_socktype = networking::Protocol::TCP;
        hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

        int status = getaddrinfo(NULL, port.c_str(), (::addrinfo*) &hints, (::addrinfo**) &res);

        if (status)
            throw SocketException(gai_strerror(status), status);
        if (!res) {
            prvt::SetError(EADDRNOTAVAIL);
            throw SocketException();
        }


        ///*Create socket
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sock == -1)
            throw SocketException();


        ///*Bind
        int yes = 1;
#ifdef _WIN32
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));
#else
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
#endif
        status = bind(sock, res->ai_addr, res->ai_addrlen);
        if (status == -1)
            throw SocketException();


        ///*Listen
        status = listen(sock, maxconnqueue);
        if (status == -1)
            throw SocketException();


        state = Listening;
    }

    void TCPServer::setFamily(networking::FamilyType family) {
        if (state == Idle)
            this->family = family;
        else
            throw SocketException("Server is already bound to a protocol", EALREADY);
    }

    networking::FamilyType TCPServer::getFamily() const {
        return family;
    }

    void TCPServer::setMaxConnQueue(int family) {
        if (state == Idle)
            this->maxconnqueue = family;
        else
            throw SocketException("Server is already bound to a protocol", EALREADY);
    }

    int TCPServer::getMaxConnQueue() const {
        return maxconnqueue;
    }

}
