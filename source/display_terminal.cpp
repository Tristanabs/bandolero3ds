#include "display.h"
#include "localization.h"
#include "text_utils.h"

#include <3ds.h>
#include <cstdio>
#include <string>

#define RED_BG "\x1b[41m"
#define RED_TEXT "\x1b[1;31m"
#define WHITE_TEXT "\x1b[37m"
#define YELLOW_TEXT "\x1b[1;33m"
#define RESET "\x1b[0m"

PrintConsole topScreen;
PrintConsole bottomScreen;
namespace {
std::string save_status;
std::string current_filename;
}

bool display_init() {
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);
    return true;
}

void display_exit() {}

void print_loading_screen() {
    consoleSelect(&topScreen);
    printf(RED_BG WHITE_TEXT "\x1b[0;0H");
    printf("\n\n       +------------------------------+\n");
    printf("       |        BANDOLERO 3DS        |\n");
    printf("       +------------------------------+\n");
    printf("       |       " YELLOW_TEXT "Cargando..." WHITE_TEXT "       |\n");
    printf("       +------------------------------+\n" RESET);
    consoleSelect(&bottomScreen);
    printf("\x1b[0;0H" RED_BG WHITE_TEXT "\n\n       Modo terminal / UTF-8\n" RESET);
}

void update_screen(const File& file, unsigned int current_line) {
    consoleSelect(&topScreen);
    printf(RED_BG WHITE_TEXT "\x1b[0;0H");
    printf(" BANDOLERO 3DS  |  EDITOR TERMINAL\n");
    printf(" -----------------------------------------------\n");
    std::list<std::vector<char>>::const_iterator line = file.lines.begin();
    for (unsigned int index = 0; index < MAX_LINES; ++index) {
        printf("%s%02u %s", index == current_line ? YELLOW_TEXT "> " : "  ",
               index + 1, RESET WHITE_TEXT);
        if (line != file.lines.end()) {
            std::string value = char_vec_to_string(*line);
            if (!value.empty() && value[value.size() - 1] == '\n')
                value.erase(value.size() - 1);
            printf("%s", value.empty() ? "(linea vacia)" :
                   utf8_display_prefix(value, 44).c_str());
            ++line;
        }
        printf("\n");
    }
    printf(RED_BG WHITE_TEXT " -----------------------------------------------\n");
    printf(" Linea %u | %d lineas | UTF-8\n" RESET, current_line + 1, file.size());

    consoleSelect(&bottomScreen);
    printf("\x1b[0;0H" RED_BG WHITE_TEXT);
    printf(" BANDOLERO 3DS  |  MODO TERMINAL\n");
    printf(" ----------------------------------------\n");
    printf(" A Editar       B Nuevo\n");
    printf(" X Guardar      Y Abrir\n");
    printf(" R Buscar       L Saltar\n");
    printf(" DPad/CPad Mover   SELECT Acerca de\n");
    printf(" ----------------------------------------\n");
    printf(" %s\n", save_status.empty() ? "Listo" : save_status.c_str());
    printf(" %s\n" RESET, current_filename.empty() ? "Sin archivo" :
           current_filename.c_str());
}

void print_about() {
    consoleSelect(&topScreen);
    printf(RED_BG WHITE_TEXT "\x1b[0;0H");
    printf("\n  BANDOLERO 3DS  |  ACERCA DE\n");
    printf("  -----------------------------------------------\n");
    printf("  Editor de texto UTF-8 para Nintendo 3DS\n");
    printf("  Interfaz terminal clasica\n");
    printf("  Version 2.0\n");
    printf("  Autor: Tristanabs\n");
    printf("  Unique ID: 000400000FF3FF00\n");
    printf("  SELECT para volver\n" RESET);
}

void clear_save_status() { save_status.clear(); }
void print_save_status(std::string message) { save_status = message; }
void clear_directory_status() { current_filename.clear(); }
void print_directory_status(std::string filename) { current_filename = filename; }
void print_instructions() {}
void print_version(const char*) {}
std::string char_vec_to_string(const std::vector<char>& line) {
    return std::string(line.begin(), line.end());
}
