#include "file.h"
#include <algorithm>

#include <string.h>
void File::add_line(const std::vector<char>& new_text) {
    std::vector<char> line(new_text);
    if (line.empty() || line.back() != '\n')
        line.push_back('\n');
    lines.push_back(line);
}

void File::edit_line(const std::vector<char>& new_text, unsigned int line) {
    if (line >= lines.size())
        return;

    std::vector<char> replacement(new_text);
    if (replacement.empty() || replacement.back() != '\n')
        replacement.push_back('\n');

    auto line_iter = lines.begin();
    if (line > 0)
        advance(line_iter, line);
    line_iter = lines.erase(line_iter);
    lines.insert(line_iter, replacement);

}

int File::find(const char* search_term) {
    int line_number = 0;
    if (search_term[0] == '\0')
        return -1;
    for(auto line : this->lines) {
        auto it = std::search(line.begin(), line.end(), search_term, search_term + strlen(search_term));
        if (it != line.end())
            return line_number;
        line_number++;
        
    }

    return -1;

}

std::vector<char> char_arr_to_vector(const char* arr) {
    std::vector<char> text;
    while(*arr != '\0') {
        text.push_back(*arr);
        arr++;
    }
    return text;
}
