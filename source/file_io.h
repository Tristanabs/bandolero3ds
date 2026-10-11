#pragma once
#include <string>
#include "file.h"
#include "text_utils.h"

// Writes the UTF-8 byte sequence stored in file to filename.
// Returns false when the file cannot be opened or flushed successfully.
bool write_to_file(const std::string& filename, const File& file);

// Opens filename as a UTF-8-compatible byte stream.
// A failed read returns a File with read_success set to false.
File open_file(const std::string& filename);
