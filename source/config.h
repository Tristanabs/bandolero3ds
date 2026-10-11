#pragma once

#include "localization.h"

bool config_init();
bool config_is_first_run();
AppLanguage config_language();
unsigned int config_launch_count();
void config_set_language(AppLanguage language);
void config_commit_language();
const char* bandolero_files_path();
const char* bandolero_extensions_path();
