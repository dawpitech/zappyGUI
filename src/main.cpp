/**
 * @file main.cpp
 * @brief Main entry point for the ZappyGUI application
 * @author EPITECH PROJECT, 2025
 * @date 2025
 *
 * This file contains the main function and argument validation for the ZappyGUI
 * application. It handles command-line argument parsing and validation before
 * launching the core application.
 *
 * The program expects exactly 4 command-line arguments:
 * - Port specification: -p <port_number>
 * - Machine specification: -h <hostname/IP>
 *
 * Usage: ./zappy_gui -p port -h machine
 */

#include <iostream>
#include "core/Core.hpp"
#include "macros.hpp"

/**
 * @brief Displays the usage help message
 *
 * Prints the correct usage format for the ZappyGUI application to standard output.
 * This function is called when invalid arguments are provided or when help is needed.
 *
 * The expected format is: ./zappy_gui -p port -h machine
 * Where:
 * - -p: Specifies the port number to connect to
 * - -h: Specifies the hostname or IP address of the machine to connect to
 */
static void display_help(void)
{
    std::cout << "USAGE: ./zappy_gui -p port -h machine\n";
}

/**
 * @brief Validates command-line arguments
 *
 * Performs comprehensive validation of the command-line arguments to ensure
 * they meet the required format and constraints. The function checks:
 *
 * 1. Argument count: Must be exactly 5 (program name + 4 arguments)
 * 2. Null pointer validation: Ensures no argument is null
 * 3. Flag validation: Ensures arguments contain both -p and -h flags
 * 4. Flag positioning: Validates that flags are in correct positions (argv[1] and argv[3])
 *
 * Valid argument patterns:
 * - ./zappy_gui -p <port> -h <machine>
 * - ./zappy_gui -h <machine> -p <port>
 *
 * @param argc The number of command-line arguments
 * @param argv Array of command-line argument strings
 *
 * @return int Returns 0 if arguments are valid, 84 if invalid
 *
 * @note The function returns 84 (EPITECH error code) for any validation failure
 * @note This function calls display_help() when validation fails
 */
static int check_args(int argc, char **argv)
{
    if (argc != NB_ARGS || argv[0] == nullptr ||
        argv[1] == nullptr || argv[2] == nullptr ||
        argv[3] == nullptr || argv[4] == nullptr) {
            display_help();
            return EPITECH_FAILURE;
        }
    if ((std::string(argv[1]) != "-p" && std::string(argv[1]) != "-h") ||
        (std::string(argv[3]) != "-p" && std::string(argv[3]) != "-h")) {
        display_help();
        return EPITECH_FAILURE;
    }
    return EPITECH_SUCCESS;
}

/**
 * @brief Main entry point of the ZappyGUI application
 *
 * The main function serves as the entry point for the ZappyGUI application.
 * It performs the following operations in sequence:
 *
 * 1. Validates command-line arguments using check_args()
 * 2. If validation fails, returns with error code 84
 * 3. If validation succeeds, delegates execution to execute_zappygui()
 *
 * The function expects exactly 4 command-line arguments in addition to the
 * program name, specifying connection parameters for the Zappy server.
 *
 * @param argc The number of command-line arguments (must be 5)
 * @param argv Array of command-line argument strings containing:
 *             - argv[0]: Program name
 *             - argv[1]: Either "-p" or "-h" flag
 *             - argv[2]: Port number or hostname/IP
 *             - argv[3]: Either "-h" or "-p" flag (opposite of argv[1])
 *             - argv[4]: Hostname/IP or port number (opposite of argv[2])
 *
 * @return int Returns the exit code from execute_zappygui() if successful,
 *             or 84 if argument validation fails
 *
 * @note The function uses EPITECH's standard error code 84 for argument validation failures
 * @note Actual application logic is delegated to execute_zappygui() function from Core.hpp
 *
 * @see execute_zappygui() for the main application logic
 * @see check_args() for argument validation details
 * @see display_help() for usage information
 */
int main(int argc, char **argv)
{
    if (check_args(argc, argv) == 84)
        return EPITECH_FAILURE;
    return execute_zappygui(argv);
}
