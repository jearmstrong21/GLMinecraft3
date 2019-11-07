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


    const char* vertexShaderSource="#version 410 core\n"
                                   "layout (location=0) in vec3 aPos;\n"
                                   "void main(){\n"
                                   "    gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
                                   "}\n\0";

    const char* fragmentShaderSource="#version 410 core\n"
                                     "out vec4 FragColor;\n"
                                     "void main(){\n"
                                     "  FragColor=vec4(1.0,0.5,0.2,1.0);\n"
                                     "}\n\0";

    int vertexShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,nullptr);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertexShader,512,nullptr,infoLog);
        printf("VERTEX\n%s\n",infoLog);
    }

    int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512,nullptr,infoLog);
        printf("FRAGMENT\n%s\n",infoLog);
    }

    int shaderProgram=glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(shaderProgram,512,nullptr,infoLog);
        printf("LINK\n%s\n",infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

        glUseProgram(shaderProgram);
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