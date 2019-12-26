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
//#include <chronos>

#define EXIT_WITH_ERROR(err) {printf("EXIT_WITH_ERROR %s,%i: %s\n",__FILE__,__LINE__,err);exit(1);}
#define ASSERT_OR_EXIT(condition,err) {if(!(condition))EXIT_WITH_ERROR(err);}

namespace utils {
    std::string load_file(std::string filename);
}

#endif //GLMINECRAFT3_UTILS_H
