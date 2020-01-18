//
// Created by Jack Armstrong on 12/29/19.
//

#ifndef GLMINECRAFT3_SESSION_H
#define GLMINECRAFT3_SESSION_H

#include <boost/asio.hpp>
#include "game_room.h"
#include <memory>
#include "server_player.h"

namespace server {

    struct session : server_player, std::enable_shared_from_this<session> {
        boost::asio::ip::tcp::socket socket;
        game_room &room;

        session(boost::asio::ip::tcp::socket socket, game_room &room);

        ~session() override = default;

        void start();

        void deliver(const std::shared_ptr<nbt::nbt> &msg) override;

        void send_world(const block::world &world) override;
    };


}

#endif //GLMINECRAFT3_SESSION_H
