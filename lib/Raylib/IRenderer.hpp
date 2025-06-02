//
// EPITECH PROJECT, 2025
// IRenderer
// File description:
// IRenderer
//

#pragma once

#include <exception>
#include <memory>
#include <string>


namespace renderer {

    class RendererError : public std::exception {
       public:
        RendererError(const std::string &msg) : _msg(msg) {};

        [[nodiscard]] const char *what() const noexcept override
        {
            return this->_msg.c_str();
        };

       private:
        std::string _msg;
    };

    class IRenderer {
       public:
        virtual ~IRenderer() = default;
    };

    extern "C" std::unique_ptr<IRenderer> createRenderer(void);

}  // namespace renderer
