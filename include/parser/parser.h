#pragma once

#include "lexer.h"
#include "output_thread.h"
#include "commands.h"

#include <memory>

/** @brief Convert received command type into a command.
*
*/
class Parser {
public:
    Parser(std::shared_ptr<Lexer> l, std::list<std::shared_ptr<IOutput>>& list
            , const std::size_t count)
        : lexer(l)
        , oList(list)
        , bulk(std::make_shared<Commands>(count, oList))
        , cmdsCount(count)
        {}
    virtual ~Parser() = default;
    Lexer::CmdType nextCmd() { return lexer->nextLine(); }
    bool parse() { return expr(); }

    /** @brief Creates a command according to received type.
    * Commands are stored in dinamic or size bulks
    * @return false if end
    */
    bool expr();
    void setStoped(bool v) {
        lexer->setStoped(v);
    }
    void setDataReady(bool v) { lexer->setDataReady(v); };
protected:
    virtual std::shared_ptr<ICommands> getStaticBulk() {
        return std::make_shared<Commands>(cmdsCount, oList);
    }
    void setBulk(std::shared_ptr<ICommands> sptr) { bulk = sptr; }
    virtual void deleteBulk() { bulk = nullptr; }
private:
    std::shared_ptr<Lexer> lexer;
    std::list<std::shared_ptr<IOutput>> oList;
    std::shared_ptr<ICommands> bulk;
    const std::size_t cmdsCount;
};