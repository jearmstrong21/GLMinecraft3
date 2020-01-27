//
// Created by Jack Armstrong on 1/17/20.
//

#include <iostream>
#include "acceptor.h"
#include "session.h"

namespace server {

    void acceptor::stop() {
        acc.cancel();
    }

    void acceptor::do_accept() {
        if (game_room::instance->game_loop_is_over) {
            return;
        }
        acc.async_accept([this](boost::system::error_code err, boost::asio::ip::tcp::socket socket) {
            if (!err) {
                std::make_shared<session>(std::move(socket), room)->start();
            } else {
                if (game_room::instance->game_loop_is_over)return;
                std::cout << "ERROR IN ACCEPTOR: " << err << "\n";
            }
            do_accept();
        });
    }

    acceptor::acceptor(boost::asio::io_context &ioc, const boost::asio::ip::tcp::endpoint &endpoint) : acc(ioc,
                                                                                                           endpoint),
                                                                                                       room(ioc, this) {
        do_accept();
    }
}