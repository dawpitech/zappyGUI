#include <iostream>
#include "../map/Map.hpp"

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

            void run();

            bool connect_to_server();
        private:
            /* Map _map; */
            /* Layer _layer; */
            std::size_t _timeUnit;
            std::size_t _time;

            int _port;
            std::string _hostname;

            int _server_fd;
    };
} // namespace GUI
