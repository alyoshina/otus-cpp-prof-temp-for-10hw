#pragma once

#include "client.h"

class Server {
public:
    Server(ba::io_context& io_c, std::size_t b)
        : bulk(b)
        , ioContext(io_c)
        , holdStaticCmdsClient(b)
    {}
    ~Server() = default;
    void listen(uint16_t port) {
        ba::co_spawn(ioContext, startAccept(port), ba::detached);
    }
private:
    std::size_t bulk;
    ba::io_context& ioContext;
    std::shared_ptr<ICommands> sharedBulk;

    Client holdStaticCmdsClient;
    /** @brief  Initiates an asynchronous accept operation to wait for a new connection 
    */
    ba::awaitable<void> startAccept(uint16_t port) {
        const auto executor = co_await ba::this_coro::executor;
        tcp::acceptor acceptor{executor, {tcp::v4(), port}};
        for (;;) {
            auto socket = co_await acceptor.async_accept(ba::use_awaitable);
            ba::co_spawn(executor, workWithClient(std::move(socket)), ba::detached);
        }

        std::cout << std::this_thread::get_id() << " End _listen." << std::endl;
    }

    ba::awaitable<void> workWithClient(tcp::socket socket) {
        ba::streambuf buffer;
        std::cout << std::this_thread::get_id() << " New Client connetion." << std::endl;
        Client client(bulk);
        for (;;) {
            buffer.consume(buffer.size());
            try {
                co_await ba::async_read_until(socket, buffer, "\n", ba::use_awaitable);
            } catch (const boost::system::system_error& ex) {
                std::cout << " SOCKET END." << std::endl;
                if (ex.code() != ba::error::eof) {
                    throw;
                }
                break;
            }

            // const std::string_view logRecord{
            //     asio::buffer_cast<const char*>(buffer.data()), buffer.size()};

            // std::cout << std::this_thread::get_id() << " Client \"" << name
            //         << "\": New log record received." << std::endl;

            client.addData(ba::buffer_cast<const char*>(buffer.data()), buffer.size());
        }

        std::cout << std::this_thread::get_id() << " Client end connetion." << std::endl;
    }
};