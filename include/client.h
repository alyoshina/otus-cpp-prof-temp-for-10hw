#pragma once

#include "async.h"

#include <boost/algorithm/string/trim_all.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/streambuf.hpp>

#include <iostream>
#include <thread>

namespace ba = boost::asio;
using tcp = ba::ip::tcp;

class Client {
public:
    Client(std::size_t bulk) : h(async::connect(bulk)) {}
    ~Client() { async::disconnect(h); }
    void addData(const char *data, std::size_t size) { async::receive(h, data, size); }
private:
    async::handle_t h;
};