#pragma once

#include "parser_thread.h"

namespace {

class MainThreads {
public:
    MainThreads(std::size_t bulk);
    ~MainThreads();
    void addData(const char *data, std::size_t size) {
        parserThread->addData(data, size);
    }
private:
    std::shared_ptr<ParserThread> parserThread;
    std::list<std::shared_ptr<IOutput>> list; /**< command output methods */
};

} //namespace

namespace async {

using handle_t = void *;

handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void disconnect(handle_t handle);

} //async
