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

#define EXIT_WITH_ERROR(err) {printf("%s,%i: %s\n",__FILE__,__LINE__,err);exit(1);}
#define ASSERT_OR_EXIT(condition,err) {if(!(condition))EXIT_WITH_ERROR(err);}

namespace utils {
    std::string loadFile(std::string filename);
}

#endif //GLMINECRAFT3_UTILS_H
