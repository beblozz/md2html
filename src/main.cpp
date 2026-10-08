#include <filesystem>
#include <iostream>
#include <string>

#include "page.h"
#include "parser.h"
#include "utils.h"

void printUsage() {
    std::cout << "Usage: md2html input.md [options]\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "  -o FILE        write result to FILE instead of the console\n";
    std::cout << "  --page         make a full HTML page, not only the body\n";
    std::cout << "  --title TEXT   page title (works with --page)\n";
    std::cout << "  --css PATH     path to css file (works with --page)\n";
    std::cout << "  -h, --help     show this help\n";
}

int main(int argc, char* argv[]) {
    std::string inputPath;
    std::string outputPath;
    std::string title;
    std::string cssPath = "style.css";
    bool fullPage = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printUsage();
            return 0;
        } else if (arg == "--page") {
            fullPage = true;
        } else if (arg == "-o" || arg == "--title" || arg == "--css") {
            if (i + 1 >= argc) {
                std::cerr << "md2html: option " << arg << " needs a value\n";
                return 1;
            }
            std::string value = argv[i + 1];
            i++;
            if (arg == "-o") {
                outputPath = value;
            } else if (arg == "--title") {
                title = value;
            } else {
                cssPath = value;
            }
        } else if (startsWith(arg, "-")) {
            std::cerr << "md2html: unknown option " << arg << "\n";
            return 1;
        } else if (inputPath.empty()) {
            inputPath = arg;
        } else {
            std::cerr << "md2html: only one input file is allowed\n";
            return 1;
        }
    }

    if (inputPath.empty()) {
        printUsage();
        return 1;
    }

    std::string markdown;
    if (!readFile(inputPath, markdown)) {
        std::cerr << "md2html: cannot open file " << inputPath << "\n";
        return 1;
    }

    std::string html = markdownToHtml(markdown);

    if (fullPage) {
        if (title.empty()) {
            title = std::filesystem::path(inputPath).stem().string();
        }
        html = wrapPage(html, title, cssPath);
    }

    if (outputPath.empty()) {
        std::cout << html;
    } else if (!writeFile(outputPath, html)) {
        std::cerr << "md2html: cannot write file " << outputPath << "\n";
        return 1;
    }

    return 0;
}
