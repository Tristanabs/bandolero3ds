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
#define MAX_BOTTOM_SIZE (VISIBLE_EDITOR_LINES - 1)

int scroll = 0;
bool fast_scroll = false;
bool show_about = false; // Nueva variable para controlar la pestaña
unsigned int konami_progress = 0;
bool dedication_consumed = false;
bool dedication_screen = false;

bool update_konami_code(u32 keys) {
    static const u32 sequence[] = {
        KEY_DUP, KEY_DUP, KEY_DDOWN, KEY_DDOWN, KEY_DLEFT,
        KEY_DRIGHT, KEY_DLEFT, KEY_DRIGHT, KEY_B, KEY_A
    };
    if (keys & sequence[konami_progress]) {
        ++konami_progress;
        if (konami_progress == sizeof(sequence) / sizeof(sequence[0])) {
            konami_progress = 0;
            return true;
        }
    } else if (keys & (KEY_DUP | KEY_DDOWN | KEY_DLEFT | KEY_DRIGHT |
                       KEY_A | KEY_B)) {
        konami_progress = (keys & KEY_DUP) ? 1 : 0;
    }
    return false;
}

void move_down(const File& file);
void move_up(const File& file);

bool contains_extension(const std::vector<std::string>& ids,
                        const std::string& id) {
    return std::find(ids.begin(), ids.end(), id) != ids.end();
}

const std::vector<std::string>& supported_text_extensions() {
    static const std::vector<std::string> extensions = {
        ".txt", ".md", ".markdown", ".ini", ".cfg", ".conf", ".log",
        ".csv", ".json", ".xml", ".yaml", ".yml", ".toml", ".html",
        ".htm", ".css", ".js", ".ts", ".c", ".h", ".cpp", ".hpp",
        ".cc", ".java", ".py", ".rb", ".lua", ".sh", ".bat", ".ps1",
        ".rs", ".go", ".swift", ".kt", ".asm", ".s", ".sql", ".tex",
        ".gitignore", ".makefile"
    };
    return extensions;
}

bool supported_text_extension(const std::string& extension) {
    const std::vector<std::string>& supported = supported_text_extensions();
    return std::find(supported.begin(), supported.end(), extension) != supported.end();
}

std::string choose_save_extension() {
    const std::vector<std::string>& formats = supported_text_extensions();
    unsigned int selected = 0;
    while (aptMainLoop()) {
        print_format_menu(formats[selected]);
        hidScanInput();
        const u32 keys = hidKeysDown();
        if (keys & KEY_B)
            return std::string();
        if ((keys & KEY_DDOWN) && selected + 1 < formats.size())
            ++selected;
        if ((keys & KEY_DUP) && selected > 0)
            --selected;
        if (keys & KEY_A)
            return formats[selected];
        gspWaitForVBlank();
    }
    return std::string();
}

bool choose_file(std::string& filename) {
    std::vector<std::string> files = list_files();
    unsigned int selected = 0;
    while (aptMainLoop()) {
        print_file_browser(files, selected);
        hidScanInput();
        const u32 keys = hidKeysDown();
        if (keys & KEY_B) {
            if (file_io_directory() == bandolero_files_path())
                return false;
            std::string parent = file_io_directory();
            while (!parent.empty() && parent.back() == '/')
                parent.pop_back();
            const std::string::size_type slash = parent.find_last_of('/');
            if (slash != std::string::npos) {
                parent.erase(slash + 1);
                file_io_set_directory(parent);
            }
            files = list_files();
            selected = 0;
            continue;
        }
        if ((keys & KEY_DDOWN) && selected + 1 < files.size())
            ++selected;
        if ((keys & KEY_DUP) && selected > 0)
            --selected;
        if (keys & KEY_TOUCH) {
            touchPosition touch;
            hidTouchRead(&touch);
            if (touch.px >= 18 && touch.px <= 382 && touch.py >= 48 &&
                touch.py < 218) {
                const unsigned int row = (touch.py - 48) / 11;
                const unsigned int first = selected > 12 ? selected - 12 : 0;
                if (first + row < files.size())
                    selected = first + row;
            }
        }
        if ((keys & KEY_A) && !files.empty()) {
            const std::string& selected_name = files[selected];
            if (!selected_name.empty() && selected_name.back() == '/') {
                file_io_set_directory(file_io_directory() + selected_name);
                files = list_files();
                selected = 0;
            } else {
                filename = selected_name;
                return true;
            }
        }
        gspWaitForVBlank();
    }
    return false;
}

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
    const std::vector<ExtensionInfo> extensions =
        config_extensions_enabled() ? load_extension_catalog() :
        std::vector<ExtensionInfo>();
    config_log_event("Aplicación abierta");
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

    std::vector<std::string> known_extensions = config_known_extensions();
    if (config_extension_notifications()) {
        for (const ExtensionInfo& extension : extensions) {
            if (extension.id.empty() || contains_extension(known_extensions, extension.id))
                continue;
            while (aptMainLoop()) {
                print_extension_notice(extension.name.empty() ? extension.id :
                                           extension.name,
                                       extension.description);
                hidScanInput();
                const u32 notice_keys = hidKeysDown();
                if (notice_keys & KEY_A)
                    break;
                gspWaitForVBlank();
            }
            known_extensions.push_back(extension.id);
            config_log_event("Nueva extensión detectada: " +
                             (extension.name.empty() ? extension.id : extension.name));
        }
    } else {
        for (const ExtensionInfo& extension : extensions) {
            if (!extension.id.empty() &&
                !contains_extension(known_extensions, extension.id))
                known_extensions.push_back(extension.id);
        }
    }
    config_set_known_extensions(known_extensions);
    config_commit();

    update_screen(file, curr_line);

	while (aptMainLoop())
	{

		hidScanInput();

		u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();
        const bool dedication_unlocked = update_konami_code(kDown);
        if (dedication_unlocked && !dedication_consumed) {
            dedication_consumed = true;
            dedication_screen = true;
            show_about = true;
            print_dedication();
            kDown = 0;
        }

		if (kDown & KEY_TOUCH) {
		    touchPosition touch;
		    hidTouchRead(&touch);
		    if (touch.py >= 72 && touch.py < 91) {
		        if (touch.px < 160)
		            kDown |= KEY_A;
		        else
		            kDown |= KEY_B;
		    } else if (touch.py >= 95 && touch.py < 114) {
		        if (touch.px < 160)
		            kDown |= KEY_X;
		        else
		            kDown |= KEY_Y;
		    } else if (touch.py >= 118 && touch.py < 137) {
		        if (touch.px < 160)
		            kDown |= KEY_R;
		        else
		            kHeld |= KEY_L;
		    } else if (touch.py >= 145 && touch.py < 164) {
		        if (touch.px < 160)
		            kDown |= KEY_DDOWN;
		        else
		            kDown |= KEY_SELECT;
		    }
		}

        if (kDown & KEY_START)
            break;

        if (dedication_screen) {
            if (kDown & (KEY_A | KEY_B | KEY_SELECT)) {
                dedication_screen = false;
                print_about();
            }
            gspWaitForVBlank();
            continue;
        }

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
            if (kDown & KEY_TOUCH) {
                touchPosition about_touch;
                hidTouchRead(&about_touch);
                if (about_touch.py >= 190 && about_touch.py < 211 &&
                    about_touch.px < 155) {
                    config_set_extension_notifications(
                        !config_extension_notifications());
                    config_commit();
                    print_about();
                } else if (about_touch.py >= 212 && about_touch.py < 239) {
                    if (about_touch.px < 155) {
                        show_about = false;
                        update_screen(file, curr_line);
                    } else if (about_touch.px >= 160) {
                        language = static_cast<AppLanguage>((language + 1) % 6);
                        localization_set_language(language);
                        config_set_language(language);
                        config_commit_language();
                        print_about();
                    }
                }
            }
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
            if (kDown & KEY_X) {
                config_set_extension_notifications(
                    !config_extension_notifications());
                config_commit();
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
                const size_t copy_length = std::min(line->size(), sizeof(current_text) - 1);
                std::copy(line->begin(), line->begin() + copy_length, current_text);
                current_text[copy_length] = '\0';
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
            const std::string::size_type dot = filename.find_last_of('.');
            const std::string extension =
                dot == std::string::npos ? std::string() : filename.substr(dot);
            std::string selected_extension = extension;
            if (selected_extension.empty() ||
                !supported_text_extension(selected_extension)) {
                selected_extension = choose_save_extension();
                if (selected_extension.empty()) {
                    update_screen(file, curr_line);
                    continue;
                }
                if (extension.empty())
                    filename += selected_extension;
                else {
                    filename.erase(dot);
                    filename += selected_extension;
                }
            }
            if (filename.find('.') == std::string::npos)
                filename += ".txt";

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
            curr_line = 0;
            scroll = 0;
            std::string filename;
            if (!choose_file(filename)) {
                update_screen(file, curr_line);
                continue;
            }
            File oldfile = file;
            file = open_file(filename);
            const std::string::size_type extension_start = filename.find_last_of('.');
            const std::string extension = extension_start == std::string::npos
                ? std::string() : filename.substr(extension_start);
            display_set_markdown_preview(extension == ".md" ||
                                         extension == ".markdown");
            config_log_event("Archivo abierto: " + filename);
            
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
            scroll = curr_line > MAX_BOTTOM_SIZE ? curr_line - MAX_BOTTOM_SIZE : 0;

        } else {

            if (curr_line >= static_cast<unsigned int>(scroll + MAX_BOTTOM_SIZE) &&
                curr_line < file.lines.size()) {
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
