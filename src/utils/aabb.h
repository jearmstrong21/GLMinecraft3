//
// Created by Jack Armstrong on 1/16/20.
//

#ifndef GLMINECRAFT3_AABB_H
#define GLMINECRAFT3_AABB_H

#include <glm/glm.hpp>
#include <functional>
#include "nbt/nbt.h"
#include "utils/utils.h"

namespace utils {

    struct aabb {

        glm::vec3 pos, size;

        [[nodiscard]] glm::vec3 min() const {
            return pos + size * glm::vec3{-0.5, 0, -0.5};
        }

        [[nodiscard]] glm::vec3 max() const {
            return pos + size * glm::vec3{0.5, 1, 0.5};
        }

        [[nodiscard]] glm::ivec3 imin() const {
            return glm::ivec3(min());
        }

        [[nodiscard]] glm::ivec3 imax() const {
            return glm::ivec3(max());
        }

        typedef std::function<void(glm::ivec3)> point_process;

        void foreach(const point_process &func) const {
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

        void save(const nbt::nbt_compound_ptr &tag) const {
            tag->value["pos"] = utils::cast3(pos);
            tag->value["size"] = utils::cast3(size);
        }

        void load(const nbt::nbt_compound_ptr &tag) {
            pos = utils::cast3(tag->value["pos"]);
            size = utils::cast3(tag->value["size"]);
        }

    };

}

#endif //GLMINECRAFT3_AABB_H
