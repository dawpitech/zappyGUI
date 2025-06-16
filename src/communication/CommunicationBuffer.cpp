/*
** EPITECH PROJECT, 2025
** ZAPPYGUI
** File description:
** communication buffer
*/

#include "CommunicationBuffer.hpp"

void GUI::CommunicationBuffer::append_data(const char* data)
{
    if (data != nullptr)
        _input_buffer += data;
}

void GUI::CommunicationBuffer::append_data(const std::string& data)
{
    _input_buffer += data;
}

bool GUI::CommunicationBuffer::has_complete_message() const
{
    return _input_buffer.find('\n') != std::string::npos;
}

std::string GUI::CommunicationBuffer::extract_next_message()
{
    size_t pos = _input_buffer.find('\n');
    if (pos == std::string::npos) {
        return "";
    }
    
    std::string message = _input_buffer.substr(0, pos);
    _input_buffer = _input_buffer.substr(pos + 1);
    
    return message;
}

std::vector<std::string> GUI::CommunicationBuffer::extract_all_messages()
{
    std::vector<std::string> messages;
    
    while (has_complete_message()) {
        std::string message = extract_next_message();
        if (!message.empty()) {
            messages.push_back(message);
        }
    }
    
    return messages;
}

void GUI::CommunicationBuffer::clear()
{
    _input_buffer.clear();
}

size_t GUI::CommunicationBuffer::size() const
{
    return _input_buffer.size();
}

bool GUI::CommunicationBuffer::empty() const
{
    return _input_buffer.empty();
}

const std::string &GUI::CommunicationBuffer::get_raw_buffer() const
{
    return _input_buffer;
}
