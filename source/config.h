#pragma once

#include "localization.h"
#include <string>
#include <vector>

bool config_init();
bool config_is_first_run();
AppLanguage config_language();
unsigned int config_launch_count();
void config_set_language(AppLanguage language);
void config_commit_language();
bool config_extension_notifications();
void config_set_extension_notifications(bool enabled);
bool config_extensions_enabled();
void config_log_event(const std::string& event);
const std::vector<std::string>& config_known_extensions();
void config_set_known_extensions(const std::vector<std::string>& extensions);
void config_commit();
const char* bandolero_files_path();
const char* bandolero_extensions_path();
