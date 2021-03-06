//
// Created by Jack Armstrong on 11/5/19.
//

#ifndef GLMINECRAFT3_UTILS_H
#define GLMINECRAFT3_UTILS_H

#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <glm/glm.hpp>
#include "noise/noise.h"
#include <algorithm>
#include <thread>
#include <vector>
#include <nbt/nbt.h>
//#include <chronos>
#include <csignal>

#define RAISE_ERROR(err){printf("RAISE_ERROR %s,%i: %s\n",__FILE__,__LINE__,err);std::raise(11);}
#define EXIT_WITH_ERROR(err) {printf("EXIT_WITH_ERROR %s,%i: %s\n",__FILE__,__LINE__,err);exit(1);}
#define ASSERT_OR_EXIT(condition, err) {if(!(condition))EXIT_WITH_ERROR(err);}
#define TRANSIENT
#define DATA
#define TICKS

namespace utils {

    //TODO: let every parameter have different type

    template<typename T>
    T lerp(T amount, T a, T b) {
        return a + (b - a) * amount;
    }

    template<typename T>
    T norm(T value, T a, T b) {
        return (value - a) / (b - a);
    }

    template<typename T>
    T map(T value, T s1, T e1, T s2, T e2) {
        return lerp(norm(value, s1, e1), s2, e2);
    }

    template<typename T>
    T clamp(T value, T a, T b) {
        if (value < a)return a;
        if (value > b)return b;
        return value;
    }

    template<typename T>
    T min(T a, T b) {
        return a < b ? a : b;
    }

    template<typename T>
    T max(T a, T b) {
        return a > b ? a : b;
    }

//    namespace

    float &getx(const std::shared_ptr<nbt::nbt> &list);

    float &gety(const std::shared_ptr<nbt::nbt> &list);

    float &getz(const std::shared_ptr<nbt::nbt> &list);

    glm::vec3 cast3(const std::shared_ptr<nbt::nbt> &list);

    std::shared_ptr<nbt::nbt> cast3(glm::vec3 v);

    glm::vec2 cast2(const std::shared_ptr<nbt::nbt> &list);

    std::shared_ptr<nbt::nbt> cast2(glm::vec2 v);

}

#endif //GLMINECRAFT3_UTILS_H
