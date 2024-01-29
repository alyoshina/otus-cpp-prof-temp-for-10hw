#pragma once

#include <iostream>
#include <unordered_map>

/** @brief Converting string received from console into a command type
*
*/
class Lexer {
public:
    Lexer(std::istream &in) : c_in(in) {}
    ~Lexer() = default;
    const std::string &getCmd() const { return strCmd; }

    /** @enum CmdType
    * @brief Command type
    */
    enum class CmdType {
        Cmd, /**< is command */
        Lbrace, /**< is left brace "{" */
        Rbrace, /**< is right brace "}"*/
        End, /**< is input end */
    };

    bool end() const { return c_in.eof(); }

    /** @brief Read line from console and convert into a command type
    * @return command type
    */
    CmdType nextLine() {
        if (!std::getline(c_in, strCmd)) {
            return CmdType::End;
        }
        if (сmdType.contains(strCmd)) {
            return сmdType[strCmd];
        }
        return CmdType::Cmd;
    }
public:
    std::istream &c_in;
    std::string strCmd;
    std::unordered_map<std::string, CmdType> сmdType {
        {"{", CmdType::Lbrace},
        {"}", CmdType::Rbrace},
        {"End", CmdType::End}
    };
};