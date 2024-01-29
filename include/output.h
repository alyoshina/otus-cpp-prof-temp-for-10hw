#pragma once

#include <ostream>
#include <fstream>

class IOutput {
public:
    IOutput() {}
    virtual ~IOutput() = default;
    virtual void setName(const std::string& str) = 0;
    virtual IOutput& operator<<(const std::string& str) = 0;
    virtual IOutput& endl() = 0; 
    virtual bool presetting() = 0;
    virtual void postsetting() = 0;
    bool getStatus() const { return status; }
protected:
    bool status {true};
};

/** @brief Output command information to the console.
*
*/
class ConsoleOutput : public IOutput {
public:
    ConsoleOutput(std::ostream &o) : out(o) {}
    ~ConsoleOutput() = default;
    void setName(const std::string& ) override {}
    IOutput& operator<<(const std::string& str) override {
        out << str;
        return *this;
    }
    IOutput& endl() override {
        out << std::endl;
        return *this;
    }
    bool presetting() override { return true; }
    void postsetting() override {}
private:
    std::ostream& out;
};

/** @brief Output command information to the file.
*
*/
class FileOutput : public IOutput {
public:
    FileOutput() {}
    ~FileOutput() = default;
    void setName(const std::string& str) override { fileName = str; }
    IOutput& operator<<(const std::string& str) override {
        if (getStatus()) {
            out << str;
        }
        return *this;
    }
    IOutput& endl() override {
        if (getStatus()) {
            out << std::endl;
        }
        return *this;
    }
    bool presetting() override {
        out.open(fileName, std::ios::binary);
        return status = out.is_open();
    }
    void postsetting() override {
        if (getStatus()) {
            out.close();
        }
    }
private:
    std::string fileName;
    std::ofstream out;
};
