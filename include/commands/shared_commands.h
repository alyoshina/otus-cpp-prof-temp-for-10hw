#pragma once

#include "commands.h"

#include <mutex>
#include <iostream>

/** @brief Size bulk with shared list of commands
*/
class SharedCommands : public ICommands {
public:
    SharedCommands(const std::size_t count, std::list<std::shared_ptr<IOutput>>& list)
        : ICommands(std::string("SharedCommands"), list), n(count) {
            {
                std::lock_guard lk(objCountMutex);
                objCount++;
            }            
        }
    ~SharedCommands() {
        {
            std::lock_guard lk(objCountMutex);
            if (!(--objCount)) {
                execRest();
            }
        }
    };
    void execRest() override { execInAnyCase(); }
    void exec() override {
        std::lock_guard lk(sharedCmdsMutex);
        if (getCmdList().size() == n) {
            ICommands::execInAnyCase();
        }
    }
    void addCmd(std::shared_ptr<ICmd> cmd) override {
        std::lock_guard lk(sharedCmdsMutex);
        getCmdList().emplace_back(cmd);
    }
    void execInAnyCase() override {
        std::lock_guard lk(sharedCmdsMutex);
        ICommands::execInAnyCase();
    }
protected:
    static std::list<std::shared_ptr<ICmd>> sharedCmds; /**< is shared list of commands for execute*/
    std::list<std::shared_ptr<ICmd>>& getCmdList() override { return sharedCmds; }
private:
    const std::size_t n; /**< is bulk size */
    static std::mutex sharedCmdsMutex;
    static std::mutex objCountMutex;
    static std::size_t objCount; /**< execute remaining commands from the list (sharedCmds) if the last SharedCommands object (objCount==0) is deleted */
};