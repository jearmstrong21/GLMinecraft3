//
// Created by Jack Armstrong on 11/5/19.
//

#include "gl/mesh.h"

namespace gl {

    Mesh::Mesh(int vertCount,gl::MeshData data) {
        glGenVertexArrays(1,&vao);
        this->vertCount=vertCount;

        for(int i=0;i<data.numBuffers();i++){
            GLuint vbo;
            glGenBuffers(1,&vbo);

            glBindBuffer(GL_ARRAY_BUFFER,vbo);
            glBufferData(GL_ARRAY_BUFFER,data.buffer(i).data.size(),data.buffer(i).data.data(),GL_STATIC_DRAW);
            glVertexAttribPointer(i,data.buffer(i).elemSize,GL_FLOAT,false,data.buffer(i).elemSize*4,0);
            glEnableVertexAttribArray(i);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
        glBindVertexArray(0);
    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1,&vao);
    }

    void Mesh::renderTriangles() {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,0,vertCount);
    }

}