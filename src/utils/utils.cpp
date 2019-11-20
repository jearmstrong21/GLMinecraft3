//
// Created by Jack Armstrong on 11/5/19.
//

#include "utils.h"

namespace utils {
    std::string load_file(std::string filename){
        std::ifstream ifs(filename);
        return std::string((std::istreambuf_iterator<char>(ifs) ),
                           (std::istreambuf_iterator<char>()    ));
//        return std::string(static_cast<std::stringstream const&>(std::stringstream()<<std::ifstream(filename).rdbuf()).str());
    }
}