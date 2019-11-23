//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3___NBT_UTILS_H
#define GLMINECRAFT3___NBT_UTILS_H

#include <istream>
#include <ostream>

namespace nbt {

    enum nbt_type {
        nbt_type_short=0,
        nbt_type_int=1,
        nbt_type_long=2,
        nbt_type_float=3,
        nbt_type_string=4,
        nbt_type_compound=5,
        nbt_type_list=6,
//        nbt_type_none
    };

    void assert_next_int(std::istream &in, int expected);

    template<typename T>
    void bin_write(std::ostream &out, T t){
        out.write(reinterpret_cast<const char*>(&t),sizeof(T));
    }

    template<typename T>
    void bin_read(std::istream &in, T* t){
        in.read(reinterpret_cast<char*>(t),sizeof(T));
    }


}

#endif //GLMINECRAFT3___NBT_UTILS_H
