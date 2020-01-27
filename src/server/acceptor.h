//
// Created by Jack Armstrong on 12/29/19.
//

#ifndef GLMINECRAFT3_ACCEPTOR_H
#define GLMINECRAFT3_ACCEPTOR_H

#include <boost/asio.hpp>
#include <memory>
#include "game_room.h"

namespace server {

    struct game_room;

    struct acceptor {

        boost::asio::ip::tcp::acceptor acc;
        game_room room;

        void stop();

        void do_accept();

        acceptor(boost::asio::io_context &ioc, const boost::asio::ip::tcp::endpoint &endpoint);

    };

}

#endif //GLMINECRAFT3_ACCEPTOR_H
