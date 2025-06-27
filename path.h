#pragma once

#include <string>

std::string resolvePath(const std::string& filePath) {

    std::string basePath = __FILE__;
    size_t lastSlash = basePath.find_last_of("/\\");
    std::string dir = (lastSlash != std::string::npos) ? basePath.substr(0, lastSlash + 1) : "";
    return dir + filePath;
}

bool isAbsolutePath(const std::string& path) {
#ifdef _WIN32
        return path.size() > 1 && path[1] == ':';
#else
        return !path.empty() && path[0] == '/';
#endif
}