//
// Created by Jack Armstrong on 11/5/19.
//

#include "gl/mesh.h"

namespace gl {

    Mesh::Mesh(int vertCount,gl::MeshData data) {
//        this->vertCount=vertCount;



    }

    Mesh::~Mesh() {
        glDeleteVertexArrays(1,&vao);
    }

    void Mesh::renderTriangles() {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    }

    int Mesh::vaoID() {
        return vao;
    }

    int Mesh::vertNum() {
        return vertCount;
    }

}