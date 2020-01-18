//
// Created by Jack Armstrong on 11/23/19.
//

#include "nbt.h"

namespace nbt {

    std::shared_ptr<nbt> read_nbt(std::istream &in) noexcept {
        int type;
        bin_read(in, &type);
        switch (type) {
            case nbt_type_short:
                return std::shared_ptr<nbt>(new nbt_short(in, false));
            case nbt_type_int:
                return std::shared_ptr<nbt>(new nbt_int(in, false));
            case nbt_type_long:
                return std::shared_ptr<nbt>(new nbt_long(in, false));
            case nbt_type_float:
                return std::shared_ptr<nbt>(new nbt_float(in, false));
            case nbt_type_string:
                return std::shared_ptr<nbt>(new nbt_string(in, false));
            case nbt_type_compound:
                return std::shared_ptr<nbt>(new nbt_compound(in, false));
            case nbt_type_list:
                return std::shared_ptr<nbt>(new nbt_list(in, false));
            default:
                return nullptr;
        }
    }

    nbt_compound_ptr cast_compound(nbt_ptr p) { return std::dynamic_pointer_cast<nbt_compound>(p); }

    nbt_ptr cast_compound(nbt_compound_ptr p) { return std::dynamic_pointer_cast<nbt>(p); }

    nbt_float_ptr cast_float(nbt_ptr p) { return std::dynamic_pointer_cast<nbt_float>(p); }

    nbt_ptr cast_float(nbt_float_ptr p) { return std::dynamic_pointer_cast<nbt>(p); }

    nbt_int_ptr cast_int(nbt_ptr p) { return std::dynamic_pointer_cast<nbt_int>(p); }

    nbt_ptr cast_int(nbt_int_ptr p) { return std::dynamic_pointer_cast<nbt>(p); }

    nbt_list_ptr cast_list(nbt_ptr p) { return std::dynamic_pointer_cast<nbt_list>(p); }

    nbt_ptr cast_list(nbt_list_ptr p) { return std::dynamic_pointer_cast<nbt>(p); }

    nbt_long_ptr cast_long(nbt_ptr p) { return std::dynamic_pointer_cast<nbt_long>(p); }

    nbt_ptr cast_long(nbt_long_ptr p) { return std::dynamic_pointer_cast<nbt>(p); }

    nbt_short_ptr cast_short(nbt_ptr p) { return std::dynamic_pointer_cast<nbt_short>(p); }

    nbt_ptr cast_short(nbt_short_ptr p) { return std::dynamic_pointer_cast<nbt>(p); }

    nbt_string_ptr cast_string(nbt_ptr p) { return std::dynamic_pointer_cast<nbt_string>(p); }

    nbt_ptr cast_string(nbt_string_ptr p) { return std::dynamic_pointer_cast<nbt>(p); }
}