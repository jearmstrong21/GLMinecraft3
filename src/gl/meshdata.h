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

    class MeshData {

    private:
        std::vector<BufferData>buffers;

    public:
        MeshData();

        void addBuffer(BufferData buffer);
        const BufferData& buffer(int attrib);
        int numBuffers();

    };

}

#endif //GLMINECRAFT3_MESHDATA_H
