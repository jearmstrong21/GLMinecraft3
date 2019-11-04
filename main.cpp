#include <iostream>

#include "gl.h"

void sig_handler(int sig){
    printf("Oh noes: signal %i was sent.\n",sig);
    exit(EXIT_FAILURE);
}

static void on_glfw_error(int error, const char* description)
{
    fprintf(stderr, "Error: %i,%s\n", error, description);
}

int main() {
    signal(SIGSEGV,sig_handler);

    glfwSetErrorCallback(on_glfw_error);
    if(!glfwInit()){
        printf("GLFW NOT INITIALIZED\n");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window=glfwCreateWindow(500,500,"GLMinecraft3",NULL,NULL);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    gl_check_error();

    bool is_first_frame=true;

    while(!glfwWindowShouldClose(window)){
        glGetError();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_FRAMEBUFFER_SRGB);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if(is_first_frame){
            glfwSetWindowPos(window,150,150);
            is_first_frame=false;
        }

        gl_check_error();
    }
    glfwTerminate();

    return 0;
}