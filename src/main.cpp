#include <iostream>

static void display_help(void)
{
    std::cout << "USAGE: ./zappy_gui -p port -h machine\n"; 
}

static int check_args(int argc, char **argv)
{
    if (argc != 5 || argv[0] == nullptr ||
        argv[1] == nullptr || argv[2] == nullptr ||
        argv[3] == nullptr || argv[4] == nullptr) {
            display_help();
            return 84;
        }
    if ((std::string(argv[1]) != "-p" && std::string(argv[1]) != "-h") ||
        (std::string(argv[3]) != "-p" && std::string(argv[3]) != "-h")) {
        display_help();
        return 84;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (check_args(argc, argv) == 84)
        return 84;
    return 0;
}