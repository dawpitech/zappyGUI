/**
 * @file CommunicationBuffer.cpp
 * @brief Implementation of the CommunicationBuffer class for the ZappyGUI project
 * @author EPITECH PROJECT, 2025
 * @date 2025
 *
 * This file contains the implementation of the CommunicationBuffer class which
 * manages network communication data buffering. It handles incoming data streams,
 * message parsing, and extraction of complete messages delimited by newline characters.
 *
 * The buffer is designed to handle partial messages and accumulate data until
 * complete messages (terminated by '\n') are available for processing.
 */

/*
** EPITECH PROJECT, 2025
** ZAPPYGUI
** File description:
** communication buffer
*/

#include "CommunicationBuffer.hpp"

/**
 * @brief Appends data from a C-style string to the buffer
 *
 * Adds the provided C-style string data to the internal input buffer.
 * This method performs null pointer checking to ensure data validity
 * before appending. If the data pointer is null, no operation is performed.
 *
 * This method is typically used when receiving data from network sockets
 * or other C-style string sources.
 *
 * @param data Pointer to null-terminated C-style string to append
 *
 * @note If data is nullptr, the function safely returns without modification
 * @note The data is appended to the existing buffer content
 */
void GUI::CommunicationBuffer::append_data(const char *data)
{
    if (data != nullptr)
        _input_buffer += data;
}

/**
 * @brief Appends data from a std::string to the buffer
 *
 * Adds the provided std::string data to the internal input buffer.
 * This method directly appends the string content without additional
 * validation since std::string handles its own memory management.
 *
 * This method is typically used when receiving data from C++ string
 * sources or when converting data formats.
 *
 * @param data Reference to std::string containing data to append
 *
 * @note The data is appended to the existing buffer content
 * @note No null checking is required as std::string is passed by reference
 */
void GUI::CommunicationBuffer::append_data(const std::string &data)
{
    _input_buffer += data;
}

/**
 * @brief Checks if the buffer contains at least one complete message
 *
 * Determines whether the buffer contains a complete message by searching
 * for the newline character ('\n') which serves as the message delimiter.
 * This method is used to determine if message extraction is possible.
 *
 * @return bool True if at least one complete message is available, false otherwise
 *
 * @note A complete message is defined as any sequence of characters followed by '\n'
 * @note This method does not modify the buffer state
 */
bool GUI::CommunicationBuffer::has_complete_message() const
{
    return _input_buffer.find('\n') != std::string::npos;
}

/**
 * @brief Extracts the next complete message from the buffer
 *
 * Retrieves and removes the next complete message from the buffer.
 * The message is extracted up to (but not including) the first newline
 * character. The newline character and the extracted message are removed
 * from the buffer, leaving any remaining data for subsequent extractions.
 *
 * @return std::string The extracted message without the newline delimiter,
 *                     or an empty string if no complete message is available
 *
 * @note This method modifies the buffer by removing the extracted message
 * @note The newline delimiter is consumed but not included in the return value
 * @note If no complete message exists, returns an empty string and buffer remains unchanged
 */
std::string GUI::CommunicationBuffer::extract_next_message()
{
    size_t pos = _input_buffer.find('\n');
    if (pos == std::string::npos)
        return "";

    std::string message = _input_buffer.substr(0, pos);
    _input_buffer = _input_buffer.substr(pos + 1);

    return message;
}

/**
 * @brief Extracts all complete messages from the buffer
 *
 * Retrieves and removes all complete messages currently available in the buffer.
 * Each message is extracted sequentially until no more complete messages remain.
 * Empty messages (consecutive newlines) are filtered out and not included
 * in the result vector.
 *
 * This method is useful for batch processing of accumulated messages,
 * particularly when multiple messages have been received in a single
 * network operation.
 *
 * @return std::vector<std::string> Vector containing all extracted messages
 *                                  in the order they appeared in the buffer
 *
 * @note This method modifies the buffer by removing all extracted messages
 * @note Empty messages are automatically filtered out
 * @note The buffer may still contain incomplete message data after extraction
 * @note Messages are returned in FIFO (First In, First Out) order
 */
std::vector<std::string> GUI::CommunicationBuffer::extract_all_messages()
{
    std::vector<std::string> messages;

    while (has_complete_message()) {
        std::string message = extract_next_message();
        if (!message.empty())
            messages.push_back(message);
    }

    return messages;
}

/**
 * @brief Clears all data from the buffer
 *
 * Removes all accumulated data from the internal buffer, effectively
 * resetting it to an empty state. This method is useful for cleanup
 * operations or when restarting communication sessions.
 *
 * @note After calling this method, the buffer will be empty
 * @note Any incomplete messages in the buffer will be lost
 */
void GUI::CommunicationBuffer::clear()
{
    _input_buffer.clear();
}

/**
 * @brief Returns the current size of the buffer
 *
 * Provides the number of characters currently stored in the buffer,
 * including both complete and incomplete message data. This method
 * is useful for monitoring buffer usage and debugging communication issues.
 *
 * @return size_t The number of characters in the buffer
 *
 * @note This includes all data: complete messages, incomplete messages, and delimiters
 * @note The size represents the total character count, not the number of messages
 */
size_t GUI::CommunicationBuffer::size() const
{
    return _input_buffer.size();
}

/**
 * @brief Checks if the buffer is empty
 *
 * Determines whether the buffer contains any data. This method is useful
 * for conditional processing and state validation.
 *
 * @return bool True if the buffer contains no data, false otherwise
 *
 * @note An empty buffer means no data has been appended or all data has been cleared
 * @note This method does not indicate whether complete messages are available
 */
bool GUI::CommunicationBuffer::empty() const
{
    return _input_buffer.empty();
}

/**
 * @brief Provides read-only access to the raw buffer content
 *
 * Returns a constant reference to the internal buffer string, allowing
 * inspection of the raw buffer content without modification. This method
 * is useful for debugging, logging, or advanced buffer analysis.
 *
 * @return const std::string& Constant reference to the internal buffer
 *
 * @warning This method exposes the internal buffer state and should be used carefully
 * @note The returned reference is valid only as long as the buffer object exists
 * @note Any buffer modifications will affect the content accessible through this reference
 */
const std::string &GUI::CommunicationBuffer::get_raw_buffer() const
{
    return _input_buffer;
}
