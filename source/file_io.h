#pragma once
#include <string>
#include "file.h"
#include "text_utils.h"
#include <vector>

// Writes the UTF-8 byte sequence stored in file to filename.
// Returns false when the file cannot be opened or flushed successfully.
bool write_to_file(const std::string& filename, const File& file);

// Opens filename as a UTF-8-compatible byte stream.
// A failed read returns a File with read_success set to false.
File open_file(const std::string& filename);

// Lists regular files available in the Bandolero document directory.
std::vector<std::string> list_files();
void file_io_set_directory(const std::string& path);
const std::string& file_io_directory();
