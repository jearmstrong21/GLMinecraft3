//
// Created by Jack Armstrong on 11/5/19.
//

#ifndef GLMINECRAFT3_MESH_H
#define GLMINECRAFT3_MESH_H

#include "gl/gl.h"
#include "gl/meshdata.h"

namespace gl {

    class Mesh {
    private:
        GLuint vao;
        GLuint ebo;
        int vertCount;

    public:
        Mesh(MeshData data);
        ~Mesh();

        void renderTriangles();
        int vaoID();
        int vertNum();
    };

}

#endif //GLMINECRAFT3_MESH_H
