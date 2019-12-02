//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3_NBT_H
#define GLMINECRAFT3_NBT_H


#include <ostream>
#include <istream>
#include <map>
#include <memory>
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

    const auto& make_short=make<nbt_short,short>;
    const auto& make_int=make<nbt_int,int>;
    const auto& make_float=make<nbt_float,float>;
    const auto& make_long=make<nbt_long,long>;
    const auto& make_string=make<nbt_string,std::string>;
    const auto& make_compound=make<nbt_compound,std::map<std::string,std::shared_ptr<nbt>>>;
    const auto& make_list=make<nbt_list,std::vector<std::shared_ptr<nbt>>>;


    template<typename T>
    std::shared_ptr<T> cast(const std::shared_ptr<nbt>&value){
        return std::shared_ptr<T>((T*)value.get());
    }

    const auto& cast_short=cast<nbt_short>;
    const auto& cast_int=cast<nbt_int>;
    const auto& cast_float=cast<nbt_float>;
    const auto& cast_long=cast<nbt_long>;
    const auto& cast_string=cast<nbt_string>;
    const auto& cast_compound=cast<nbt_compound>;
    const auto& cast_list=cast<nbt_list>;

    void merge(std::shared_ptr<nbt>target,std::shared_ptr<nbt>extra);

}


#endif //GLMINECRAFT3_NBT_H
