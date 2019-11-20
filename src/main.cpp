#include <iostream>
#include <csignal>
//#include <server/registry.h>

#include "gl/gl.h"
#include "gl/texture.h"
#include "gl/shader.h"
#include "gl/meshdata.h"
#include "gl/mesh.h"
#include "client/game.h"
#include <glm/glm.hpp>
#include <execinfo.h>

#define SIZE 100

void sig_handler(int sig) {
    //https://stackoverflow.com/questions/9207599/printing-stack-trace-from-a-signal-handler
    int j, nptrs;
    void *buffer[100];
    char **strings;

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