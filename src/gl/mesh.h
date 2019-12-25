//
// Created by Jack Armstrong on 11/5/19.
//

#ifndef GLMINECRAFT3_MESH_H
#define GLMINECRAFT3_MESH_H

#include "gl/gl.h"
#include "gl/meshdata.h"

namespace gl {

    class mesh {
    private:
        GLuint vao;
        GLuint ebo;
        int vert_count;

    public:
        mesh(mesh_data* data);
        ~mesh();

        void render_triangles();
        void render_lines();
        int vao_id();
        int vert_num();
    };

}

#endif //GLMINECRAFT3_MESH_H
