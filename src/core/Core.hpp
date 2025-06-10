#pragma once

#include <iostream>
#include <string>

int execute_zappygui(char **argv);

namespace GUI {
    class Core {
        public:
            class CoreError : public std::exception {
                public:
                    CoreError(const std::string &msg) : _msg(msg) {};
                    [[nodiscard]] const char *what() const noexcept override
                    {
                        return this->_msg.c_str();
                    }

                private:
                    std::string _msg;
            };
            
            Core(char **argv);
            ~Core();

            void run();

        private:
            bool connect_to_server();
            bool authenticate();
            void handle_server_message(const std::string& message);
            void send_command(const std::string& command);

            int _server_fd;
            bool _connected;
            std::string _input_buffer;
            
            int _port = 0;
            std::string _hostname;
            
            std::size_t _timeUnit = 0;
            std::size_t _time = 0;
    };
} // namespace GUI