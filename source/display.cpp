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
    consoleSelect(&bottomScreen);
    printf(SAVE_STATUS_LINE);
    printf("                                               ");
    printf(SAVE_STATUS_LINE);
}

void print_version(std::string version) {
    consoleSelect(&bottomScreen);
    printf(VERSION_LINE);
    std::cout << version << std::endl;
}

void print_instructions() {
    consoleSelect(&bottomScreen);
    printf(INSTRUCTION_LINE);
	printf("Pulse A para seleccionar la linea actual\n");
	printf("Pulse B para crear un nuevo archivo\n");
	printf("Pulse X para guardar el archivo\n");
	printf("Pulse Y para abrir el archivo\n");
    printf("Pulse R para buscar un archivo\n");
    printf("Manten pulsado L para saltar a la parte superior/final con arriba/abajo\n");
    printf("Pulse DPad o CPad para subir/bajar\n");
	printf("Pulse START para salir\n");
    printf("2025(c)Tristanabs");
}


std::string char_vec_to_string(std::vector<char>& line) {

                std::string temp_str = "";
                int letters = 0;
                for (const auto& ch : line) {
                    if (letters != MAX_WIDTH) {
                        //Store characters to display
                        temp_str.push_back(ch); 
                        letters++;
                    } else {
                        //Too much text, display new line
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
    consoleSelect(&bottomScreen);
    printf(LINE_STATUS_LINE);
    printf("                                               ");
    printf(LINE_STATUS_LINE);
}

void print_line_status(unsigned int current_line) {
    clear_line_status();
    std::cout << "Current line: " << current_line+1;
}

void clear_directory_status() {
    consoleSelect(&bottomScreen);
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
    consoleSelect(&bottomScreen);
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

