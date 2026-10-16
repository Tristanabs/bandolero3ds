#include "text_utils.h"

namespace {
unsigned int utf8_sequence_length(unsigned char first) {
    if ((first & 0x80) == 0) return 1;
    if ((first & 0xe0) == 0xc0) return 2;
    if ((first & 0xf0) == 0xe0) return 3;
    if ((first & 0xf8) == 0xf0) return 4;
    return 1;
}
}

std::string utf8_display_prefix(const std::string& value, unsigned int max_bytes) {
    std::string result;
    unsigned int offset = 0;
    while (offset < value.size()) {
        if (value[offset] == '\n' || value[offset] == '\r')
            break;
        const unsigned int length = utf8_sequence_length(
            static_cast<unsigned char>(value[offset]));
        if (offset + length > value.size() || offset + length > max_bytes)
            break;
        result.append(value, offset, length);
        offset += length;
    }

    return result;
}

std::string normalize_text_filename(const std::string& filename) {
    if (filename.empty() || filename == "." || filename == "..")
        return filename;
    const std::string::size_type slash = filename.find_last_of("/\\");
    const std::string::size_type dot = filename.find_last_of('.');
    if (dot == std::string::npos || (slash != std::string::npos && dot < slash))
        return filename + ".txt";
    return filename;
}

bool utf8_is_valid(const std::string& value) {
    unsigned int offset = 0;
    while (offset < value.size()) {
        const unsigned char first = static_cast<unsigned char>(value[offset]);
        const unsigned int length = utf8_sequence_length(first);
        if (length == 1 && (first & 0x80))
            return false;
        if (offset + length > value.size())
            return false;
        for (unsigned int index = 1; index < length; ++index) {
            if ((static_cast<unsigned char>(value[offset + index]) & 0xc0) != 0x80)
                return false;
        }
        offset += length;
    }
    return true;
}

std::string markdown_display_text(const std::string& value) {
    std::string result = value;
    if (result.size() >= 2 && result[0] == '#' && result[1] == ' ')
        result.erase(0, 2);
    while (result.find("**") != std::string::npos)
        result.erase(result.find("**"), 2);
    while (result.find("__") != std::string::npos)
        result.erase(result.find("__"), 2);
    if (result.size() >= 2 && result[0] == '-' && result[1] == ' ')
        result[0] = '*';
    return result;
}
