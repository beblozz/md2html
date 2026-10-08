#pragma once

#include <string>
#include <vector>

std::string trim(const std::string& text);
bool startsWith(const std::string& text, const std::string& prefix);
std::string escapeHtml(const std::string& text);
std::vector<std::string> splitLines(const std::string& text);
bool readFile(const std::string& path, std::string& content);
bool writeFile(const std::string& path, const std::string& content);
