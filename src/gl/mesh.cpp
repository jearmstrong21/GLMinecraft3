//
// Created by Jack Armstrong on 11/5/19.
//

#include "gl/mesh.h"

namespace gl {

    Mesh::Mesh(gl::MeshData data) {
        this->vertCount=data.vertCount;

        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);

        std::vector<unsigned int> triVec=data.tri;
        unsigned int*t=triVec.data();

        glGenBuffers(1,&ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*triVec.size(),t,GL_STATIC_DRAW);

        for(int i=0;i<data.buffers.size();i++){
            std::vector<float>floatVec=data.buffers[i].data;
            float*v=floatVec.data();
            GLuint vbo;
            glGenBuffers(1,&vbo);
            glBindBuffer(GL_ARRAY_BUFFER,vbo);
            glBufferData(GL_ARRAY_BUFFER,sizeof(float)*floatVec.size(),v,GL_STATIC_DRAW);
            glVertexAttribPointer(i,data.buffers[i].elemSize,GL_FLOAT,GL_FALSE,data.buffers[i].elemSize*sizeof(float),(void*)0);
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
        glDrawElements(GL_TRIANGLES,vertCount,GL_UNSIGNED_INT,0);
    }

    int Mesh::vaoID() {
        return vao;
    }

    int Mesh::vertNum() {
        return vertCount;
    }

}