#pragma once

#include "parser.h"
#include "shared_commands.h"

class SharedBulkForStaticCmdParser : public Parser {
public:
    SharedBulkForStaticCmdParser(std::shared_ptr<Lexer> l
                , std::list<std::shared_ptr<IOutput>>& list
                , const std::size_t count)
            : Parser(l, list, count)
            , sharedBulk(std::make_shared<SharedCommands>(count, list)) {
        setBulk(getStaticBulk());
    }
    ~SharedBulkForStaticCmdParser() = default;
protected:
    std::shared_ptr<ICommands> getStaticBulk() override {
        return sharedBulk;
    }
    void deleteBulk() override {
        Parser::deleteBulk();
        sharedBulk.reset();
    }

private:
    std::shared_ptr<ICommands> sharedBulk;
};