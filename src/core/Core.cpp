#include <iostream>
#include "Core.hpp"

GUI::Core::Core(char **argv)
{
    for (int i = 1; i <= 3; ++i) {
        std::string arg = argv[i];
        if (arg == "-p") {
            try {
                _port = std::stoi(argv[i + 1]);
            } catch (const std::invalid_argument &) {
                throw CoreError("Invalid port: not a number");
            } catch (const std::out_of_range &) {
                throw CoreError("Invalid port: number out of range");
            }
        } else if (arg == "-h")
            _hostname = argv[i + 1];
    }

    if (_port == 0 || _hostname.empty())
        throw CoreError("Missing -p or -h argument");
}

void GUI::Core::run()
{
    std::cout << _port << " && " << _hostname;
}

int execute_zappygui(char **argv)
{
    try {
        GUI::Core core(argv);

        core.run();
    } catch (const GUI::Core::CoreError &error) {
        std::cerr << "Core error: " << error.what() << std::endl;
    }
    return 0;
}