//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3_NBT_H
#define GLMINECRAFT3_NBT_H


#include <ostream>
#include <istream>
#include <map>
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

    std::shared_ptr<nbt> make_short(short value);
    std::shared_ptr<nbt> make_int(int value);
    std::shared_ptr<nbt> make_float(float value);
    std::shared_ptr<nbt> make_long(long value);
    std::shared_ptr<nbt> make_string(std::string value);
    std::shared_ptr<nbt> make_compound(std::map<std::string,std::shared_ptr<nbt>>value);
    std::shared_ptr<nbt> make_list(std::vector<std::shared_ptr<nbt>>value);

}


#endif //GLMINECRAFT3_NBT_H
