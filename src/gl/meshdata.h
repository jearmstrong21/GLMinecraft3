//
// Created by Jack Armstrong on 11/5/19.
//

#ifndef GLMINECRAFT3_MESHDATA_H
#define GLMINECRAFT3_MESHDATA_H

#include "gl/gl.h"

namespace gl {

    struct buffer_data{
        int elem_size;
        std::vector<float>data;
    };

    struct mesh_data {
        std::vector<buffer_data>buffers;
        std::vector<unsigned int>tri;
    };

}

#endif //GLMINECRAFT3_MESHDATA_H
