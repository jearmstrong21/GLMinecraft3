#include <iostream>

#include "gl/gl.h"
#include "gl/texture.h"
#include "gl/shader.h"
#include "gl/meshdata.h"
#include "gl/mesh.h"

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

    gl::Shader shader("../shaders/test.vert","../shaders/test.frag");

    float vertices[]={
            .5,.5,0,
            .5,-.5,0,
            -.5,-.5,0,
            -.5,.5,0
    };

    unsigned int indices[]={
            0,1,3,
            1,2,3
    };

    unsigned int vbo,vao,ebo;

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)){
        glGetError();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.2,0.3,0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

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