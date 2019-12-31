//
// Created by paladin on 12/14/19.
//

#include <algorithm>
#include <iostream>
#include <boost/asio.hpp>
#include <nbt/__nbt_float.h>
#include <nbt/nbt.h>
#include "world/world.h"
#include "both.h"
#include <memory>
#include "entity/entity.h"
#include <boost/thread.hpp>
#include "server/server_player.h"
#include "server/game_room.h"
#include "server/session.h"
#include "server/acceptor.h"

namespace networking {

    int server(int port) {
        try {
            boost::asio::io_context io_context;
            boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

            server::acceptor the_server_operated_and_owned_jointly_by_joe_and_ben(io_context, endpoint);

            io_context.run();
        } catch (std::exception &e) {
            std::cerr << "server ex caught: " << e.what() << "\n";
        }

        return 0;
    }
}