#include "inline.h"

#include <regex>

#include "utils.h"

static const std::regex imagePattern(R"(!\[([^\]]*)\]\(([^)\s]+)\))");
static const std::regex linkPattern(R"(\[([^\]]+)\]\(([^)\s]+)\))");
static const std::regex boldPattern(R"(\*\*([^*]+)\*\*)");
static const std::regex italicPattern(R"(\*([^*]+)\*)");

static std::string applyFormatting(const std::string& text) {
    std::string result = text;
    result = std::regex_replace(result, imagePattern, "<img src=\"$2\" alt=\"$1\">");
    result = std::regex_replace(result, linkPattern, "<a href=\"$2\">$1</a>");
    result = std::regex_replace(result, boldPattern, "<strong>$1</strong>");
    result = std::regex_replace(result, italicPattern, "<em>$1</em>");
    return result;
}

std::string renderInline(const std::string& text) {
    std::string result;
    std::string plain;
    size_t i = 0;

    while (i < text.size()) {
        if (text[i] == '`') {
            size_t end = text.find('`', i + 1);
            if (end != std::string::npos) {
                result += applyFormatting(escapeHtml(plain));
                plain.clear();
                std::string code = text.substr(i + 1, end - i - 1);
                result += "<code>" + escapeHtml(code) + "</code>";
                i = end + 1;
                continue;
            }
        }
        plain += text[i];
        i++;
    }

    result += applyFormatting(escapeHtml(plain));
    return result;
}
