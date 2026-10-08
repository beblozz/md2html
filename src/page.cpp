#include "page.h"

#include "utils.h"

std::string wrapPage(const std::string& body, const std::string& title, const std::string& cssPath) {
    std::string html;
    html += "<!DOCTYPE html>\n";
    html += "<html lang=\"ru\">\n";
    html += "<head>\n";
    html += "<meta charset=\"UTF-8\">\n";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
    html += "<title>" + escapeHtml(title) + "</title>\n";
    html += "<link rel=\"stylesheet\" href=\"" + escapeHtml(cssPath) + "\">\n";
    html += "</head>\n";
    html += "<body>\n";
    html += "<main class=\"markdown-body\">\n";
    html += body;
    html += "</main>\n";
    html += "</body>\n";
    html += "</html>\n";
    return html;
}
