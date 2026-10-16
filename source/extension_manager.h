#pragma once

#include <string>
#include <vector>

struct ExtensionInfo {
    std::string id;
    std::string name;
    std::string extension;
    std::string description;
    std::string theme;
    std::string background;
    std::string panel;
    std::string accent;
    std::string text;
    std::string cyan;
};

// Reads safe, data-only .ban3 manifests from sdmc:/3ds/bandolero/extensions.
// Native code is never loaded.
std::vector<ExtensionInfo> load_extension_catalog();
