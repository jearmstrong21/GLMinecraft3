//
// Created by Jack Armstrong on 11/26/19.
//

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
            {"bb_size",nbt::make_list({nbt::make_float(0),nbt::make_float(0),nbt::make_float(0)})}
        });
    }

    std::shared_ptr<nbt::nbt> entity_type_mob::initialize() const {
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

    std::shared_ptr<nbt::nbt> entity_type_zombie::initialize() const {
        std::shared_ptr<nbt::nbt>res = entity_type_mob::initialize();
        nbt::merge(res,nbt::make_compound({
            {"name",nbt::make_string("Zombie")},
            {"health",nbt::make_compound({
                {"amount",nbt::make_float(20)},
                {"max",nbt::make_float(20)}
            })}
        }));
        return res;
    }

    void entity_type_zombie::update(std::shared_ptr<nbt::nbt> data, server *server) const {

    }

    void entity_type_zombie::initialize_render() const {

    }

    void entity_type_zombie::render(std::shared_ptr<nbt::nbt> entity) const {

    }
}