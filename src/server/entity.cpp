//
// Created by Jack Armstrong on 11/26/19.
//

#include <iostream>
#include "entity.h"

namespace server {

    std::shared_ptr<entity_type> get_entity_type(const std::shared_ptr<nbt::nbt>&data){
        int id=nbt::cast_int(nbt::cast_compound(data)->value["id"])->value;
//        switch(id){
//            case ENTITY_ID_ZOMBIE:
//                return ...
//            default:
//                return
//        }
        return std::shared_ptr<entity_type>(nullptr);
    }

    std::shared_ptr<nbt::nbt> entity_type_base::initialize() const {
        return nbt::make_compound({
            {"id",nbt::make_int(-1)},
            {"position",nbt::make_list({nbt::make_float(0),nbt::make_float(0),nbt::make_float(0)})},
            {"velocity",nbt::make_list({nbt::make_float(0),nbt::make_float(0),nbt::make_float(0)})},
            {"motion",nbt::make_list({nbt::make_float(0),nbt::make_float(0),nbt::make_float(0)})},
            {"rotation",nbt::make_list({nbt::make_float(0),nbt::make_float(0)})},
            {"bb_size",nbt::make_list({nbt::make_float(0),nbt::make_float(0),nbt::make_float(0)})},
            //todo health
        });
    }

    std::shared_ptr<nbt::nbt> entity_type_player::initialize() const {
        std::shared_ptr<nbt::nbt>res=entity_type_base::initialize();
        nbt::merge(res,nbt::make_compound({
            {"name",nbt::make_string("default name")},
            {"health",nbt::make_compound({
                {"amount",nbt::make_float(0)},
                {"max",nbt::make_float(0)}
            })}
        }));
        return res;
    }

    void entity_type_player::update(std::shared_ptr<nbt::nbt> data, server *server) const {
        std::cout<<"update player\n";
    }

    void entity_type_player::initialize_render() const {
        std::cout<<"init rendd play\n";
    }

    void entity_type_player::render(std::shared_ptr<nbt::nbt> entity) const {
        std::cout<<"rend play\n";
    }
}