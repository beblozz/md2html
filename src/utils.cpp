#include "utils.h"

#include <fstream>
#include <sstream>

std::string trim(const std::string& text) {
    const std::string spaces = " \t\r\n";
    size_t first = text.find_first_not_of(spaces);
    if (first == std::string::npos) {
        return "";
    }
    size_t last = text.find_last_not_of(spaces);
    return text.substr(first, last - first + 1);
}

bool startsWith(const std::string& text, const std::string& prefix) {
    if (prefix.size() > text.size()) {
        return false;
    }
    return text.compare(0, prefix.size(), prefix) == 0;
}

std::string escapeHtml(const std::string& text) {
    std::string result;
    for (char c : text) {
        if (c == '&') {
            result += "&amp;";
        } else if (c == '<') {
            result += "&lt;";
        } else if (c == '>') {
            result += "&gt;";
        } else if (c == '"') {
            result += "&quot;";
        } else {
            result += c;
        }
    }
    return result;
}

std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

bool readFile(const std::string& path, std::string& content) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    content = buffer.str();
    return true;
}

bool writeFile(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    return true;
}
