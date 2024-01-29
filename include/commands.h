#pragma once

#include "output.h"

#include <string>
#include <list>
#include <memory>
#include <algorithm>
#include <chrono>
#include <ostream>
#include <typeinfo>
#include <format>
#include <fstream>

class ICmd {
public:
    ICmd(const std::string &strCmd)
    : name(strCmd)
    , createTime(std::chrono::system_clock::now()) {}
    virtual ~ICmd() = default;
    virtual void exec() = 0;
    auto getCreateTime() const { return createTime; }
    auto getCreateTimeSec() const {
        return std::chrono::floor<std::chrono::seconds>
                                (getCreateTime().time_since_epoch()).count();
    }
    const std::string &getName() const { return name; }
private:
    std::string name;
    const std::chrono::time_point<std::chrono::system_clock> createTime;
};

/** @brief Command
*/
class Cmd : public ICmd {
public:
    Cmd(const std::string& strCmd) : ICmd(strCmd) {}
    ~Cmd() = default;
    void exec() override {}
};

/** @brief For bulk
*/
class ICommands : public ICmd {
public:
    ICommands(const std::string& strCmd, std::list<std::shared_ptr<IOutput>>& list)
        : ICmd(strCmd)
        , oList(list) {}
    virtual ~ICommands() = default;
    /** @brief Executes (command output) rest commands in bulk
    *
    */
    void virtual execRest() = 0;
    bool virtual isDinamic() { return false; }
    void virtual addLbrace() {}
    void virtual addRbrace() {}
    bool virtual isEnd() const { return false; }
    /** @brief Executes (command output) commands in bulk
    *
    */
    void execInAnyCase();
    void addCmd(std::shared_ptr<ICmd> cmd) { cmds.emplace_back(cmd); }
protected:
    std::list<std::shared_ptr<ICmd>> cmds; /**< is list of commands for execute*/
    std::list<std::shared_ptr<IOutput>> oList; /**< is list of objects used for output*/
};

/** @brief Size bulk
*/
class Commands : public ICommands {
public:
    Commands(const std::size_t count, std::list<std::shared_ptr<IOutput>>& list)
        : ICommands(std::string("Commands"), list), n(count) {}
    ~Commands() { execRest(); };
    void execRest() override { execInAnyCase(); }
    void exec() override {
        if (cmds.size() == n) {
            execInAnyCase();
        }
    }
private:
    const std::size_t n; /**< is bulk size */
};

/** @brief Dinamic bulk
*/
class DinamicCommands : public ICommands {
public:
    DinamicCommands(std::list<std::shared_ptr<IOutput>>& list)
        : ICommands(std::string("DinamicCommands"), list) {}
    ~DinamicCommands() = default;
    void execRest() override {}
    bool isDinamic() override { return true; }
    bool isEnd() const override { return lbraceCount == 0; }
    void addLbrace() override { lbraceCount++; }
    void addRbrace() override { if (lbraceCount > 0) lbraceCount--; }
    void exec() override {
        if (isEnd()) {
            execInAnyCase();
        }
    }
private:
    std::size_t lbraceCount{0}; /**< is '{', '}' brace count */
};