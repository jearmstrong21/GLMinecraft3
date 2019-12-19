//
// Created by paladin on 12/19/19.
//

#ifndef GLMINECRAFT3_CLIENT_WORLD_H
#define GLMINECRAFT3_CLIENT_WORLD_H

#include<functional>
#include <string>
#include <utils/eventable.h>

namespace client {
    struct client_world : public eventable<std::string> {

    };
}


#endif //GLMINECRAFT3_CLIENT_WORLD_H
