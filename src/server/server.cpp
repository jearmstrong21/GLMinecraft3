//
// Created by Jack Armstrong on 12/19/19.
//

#include "server.h"

namespace server {

    void server::setup_server(){
        world=std::make_shared<block::world>();
        entities=std::make_shared<entity_manager>();
    }

}