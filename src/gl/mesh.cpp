//
// Created by Jack Armstrong on 11/5/19.
//

#include "gl/mesh.h"

namespace gl {

    mesh::mesh(gl::mesh_data* data) {
        this->vert_count=data->tri.size();

        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);

        std::vector<unsigned int> tri_vec=data->tri;
        unsigned int*t=tri_vec.data();

        glGenBuffers(1,&ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tri_vec.size(), t, GL_STATIC_DRAW);

        for(int i=0;i<data->buffers.size();i++){
            std::vector<float>float_vec=data->buffers[i].data;
            float*v=float_vec.data();
            GLuint vbo;
            glGenBuffers(1,&vbo);
            glBindBuffer(GL_ARRAY_BUFFER,vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * float_vec.size(), v, GL_STATIC_DRAW);
            glVertexAttribPointer(i, data->buffers[i].elem_size, GL_FLOAT, GL_FALSE, data->buffers[i].elem_size * sizeof(float), (void*)0);
            glEnableVertexAttribArray(i);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }

        glBindVertexArray(0);
    }

    mesh::~mesh() {
        glDeleteVertexArrays(1,&vao);
    }

    void mesh::render_triangles() {
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glDrawElements(GL_TRIANGLES, vert_count, GL_UNSIGNED_INT, 0);
    }

    int mesh::vao_id() {
        return vao;
    }

    int mesh::vert_num() {
        return vert_count;
    }

}