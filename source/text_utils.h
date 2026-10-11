#pragma once

#include <string>

// Keeps complete UTF-8 sequences while limiting display width in bytes.
std::string utf8_display_prefix(const std::string& value, unsigned int max_bytes);

// Adds .txt only when the user did not provide an extension.
std::string normalize_text_filename(const std::string& filename);
