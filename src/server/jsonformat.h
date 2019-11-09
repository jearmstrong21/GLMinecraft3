//
// Created by paladin on 11/8/19.
//

#ifndef GLMINECRAFT3_JSONFORMAT_H
#define GLMINECRAFT3_JSONFORMAT_H

#include <variant>
#include <string>
#include <type_traits>

namespace json {
    std::string jsonformat(std::string inp) {
        return '"' + inp + '"';
    }

    std::string jsonformat(int inp) {
        return std::to_string(inp);
    }

    template<class T>
    std::string jsonformat(T inp) {
        std::cerr << typeid(inp).name() << " is not convertible!" << std::endl;
        throw std::bad_cast();
    }
}

#endif //GLMINECRAFT3_JSONFORMAT_H
