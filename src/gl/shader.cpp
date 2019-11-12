//
// Created by Jack Armstrong on 11/5/19.
//

#include "gl/shader.h"

namespace gl {

    Shader::Shader(const std::string& vert, const std::string& frag) {
        std::string vertCode=utils::loadFile("../src/assets/shaders/"+vert+".vert");
        std::string fragCode=utils::loadFile("../src/assets/shaders/"+frag+".frag");
        const char*vertexShaderSource=vertCode.c_str();
        const char*fragmentShaderSource=fragCode.c_str();
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

        id=glCreateProgram();
        glAttachShader(id,vertexShader);
        glAttachShader(id,fragmentShader);
        glLinkProgram(id);
        glGetProgramiv(id,GL_LINK_STATUS,&success);
        if(!success){
            glGetProgramInfoLog(id,512,nullptr,infoLog);
            printf("LINK\n%s\n",infoLog);
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader() {
        glDeleteProgram(id);
    }

    void Shader::bind() {
        if(!glIsProgram(id)){
            printf("ERROR %i IS NOT PROGRAM %s,%i\n",id,__FILE__,__LINE__);
        }
        glUseProgram(id);
    }

    int Shader::programID() {
        return id;
    }

    void Shader::uniform4x4(const std::string& name, glm::mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(id,name.c_str()),1,GL_FALSE,&value[0][0]);
    }

    void Shader::texture(const std::string &name, std::shared_ptr<gl::Texture> texture, int unit) {
        texture->bind(unit);
        glUniform1i(glGetUniformLocation(id,name.c_str()),unit);
    }

}