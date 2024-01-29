#pragma once

#include "lexer.h"

#include <thread>
#include <mutex>
#include <condition_variable>

class LexerThreadSafe : public Lexer {
public:
    LexerThreadSafe(std::istream &in, std::mutex &in_m, std::condition_variable &in_cv)
        : Lexer(in)
        , m(in_m)
        , cv(in_cv)
        {}
    ~LexerThreadSafe() = default;

    void setStoped(bool v) override {
        std::lock_guard lk(m);
        stopped.store(v);
    };

protected:
    /** @brief Read commands from stream.
    *
    */
    bool readLine(std::string &str) override {
        str.clear();
        std::string tempStr;
        std::unique_lock lk(m);

        //wait data in stream
        while (!stopped.load() && !std::getline(c_in, tempStr)) {
            c_in.clear();
            cv.wait(lk, [this]{ return ready.load() || stopped.load(); });
            ready.store(false);
        }
        //wait '\n' in stream
        while (!stopped.load() && c_in.eof()) {
            str += tempStr;
            c_in.clear();
            cv.wait(lk, [this]{ return ready.load() || stopped.load(); });
            ready.store(false);
            if (!stopped.load()) {
                std::getline(c_in, tempStr);
            }
        }
        if (stopped.load()) {
            bool retVal = bool(std::getline(c_in, tempStr));
            str += tempStr;
            return retVal;
        }
        str += tempStr;
        return true;
    }
    
private:
    std::mutex &m;
    std::condition_variable &cv;
};