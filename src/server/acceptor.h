//
// Created by Jack Armstrong on 12/29/19.
//

#ifndef GLMINECRAFT3_ACCEPTOR_H
#define GLMINECRAFT3_ACCEPTOR_H

#include <boost/asio.hpp>
#include <memory>

namespace server {

    struct acceptor {

        boost::asio::ip::tcp::acceptor acc;
        game_room room;

        void do_accept() {
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

        acceptor(boost::asio::io_context &ioc, const boost::asio::ip::tcp::endpoint &endpoint) : acc(ioc, endpoint),
                                                                                                 room(ioc) {
            do_accept();
        }

    };

}

#endif //GLMINECRAFT3_ACCEPTOR_H
