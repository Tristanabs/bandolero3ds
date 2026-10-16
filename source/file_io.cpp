#include <fstream>
#include "file_io.h"
#include "config.h"
#include <dirent.h>
#include <algorithm>

namespace {
std::string current_directory = bandolero_files_path();

std::string document_path(const std::string& filename) {
    return current_directory + filename;
}
}

bool write_to_file(const std::string& filename, const File& file) {
    const std::string path = document_path(filename);
    std::ofstream new_file(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!new_file.is_open()) {
        return false;
    }

    for (std::list<std::vector<char>>::const_iterator iter = file.lines.begin();
         iter != file.lines.end(); ++iter) {
        if (!iter->empty()) {
            new_file.write(&(*iter)[0], static_cast<std::streamsize>(iter->size()));
        }
    }

    new_file.flush();
    return new_file.good();
}

File open_file(const std::string& filename) {
    const std::string path = document_path(filename);
    std::ifstream file_open(path.c_str(), std::ios::binary);
    File file;
    // The constructor provides the editable empty line. Replace it with
    // the actual contents only after the stream has been opened.
    file.lines.pop_front();

    if (!file_open.is_open()) {
        file.lines.push_back(std::vector<char>{'\n'});
        return file;
    }

    std::string line;
    while (std::getline(file_open, line)) {
        std::vector<char> line_vec(line.begin(), line.end());
        line_vec.push_back('\n');
        file.lines.push_back(line_vec);
    }

    if (file_open.bad()) {
        file.lines.clear();
        file.lines.push_back(std::vector<char>{'\n'});
        return file;
    }

    // Keep one editable line for an empty file and for a file ending in a
    // newline, without adding a duplicate newline to non-empty final lines.
    if (file.lines.empty() || file.lines.back().empty() ||
        file.lines.back().back() != '\n') {
        file.lines.push_back(std::vector<char>{'\n'});
    }

    file.read_success = true;
    return file;
}

std::vector<std::string> list_files() {
    std::vector<std::string> result;
    DIR* directory = opendir(current_directory.c_str());
    if (!directory)
        return result;
    struct dirent* entry = nullptr;
    while ((entry = readdir(directory)) != nullptr) {
        const std::string name(entry->d_name);
        if (name == "." || name == "..")
            continue;
        if (entry->d_type == DT_DIR)
            result.push_back(name + "/");
        else if (entry->d_type == DT_REG || entry->d_type == DT_UNKNOWN)
            result.push_back(name);
    }
    closedir(directory);
    std::sort(result.begin(), result.end());
    return result;
}

void file_io_set_directory(const std::string& path) {
    DIR* directory = opendir(path.c_str());
    if (!directory)
        return;
    closedir(directory);
    current_directory = path;
    if (current_directory.empty() || current_directory.back() != '/')
        current_directory += '/';
}

const std::string& file_io_directory() {
    return current_directory;
}
