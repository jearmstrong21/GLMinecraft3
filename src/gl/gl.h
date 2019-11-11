//
// Created by Jack Armstrong on 3/23/19.
//

#ifndef GLMC_GL_H
#define GLMC_GL_H

#include "glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "utils/utils.h"
#include <map>
#include <vector>
#include <glm/glm.hpp>

void _gl_check_error(const char*file,int line);
#define gl_check_error() _gl_check_error(__FILE__,__LINE__)

#endif //GLMC_GL_H
