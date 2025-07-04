/*
** EPITECH PROJECT, 2025
** ZAPPY GUI
** File description:
** communicationbuffer.hpp
*/

#pragma once

#include <string>
#include <vector>

namespace GUI {

class CommunicationBuffer {
    public:
        CommunicationBuffer() = default;
        ~CommunicationBuffer() = default;
        
        CommunicationBuffer(const CommunicationBuffer&) = delete;
        CommunicationBuffer &operator=(const CommunicationBuffer&) = delete;
        
        void append_data(const char *data);
        void append_data(const std::string &data);

        [[nodiscard]] bool has_complete_message() const;
        
        std::string extract_next_message();
        std::vector<std::string> extract_all_messages();
        
        void clear();
        
        [[nodiscard]] size_t size() const;
        
        [[nodiscard]] bool empty() const;
        [[nodiscard]] const std::string &get_raw_buffer() const;

    private:
        std::string _input_buffer;
    };

} // namespace GUI
