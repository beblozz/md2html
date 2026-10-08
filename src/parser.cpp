#include "parser.h"

#include <regex>
#include <vector>

#include "inline.h"
#include "utils.h"

static const std::regex headingPattern(R"(^(#{1,6})\s+(.*)$)");
static const std::regex bulletPattern(R"(^\s{0,3}[-*+]\s+(.*)$)");
static const std::regex numberPattern(R"(^\s{0,3}\d+\.\s+(.*)$)");
static const std::regex rulePattern(R"(^\s{0,3}((\*\s*){3,}|(-\s*){3,}|(_\s*){3,})$)");

static bool isBlank(const std::string& line) {
    return trim(line).empty();
}

static bool isCodeFence(const std::string& line) {
    return startsWith(trim(line), "```");
}

static bool isHeading(const std::string& line) {
    return std::regex_match(line, headingPattern);
}

static bool isRule(const std::string& line) {
    return std::regex_match(line, rulePattern);
}

static bool isBullet(const std::string& line) {
    return std::regex_match(line, bulletPattern);
}

static bool isNumber(const std::string& line) {
    return std::regex_match(line, numberPattern);
}

static bool isQuote(const std::string& line) {
    return startsWith(trim(line), ">");
}

static bool startsNewBlock(const std::string& line) {
    return isBlank(line) || isCodeFence(line) || isHeading(line) ||
           isRule(line) || isBullet(line) || isNumber(line) || isQuote(line);
}

static std::string parseCodeBlock(const std::vector<std::string>& lines, size_t& i) {
    std::string language = trim(trim(lines[i]).substr(3));
    i++;

    std::string code;
    while (i < lines.size() && !isCodeFence(lines[i])) {
        code += escapeHtml(lines[i]) + "\n";
        i++;
    }
    if (i < lines.size()) {
        i++;
    }

    std::string html = "<pre><code";
    if (!language.empty()) {
        html += " class=\"language-" + escapeHtml(language) + "\"";
    }
    html += ">" + code + "</code></pre>\n";
    return html;
}

static std::string parseHeading(const std::vector<std::string>& lines, size_t& i) {
    std::smatch match;
    std::regex_match(lines[i], match, headingPattern);
    i++;

    std::string level = std::to_string(match[1].length());
    std::string text = trim(match[2].str());
    return "<h" + level + ">" + renderInline(text) + "</h" + level + ">\n";
}

static std::string parseList(const std::vector<std::string>& lines, size_t& i, bool ordered) {
    std::string tag = ordered ? "ol" : "ul";
    const std::regex& pattern = ordered ? numberPattern : bulletPattern;

    std::string html = "<" + tag + ">\n";
    std::smatch match;
    while (i < lines.size() && !isRule(lines[i]) && std::regex_match(lines[i], match, pattern)) {
        html += "<li>" + renderInline(trim(match[1].str())) + "</li>\n";
        i++;
    }
    html += "</" + tag + ">\n";
    return html;
}

static std::string parseQuote(const std::vector<std::string>& lines, size_t& i) {
    std::string text;
    while (i < lines.size() && isQuote(lines[i])) {
        std::string part = trim(trim(lines[i]).substr(1));
        if (!text.empty()) {
            text += "\n";
        }
        text += part;
        i++;
    }
    return "<blockquote>\n<p>" + renderInline(text) + "</p>\n</blockquote>\n";
}

static std::string parseParagraph(const std::vector<std::string>& lines, size_t& i) {
    std::string text = trim(lines[i]);
    i++;
    while (i < lines.size() && !startsNewBlock(lines[i])) {
        text += "\n" + trim(lines[i]);
        i++;
    }
    return "<p>" + renderInline(text) + "</p>\n";
}

std::string markdownToHtml(const std::string& markdown) {
    std::vector<std::string> lines = splitLines(markdown);
    std::string html;
    size_t i = 0;

    while (i < lines.size()) {
        const std::string& line = lines[i];

        if (isBlank(line)) {
            i++;
        } else if (isCodeFence(line)) {
            html += parseCodeBlock(lines, i);
        } else if (isHeading(line)) {
            html += parseHeading(lines, i);
        } else if (isRule(line)) {
            html += "<hr>\n";
            i++;
        } else if (isBullet(line)) {
            html += parseList(lines, i, false);
        } else if (isNumber(line)) {
            html += parseList(lines, i, true);
        } else if (isQuote(line)) {
            html += parseQuote(lines, i);
        } else {
            html += parseParagraph(lines, i);
        }
    }

    return html;
}
