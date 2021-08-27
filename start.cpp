#include <iostream>
#include <boost/program_options.hpp>

#include "handler/manager.h"
#include "tools/cplogger.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char** argv) {
    bool is_dedicated_server = false;

    Logger::init_log();
    Logger::i("Started car-pet server.");

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("dedicate_server,d", "open server as dedicated server")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
    if (vm.count("dedicate_server")) is_dedicated_server = true;

    auto manager = Manager(is_dedicated_server);

    manager.start_listen_for();

    return 0;
}
