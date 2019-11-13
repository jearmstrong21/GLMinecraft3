//
// Created by Jack Armstrong on 11/12/19.
//

#include "block_rendering.h"

namespace block {
    namespace client {


        void meshQuad(gl::MeshData* md, QuadDescr qd, int x, int y, int z) {
            ASSERT_OR_EXIT(md->buffers[0].elemSize == 3, "First buffer must have size 3");
            ASSERT_OR_EXIT(md->buffers[1].elemSize == 2, "Second buffer must have size 2");
            ASSERT_OR_EXIT(md->buffers[2].elemSize == 2, "Third buffer must have size 2");
            ASSERT_OR_EXIT(md->buffers[3].elemSize == 3, "Fourth buffer must have size 3");
            ASSERT_OR_EXIT(md->buffers[4].elemSize == 3, "Fifth buffer must have size 3");

            unsigned int n = md->buffers[0].data.size() / 3;

            float x0 = qd.start.x, y0 = qd.start.y, z0 = qd.start.z;
            float dx0 = qd.d0.x, dy0 = qd.d0.y, dz0 = qd.d0.z;
            float dx1 = qd.d1.x, dy1 = qd.d1.y, dz1 = qd.d1.z;

            x0 += x;
            y0 += y;
            z0 += z;

            //TODO: qd.texture.[first/second].color

            std::vector<float> pos = {x0, y0, z0, x0 + dx0, y0 + dy0, z0 + dz0, x0 + dx1, y0 + dy1, z0 + dz1,
                                      x0 + dx0 + dx1, y0 + dy0 + dy1, z0 + dz0 + dz1};

            glm::vec2 uv0 = qd.texture.first.getUV();
            glm::vec2 uv1 = qd.texture.second.getUV();
            float f = 1.0F / 32.0F;

//            uv0=glm::vec2(0,0);
//            uv1=glm::vec2(0,0);
//            f=1;

            std::vector<float> b_uv0 = {uv0.x, uv0.y, uv0.x + f, uv0.y, uv0.x, uv0.y + f, uv0.x + f, uv0.y + f};
            std::vector<float> b_uv1 = {uv1.x, uv1.y, uv1.x + f, uv1.y, uv1.x, uv1.y + f, uv1.x + f, uv1.y + f};
            std::vector<unsigned int> tri = {n, n + 1, n + 2, n + 1, n + 2, n + 3};

            for(float a:pos)md->buffers[0].data.push_back(a);
            for(float a:b_uv0)md->buffers[1].data.push_back(a);
            for(float a:b_uv1)md->buffers[2].data.push_back(a);
            for(unsigned int a:tri)md->tri.push_back(a);
            for(int i=0;i<4;i++){
                md->buffers[3].data.push_back(qd.texture.first.color.x);
                md->buffers[3].data.push_back(qd.texture.first.color.y);
                md->buffers[3].data.push_back(qd.texture.first.color.z);
                md->buffers[4].data.push_back(qd.texture.second.color.x);
                md->buffers[4].data.push_back(qd.texture.second.color.y);
                md->buffers[4].data.push_back(qd.texture.second.color.z);
            }

//            md->buffers[0].data.insert(md->buffers[0].data.end(), pos.begin(), pos.end());
//            md->buffers[1].data.insert(md->buffers[1].data.end(), b_uv0.begin(), b_uv0.end());
//            md->buffers[2].data.insert(md->buffers[2].data.end(), b_uv1.begin(), b_uv1.end());
//            md->tri.insert(md->tri.end(), tri.begin(), tri.end());

        }

        void initChunkBuffers(gl::MeshData*md){
            md->tri={};
            md->buffers.push_back({3,{}});
            md->buffers.push_back({2,{}});
            md->buffers.push_back({2,{}});
            md->buffers.push_back({3,{}});
            md->buffers.push_back({3,{}});
        }

        void QuadDescr::xmi() {
            start={0,0,0};
            d0={0,0,1};
            d1={0,1,0};
        }

        void QuadDescr::xpl() {
            start={1,0,0};
            d0={0,0,1};
            d1={0,1,0};
        }

        void QuadDescr::ymi() {
            start={0,0,0};
            d0={1,0,0};
            d1={0,0,1};
        }

        void QuadDescr::ypl() {
            start={0,1,0};
            d0={1,0,0};
            d1={0,0,1};
        }

        void QuadDescr::zmi() {
            start={0,0,0};
            d0={1,0,0};
            d1={0,1,0};
        }

        void QuadDescr::zpl() {
            start={0,0,1};
            d0={1,0,0};
            d1={0,1,0};
        }
    }
}