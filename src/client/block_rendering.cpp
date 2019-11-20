//
// Created by Jack Armstrong on 11/12/19.
//

#include "block_rendering.h"

namespace block {
    namespace client {


        void mesh_quad(gl::mesh_data* md, quad_descr qd, int x, int y, int z) {
            ASSERT_OR_EXIT(md->buffers[0].elem_size == 3, "First buffer must have size 3");
            ASSERT_OR_EXIT(md->buffers[1].elem_size == 2, "Second buffer must have size 2");
            ASSERT_OR_EXIT(md->buffers[2].elem_size == 2, "Third buffer must have size 2");
            ASSERT_OR_EXIT(md->buffers[3].elem_size == 3, "Fourth buffer must have size 3");
            ASSERT_OR_EXIT(md->buffers[4].elem_size == 3, "Fifth buffer must have size 3");

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

            glm::vec2 uv0 = qd.texture.first.get_uv();
            glm::vec2 uv1 = qd.texture.second.get_uv();
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
        }

        void init_chunk_buffers(gl::mesh_data*md){
            md->tri={};
            md->buffers.push_back({3,{}});
            md->buffers.push_back({2,{}});
            md->buffers.push_back({2,{}});
            md->buffers.push_back({3,{}});
            md->buffers.push_back({3,{}});
        }

        void quad_descr::xmi() {
            start={0,0,0};
            d0={0,0,1};
            d1={0,1,0};
        }

        void quad_descr::xpl() {
            start={1,0,0};
            d0={0,0,1};
            d1={0,1,0};
        }

        void quad_descr::ymi() {
            start={0,0,0};
            d0={1,0,0};
            d1={0,0,1};
        }

        void quad_descr::ypl() {
            start={0,1,0};
            d0={1,0,0};
            d1={0,0,1};
        }

        void quad_descr::zmi() {
            start={0,0,0};
            d0={1,0,0};
            d1={0,1,0};
        }

        void quad_descr::zpl() {
            start={0,0,1};
            d0={1,0,0};
            d1={0,1,0};
        }
    }
}