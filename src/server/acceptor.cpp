//
// Created by Jack Armstrong on 1/17/20.
//

#include <iostream>
#include "acceptor.h"
#include "session.h"

namespace server {

    void acceptor::do_accept() {
        if(game_room::instance->game_loop_is_over)return;
        acc.async_accept([this](boost::system::error_code err, boost::asio::ip::tcp::socket socket) {
            std::cout << "async_accept\n ";
            if (!err) {
                std::cout << "\t!err\n";
                std::make_shared<session>(std::move(socket), room)->start();
            } else {
                std::cout << "\terr " << err << "\n";
            }
            do_accept();
        });
    }

    acceptor::acceptor(boost::asio::io_context &ioc, const boost::asio::ip::tcp::endpoint &endpoint) : acc(ioc,
                                                                                                           endpoint),
                                                                                                       room(ioc) {
        do_accept();
    }
}