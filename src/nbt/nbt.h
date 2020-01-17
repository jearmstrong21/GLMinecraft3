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
#include <memory>

namespace nbt {

    nbt_ptr read_nbt(std::istream &in) noexcept;

    void merge(std::shared_ptr<nbt> target, std::shared_ptr<nbt> extra);

    nbt_compound_ptr cast_compound(nbt_ptr p);
    nbt_ptr cast_compound(nbt_compound_ptr p);
    nbt_float_ptr cast_float(nbt_ptr p);
    nbt_ptr cast_float(nbt_float_ptr p);
    nbt_int_ptr cast_int(nbt_ptr p);
    nbt_ptr cast_int(nbt_int_ptr p);
    nbt_list_ptr cast_list(nbt_ptr p);
    nbt_ptr cast_list(nbt_list_ptr p);
    nbt_long_ptr cast_long(nbt_ptr p);
    nbt_ptr cast_long(nbt_long_ptr p);
    nbt_short_ptr cast_short(nbt_ptr p);
    nbt_ptr cast_short(nbt_short_ptr p);
    nbt_string_ptr cast_string(nbt_ptr p);
    nbt_ptr cast_string(nbt_string_ptr p);

}


#endif //GLMINECRAFT3_NBT_H
