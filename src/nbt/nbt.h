//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3_NBT_H
#define GLMINECRAFT3_NBT_H


#include <ostream>
#include <istream>
#include <map>
#include <memory>
#include <functional>
#include "__nbt_utils.h"
#include "__nbt.h"
#include "__nbt_int.h"
#include "__nbt_float.h"
#include "__nbt_long.h"
#include "__nbt_string.h"
#include "__nbt_compound.h"
#include "__nbt_list.h"
#include "__nbt_short.h"

namespace nbt {

    std::shared_ptr<nbt> read_nbt(std::istream&in);

    template<typename T,typename V>
    std::shared_ptr<nbt> make(const V& value){
        return std::shared_ptr<nbt>(new T(value));
    }

    template<typename T>
    std::shared_ptr<T> cast(const std::shared_ptr<nbt>&value){
        return std::dynamic_pointer_cast<T>(value);
    }

    extern const std::function<std::shared_ptr<nbt>(const short&value)> make_short;
    extern const std::function<std::shared_ptr<nbt>(const int&value)> make_int;
    extern const std::function<std::shared_ptr<nbt>(const float&value)> make_float;
    extern const std::function<std::shared_ptr<nbt>(const long&value)> make_long;
    extern const std::function<std::shared_ptr<nbt>(const std::string&value)> make_string;
    extern const std::function<std::shared_ptr<nbt>(const std::map<std::string,std::shared_ptr<nbt>>&value)> make_compound;
    extern const std::function<std::shared_ptr<nbt>(const std::vector<std::shared_ptr<nbt>>&value)> make_list;

    extern const std::function<std::shared_ptr<nbt_short>(const std::shared_ptr<nbt>&value)> cast_short;
    extern const std::function<std::shared_ptr<nbt_int>(const std::shared_ptr<nbt>&value)> cast_int;
    extern const std::function<std::shared_ptr<nbt_float>(const std::shared_ptr<nbt>&value)> cast_float;
    extern const std::function<std::shared_ptr<nbt_long>(const std::shared_ptr<nbt>&value)> cast_long;
    extern const std::function<std::shared_ptr<nbt_string>(const std::shared_ptr<nbt>&value)> cast_string;
    extern const std::function<std::shared_ptr<nbt_compound>(const std::shared_ptr<nbt>&value)> cast_compound;
    extern const std::function<std::shared_ptr<nbt_list>(const std::shared_ptr<nbt>&value)> cast_list;


    void merge(std::shared_ptr<nbt>target,std::shared_ptr<nbt>extra);

}


#endif //GLMINECRAFT3_NBT_H
