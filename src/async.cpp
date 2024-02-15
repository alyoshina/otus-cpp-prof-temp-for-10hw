#include "async.h"

namespace {

MainThreads::MainThreads(std::size_t bulk) {
    //log
    list.push_back(std::make_shared<ConsoleOutputThread>(std::cout));
    //file1, file2
    list.push_back(std::make_shared<FileOutputThreads>());
    parserThread = std::make_shared<ParserThread>(list, bulk);
}

void MainThreads::stop() {
    parserThread->dataEnd();
    parserThread->join();
    for (auto io: list) {
        io->stop();
    }        
}
MainThreads::~MainThreads() {
    stop();     
}

} //namespace

namespace async {

handle_t connect(std::size_t bulk) {
    MainThreads *m = MainThreadsSingleton::getInstance(bulk);
    return static_cast<handle_t>(m);
}

void receive([[maybe_unused]] handle_t handle, const char *data, std::size_t size) {
    MainThreadsSingleton::addData(data, size);
}

void disconnect([[maybe_unused]] handle_t handle) {
}

} //async
