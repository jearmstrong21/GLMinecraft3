//
// Created by Jack Armstrong on 1/16/20.
//

#ifndef GLMINECRAFT3_AABB_H
#define GLMINECRAFT3_AABB_H

#include <glm/glm.hpp>
#include <functional>

namespace entity {

    struct aabb {

        glm::vec3 pos, size;

        glm::vec3 min() {
            return pos + size * glm::vec3{-0.5, 0, -0.5};
        }

        glm::vec3 max() {
            return pos + size * glm::vec3{0.5, 1, 0.5};
        }

        glm::ivec3 imin() {
            return glm::ivec3(min());
        }

        glm::ivec3 imax() {
            return glm::ivec3(max());
        }

        typedef std::function<void(glm::ivec3)> point_process;

        void foreach(const point_process &func) {
            glm::ivec3 min = imin();
            glm::ivec3 max = imax();
            for (int x = min.x; x <= max.x; x++) {
                for (int y = min.y; y <= max.y; y++) {
                    for (int z = min.z; z <= max.z; z++) {
                        func({x, y, z});
                    }
                }
            }
        }

    };

}

#endif //GLMINECRAFT3_AABB_H
