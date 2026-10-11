#include "display.h"
#include "localization.h"
#include "text_utils.h"

#include <3ds.h>
#include <citro2d.h>
#include <c2d/spritesheet.h>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <algorithm>

namespace {
constexpr float TOP_WIDTH = 400.0f;
constexpr float SCREEN_HEIGHT = 240.0f;
constexpr float BOTTOM_WIDTH = 320.0f;
constexpr float READABILITY_SCALE = 1.45f;
constexpr char APP_VERSION[] = "VERSION 2.0 (CITRO2D)";
constexpr char APP_BUILD[] = "BUILD 2026.10.11";
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

void draw_editor(const File& file, unsigned int current_line) {
    C2D_TargetClear(top_target, background_color);
    C2D_SceneBegin(top_target);
    draw_header(TOP_WIDTH, tr(TEXT_APP_TITLE));
    C2D_DrawRectSolid(0, 38, 0.5f, TOP_WIDTH, 3, CYAN);

    std::list<std::vector<char>>::const_iterator line = file.lines.begin();
    constexpr float editor_line_height = 9.35f;
    constexpr unsigned int visible_lines = 19;
    for (unsigned int index = 0; index < visible_lines; ++index) {
        const float y = 43.0f + index * editor_line_height;
        const bool selected = index == current_line;
        if (selected)
            C2D_DrawRectSolid(5, y - 1, 0.45f, 390, 10, panel_alt_color);

        char number[8];
        std::snprintf(number, sizeof(number), "%02u", index + 1);
        draw_text(number, 9, y, 0.25f, selected ? selected_color : muted_color);

        if (line != file.lines.end()) {
            std::string value = char_vec_to_string(*line);
            if (!value.empty() && value[value.size() - 1] == '\n')
                value.erase(value.size() - 1);
            if (value.empty())
                value = tr(TEXT_EMPTY_LINE);
            draw_text(utf8_display_prefix(value, 53), 43, y, 0.28f,
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
}

void draw_bottom() {
    C2D_TargetClear(bottom_target, background_color);
    C2D_SceneBegin(bottom_target);
    draw_header(BOTTOM_WIDTH, tr(TEXT_MENU));
    C2D_DrawRectSolid(8, 48, 0.5f, 304, 142, panel_color);
    C2D_DrawRectSolid(8, 48, 0.5f, 5, 142, accent_color);
    draw_centered(tr(TEXT_ACTIONS), 160, 59, 0.43f, cyan_color);
    draw_text(tr(TEXT_EDIT), 20, 79, 0.37f, text_color);
    draw_text(tr(TEXT_NEW_FILE), 164, 79, 0.37f, text_color);
    draw_text(tr(TEXT_SAVE_FILE), 20, 99, 0.37f, text_color);
    draw_text(tr(TEXT_OPEN_FILE), 164, 99, 0.37f, text_color);
    draw_text(tr(TEXT_SEARCH), 20, 119, 0.37f, text_color);
    draw_text(tr(TEXT_JUMP), 164, 119, 0.37f, text_color);
    draw_text(tr(TEXT_MOVE), 20, 141, 0.35f, muted_color);
    draw_text(tr(TEXT_EXIT), 20, 161, 0.35f, muted_color);
    draw_text(tr(TEXT_ABOUT), 164, 161, 0.35f, muted_color);
    C2D_DrawRectSolid(8, 196, 0.5f, 304, 30, panel_alt_color);
    draw_text(save_status.empty() ? tr(TEXT_READY) : save_status, 18, 204, 0.33f,
              save_status.empty() ? cyan_color : selected_color);
    draw_text(current_filename.empty() ? tr(TEXT_NO_FILE) : current_filename,
              18, 219, 0.30f, muted_color);
}
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
    if (program_icon.tex)
        C2D_DrawImageAt(program_icon, 22, 76, 0.5f, nullptr, 0.88f, 0.88f);
    draw_text(tr(TEXT_APP_TITLE), 92, 60, 0.45f, TEXT);
    draw_text(tr(TEXT_APP_SUBTITLE), 92, 84, 0.34f, CYAN);
    draw_text(APP_VERSION, 92, 108, 0.35f, SELECTED);
    draw_text(APP_BUILD, 92, 130, 0.33f, SELECTED);
    draw_text(tr(TEXT_DEVELOPER), 92, 151, 0.34f, MUTED);
    draw_text(tr(TEXT_COPYRIGHT), 92, 173, 0.34f, MUTED);
    draw_text(tr(TEXT_GITHUB), 92, 195, 0.30f, TEXT);
    draw_text(tr(TEXT_LANGUAGE), 92, 217, 0.32f, CYAN);
    draw_text(language_name(localization_language()), 160, 217, 0.32f, SELECTED);
    draw_text(tr(TEXT_LANGUAGE_CHANGE), 224, 217, 0.30f, MUTED);
    if (flagwave_logo.tex)
        C2D_DrawImageAt(flagwave_logo, 286, 73, 0.5f, nullptr, 0.42f, 0.42f);

    C2D_TargetClear(bottom_target, BACKGROUND);
    C2D_SceneBegin(bottom_target);
    draw_header(BOTTOM_WIDTH, "GITHUB");
    C2D_DrawRectSolid(12, 48, 0.5f, 296, 178, PANEL);
    if (github_qr.tex)
        C2D_DrawImageAt(github_qr, 124, 59, 0.5f, nullptr, 0.60f, 0.60f);
    std::string extension_summary = "EXT: ";
    for (unsigned int index = 0; index < loaded_extensions.size(); ++index) {
        if (index != 0)
            extension_summary += ", ";
        extension_summary += loaded_extensions[index];
    }
    if (loaded_extensions.empty())
        extension_summary += "NONE";
    draw_centered(utf8_display_prefix(extension_summary, 36), 160, 139, 0.31f,
                  SELECTED);
    draw_centered("ESCANEA EL QR PARA ABRIR", 160, 186, 0.31f, TEXT);
    draw_centered("EL GITHUB DESDE TU MOVIL", 160, 201, 0.31f, CYAN);
    draw_centered("ADHERIDO A FLAGWAVE OPEN CODE", 160, 217, 0.25f, SELECTED);
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
