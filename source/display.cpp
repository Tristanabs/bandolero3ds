#include "display.h"
#include <iostream>
#include <string>
#include <iterator>
#include <stdio.h>
#include <math.h>
#include <cstring>

// Añade códigos de color ANSI para fondo rojo y texto blanco/negro
#define RED_BG "\x1b[41m"
#define WHITE_TEXT "\x1b[37m"
#define BLACK_TEXT "\x1b[30m"
#define RESET_COLORS "\x1b[0m"
#define BORDER_WIDTH 50

// Códigos de color ANSI para amarillo
#define YELLOW_TEXT "\x1b[33m"

// Cambia el color amarillo a uno más brillante (fosforito) compatible con la 3DS
// Algunos emuladores/consolas solo soportan \x1b[1;33m para amarillo brillante
#undef BRIGHT_YELLOW_TEXT
#define BRIGHT_YELLOW_TEXT "\x1b[1;33m"

void clear_screen() {
    consoleSelect(&topScreen);
    printf(RED_BG WHITE_TEXT);
    printf(SCREEN_START_POINT);
    for (int i = 0; i < CLEAR_SCREEN_LINES; i++)
        printf("%s\n", std::string(BORDER_WIDTH, ' ').c_str());
    printf(SCREEN_START_POINT);
    printf(RESET_COLORS);
}

void clear_save_status() {
    printf(SAVE_STATUS_LINE);
    printf("                                               ");
    printf(SAVE_STATUS_LINE);
}

void print_version(const char* version) {
    // Versión en amarillo brillante en la pantalla superior
    consoleSelect(&topScreen);
    printf(BRIGHT_YELLOW_TEXT "Bandolero 3Ds version 1.5\n" RESET_COLORS);
}

// Decora la pestaña "Acerca de" y usa fondo rojo
void print_about() {
    consoleSelect(&topScreen);
    printf(RED_BG WHITE_TEXT);
    printf(SCREEN_START_POINT);
    printf("+------------------------------------------------+\n");
    printf("|                Bandolero 3DS                   |\n");
    printf("|        Version: Bandolero 3Ds 1.5              |\n");
    printf("|              Autor: Tristanabs                 |\n");
    printf("|           Basado en Notepad3DS                 |\n");
    printf("|            2025 (c) Tristanabs                 |\n");
    printf("|  github.com/Tristanabs/Bandolero3DS            |\n");
    printf("|  Soporte: dc.gg/hbcentral                      |\n");
    printf("+------------------------------------------------+\n");
    printf("|         Pulsa SELECT para volver               |\n");
    printf("+------------------------------------------------+\n");
    printf(RESET_COLORS);
}

// Pantalla de carga mejorada y alineada, Level T visible y todo bien centrado
void print_loading_screen() {
    // Pantalla de carga arriba (rojo)
    consoleSelect(&topScreen);
    printf(RED_BG WHITE_TEXT);
    printf(SCREEN_START_POINT);
    for (int i = 0; i < 7; i++) printf("\n");
    printf("           +------------------------------+\n");
    printf("           |        Bandolero 3DS         |\n");
    printf("           +------------------------------+\n");
    printf("           |      " BRIGHT_YELLOW_TEXT "Cargando..." WHITE_TEXT "         |\n");
    printf("           +------------------------------+\n");
    for (int i = 0; i < 13; i++) printf("\n");
    printf(RESET_COLORS);

    // Pantalla de carga abajo (negro)
    consoleSelect(&bottomScreen);
    printf(SCREEN_START_POINT);
    for (int i = 0; i < 10; i++) printf("\n");
    printf("           +------------------+\n");
    printf("           |   " BRIGHT_YELLOW_TEXT "Level T" RESET_COLORS "      |\n");
    printf("           +------------------+\n");
    for (int i = 0; i < 10; i++) printf("\n");
}

void print_instructions() {
    consoleSelect(&bottomScreen);
    printf(SCREEN_START_POINT);
    // Título y versión en amarillo brillante
    printf(BRIGHT_YELLOW_TEXT "Bandolero 3Ds version 1.5 " RESET_COLORS "\n");
    printf("----------------------------------------\n");
    printf(" A: Editar   B: Nuevo   X: Guardar\n");
    printf(" Y: Abrir    R: Buscar  L: Saltar  \n");
    printf("       DPad/C-Pad: Mover \n");
    printf(" START: Salir    SELECT: Info\n");
    printf("----------------------------------------\n");
    printf("2025 (c) Tristanabs\n");
}

std::string char_vec_to_string(std::vector<char>& line) {
    std::string temp_str = "";
    int letters = 0;
    for (const auto& ch : line) {
        if (letters != MAX_WIDTH) {
            temp_str.push_back(ch); 
            letters++;
        } else {
            temp_str.push_back('\n');
            break;
        }
    }
    return temp_str;
}

void print_text(const char* str, unsigned int count, unsigned int selected_line) {
    printf(RED_BG WHITE_TEXT);
    if (count == selected_line) {
        if (str[0] == '\n' || str[0] == '\0') {
            printf(BRIGHT_YELLOW_TEXT "(vacio)" RESET_COLORS);
            printf("\n");
        } else {
            printf(BRIGHT_YELLOW_TEXT "%s" RESET_COLORS, str);
        }
    } else {
        printf(WHITE_TEXT "%s" RESET_COLORS, str);
    }
    printf(RESET_COLORS);
}

void print_save_status(std::string message) {
    consoleSelect(&bottomScreen);
    printf("\x1b[18;0H");
    printf(BRIGHT_YELLOW_TEXT "%s\n" RESET_COLORS, message.c_str());
}

void clear_line_status() {
    printf(LINE_STATUS_LINE);
    printf("                                               ");
    printf(LINE_STATUS_LINE);
}

void print_line_status(unsigned int current_line) {
    consoleSelect(&bottomScreen);
    printf("\x1b[20;0H");
    printf(BRIGHT_YELLOW_TEXT "Linea actual: %u\n" RESET_COLORS, current_line + 1);
    consoleSelect(&topScreen);
}

void clear_directory_status() {
    printf(DIRECTORY_LINE);
    printf("                                               ");
    printf(DIRECTORY_LINE);
}

void print_directory_status(std::string filename) {
    consoleSelect(&bottomScreen);
    printf("\x1b[19;0H");
    printf("Archivo actual: %s\n", filename.c_str());
}

void update_screen(File& file, unsigned int current_line) {
    clear_screen();
    consoleSelect(&topScreen);
    printf(RED_BG WHITE_TEXT);
    printf("+------------------------------------------------+\n");
    printf("|                Bandolero 3DS                   |\n");
    printf("+------------------------------------------------+\n");

    unsigned int count = 0;
    auto iter = file.lines.begin();
    bool contenido = false;
    for (int i = 0; i < MAX_LINES; i++) {
        printf("| ");
        std::string temp;
        if (count < file.lines.size() && iter != file.lines.end()) {
            temp = char_vec_to_string(*iter);
            // Detecta si hay contenido en la línea
            for (char c : temp) {
                if (c != '\n' && c != ' ' && c != '\0') {
                    contenido = true;
                    break;
                }
            }
        }
        // Línea seleccionada resaltada
        if (count == current_line) {
            printf("> ");
        } else {
            printf("  ");
        }
        // Mostrar texto o (vacio) solo si la línea existe y está vacía
        if (count < file.lines.size() && iter != file.lines.end()) {
            if (!contenido || temp.empty() || temp == "\n") {
                printf("(vacio)");
            } else {
                // Limitar la longitud para no romper el borde
                for (size_t c = 0; c < temp.length() && c < 38; ++c) {
                    if (temp[c] == '\n') break;
                    putchar(temp[c]);
                }
            }
            ++iter;
        }
        // No mostrar nada en líneas nuevas después de la última línea escrita
        else {
            printf("      ");
        }
        // Rellenar hasta el borde derecho
        int written = 4;
        if (count < file.lines.size() && contenido && !temp.empty() && temp != "\n") {
            written += (int)std::min<size_t>(temp.length(), 38);
        } else if (count < file.lines.size() && (!contenido || temp.empty() || temp == "\n")) {
            written += 7; // "(vacio)"
        } else {
            written += 6; // espacios
        }
        for (int j = written; j < 48; j++) printf(" ");
        printf("|\n");
        count++;
        contenido = false;
    }
    printf("+------------------------------------------------+\n");
    printf(RESET_COLORS);
    print_line_status(current_line);
}

