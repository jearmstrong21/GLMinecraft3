//
// Created by Jack Armstrong on 11/5/19.
//

#include "meshdata.h"

namespace gl {

    MeshData::MeshData() {

    }

    void MeshData::addBuffer(gl::BufferData buffer) {
        buffers.push_back(buffer);
    }

    const BufferData& MeshData::buffer(int attrib) {
        return buffers[attrib];
    }

    int MeshData::numBuffers() {
        return buffers.size();
    }

}