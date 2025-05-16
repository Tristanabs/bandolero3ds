#include "display.h"
#include <iostream>
#include <string>
#include <iterator>
#include <stdio.h>
#include <math.h>
void clear_screen() {
    consoleSelect(&topScreen);
    //Cursor to top left
    std::cout << SCREEN_START_POINT;
    //Clear screen with empty lines
    for (int i = 0; i < CLEAR_SCREEN_LINES; i++)
        std::cout << std::string(70, ' ');
    std::cout << SCREEN_START_POINT;
}

void clear_save_status() {

    printf(SAVE_STATUS_LINE);
    printf("                                               ");
    printf(SAVE_STATUS_LINE);
}

void print_version(const char* version) {
    printf(VERSION_LINE);
    printf("%s\n", version);
}

void print_about() {
    consoleSelect(&bottomScreen);
    printf("\x1b[18;0H"); // Linea inferior para evitar solapar instrucciones
    printf("Bandolero 3DS\n");
    printf("Version: Bandolero 3Ds version 1.2 BETA\n");
    printf("Autor: Tristanabs\n");
    printf("Basado en Notepad3DS\n");
    printf("2025 (c) Tristanabs\n");
    printf("Mas info: github.com/Tristanabs/Bandolero3DS\n");
    printf("Soporte: dc.gg/hbcentral\n");
}

void print_instructions() {
    consoleSelect(&bottomScreen);
    printf(INSTRUCTION_LINE);
    printf("Pulsa A para seleccionar la linea actual\n");
    printf("Pulsa B para crear un archivo nuevo\n");
    printf("Pulsa X para guardar el archivo\n");
    printf("Pulsa Y para abrir un archivo\n");
    printf("Pulsa R para buscar un archivo\n");
    printf("Mantener L para saltar arriba/abajo con flechas\n");
    printf("Usa DPad o CPad para subir o bajar\n");
    printf("Pulsa START para salir\n");
    printf("Pulsa SELECT para ver info\n");
    printf("2025 (c) Tristanabs");
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

                if (count == selected_line)
                    if (str[0] == '\n') {
                        printf(SELECTED_TEXT_COLOUR);
                        printf("(vacio)");
                        printf(DEFAULT_TEXT_COLOUR);
                        printf("\n");
                    } else {
                        printf(SELECTED_TEXT_COLOUR);
                        printf("%s", str);
                        printf(DEFAULT_TEXT_COLOUR);
                    }
                else {
                    printf(DEFAULT_TEXT_COLOUR);
                    printf("%s", str);
                }
}

void print_save_status(std::string message) {
    clear_save_status();
    std::cout << message << std::endl;
}

void clear_line_status() {

    printf(LINE_STATUS_LINE);
    printf("                                               ");
    printf(LINE_STATUS_LINE);
}

void print_line_status(unsigned int current_line) {
    clear_line_status();
    consoleSelect(&bottomScreen);
    printf(LINE_STATUS_LINE);
    printf("Línea actual: %u\n", current_line + 1);
    consoleSelect(&topScreen);
}

void clear_directory_status() {

    printf(DIRECTORY_LINE);
    printf("                                               ");
    printf(DIRECTORY_LINE);
}

void print_directory_status(std::string filename) {
    clear_directory_status();
    std::cout << "Current directory: " << filename;

}

void update_screen(File& file, unsigned int current_line) {
    clear_screen();

    print_line_status(current_line);
    consoleSelect(&topScreen);
    unsigned int count = 0;
    
    //No scrolling needed
    if (file.lines.size() - 1 <= MAX_LINES) {
        for (auto iter = file.lines.begin(); iter != file.lines.end(); iter++) {
            //Print everything in the vector<char> that iterator points to
            std::string temp = char_vec_to_string(*iter);
            const char* str_to_print = temp.c_str();
            print_text(str_to_print, count, current_line);
            count++;
        }
        
    //Scrolling needed
    } else {
    
        auto iter = file.lines.begin();
        
        if (current_line > 1 ) {
            advance(iter, (current_line -1));
        }
        else {
            advance(iter, current_line);
        }
        
        if (scroll == 0) {        
            for (int line = 0; line <= MAX_LINES; line++) {
                iter = file.lines.begin();
                advance(iter, line);
                
                std::string temp = char_vec_to_string(*iter);
                const char* str_to_print = temp.c_str();
                print_text(str_to_print, count, current_line);
                count++;
            }
            
        } else {
            for (int line = scroll; line <= MAX_LINES + scroll; line++) {
                

                iter = file.lines.begin();
                advance(iter, line);
                std::string temp = char_vec_to_string(*iter);
                const char* str_to_print = temp.c_str();
                print_text(str_to_print, count, current_line-scroll);
                count++;
                
            }
        }
    
    }

}

