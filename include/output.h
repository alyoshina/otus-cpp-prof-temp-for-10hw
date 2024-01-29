#pragma once

#include <ostream>
#include <fstream>
#include <memory>

struct Data {
    std::string name;
    std::string data;
};

class IOutput {
public:
    IOutput() {}
    virtual ~IOutput() = default;
    virtual void stop() {};
    virtual void addData(std::shared_ptr<Data> s) = 0;
};

/** @brief Output command information to the console.
*
*/
class ConsoleOutput : public IOutput {
public:
    ConsoleOutput(std::ostream &o) : out(o) {}
    ~ConsoleOutput() = default;
    void stop() override {}
    void addData(std::shared_ptr<Data> s) override {
        out << s->data;
        out.flush();
    }
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
    void stop() override {}
    void addData(std::shared_ptr<Data> s) override {
        out.open(s->name + std::string(".log"), std::ios::binary);
        if (out.is_open()) {
            out << s->data;
            out.close();
        }
    }
private:
    std::ofstream out;
};
