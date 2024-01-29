#include "async.h"

namespace {

MainThreads::MainThreads(std::size_t bulk) {
    //log
    list.push_back(std::make_shared<ConsoleOutputThread>(std::cout));
    //file1, file2
    list.push_back(std::make_shared<FileOutputThreads>());
    parserThread = std::make_shared<ParserThread>(list, bulk);
}
MainThreads::~MainThreads() {
    parserThread->dataEnd();
    parserThread->join();
    for (auto io: list) {
        io->stop();
    }        
}

} //namespace

namespace async {

handle_t connect(std::size_t bulk) {
    MainThreads *m = new MainThreads(bulk);
    return static_cast<handle_t>(m);
}

void receive(handle_t handle, const char *data, std::size_t size) {
    MainThreads *m = static_cast<MainThreads *>(handle);
    m->addData(data, size);
}

void disconnect(handle_t handle) {
    MainThreads *m = static_cast<MainThreads *>(handle);
    delete m;
}

} //async
