#pragma once

#include "file.h"
#include "localization.h"
#include <string>
#include <vector>

#define MAX_LINES 28
#define MAX_WIDTH 49
extern int scroll;

bool display_init();
void display_exit();

void update_screen(const File& file, unsigned int current_line);
void print_loading_screen();
void clear_save_status();
void print_save_status(std::string message);
void clear_directory_status();
void print_directory_status(std::string filename);
void print_instructions();
void print_version(const char* version);
void print_about();
void print_language_setup(AppLanguage language);
void print_first_run_info();
void display_set_extensions(const std::vector<std::string>& names);
void display_apply_theme(const std::string& background, const std::string& panel,
                         const std::string& accent, const std::string& text,
                         const std::string& cyan);

std::string char_vec_to_string(const std::vector<char>& line);
