#include "display.h"
#include "localization.h"
#include "text_utils.h"
#include "config.h"

#include <3ds.h>
#include <citro2d.h>
#include <c2d/spritesheet.h>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <algorithm>

namespace {
bool markdown_preview = false;
}

namespace {
constexpr float TOP_WIDTH = 400.0f;
constexpr float SCREEN_HEIGHT = 240.0f;
constexpr float BOTTOM_WIDTH = 320.0f;
constexpr float READABILITY_SCALE = 1.45f;
constexpr char APP_VERSION[] = "VERSION 2.3.1 (CITRO2D)";
constexpr char APP_BUILD[] = "BUILD 2026.10.16";
u32 background_color = C2D_Color32(14, 19, 34, 255);
u32 panel_color = C2D_Color32(25, 33, 55, 255);
u32 panel_alt_color = C2D_Color32(31, 42, 70, 255);
u32 accent_color = C2D_Color32(190, 25, 45, 255);
u32 cyan_color = C2D_Color32(68, 210, 224, 255);
u32 text_color = C2D_Color32(244, 247, 255, 255);
u32 muted_color = C2D_Color32(155, 170, 195, 255);
u32 selected_color = C2D_Color32(250, 190, 74, 255);
u32& BACKGROUND = background_color;
u32& PANEL = panel_color;
u32& PANEL_ALT = panel_alt_color;
u32& ACCENT = accent_color;
u32& CYAN = cyan_color;
u32& TEXT = text_color;
u32& MUTED = muted_color;
u32& SELECTED = selected_color;
constexpr size_t TEXT_BUFFER_SIZE = 262144;

C3D_RenderTarget* top_target = nullptr;
C3D_RenderTarget* bottom_target = nullptr;
C2D_TextBuf text_buffer = nullptr;
C2D_Font custom_font = nullptr;
C2D_SpriteSheet icon_sheet = nullptr;
C2D_SpriteSheet qr_sheet = nullptr;
C2D_SpriteSheet logo_sheet = nullptr;
C2D_Image program_icon = {};
C2D_Image github_qr = {};
C2D_Image flagwave_logo = {};
std::string save_status;
std::string current_filename;
const File* current_file = nullptr;
unsigned int selected_line = 0;
bool about_visible = false;
std::vector<std::string> loaded_extensions;

u32 parse_color(const std::string& value, u32 fallback) {
    if (value.size() != 7 || value[0] != '#')
        return fallback;
    char* end = nullptr;
    const unsigned long rgb = std::strtoul(value.c_str() + 1, &end, 16);
    if (!end || *end != '\0' || rgb > 0xffffff)
        return fallback;
    return C2D_Color32((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff, 255);
}

void apply_theme_value(u32& target, const std::string& value) {
    target = parse_color(value, target);
}

void text(C2D_Text& output, const char* value) {
    if (custom_font)
        C2D_TextFontParse(&output, custom_font, text_buffer, value);
    else
        C2D_TextParse(&output, text_buffer, value);
}

void draw_text(const char* value, float x, float y, float scale, u32 color,
               u32 flags = C2D_WithColor) {
    C2D_Text output;
    text(output, value);
    const float readable_scale = scale * READABILITY_SCALE;
    C2D_DrawText(&output, flags, x, y, 0.5f, readable_scale, readable_scale,
                 color);
}

void draw_text(const std::string& value, float x, float y, float scale, u32 color,
               u32 flags = C2D_WithColor) {
    draw_text(value.c_str(), x, y, scale, color, flags);
}

void draw_centered(const char* value, float center_x, float y, float scale,
                   u32 color) {
    C2D_Text output;
    text(output, value);
    const float readable_scale = scale * READABILITY_SCALE;
    float width = 0.0f;
    float height = 0.0f;
    C2D_TextGetDimensions(&output, readable_scale, readable_scale, &width,
                          &height);
    C2D_DrawText(&output, C2D_WithColor, center_x - width * 0.5f, y, 0.5f,
                 readable_scale, readable_scale, color);
}

void draw_centered(const std::string& value, float center_x, float y, float scale,
                   u32 color) {
    draw_centered(value.c_str(), center_x, y, scale, color);
}

void draw_header(float width, const char* section) {
    C2D_DrawRectSolid(0, 0, 0.5f, width, 38, accent_color);
    draw_text("BANDOLERO 3DS", 12, 7, 0.63f, text_color);
    draw_text(section, width - 126, 11, 0.42f, text_color);
}

void draw_action_button(float x, float y, float width, const char* label,
                        u32 color) {
    C2D_DrawRectSolid(x, y, 0.5f, width, 19, panel_alt_color);
    C2D_DrawRectSolid(x, y, 0.5f, 3, 19, color);
    draw_text(label, x + 8, y + 5, 0.29f, text_color);
}

void draw_markdown_line(const std::string& source, float x, float y,
                        float scale, u32 color) {
    std::string value = source;
    bool heading = false;
    if (value.size() >= 2 && value[0] == '#' && value[1] == ' ') {
        value.erase(0, 2);
        heading = true;
    }
    const float line_scale = heading ? scale + 0.03f : scale;
    bool bold = false;
    float cursor = x;
    std::string segment;
    for (size_t index = 0; index <= value.size(); ++index) {
        const bool marker = index + 1 < value.size() &&
                            value[index] == '*' && value[index + 1] == '*';
        if (marker || index == value.size()) {
            if (!segment.empty()) {
                draw_text(segment, cursor, y, line_scale,
                          heading ? selected_color : color);
                C2D_Text measured;
                text(measured, segment.c_str());
                float width = 0.0f;
                float height = 0.0f;
                C2D_TextGetDimensions(&measured, line_scale * READABILITY_SCALE,
                                      line_scale * READABILITY_SCALE, &width, &height);
                if (bold) {
                    draw_text(segment, cursor + 0.7f, y, line_scale,
                              heading ? selected_color : color);
                }
                cursor += width;
                segment.clear();
            }
            if (marker) {
                bold = !bold;
                ++index;
            }
        } else {
            segment += value[index];
        }
    }
}

void draw_editor(const File& file, unsigned int current_line) {
    C2D_TargetClear(top_target, background_color);
    C2D_SceneBegin(top_target);
    draw_header(TOP_WIDTH, tr(TEXT_APP_TITLE));
    C2D_DrawRectSolid(0, 38, 0.5f, TOP_WIDTH, 3, CYAN);

    std::list<std::vector<char>>::const_iterator line = file.lines.begin();
    const unsigned int first_line = static_cast<unsigned int>(scroll);
    if (first_line < file.lines.size())
        std::advance(line, first_line);
    constexpr float editor_line_height = 9.35f;
    constexpr unsigned int visible_lines = VISIBLE_EDITOR_LINES;
    for (unsigned int index = 0; index < visible_lines; ++index) {
        const float y = 43.0f + index * editor_line_height;
        const unsigned int line_number = first_line + index;
        const bool selected = line_number == current_line;
        if (selected)
            C2D_DrawRectSolid(5, y - 1, 0.45f, 390, 10, panel_alt_color);

        char number[8];
        std::snprintf(number, sizeof(number), "%02u", line_number + 1);
        draw_text(number, 9, y, 0.25f, selected ? selected_color : muted_color);

        if (line != file.lines.end()) {
            std::string value = char_vec_to_string(*line);
            if (!value.empty() && value[value.size() - 1] == '\n')
                value.erase(value.size() - 1);
            if (value.empty())
                value = tr(TEXT_EMPTY_LINE);
            const bool glyphs_valid = utf8_is_valid(value);
            const std::string display_value = utf8_display_prefix(value, 53);
            if (markdown_preview)
                draw_markdown_line(display_value, 43, y, 0.28f,
                                   !glyphs_valid ? accent_color :
                                   selected ? text_color : muted_color);
            else
                draw_text(display_value, 43, y, 0.28f,
                          !glyphs_valid ? accent_color :
                          selected ? text_color : muted_color);
            ++line;
        }
    }

    C2D_DrawRectSolid(0, 226, 0.5f, TOP_WIDTH, 14, panel_color);
    char footer[64];
    std::snprintf(footer, sizeof(footer), tr(TEXT_CURRENT_LINE), current_line + 1);
    char line_suffix[32];
    std::snprintf(line_suffix, sizeof(line_suffix), "  |  %d  |  UTF-8",
                  file.size());
    std::strncat(footer, line_suffix, sizeof(footer) - std::strlen(footer) - 1);
    draw_text(footer, 12, 229, 0.28f, cyan_color);
    if (first_line + visible_lines < file.lines.size())
        draw_text("...", 378, 229, 0.28f, selected_color);
    draw_text(markdown_preview ? "MD PREVIEW" : "UTF-8 GLYPHS OK",
              260, 229, 0.22f, markdown_preview ? cyan_color : muted_color);
}

void draw_bottom() {
    C2D_TargetClear(bottom_target, background_color);
    C2D_SceneBegin(bottom_target);
    draw_header(BOTTOM_WIDTH, tr(TEXT_MENU));
    C2D_DrawRectSolid(8, 48, 0.5f, 304, 142, panel_color);
    draw_centered(tr(TEXT_ACTIONS), 160, 54, 0.36f, cyan_color);
    draw_action_button(14, 72, 143, tr(TEXT_EDIT), accent_color);
    draw_action_button(163, 72, 143, tr(TEXT_NEW_FILE), cyan_color);
    draw_action_button(14, 95, 143, tr(TEXT_SAVE_FILE), selected_color);
    draw_action_button(163, 95, 143, tr(TEXT_OPEN_FILE), cyan_color);
    draw_action_button(14, 118, 143, tr(TEXT_SEARCH), accent_color);
    draw_action_button(163, 118, 143, tr(TEXT_JUMP), selected_color);
    draw_action_button(14, 145, 143, tr(TEXT_MOVE), muted_color);
    draw_action_button(163, 145, 143, tr(TEXT_ABOUT), accent_color);
    C2D_DrawRectSolid(8, 196, 0.5f, 304, 22, panel_alt_color);
    std::string status = save_status.empty() ? tr(TEXT_READY) : save_status;
    if (!current_filename.empty()) {
        status += " | ";
        status += current_filename;
    }
    draw_text(utf8_display_prefix(status, 48), 15, 203, 0.27f,
              save_status.empty() ? cyan_color : selected_color);
}
}

void display_set_markdown_preview(bool enabled) {
    markdown_preview = enabled;
}

bool display_init() {
    if (!C3D_Init(C3D_DEFAULT_CMDBUF_SIZE))
        return false;
    if (!C2D_Init(C2D_DEFAULT_MAX_OBJECTS)) {
        C3D_Fini();
        return false;
    }
    C2D_Prepare();
    custom_font = C2D_FontLoad("romfs:/NotoSansCJKjp.bcfnt");
    icon_sheet = C2D_SpriteSheetLoad("romfs:/icon.t3x");
    qr_sheet = C2D_SpriteSheetLoad("romfs:/github-qr.t3x");
    logo_sheet = C2D_SpriteSheetLoad("romfs:/flagwave-logo.t3x");
    top_target = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom_target = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    text_buffer = C2D_TextBufNew(TEXT_BUFFER_SIZE);
    if (icon_sheet)
        program_icon = C2D_SpriteSheetGetImage(icon_sheet, 0);
    if (qr_sheet)
        github_qr = C2D_SpriteSheetGetImage(qr_sheet, 0);
    if (logo_sheet)
        flagwave_logo = C2D_SpriteSheetGetImage(logo_sheet, 0);
    return top_target != nullptr && bottom_target != nullptr &&
           text_buffer != nullptr;
}

void display_exit() {
    if (icon_sheet)
        C2D_SpriteSheetFree(icon_sheet);
    if (qr_sheet)
        C2D_SpriteSheetFree(qr_sheet);
    if (logo_sheet)
        C2D_SpriteSheetFree(logo_sheet);
    if (custom_font)
        C2D_FontFree(custom_font);
    if (text_buffer)
        C2D_TextBufDelete(text_buffer);
    C2D_Fini();
    C3D_Fini();
}

void update_screen(const File& file, unsigned int current_line) {
    about_visible = false;
    current_file = &file;
    selected_line = current_line;
    if (!about_visible) {
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TextBufClear(text_buffer);
        draw_editor(file, current_line);
        draw_bottom();
        C3D_FrameEnd(0);
    }
}

void print_loading_screen() {
    for (int frame = 0; frame < 30; ++frame) {
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TextBufClear(text_buffer);
        C2D_TargetClear(top_target, BACKGROUND);
        C2D_SceneBegin(top_target);
        draw_centered("BANDOLERO 3DS", 200, 62, 0.68f, TEXT);
        draw_centered(tr(TEXT_LOADING_SUBTITLE), 200, 91, 0.34f, CYAN);
        draw_centered(tr(TEXT_LOADING_VERSION), 200, 116, 0.37f, SELECTED);
        const float angle = static_cast<float>(frame) * 0.314159f;
        const float cx = 200.0f;
        const float cy = 166.0f;
        for (int dot = 0; dot < 8; ++dot) {
            const float a = angle + dot * 0.785398f;
            const float x = cx + std::cos(a) * 22.0f;
            const float y = cy + std::sin(a) * 22.0f;
            const u32 color = dot == 0 ? TEXT : ACCENT;
            C2D_DrawCircleSolid(x, y, 0.5f, 4.0f, color);
        }
        draw_centered(tr(TEXT_LOADING), 200, 203, 0.38f, MUTED);
        C2D_TargetClear(bottom_target, BACKGROUND);
        C2D_SceneBegin(bottom_target);
        C2D_DrawRectSolid(44, 72, 0.5f, 232, 94, PANEL);
        draw_centered("BANDOLERO 3DS", 160, 92, 0.56f, TEXT);
        draw_centered(tr(TEXT_VERSION), 160, 121, 0.30f, CYAN);
        draw_centered("TRISTANABS", 160, 148, 0.35f, MUTED);
        draw_centered(tr(TEXT_LOADING), 160, 181, 0.39f, SELECTED);
        C3D_FrameEnd(0);
        svcSleepThread(100000000LL);
    }
}

void print_about() {
    about_visible = true;
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top_target, BACKGROUND);
    C2D_SceneBegin(top_target);
    C2D_TextBufClear(text_buffer);
    draw_header(TOP_WIDTH, tr(TEXT_ABOUT_TITLE));
    C2D_DrawRectSolid(0, 38, 0.5f, TOP_WIDTH, 3, CYAN);
    C2D_DrawRectSolid(12, 52, 0.5f, 376, 176, PANEL);
    C2D_DrawRectSolid(22, 63, 0.5f, 64, 64, PANEL_ALT);
    if (program_icon.tex)
        C2D_DrawImageAt(program_icon, 27, 68, 0.5f, nullptr, 0.76f, 0.76f);
    draw_text(tr(TEXT_APP_TITLE), 101, 61, 0.45f, TEXT);
    draw_text(tr(TEXT_APP_SUBTITLE), 101, 84, 0.30f, CYAN);
    draw_text(APP_VERSION, 101, 108, 0.32f, SELECTED);
    draw_text(APP_BUILD, 101, 127, 0.30f, SELECTED);
    draw_text(tr(TEXT_DEVELOPER), 101, 147, 0.30f, MUTED);
    draw_text(tr(TEXT_COPYRIGHT), 101, 166, 0.30f, MUTED);
    draw_text("Licencia MIT", 101, 185, 0.28f, CYAN);
    if (flagwave_logo.tex)
        C2D_DrawImageAt(flagwave_logo, 300, 184, 0.5f, nullptr, 0.34f, 0.34f);

    C2D_TargetClear(bottom_target, BACKGROUND);
    C2D_SceneBegin(bottom_target);
    draw_header(BOTTOM_WIDTH, tr(TEXT_ABOUT_LINKS));
    C2D_DrawRectSolid(12, 48, 0.5f, 296, 142, PANEL);
    if (github_qr.tex)
        C2D_DrawImageAt(github_qr, 28, 60, 0.5f, nullptr, 0.68f, 0.68f);
    draw_text(tr(TEXT_ABOUT_PROJECT), 174, 65, 0.38f, CYAN);
    draw_text(tr(TEXT_ABOUT_SCAN_QR), 174, 88, 0.32f, TEXT);
    draw_text(tr(TEXT_GITHUB), 174, 107, 0.25f, MUTED);
    std::string extension_summary = tr(TEXT_ABOUT_EXTENSIONS);
    extension_summary += ": ";
    for (unsigned int index = 0; index < loaded_extensions.size(); ++index) {
        if (index != 0)
            extension_summary += ", ";
        extension_summary += loaded_extensions[index];
    }

    if (loaded_extensions.empty())
        extension_summary += "N/A";
    draw_text(utf8_display_prefix(extension_summary, 29), 18, 171, 0.27f,
              SELECTED);
    draw_action_button(14, 190, 150, "X: AVISOS .BAN3",
                       accent_color);
    draw_text(config_extension_notifications() ? tr(TEXT_ABOUT_ENABLED) :
              tr(TEXT_ABOUT_DISABLED), 157, 196, 0.24f,
              config_extension_notifications() ? CYAN : MUTED);
    draw_action_button(14, 213, 136, ("SELECT  " +
                       std::string(tr(TEXT_ABOUT_BACK))).c_str(), muted_color);
    draw_action_button(163, 213, 143, "R/L: IDIOMA", cyan_color);
    C3D_FrameEnd(0);
}

void print_dedication() {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top_target, background_color);
    C2D_SceneBegin(top_target);
    draw_header(TOP_WIDTH, "DEDICATORIA");
    C2D_DrawRectSolid(18, 52, 0.5f, 364, 164, panel_color);
    draw_text("Basado originalmente en Notepad3DS", 32, 68, 0.30f, cyan_color);
    draw_text("He pensado que podría incluir aquí una pequeña dedicatoria,", 32, 94,
              0.22f, text_color);
    draw_text("como los autores ponen en sus libros, así que muchas gracias a", 32, 110,
              0.22f, text_color);
    draw_text("todas las personas que hacen la scene hispana, a todos los usuarios", 32,
              126, 0.22f, text_color);
    draw_text("que nos apoyan día a día pero sobre todo a mis amigos y compañeros", 32,
              142, 0.22f, text_color);
    draw_text("de Homebrew Central y La presa, De corazón, gracias.", 32, 158,
              0.22f, text_color);
    draw_text("-Tristanabs", 32, 186, 0.28f, selected_color);
    C2D_TargetClear(bottom_target, background_color);
    C2D_SceneBegin(bottom_target);
    draw_header(BOTTOM_WIDTH, "SECRETO");
    C2D_DrawRectSolid(20, 58, 0.5f, 280, 116, panel_alt_color);
    draw_centered("DEDICATORIA DESBLOQUEADA", 160, 78, 0.31f, selected_color);
    draw_centered("Gracias por apoyar Bandolero 3DS", 160, 110, 0.28f, text_color);
    draw_centered("A / B / SELECT: VOLVER", 160, 146, 0.28f, cyan_color);
    C3D_FrameEnd(0);
}

void print_file_browser(const std::vector<std::string>& files,
                        unsigned int selected) {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TextBufClear(text_buffer);
    C2D_TargetClear(top_target, BACKGROUND);
    C2D_SceneBegin(top_target);
    draw_header(TOP_WIDTH, tr(TEXT_FILE_BROWSER));
    C2D_DrawRectSolid(12, 48, 0.5f, 376, 178, PANEL);
    const unsigned int first = selected > 12 ? selected - 12 : 0;
    for (unsigned int index = first; index < files.size() && index < first + 15;
         ++index) {
        const float y = 57.0f + (index - first) * 11.0f;
        const bool active = index == selected;
        if (active)
            C2D_DrawRectSolid(18, y - 1, 0.45f, 364, 11, PANEL_ALT);
        draw_text(utf8_display_prefix(files[index], 48), 25, y, 0.31f,
                  active ? SELECTED : TEXT);
    }
    if (files.empty())
        draw_centered(tr(TEXT_NO_FILE), 200, 112, 0.42f, MUTED);
    draw_text("A ABRIR   B CANCELAR   DPAD MOVER", 22, 216, 0.29f, CYAN);
    C2D_TargetClear(bottom_target, BACKGROUND);
    C2D_SceneBegin(bottom_target);
    draw_header(BOTTOM_WIDTH, "EXPLORADOR");
    draw_centered("SELECCIONA UN ARCHIVO", 160, 78, 0.42f, TEXT);
    draw_centered("A para abrir", 160, 112, 0.38f, SELECTED);
    draw_centered("B para volver", 160, 140, 0.35f, MUTED);
    C3D_FrameEnd(0);
}

void print_extension_notice(const std::string& name,
                            const std::string& description) {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TextBufClear(text_buffer);
    C2D_TargetClear(top_target, BACKGROUND);
    C2D_SceneBegin(top_target);
    draw_header(TOP_WIDTH, tr(TEXT_NEW_EXTENSION));
    draw_centered(tr(TEXT_NEW_EXTENSION), 200, 72, 0.46f, SELECTED);
    draw_centered(utf8_display_prefix(name, 30), 200, 112, 0.46f, TEXT);
    draw_centered(utf8_display_prefix(description, 52), 200, 142, 0.30f, CYAN);
    draw_centered("Pulsa A para continuar", 200, 201, 0.34f, MUTED);
    C2D_TargetClear(bottom_target, BACKGROUND);
    C2D_SceneBegin(bottom_target);
    draw_header(BOTTOM_WIDTH, "EXTENSION");
    draw_centered("Manifiesto de datos seguro", 160, 86, 0.37f, TEXT);
    draw_centered("No ejecuta codigo nativo", 160, 119, 0.35f, CYAN);
    draw_centered("A continuar", 160, 166, 0.35f, SELECTED);
    C3D_FrameEnd(0);
}

void print_format_menu(const std::string& current_format) {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TextBufClear(text_buffer);
    C2D_TargetClear(top_target, BACKGROUND);
    C2D_SceneBegin(top_target);
    draw_header(TOP_WIDTH, tr(TEXT_FORMAT_MENU));
    draw_centered(tr(TEXT_FORMAT_PROMPT), 200, 64, 0.46f, TEXT);
    draw_centered("TXT   MD   INI   C   CPP   H   PY", 200, 105, 0.34f, CYAN);
    draw_centered("Actual: " + current_format, 200, 139, 0.36f, SELECTED);
    draw_centered("A OK   B " + std::string(tr(TEXT_FORMAT_CANCEL)), 200, 205, 0.34f, MUTED);
    C2D_TargetClear(bottom_target, BACKGROUND);
    C2D_SceneBegin(bottom_target);
    draw_header(BOTTOM_WIDTH, tr(TEXT_FORMAT_MENU));
    draw_centered("Texto compatible", 160, 82, 0.40f, TEXT);
    draw_centered("Sin ejecucion de codigo", 160, 119, 0.32f, CYAN);
    draw_centered("A aceptar  B cancelar", 160, 165, 0.33f, SELECTED);
    C3D_FrameEnd(0);
}

void print_language_setup(AppLanguage language) {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TextBufClear(text_buffer);
    C2D_TargetClear(top_target, BACKGROUND);
    C2D_SceneBegin(top_target);
    draw_centered(tr(TEXT_LANGUAGE_PROMPT), 200, 72, 0.55f, TEXT);
    draw_centered(language_name(language), 200, 122, 0.62f, SELECTED);
    draw_centered(tr(TEXT_LANGUAGE_HELP), 200, 188, 0.32f, CYAN);
    C2D_TargetClear(bottom_target, BACKGROUND);
    C2D_SceneBegin(bottom_target);
    draw_centered(tr(TEXT_LANGUAGE), 160, 72, 0.48f, TEXT);
    draw_centered(language_name(language), 160, 112, 0.45f, SELECTED);
    draw_centered("R / L", 160, 154, 0.35f, MUTED);
    C3D_FrameEnd(0);
}

void print_first_run_info() {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TextBufClear(text_buffer);
    C2D_TargetClear(top_target, BACKGROUND);
    C2D_SceneBegin(top_target);
    draw_centered(tr(TEXT_FIRST_RUN_TITLE), 200, 50, 0.58f, TEXT);
    draw_centered(tr(TEXT_FIRST_RUN_LINE_1), 200, 92, 0.34f, CYAN);
    draw_centered(tr(TEXT_FIRST_RUN_LINE_2), 200, 116, 0.34f, TEXT);
    draw_centered(tr(TEXT_FIRST_RUN_LINE_3), 200, 151, 0.30f, MUTED);
    draw_centered(tr(TEXT_FIRST_RUN_CONTINUE), 200, 202, 0.36f, SELECTED);
    C2D_TargetClear(bottom_target, BACKGROUND);
    C2D_SceneBegin(bottom_target);
    draw_centered(tr(TEXT_APP_TITLE), 160, 98, 0.55f, TEXT);
    draw_centered(tr(TEXT_APP_SUBTITLE), 160, 130, 0.29f, CYAN);
    C3D_FrameEnd(0);
}

void print_save_status(std::string message) {
    save_status = message;
}

void clear_save_status() {
    save_status.clear();
}

void clear_directory_status() {
    current_filename.clear();
}

void print_directory_status(std::string filename) {
    current_filename = filename;
}

void print_instructions() {}
void print_version(const char*) {}
void display_set_extensions(const std::vector<std::string>& names) {
    loaded_extensions = names;
}

void display_apply_theme(const std::string& background, const std::string& panel,
                         const std::string& accent, const std::string& text,
                         const std::string& cyan) {
    apply_theme_value(background_color, background);
    apply_theme_value(panel_color, panel);
    apply_theme_value(accent_color, accent);
    apply_theme_value(text_color, text);
    apply_theme_value(cyan_color, cyan);
    panel_alt_color = panel_color;
}

std::string char_vec_to_string(const std::vector<char>& line) {
    return std::string(line.begin(), line.end());
}
