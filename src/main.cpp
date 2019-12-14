#include <iostream>
#include <csignal>
#include "staging/select_profile.h"
//#include <server/registry.h>

#include "gl/gl.h"
#include "gl/texture.h"
#include "gl/shader.h"
#include "gl/meshdata.h"
#include "gl/mesh.h"
#include "client/game.h"
#include <glm/glm.hpp>
#include <execinfo.h>
#include "server/server.h"
#include "../SocketLib/HostInfo.h"
#include "../SocketLib/TCPServer.h"

#define SIZE 100

void sig_handler(int sig) {
    //https://stackoverflow.com/questions/9207599/printing-stack-trace-from-a-signal-handler
    int j, nptrs;
    void* buffer[100];
    char** strings;

    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    free(strings);
    exit(sig);
}

static void on_glfw_error(int error, const char* description) {
    fprintf(stderr, "Error: %i,%s\n", error, description);
}

#ifdef PROFILE_GAME
int main() {
    std::signal(SIGSEGV, sig_handler);

    glfwSetErrorCallback(on_glfw_error);
    if (!glfwInit()) {
        printf("GLFW NOT INITIALIZED\n");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(500, 500, "GLMinecraft3", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    gl_check_error();

    bool is_first_frame = true;

    client::game game(window);
    game.initialize();

    while (!glfwWindowShouldClose(window)) {
        glGetError();

        game.loop();

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (is_first_frame) {
            glfwSetWindowPos(window, 150, 150);
            is_first_frame = false;
        }

        gl_check_error();
    }
    game.end();
    glfwTerminate();

    return 0;
}
#endif

#ifdef PROFILE_NETWORKING

void received(socketlib::TCPSocketStream::received_params params,
              socketlib::TCPSocketStream &socket) {

    int cnt=params.available;
    if(cnt>1024) {
        cnt=1024;
    }
    char data[1024];

    socket.read(data, cnt);
    std::cout.write (data, cnt);

    params.shouldrecall=true;
    //If data in the buffer is larger than 1k
    //this event handler will be called again
}

void disconnected() {
    std::cout<<"Disconnected."<<std::endl;
}

void connect(socketlib::TCPServer::accept_params params) {
    std::cout << "Connection received from " << params.addrinfo.IPAddress() << std::endl;
    params.socket << "Hello" << std::endl;
    params.socket.Close();
}

int main(int argc, char** argv) {
    std::signal(SIGSEGV, sig_handler);
    if (argv[2][0] == 'c') {
        std::cout << "Starting client." << std::endl;
        socketlib::TCPSocketStream client;

        client.Received.Register(&received);
        client.Disconnected.Register(&disconnected);
        std::cout << "Connecting to port " << argv[1] << std::endl;
        client.Connect("127.0.0.1", std::string(argv[1]));
    } else {
        std::cout << "Starting server" << std::endl;
        socketlib::TCPServer server;
        server.Listen(std::string(argv[1]));
        server.ConnectionReceived.Register(&connect);
        server.StartAccept();
        std::cout << "Accepting on port " << argv[1] << std::endl;
    }

    std::cin.sync();
    std::cin.ignore(1);

    return 0;
}

#endif