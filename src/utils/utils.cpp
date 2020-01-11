//
// Created by Jack Armstrong on 1/8/20.
//

#include "utils.h"

namespace utils {

    float &getx(const std::shared_ptr<nbt::nbt> &list) {
        return nbt::cast_float(nbt::cast_list(list)->value[0])->value;
    }

    float &gety(const std::shared_ptr<nbt::nbt> &list) {
        return nbt::cast_float(nbt::cast_list(list)->value[1])->value;
    }

    float &getz(const std::shared_ptr<nbt::nbt> &list) {
        return nbt::cast_float(nbt::cast_list(list)->value[2])->value;
    }

    glm::vec3 cast3(const std::shared_ptr<nbt::nbt> &list) {
        return glm::vec3{nbt::cast_float(nbt::cast_list(list)->value[0])->value,
                         nbt::cast_float(nbt::cast_list(list)->value[1])->value,
                         nbt::cast_float(nbt::cast_list(list)->value[2])->value};
    }

    std::shared_ptr<nbt::nbt> cast3(glm::vec3 v) {
        return nbt::make_list({nbt::make_float(v.x), nbt::make_float(v.y), nbt::make_float(v.z)});
    }

    glm::vec2 cast2(const std::shared_ptr<nbt::nbt> &list) {
        return glm::vec2{nbt::cast_float(nbt::cast_list(list)->value[0])->value,
                         nbt::cast_float(nbt::cast_list(list)->value[1])->value};
    }

    std::shared_ptr<nbt::nbt> cast2(glm::vec2 v) {
        return nbt::make_list({nbt::make_float(v.x), nbt::make_float(v.y)});
    }

}