//
// Created by Jack Armstrong on 11/23/19.
//

#include "__nbt_list.h"

namespace nbt {

    nbt_list::nbt_list():value{}{

    }
    nbt_list::nbt_list(std::istream&in,bool read_type){
        read(in,read_type);
    }
    nbt_list::nbt_list(std::vector<std::shared_ptr<nbt>>value){
        this->value=std::move(value);
    }
    nbt_list::~nbt_list()=default;
    void nbt_list::write(std::ostream&out)const{
        bin_write<int>(out,nbt_type_list);
        bin_write<int>(out,value.size());
        for(const auto & i : value){
            i->write(out);
        }
    }
    void nbt_list::read(std::istream&in,bool read_type){
        if(read_type)assert_next_int(in,nbt_type_list);
        int size;
        bin_read<int>(in,&size);
        value.clear();
        for(int i=0;i<size;i++){
            value.push_back(read_nbt(in));
        }
    }
    nbt_type nbt_list::type()const{
        return nbt_type_list;
    }
    std::string nbt_list::to_str(std::string indent)const{
        if(value.empty())return "[]";
        std::string s="[";
        s+=value[0]->to_str("");
        for(int i=1;i<value.size();i++){
            s+=", ";
            s+=value[i]->to_str("");
        }
        s+="]";
        return s;
    }

}