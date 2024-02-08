#include "tp_transpiler.h"

int check_for_toml(std::string file_path) {
    std::regex toml_pattern(R"(.+\.tp$)");

    if (!std::regex_search(file_path, toml_pattern)) {
        return -1;
    }

    return 0;
}

int check_args(int argc, char* argv[]) {
    if (argc == 1) {
        throw std::logic_error("TP Roller: $ TP_Roller <name_of_tp_file>");
        return -1;
    }

    std::string transpile_file = argv[1];
    if (check_for_toml(transpile_file) == -1) {
        throw std::logic_error("TP Roller: file given is not a .tp file.");
        return -1;
    }

    return 0;
}

std::string regex_tp_heading_to_html_heading(std::string current_line) {
    // Heading regex patterns
    std::regex h1_pattern(R"(\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\])");
    std::regex h2_pattern(R"(\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\])");
    std::regex h3_pattern(R"(\[\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\]\])");
    std::regex h4_pattern(R"(\[\[\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\]\]\])");
    std::regex h5_pattern(R"(\[\[\[\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\]\]\]\])");
    std::regex h6_pattern(R"(\[\[\[\[\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\]\]\]\]\])");

    std::smatch match;

    if (std::regex_match(current_line, match, h1_pattern)) {
        return "<h1>" + match[1].str() + "</h1>";
    }
    else if (std::regex_match(current_line, match, h2_pattern)) {
        return "<h2>" + match[1].str() + "</h2>";
    }
    else if (std::regex_match(current_line, match, h3_pattern)) {
        return "<h3>" + match[1].str() + "</h3>";
    }
    else if (std::regex_match(current_line, match, h4_pattern)) {
        return "<h4>" + match[1].str() + "</h4>";
    }
    else if (std::regex_match(current_line, match, h5_pattern)) {
        return "<h5>" + match[1].str() + "</h5>";
    }
    else if (std::regex_match(current_line, match, h6_pattern)) {
        return "<h6>" + match[1].str() + "</h6>";
    }

    return current_line;
}

std::string regex_tp_image_to_html_image_tag(std::string current_line) {
    std::regex img_pattern(R"(\[img=\"([^\"]*[^\\])\"]$)");

    std::smatch match;
    if (std::regex_match(current_line, match, img_pattern)) {
        return "<img src=\"" + match[1].str() + "\">";
    }

    return current_line;
}

std::string regex_tp_video_to_html_video_tag(std::string current_line) {
    std::regex video_pattern(R"(\[vid=\"([^\"]*[^\\])\"]$)");

    std::smatch match;
    if (std::regex_match(current_line, match, video_pattern)) {
        std::string video_src = match[1].str();
        std::string html_video_tag = "<video controls>\n<source src=\"" + video_src + "\">\nYour Browser does not support html <video> feature.\n</video>";

        return html_video_tag;
    }

    return current_line;
}

std::string regex_tp_audio_to_html_audio_tag(std::string current_line) {
    std::regex audio_pattern(R"(\[aud=\"([^\"]*[^\\])\"]$)");

    std::smatch match;
    if (std::regex_match(current_line, match, audio_pattern)) {
        std::string video_src = match[1].str();
        std::string html_video_tag = "<audio controls>\n<source src=\"" + video_src + "\">\nYour Browser does not support html <audio> feature.\n</audio>";

        return html_video_tag;
    }

    return current_line;
}

std::string regex_tp_href_to_html_href_tag(std::string current_line) {
    std::regex href_pattern(R"(\[url=\"([^\"]*[^\\])\"]$)");

    std::smatch match;
    if (std::regex_match(current_line, match, href_pattern)) {
        std::string href_src = match[1].str();
        std::string href_tag = "<a href=\"" + href_src + "\">";

        return href_tag;
    }

    return current_line;
}

std::string regex_tp_paragraph_to_html_paragraph_tag(std::string current_line) {
    std::regex paragraph_pattern(R"(^(?!(~~~|\[url=\"\"|\[vid=\"\"|\[aud=\"\"|\[img=\"\"|\[|\])).*[^[\]].*$)");

    std::smatch match;
    if (std::regex_match(current_line, match, paragraph_pattern)) {
        std::string para_src = match.str();
        std::string para_tag = "<p>" + para_src + "</p>";

        return para_tag;
    }

    return current_line;
}

std::string regex_tp_code_to_html_code_tag(std::string current_line) {
    std::regex code_block_start_pattern("(^|[^~])~~~(\\s*[^~]+)");
    std::regex code_block_end_pattern("(^|[^~])~~~(?![~\\s])");

    std::smatch match;
    if (std::regex_search(current_line, match, code_block_start_pattern)) {
        return "<code>";
    }
    else if (std::regex_search(current_line, match, code_block_end_pattern)) {
        return "</code>";
    }

    return current_line;
}

std::string regex_tp_table_colomn_to_html(std::string colomn_data) {
    std::string html;

    std::istringstream colomn_stream(colomn_data);
    std::string cell_data;
    while (std::getline(colomn_stream, cell_data, ',')) {
        html += "<td>" + cell_data + "</td>\n";
    }

    return html;
}

std::string regex_tp_table_row_to_html(std::string current_line) {
    std::string html;
    std::regex table_row_pattern(R"(^\w+\s*=\s*\{([0-9a-zA-Z,\s]*)\})");

    std::smatch match;
    if (std::regex_search(current_line, match, table_row_pattern)) {
        // Extract row data and convert to HTML table row
        std::string row_data = match[1].str();
        html += "<tr>\n";
        html += regex_tp_table_colomn_to_html(row_data);
        html += "</tr>\n";
    }

    return html;
}

std::string regex_tp_table_column_to_html_table_tag(std::string current_line) {
    static bool in_table = false;
    std::regex table_colomn_pattern(R"(^(\w+)\s*=\s*\{([0-9a-zA-Z,\s]*)\})");
    std::string html;

    std::smatch match;
    if (std::regex_search(current_line, match, table_colomn_pattern)) {
        if (!in_table) {
            in_table = true;
            html += "<table>\n";
        }

        html += regex_tp_table_row_to_html(current_line);
    }
    else {
        if (in_table) {
            html += "</table>\n";
            in_table = false;
        }
    }

    return html;
}

std::string regex_tp_unordered_list_to_html(std::string current_line, bool& in_unordered_list) {
    std::string html;

    std::regex unordered_list_pattern(R"(^\-\s+(.+)$)");

    std::smatch match;
    if (std::regex_search(current_line, match, unordered_list_pattern)) {
        if (!in_unordered_list) {
            in_unordered_list = true;
            html += "<ul>\n";
        }
        html += "<li>" + match[1].str() + "</li>\n";
        return html;
    }

    if (in_unordered_list && current_line.empty()) {
        in_unordered_list = false;
        html += "</ul>\n";
    }

    return html;
}

std::string regex_tp_numbered_list_to_html(std::string current_line, bool& in_numbered_list) {
    std::string html;

    std::regex numbered_list_pattern(R"(^\d+\.\s+(.+)$)");

    std::smatch match;
    if (std::regex_search(current_line, match, numbered_list_pattern)) {
        if (!in_numbered_list) {
            in_numbered_list = true;
            html += "<ol>\n";
        }
        html += "<li>" + match[1].str() + "</li>\n";
        return html;
    }

    if (in_numbered_list && current_line.empty()) {
        in_numbered_list = false;
        html += "</ol>\n";
    }

    return html;
}

std::string regex_tp_alphabetical_list_to_html(std::string current_line, bool& in_alphabetical_list) {
    std::string html;

    std::regex alphabetical_list_pattern(R"(^[a-z]\.\s+(.+)$)");

    std::smatch match;
    if (std::regex_search(current_line, match, alphabetical_list_pattern)) {
        if (!in_alphabetical_list) {
            in_alphabetical_list = true;
            html += "<ol type=\"a\">\n";
        }
        html += "<li>" + match[1].str() + "</li>\n";
        return html;
    }

    if (in_alphabetical_list && current_line.empty()) {
        in_alphabetical_list = false;
        html += "</ol>\n";
    }

    return html;
}

std::string transpile_to_html(std::string current_line) {
    static bool in_code_block = false;
    static bool in_unordered_list = false;
    static bool in_numbered_list = false;
    static bool in_alphabetical_list = false;
    static std::string accumulated_result;

    if (!in_code_block && current_line.find("~~~") != std::string::npos) {
        in_code_block = true;
        return "<code>";
    }

    if (in_code_block && current_line.find("~~~") != std::string::npos) {
        in_code_block = false;
        std::string result = "</code>";
        accumulated_result.clear();
        return result;
    }

    if (in_code_block) {
        accumulated_result += current_line;
        return current_line;
    }

    std::string list_html;

    list_html = regex_tp_unordered_list_to_html(current_line, in_unordered_list);
    if (!list_html.empty()) {
        return list_html;
    }

    list_html = regex_tp_numbered_list_to_html(current_line, in_numbered_list);
    if (!list_html.empty()) {
        return list_html;
    }

    list_html = regex_tp_alphabetical_list_to_html(current_line, in_alphabetical_list);
    if (!list_html.empty()) {
        return list_html;
    }

    if (!in_unordered_list && !in_numbered_list && !in_alphabetical_list && current_line.empty()) {
        // Close any open list tags
        std::string closing_tags;
        if (in_unordered_list) {
            closing_tags += "</ul>\n";
            in_unordered_list = false;
        }
        if (in_numbered_list) {
            closing_tags += "</ol>\n";
            in_numbered_list = false;
        }
        if (in_alphabetical_list) {
            closing_tags += "</ol>\n";
            in_alphabetical_list = false;
        }
        return closing_tags;
    }

    // Process other types of elements
    std::map<std::string, std::string (*)(std::string)> variables;
    variables["heading"] = regex_tp_heading_to_html_heading;
    variables["paragraph"] = regex_tp_paragraph_to_html_paragraph_tag;
    variables["image"] = regex_tp_image_to_html_image_tag;
    variables["video"] = regex_tp_video_to_html_video_tag;
    variables["audio"] = regex_tp_audio_to_html_audio_tag;
    variables["href"] = regex_tp_href_to_html_href_tag;

    for (const auto& pair : variables) {
        std::string value = pair.second(current_line);
        if (value != current_line) {
            return value;
        }
    }

    return current_line;
}

std::string read_file(std::string file_path) {
    std::string buff;
    std::string file_data;
    std::ifstream ReadFile(file_path);

    if (!ReadFile.is_open()) {
        throw std::logic_error("Error: Could not open file " + file_path);
    }

    while (std::getline(ReadFile, buff)) {
        file_data += transpile_to_html(buff) + "\n";
    }
    std::cout << file_data << std::endl;

    return file_data;
}
