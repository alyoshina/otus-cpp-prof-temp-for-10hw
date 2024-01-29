#include "lexer.h"
#include "parser.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

/** @brief Program for package processing of commands.
*
* Processing сommand line argument, converting string received from console into a command
* and output it as bulk
*/
int main(int argc, char** argv) {
    //processing сommand line argument
    po::options_description desc {"Options"};
    desc.add_options()
            ("help,h", "Program for package processing of commands")
            ("bulk,b", po::value<std::size_t>() -> default_value(3), "bulk size");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    //command output methods
    std::list<std::shared_ptr<IOutput>> list;
    //log
    list.push_back(std::make_shared<ConsoleOutput>(std::cout));
    //file1, file2
    list.push_back(std::make_shared<FileOutput>());
    
    //converting string received from console into a command type
    auto lexer = std::make_shared<Lexer>(std::cin);

    //converting received command type into a command
    Parser parser(lexer, list, vm["bulk"].as<std::size_t>());
    while (parser.parse()) {
        ;
    }

    return 0;
}