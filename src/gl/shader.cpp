//
// Created by Jack Armstrong on 11/5/19.
//

#include "gl/shader.h"

namespace gl {

    shader::shader(const unsigned char *vertData, const int vertLen, const unsigned char *fragData, const int fragLen) {
        int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        auto vd = reinterpret_cast<const GLchar *>(vertData);
//        printf("<%s>\n",vd);
        glShaderSource(vertex_shader, 1, &vd, &vertLen);
        glCompileShader(vertex_shader);
        int success;
        char info_log[512];
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
            printf("VERTEX\n\n%s\n", info_log);
            ASSERT_OR_EXIT(false, "VERTEX SHADER FAILED");
        }

        int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        auto fd = reinterpret_cast<const GLchar *>(fragData);
//        fd[fragLen-1]='\0';
        glShaderSource(fragment_shader, 1, &fd, &fragLen);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
            printf("FRAGMENT%s:\n\n%s\n", fd, info_log);
            ASSERT_OR_EXIT(false, "FRAGMENT SHADER FAILED");
        }

        id = glCreateProgram();
        glAttachShader(id, vertex_shader);
        glAttachShader(id, fragment_shader);
        glLinkProgram(id);
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id, 512, nullptr, info_log);
            printf("\n\n%s\n", info_log);
            ASSERT_OR_EXIT(false, "SHADER LINK FAILED");
        }
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    shader::~shader() {
        glDeleteProgram(id);
    }

    void shader::bind() {
        if (!glIsProgram(id)) {
            printf("ERROR %i IS NOT PROGRAM %s,%i\n", id, __FILE__, __LINE__);
        }
        glUseProgram(id);
    }

    int shader::program_id() {
        return id;
    }

    void shader::uniform4x4(const std::string &name, glm::mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    void shader::uniform3(const std::string &name, glm::vec3 value) {
        glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }

    void shader::uniform2(const std::string &name, glm::vec2 value) {
        glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
    }

    void shader::texture(const std::string &name, gl::texture *texture, int unit) {
        texture->bind(unit);
        glUniform1i(glGetUniformLocation(id, name.c_str()), unit);
    }

    void shader::uniform1(const std::string &name, float value) {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

}