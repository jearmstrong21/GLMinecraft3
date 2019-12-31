//
// Created by paladin on 12/14/19.
//

#ifndef GLMINECRAFT3_BOTH_H
#define GLMINECRAFT3_BOTH_H

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>

namespace networking {
    int client(const std::string &host, const std::string &port);

    int server(int port);
}

#endif //GLMINECRAFT3_BOTH_H
