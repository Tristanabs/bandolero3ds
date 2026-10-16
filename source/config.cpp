#include "config.h"

#include <3ds.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace {
const char* CONFIG_PATH = "sdmc:/3ds/bandolero/config.ini";
const char* FILES_PATH = "sdmc:/3ds/bandolero/files/";
const char* EXTENSIONS_PATH = "sdmc:/3ds/bandolero/extensions/";
const char* APP_VERSION = "2.3.1";
AppLanguage selected_language = APP_LANGUAGE_ENGLISH;
unsigned int launches = 0;
bool first_run = true;
bool extension_notifications = true;
bool extensions_enabled = true;
std::vector<std::string> known_extensions;
std::vector<std::string> event_log;
std::string stored_app_version;

std::string today() {
    const std::time_t now = std::time(nullptr);
    const std::tm* local = std::localtime(&now);
    char value[16];
    if (!local || std::strftime(value, sizeof(value), "%Y-%m-%d", local) == 0)
        return "unknown-date";
    return value;
}

void purge_log() {
    const std::time_t cutoff = std::time(nullptr) - 30 * 24 * 60 * 60;
    std::vector<std::string> retained;
    for (std::vector<std::string>::const_iterator it = event_log.begin();
         it != event_log.end(); ++it) {
        if (it->size() < 11)
            continue;
        std::tm parsed = {};
        std::istringstream input(it->substr(0, 10));
        input >> std::get_time(&parsed, "%Y-%m-%d");
        if (input.fail())
            continue;
        const std::time_t timestamp = std::mktime(&parsed);
        if (timestamp >= cutoff)
            retained.push_back(*it);
    }
    event_log.swap(retained);
}

void parse_known_extensions(const std::string& value) {
    known_extensions.clear();
    std::string current;
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (*it == ',') {
            if (!current.empty())
                known_extensions.push_back(current);
            current.clear();
        } else {
            current += *it;
        }
    }
    if (!current.empty())
        known_extensions.push_back(current);
}

void write_config() {
    std::ofstream output(CONFIG_PATH, std::ios::trunc);
    if (!output.is_open())
        return;
    output << "language=" << static_cast<int>(selected_language) << "\n";
    output << "launches=" << launches << "\n";
    output << "extension_notifications=" << (extension_notifications ? 1 : 0) << "\n";
    output << "extensions_enabled=" << (extensions_enabled ? 1 : 0) << "\n";
    output << "app_version=" << APP_VERSION << "\n";
    output << "known_extensions=";
    for (std::vector<std::string>::const_iterator it = known_extensions.begin();
         it != known_extensions.end(); ++it) {
        if (it != known_extensions.begin())
            output << ",";
        output << *it;
    }
    output << "\n";
    for (std::vector<std::string>::const_iterator it = event_log.begin();
         it != event_log.end(); ++it)
        output << "log=" << *it << "\n";
}

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
        } else if (key == "extension_notifications") {
            extension_notifications = value != "0";
        } else if (key == "extensions_enabled") {
            extensions_enabled = value != "0";
        } else if (key == "app_version") {
            stored_app_version = value;
        } else if (key == "known_extensions") {
            parse_known_extensions(value);
        } else if (key == "log") {
            event_log.push_back(value);
        }
    }
    purge_log();
    if (!stored_app_version.empty() && stored_app_version != APP_VERSION)
        event_log.push_back(today() + " | Aplicación actualizada de " +
                            stored_app_version + " a " + APP_VERSION);
    stored_app_version = APP_VERSION;
    ++launches;
    write_config();
    return true;
}

bool config_is_first_run() { return first_run; }
AppLanguage config_language() { return selected_language; }
unsigned int config_launch_count() { return launches; }
void config_set_language(AppLanguage language) { selected_language = language; }

void config_commit_language() {
    write_config();
}

bool config_extension_notifications() { return extension_notifications; }
void config_set_extension_notifications(bool enabled) {
    extension_notifications = enabled;
}
bool config_extensions_enabled() { return extensions_enabled; }
void config_log_event(const std::string& event) {
    purge_log();
    event_log.push_back(today() + " | " + event);
    write_config();
}
const std::vector<std::string>& config_known_extensions() {
    return known_extensions;
}
void config_set_known_extensions(const std::vector<std::string>& extensions) {
    known_extensions = extensions;
}
void config_commit() { write_config(); }

const char* bandolero_files_path() { return FILES_PATH; }
const char* bandolero_extensions_path() { return EXTENSIONS_PATH; }
