#include "extension_manager.h"
#include "config.h"

#include <fstream>
#include <dirent.h>

namespace {
void read_manifest(const std::string& path, std::vector<ExtensionInfo>& result) {
    std::ifstream catalog(path.c_str());
    if (!catalog.is_open())
        return;

    ExtensionInfo current;
    std::string line;
    while (std::getline(catalog, line)) {
        if (line.empty()) {
            if (!current.id.empty()) {
                result.push_back(current);
                current = ExtensionInfo();
            }
            continue;
        }
        const std::string::size_type separator = line.find('=');
        if (separator == std::string::npos)
            continue;
        const std::string key = line.substr(0, separator);
        const std::string value = line.substr(separator + 1);
        if (key == "id") current.id = value;
        else if (key == "name") current.name = value;
        else if (key == "extension") current.extension = value;
        else if (key == "theme") current.theme = value;
        else if (key == "background") current.background = value;
        else if (key == "panel") current.panel = value;
        else if (key == "accent") current.accent = value;
        else if (key == "text") current.text = value;
        else if (key == "cyan") current.cyan = value;
    }
    if (!current.id.empty())
        result.push_back(current);
}
}

std::vector<ExtensionInfo> load_extension_catalog() {
    std::vector<ExtensionInfo> result;
    DIR* directory = opendir(bandolero_extensions_path());
    if (directory) {
        struct dirent* entry = nullptr;
        while ((entry = readdir(directory)) != nullptr) {
            const std::string name(entry->d_name);
            if (name.size() > 5 && name.substr(name.size() - 5) == ".ban3")
                read_manifest(std::string(bandolero_extensions_path()) + name, result);
        }
        closedir(directory);
    }
    read_manifest(std::string(bandolero_extensions_path()) + "catalog.txt", result);
    return result;
}
