#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include "file.h"
#include "display.h"
#include "file_io.h"
#include "localization.h"
#include "extension_manager.h"
#include "config.h"

#define BUFFER_SIZE 1025    //Notepad's line limit + \0
#define MAX_BOTTOM_SIZE 28

int scroll = 0;
bool fast_scroll = false;
bool show_about = false; // Nueva variable para controlar la pestaña

void move_down(const File& file);
void move_up(const File& file);

unsigned int curr_line = 0;

int main(int argc, char **argv)
{
    if (R_FAILED(cfguInit())) {
        return 1;
    }
	gfxInitDefault();
    romfsInit();
    if (!config_init()) {
        romfsExit();
        gfxExit();
        cfguExit();
        return 1;
    }
    localization_init(config_language());
    if (!display_init()) {
        gfxExit();
        cfguExit();
        return 1;
    }

    print_loading_screen();

    if (config_is_first_run()) {
        AppLanguage selected_language = APP_LANGUAGE_SPANISH;
        bool language_selected = false;
        while (aptMainLoop() && !language_selected) {
            print_language_setup(selected_language);
            hidScanInput();
            const u32 keys = hidKeysDown();
            if (keys & KEY_R)
                selected_language = static_cast<AppLanguage>((selected_language + 1) % 6);
            if (keys & KEY_L)
                selected_language = static_cast<AppLanguage>((selected_language + 5) % 6);
            if (keys & KEY_A)
                language_selected = true;
            gspWaitForVBlank();
        }
        config_set_language(selected_language);
        config_commit_language();
        localization_set_language(selected_language);
        print_first_run_info();
        while (aptMainLoop()) {
            hidScanInput();
            if (hidKeysDown() & KEY_A)
                break;
            gspWaitForVBlank();
        }
    }

    File file;      //Use as default file
    const std::vector<ExtensionInfo> extensions = load_extension_catalog();
    std::vector<std::string> extension_names;
    for (const ExtensionInfo& extension : extensions) {
        if (!extension.name.empty())
            extension_names.push_back(extension.name);
    }
    display_set_extensions(extension_names);
    if (!extensions.empty()) {
        const ExtensionInfo& theme = extensions.front();
        display_apply_theme(theme.background, theme.panel, theme.accent,
                            theme.text, theme.cyan);
    }

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
            AppLanguage language = localization_language();
            if (kDown & KEY_R)
                language = static_cast<AppLanguage>((language + 1) % 6);
            if (kDown & KEY_L)
                language = static_cast<AppLanguage>((language + 5) % 6);
            if ((kDown & (KEY_R | KEY_L)) != 0) {
                localization_set_language(language);
                config_set_language(language);
                config_commit_language();
                print_about();
            }
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
            swkbdSetHintText(&swkbd, tr(TEXT_EDIT_HINT));
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
            swkbdSetHintText(&swkbd, tr(TEXT_NEW_CONFIRM)); 
            button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
            if (mybuf[0] == 'y') {
                File blankFile;
                file = blankFile;
                curr_line = 0;
                scroll = 0;
                update_screen(file, curr_line);
                print_save_status(tr(TEXT_NEW_CREATED));
            } else
                print_save_status(tr(TEXT_NEW_CANCELLED));
        }

        if (kDown & KEY_R) {
            //find a thing
            
            //Clear buffer
            memset(mybuf, '\0', BUFFER_SIZE);
            //Get term to search for
            swkbdSetHintText(&swkbd, tr(TEXT_SEARCH_HINT)); 
            button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
            int line = file.find(mybuf);
            if (line < 0) {
                char status[BUFFER_SIZE];
                snprintf(status, sizeof(status), tr(TEXT_SEARCH_NOT_FOUND), mybuf);
                print_save_status(status);
            } else {
                char status[BUFFER_SIZE];
                snprintf(status, sizeof(status), tr(TEXT_SEARCH_FOUND), mybuf, line);
                print_save_status(status);
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
           
            swkbdSetHintText(&swkbd, tr(TEXT_FILENAME_HINT)); 
            button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
            std::string filename = "";
            for (int i = 0; mybuf[i] != '\0'; i++)
                filename.push_back(mybuf[i]);
            filename = normalize_text_filename(filename);

            //Write out characters to file
            bool success = write_to_file(filename, file);
            
            if (success) {
                char status[BUFFER_SIZE];
                snprintf(status, sizeof(status), tr(TEXT_SAVE_OK), filename.c_str());
                print_save_status(status);
                print_directory_status(filename);
            } else {
                char status[BUFFER_SIZE];
                snprintf(status, sizeof(status), tr(TEXT_SAVE_FAILED), filename.c_str());
                print_save_status(status);
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
           
            swkbdSetHintText(&swkbd, tr(TEXT_FILENAME_HINT)); 
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
                 char status[BUFFER_SIZE];
                 snprintf(status, sizeof(status), tr(TEXT_OPEN_OK), filename.c_str());
                 print_save_status(status);
                 clear_directory_status();
                 print_directory_status(filename);
                //print_directory_status(filename);
            } else {
                file = oldfile;
                update_screen(file, curr_line);
                clear_save_status();
                 char status[BUFFER_SIZE];
                 snprintf(status, sizeof(status), tr(TEXT_OPEN_FAILED), filename.c_str());
                 print_save_status(status);
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

        update_screen(file, curr_line);

		gspWaitForVBlank();
	}

    display_exit();
    romfsExit();
    cfguExit();
	gfxExit();
	return 0;
}

void move_down(const File& file) {
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

void move_up(const File& file) {
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
