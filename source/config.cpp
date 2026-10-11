#include "config.h"

#include <3ds.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <sys/stat.h>

namespace {
const char* CONFIG_PATH = "sdmc:/3ds/bandolero/config.ini";
const char* FILES_PATH = "sdmc:/3ds/bandolero/files/";
const char* EXTENSIONS_PATH = "sdmc:/3ds/bandolero/extensions/";
AppLanguage selected_language = APP_LANGUAGE_ENGLISH;
unsigned int launches = 0;
bool first_run = true;

void ensure_directory(const char* path) {
    mkdir(path, 0777);
}
}

bool config_init() {
    ensure_directory("sdmc:/3ds");
    ensure_directory("sdmc:/3ds/bandolero");
    ensure_directory(FILES_PATH);
    ensure_directory(EXTENSIONS_PATH);

    std::ifstream input(CONFIG_PATH);
    if (!input.is_open())
        return true;

    first_run = false;
    std::string line;
    while (std::getline(input, line)) {
        const std::string::size_type separator = line.find('=');
        if (separator == std::string::npos)
            continue;
        const std::string key = line.substr(0, separator);
        const std::string value = line.substr(separator + 1);
        if (key == "language") {
            const int value_number = std::atoi(value.c_str());
            if (value_number >= APP_LANGUAGE_SPANISH &&
                value_number <= APP_LANGUAGE_JAPANESE)
                selected_language = static_cast<AppLanguage>(value_number);
        } else if (key == "launches") {
            launches = static_cast<unsigned int>(std::strtoul(value.c_str(), nullptr, 10));
        }
    }
    ++launches;
    std::ofstream output(CONFIG_PATH, std::ios::trunc);
    if (!output.is_open())
        return false;
    output << "language=" << static_cast<int>(selected_language) << "\n";
    output << "launches=" << launches << "\n";
    return output.good();
}

bool config_is_first_run() { return first_run; }
AppLanguage config_language() { return selected_language; }
unsigned int config_launch_count() { return launches; }
void config_set_language(AppLanguage language) { selected_language = language; }

void config_commit_language() {
    std::ofstream output(CONFIG_PATH, std::ios::trunc);
    if (!output.is_open())
        return;
    output << "language=" << static_cast<int>(selected_language) << "\n";
    output << "launches=" << launches << "\n";
}

const char* bandolero_files_path() { return FILES_PATH; }
const char* bandolero_extensions_path() { return EXTENSIONS_PATH; }
