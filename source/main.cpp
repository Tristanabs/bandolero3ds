#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include "file.h"
#include "display.h"
#include "file_io.h"

#define BUFFER_SIZE 1025    //Notepad's line limit + \0
#define MAX_BOTTOM_SIZE 28

#define VERSION "Bandolero 3Ds version 1.1"


PrintConsole topScreen, bottomScreen;
int scroll = 0;
bool fast_scroll = false;
bool show_about = false; // Nueva variable para controlar la pestaña

void move_down(File file);
void move_up(File file);

unsigned int curr_line = 0;

int main(int argc, char **argv)
{
	gfxInitDefault();
	consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    // Pantalla de carga SOLO ASCII
    consoleSelect(&topScreen);
    printf("\x1b[41;37m"); // Fondo rojo, texto blanco
    printf("\x1b[0;0H");
    printf("+--------------------------------------------------+\n");
    printf("|                Bandolero 3DS                     |\n");
    printf("+--------------------------------------------------+\n");
    printf("|                  Cargando...                     |\n");
    printf("+--------------------------------------------------+\n");
    printf("\x1b[0m");
    gfxFlushBuffers();
    gfxSwapBuffers();
    svcSleepThread(2000000000LL); // 2 segundos

    consoleSelect(&bottomScreen);
    //Software keyboard thanks to fincs
    print_instructions();

    print_version(VERSION);
    
    File file;      //Use as default file

    update_screen(file, curr_line);

	while (aptMainLoop())
	{

		hidScanInput();

		u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        if (kDown & KEY_START)
            break;

        // Cambiar pestaña con SELECT
        if (kDown & KEY_SELECT) {
            show_about = !show_about;
            if (show_about) {
                print_about();
            } else {
                update_screen(file, curr_line);
            }
        }

        // Si está activa la pestaña de Acerca de, ignorar otras entradas
        if (show_about) {
            // Permitir salir con START o volver con SELECT
            gfxFlushBuffers();
            gfxSwapBuffers();
            gspWaitForVBlank();
            continue;
        }

		static SwkbdState swkbd;
		static char mybuf[BUFFER_SIZE];
		SwkbdButton button = SWKBD_BUTTON_NONE;
		bool didit = false;

        swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 1, -1);
        swkbdSetValidation(&swkbd, SWKBD_ANYTHING, SWKBD_ANYTHING, 2);
        swkbdSetFeatures(&swkbd, SWKBD_DARKEN_TOP_SCREEN);

        if (kDown & KEY_A) {
            //Select current line for editing
            swkbdSetHintText(&swkbd, "Introduce el texto aqui.");
            //Iterator to find current selected line
            auto line = file.lines.begin();
            if (curr_line < file.lines.size())
            {
                if (curr_line != 0)
                    advance(line, curr_line);
                
                if (curr_line == file.lines.size() - 1) {
                    file.lines.push_back(std::vector<char>{'\n'});
                }
                //Need a char array to output to keyboard
                char current_text[BUFFER_SIZE] = "";
                copy(line->begin(), line->end(), current_text);
                swkbdSetInitialText(&swkbd, current_text);
            }
            didit = true;
            button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
        }

        if (kDown & KEY_B) {
            //Create new file
            
            //Clear buffer
            memset(mybuf, '\0', BUFFER_SIZE);
            //Confirm creating a new file
            swkbdSetHintText(&swkbd, "Seguro que quieres abrir un archivo en blanco? y/n"); 
            button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
            if (mybuf[0] == 'y') {
                File blankFile;
                file = blankFile;
                curr_line = 0;
                scroll = 0;
                update_screen(file, curr_line);
                print_save_status("Archivo nuevo creado");
            } else
                print_save_status("No se creo ningun archivo nuevo");
        }

        if (kDown & KEY_R) {
            //find a thing
            
            //Clear buffer
            memset(mybuf, '\0', BUFFER_SIZE);
            //Get term to search for
            swkbdSetHintText(&swkbd, "Introduce el termino de busqueda"); 
            button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
            int line = file.find(mybuf);
            if (line < 0)
                printf("No se encontro: %s", mybuf);
            else {
                printf("Encontrado %s en linea %d", mybuf, line);
                curr_line = line;
                if (curr_line > MAX_BOTTOM_SIZE) {
                    scroll = curr_line - MAX_BOTTOM_SIZE;
                }
                update_screen(file, curr_line);
            }   

        }

        if (kHeld & KEY_L) {
            //If held, allows for jumping to end and start of file
            fast_scroll = true;
        } else {
            fast_scroll = false;
        }

        if (kDown & KEY_X) {
            //Save current file
            //Clear buffer
            memset(mybuf, '\0', BUFFER_SIZE);

            //Get file name
           
            swkbdSetHintText(&swkbd, "Introduzca aquí el nombre del archivo."); 
            button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
            std::string filename = "";
            for (int i = 0; mybuf[i] != '\0'; i++)
                filename.push_back(mybuf[i]);

            //Write out characters to file
            bool success = write_to_file(filename, file);
            
            if (success) {
                print_save_status("Archivo guardado en " + filename);
                print_directory_status(filename);
            } else {
                print_save_status("No se pudo guardar " + filename);
            }

        }

        if (kDown & KEY_Y) {
            //Similar code to pressing X, see about refactoring
            //Open a file
            curr_line = 0;
            scroll = 0;
            //Clear buffer
            memset(mybuf, '\0', BUFFER_SIZE);

            //Get file name
           
            swkbdSetHintText(&swkbd, "Introduzca aquí el nombre del archivo."); 
            button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
            std::string filename = "";
            for (int i = 0; mybuf[i] != '\0'; i++)
                filename.push_back(mybuf[i]);
            File oldfile = file;
            file = open_file(filename);
            
            //print functions here seem to crash the program
            if (file.read_success) {
                update_screen(file, curr_line);
                clear_save_status();
                 std::cout << "Archivo abierto correctamente: " << filename << std::endl;
                 clear_directory_status();
                 std::cout << "Archivo actual: " << filename;
                consoleSelect(&bottomScreen);
                printf("Archivo actual: %s\n", filename);
                //print_directory_status(filename);
            } else {
                file = oldfile;
                update_screen(file, curr_line);
                clear_save_status();
                 std::cout << "No se pudo abrir: " << filename << std::endl;
                consoleSelect(&bottomScreen);
                printf("No se pudo abrir: %s\n", filename);
                //print_save_status("Failed to open " + filename);
            }
        }

        if (kDown & KEY_DDOWN) {
            //Move a line down (towards bottom of screen)
            move_down(file); 
        }

        if (kHeld & KEY_CPAD_DOWN) {
            //Move a line down (towards bottom of screen)
            //as long as down is held
            move_down(file); 
        }
        if (kDown & KEY_DUP) {
            //Move a line up (towards top of screen)
            move_up(file);
        }


        if (kHeld & KEY_CPAD_UP) {
            //Move a line up (towards top of screen)
            //as long as up is held
            move_up(file);
        }


		if (didit)
		{
			if (button != SWKBD_BUTTON_NONE)
			{
                std::vector<char> new_text = char_arr_to_vector(mybuf);

                if (curr_line >= file.lines.size()) {
                    //Empty line, add a new one.
                    file.add_line(new_text);
                } else {
                    file.edit_line(new_text, curr_line);
                }
                update_screen(file, curr_line);
			} else
				printf("swkbd event: %d\n", swkbdGetResult(&swkbd));
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}

void move_down(File file) {
    //Move a line down (towards bottom of screen)
    if (curr_line < file.lines.size() - 1) {
        if (fast_scroll) {
            curr_line = file.lines.size()-1;
            scroll = curr_line - MAX_BOTTOM_SIZE;

        } else {

            if ( (curr_line - scroll >= MAX_BOTTOM_SIZE) && (curr_line < file.lines.size() ) ) {
                scroll++;
                curr_line++;
            } else {
                curr_line++;
            }
        }
        update_screen(file, curr_line);
    }

}

void move_up(File file) {
    //Move a line up (towards top of screen)

    if (curr_line != 0) {
        if (fast_scroll) {
            //Jump to the top
            curr_line = 0;
            scroll = 0;

        } else {

            curr_line--;
            if (curr_line - scroll <= 0 && scroll != 0) {
                scroll--;
            }
        }
        update_screen(file, curr_line);
    }
}
