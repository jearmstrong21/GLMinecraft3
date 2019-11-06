//
// Created by Jack Armstrong on 11/5/19.
//

#include "gl/shader.h"

namespace gl {

    Shader::Shader(const std::string& vert, const std::string& frag) {
//        const char*vertCode=utils::loadFile(vert).c_str();
        const char*vertCode=std::string(
                            "#version 410 core"
                            "layout (location=0) in vec2 inPos;"
                            "void main(){gl_Position=vec4(inPos,0,1);}"
        ).c_str();
//        const char*fragCode=("#version 410 core\n"+utils::loadFile(frag)).c_str();
        const char*fragCode=std::string(
                "#version 410 core\n"
                "        out vec4 fc;"

        "void main(){"
            "            fc=vec4(1,0,0,1);"
        "}"
                ).c_str();

        printf("[%s]\n",fragCode);

        GLuint vertID=glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertID,1,&vertCode,nullptr);
        glCompileShader(vertID);
        int vertSuccess;
        char vertInfoLog[1024];
        glGetShaderiv(vertID,GL_COMPILE_STATUS,&vertSuccess);
        if(!vertSuccess){
            glGetShaderInfoLog(vertID,1024,nullptr,vertInfoLog);
            printf("ERROR VERTEX SHADER %s: %s\n",vert.c_str(),vertInfoLog);
        }else{
            printf("COMPILED VERT %s\n",vert.c_str());
        }

        GLuint fragID=glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragID,1,&fragCode,nullptr);
        glCompileShader(fragID);
        int fragSuccess;
        char fragInfoLog[1024];
        glGetShaderiv(fragID,GL_COMPILE_STATUS,&fragSuccess);
        if(!fragSuccess){
            glGetShaderInfoLog(fragID,1024,nullptr,fragInfoLog);
            printf("ERROR FRAGMENT SHADER %s: %s\n",frag.c_str(),fragInfoLog);
        }else{
            printf("COMPILED FRAG %s\n",frag.c_str());
        }

        id=glCreateProgram();
        glAttachShader(id,vertID);
        glAttachShader(id,fragID);
        glLinkProgram(id);
        int linkSuccess;
        char linkInfoLog[1024];
        glGetProgramiv(id,GL_LINK_STATUS,&linkSuccess);
        if(!linkSuccess){
            glGetProgramInfoLog(id,1024,nullptr,linkInfoLog);
            printf("ERROR LINKING FRAG[%s],VERT[%s]: %s\n",frag.c_str(),vert.c_str(),linkInfoLog);
        }else{
            printf("LINKED FRAG[%s],VERT[%s]\nVERT:%s\nFRAG:%s\n",frag.c_str(),vert.c_str(),vertCode,fragCode);
        }
        glDeleteShader(vertID);
        glDeleteShader(fragID);
    }

    Shader::~Shader() {
        glDeleteProgram(id);
    }

    void Shader::bind() {
        glUseProgram(id);
    }

}