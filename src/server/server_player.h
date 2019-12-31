//
// Created by Jack Armstrong on 12/29/19.
//

#ifndef GLMINECRAFT3_SERVER_PLAYER_H
#define GLMINECRAFT3_SERVER_PLAYER_H

namespace server {

    struct server_player {
        std::string entity_id;

        virtual ~server_player() = default;

        virtual void deliver(const std::shared_ptr<nbt::nbt> &msg) = 0;

        virtual void send_world(const block::world &world) = 0;
    };

    typedef std::shared_ptr<server_player> server_player_ptr;

}

#endif //GLMINECRAFT3_SERVER_PLAYER_H
