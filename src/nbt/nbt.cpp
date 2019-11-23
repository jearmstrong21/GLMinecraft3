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

    std::shared_ptr<nbt> make_short(short value){
        return std::shared_ptr<nbt>(new nbt_short(value));
    }
    std::shared_ptr<nbt> make_int(int value){
        return std::shared_ptr<nbt>(new nbt_int(value));
    }
    std::shared_ptr<nbt> make_float(float value){
        return std::shared_ptr<nbt>(new nbt_float(value));
    }
    std::shared_ptr<nbt> make_long(long value){
        return std::shared_ptr<nbt>(new nbt_long(value));
    }
    std::shared_ptr<nbt> make_string(std::string value){
        return std::shared_ptr<nbt>(new nbt_string(value));
    }
    std::shared_ptr<nbt> make_compound(std::map<std::string,std::shared_ptr<nbt>>value){
        return std::shared_ptr<nbt>(new nbt_compound(value));
    }
    std::shared_ptr<nbt> make_list(std::vector<std::shared_ptr<nbt>>value){
        return std::shared_ptr<nbt>(new nbt_list(value));
    }

}