#include "server.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

/** @brief Server for package processing of commands.
*
*/
int main(int argc, char **argv) {
    //processing сommand line argument
    po::options_description desc {"Options"};
    desc.add_options()
            ("help,h", "Server for package processing of commands")
            ("port,p", po::value<unsigned short>() -> default_value(9000), "listening port")
            ("bulk,b", po::value<std::size_t>() -> default_value(3), "bulk size");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    try {
        ba::io_context ioContext;
        ba::signal_set signals{ioContext, SIGINT, SIGTERM};
        signals.async_wait([&](auto, auto) { ioContext.stop(); });

        //start server
        {
            Server s(ioContext, vm["bulk"].as<std::size_t>());
            s.listen(vm["port"].as<unsigned short>());
            ioContext.run();
        }

        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "Fatal error: unknown" << std::endl;
    }

    return EXIT_FAILURE;
}