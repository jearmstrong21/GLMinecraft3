//
// Created by Jack Armstrong on 11/23/19.
//

#include "nbt.h"

namespace nbt {

    std::shared_ptr<nbt> read_nbt(std::istream&in){
        int type;
        bin_read(in,&type);
        switch(type){
            case nbt_type_short:
                return std::shared_ptr<nbt>(new nbt_short(in,false));
            case nbt_type_int:
                return std::shared_ptr<nbt>(new nbt_int(in,false));
            case nbt_type_long:
                return std::shared_ptr<nbt>(new nbt_long(in,false));
            case nbt_type_float:
                return std::shared_ptr<nbt>(new nbt_float(in,false));
            case nbt_type_string:
                return std::shared_ptr<nbt>(new nbt_string(in,false));
            case nbt_type_compound:
                return std::shared_ptr<nbt>(new nbt_compound(in,false));
            case nbt_type_list:
                return std::shared_ptr<nbt>(new nbt_list(in,false));
            default:
                return nullptr;
        }
    }

    void merge(std::shared_ptr<nbt>target,std::shared_ptr<nbt>extra){
        if(target->type()==nbt_type_compound&&extra->type()==nbt_type_compound){
            cast_compound(target)->value.insert(cast_compound(extra)->value.begin(),cast_compound(extra)->value.end());
        }
    }

}