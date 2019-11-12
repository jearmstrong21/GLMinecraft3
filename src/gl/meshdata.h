//
// Created by Jack Armstrong on 11/5/19.
//

#ifndef GLMINECRAFT3_MESHDATA_H
#define GLMINECRAFT3_MESHDATA_H

#include "gl/gl.h"

namespace gl {

    struct BufferData{
        int elemSize;
        std::vector<float>data;
    };

    struct MeshData {
        std::vector<BufferData>buffers;
        std::vector<unsigned int>tri;
    };

}

#endif //GLMINECRAFT3_MESHDATA_H
