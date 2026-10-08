#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "parser.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: run_tests path/to/cases\n";
        return 1;
    }

    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(argv[1])) {
        if (entry.path().extension() == ".md") {
            files.push_back(entry.path());
        }
    }
    std::sort(files.begin(), files.end());

    int passed = 0;
    int failed = 0;

    for (const auto& mdPath : files) {
        std::filesystem::path htmlPath = mdPath;
        htmlPath.replace_extension(".html");
        std::string name = mdPath.stem().string();

        std::string markdown;
        std::string expected;
        if (!readFile(mdPath.string(), markdown) || !readFile(htmlPath.string(), expected)) {
            std::cout << "[FAIL] " << name << " (cannot read files)\n";
            failed++;
            continue;
        }

        std::string actual = markdownToHtml(markdown);
        std::vector<std::string> expectedLines = splitLines(expected);
        std::vector<std::string> actualLines = splitLines(actual);

        if (expectedLines == actualLines) {
            std::cout << "[ OK ] " << name << "\n";
            passed++;
        } else {
            std::cout << "[FAIL] " << name << "\n";
            size_t count = std::max(expectedLines.size(), actualLines.size());
            for (size_t i = 0; i < count; i++) {
                std::string a = i < expectedLines.size() ? expectedLines[i] : "<no line>";
                std::string b = i < actualLines.size() ? actualLines[i] : "<no line>";
                if (a != b) {
                    std::cout << "  line " << i + 1 << "\n";
                    std::cout << "    expected: " << a << "\n";
                    std::cout << "    actual:   " << b << "\n";
                    break;
                }
            }
            failed++;
        }
    }

    std::cout << "\nPassed: " << passed << ", failed: " << failed << "\n";

    if (failed > 0 || passed == 0) {
        return 1;
    }
    return 0;
}
