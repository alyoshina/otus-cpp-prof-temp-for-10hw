#include <parser.h>

bool Parser::expr() {
    bool retVal = true;
    switch (nextCmd()) {
        case Lexer::CmdType::Cmd:
            bulk->addCmd(std::make_shared<Cmd>(lexer->getCmd()));
            bulk->exec();
        break;
        case Lexer::CmdType::Lbrace:
            if (!bulk->isDinamic()) {
                bulk = std::make_shared<DinamicCommands>(oList);
            }
            bulk->addLbrace();
            break;
        case Lexer::CmdType::Rbrace:
            bulk->addRbrace();
            bulk->exec();
            if (bulk->isEnd()) {
                bulk = std::make_shared<Commands>(cmdsCount, oList);
            }
            break;
        case Lexer::CmdType::End:
            bulk = nullptr;
            retVal = false;
            break;
        default:
            retVal = false;
        break;
    }
    return retVal;
}