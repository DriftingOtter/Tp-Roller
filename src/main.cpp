#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <stdexcept>

int check_for_toml (std::string file_path);
int check_args (int argc, char* argv[]);
std::string regex_tp_table_to_html_heading(std::string current_line);
std::string regex_tp_image_to_html_image_tag(std::string current_line);
std::string regex_tp_video_to_html_video_tag(std::string current_line);
std::string regex_tp_audio_to_html_audio_tag(std::string current_line);
std::string regex_tp_href_to_html_href_tag(std::string current_line);
std::string regex_tp_paragraph_to_html_paragraph_tag(std::string current_line);
std::string transpile_to_html(std::string current_line);
std::string read_file(std::string file_path);

int main(int argc, char* argv[]) {
    if (check_args(argc, argv) == -1) {
        return -1;
    }

    std::string transpile_file = argv[1];
    std::string file_data = read_file(transpile_file);

    return 0;
}

int check_for_toml (std::string file_path){
    // {.} is any string before + {\.tp$} means has to have .tp as the end
    std::regex toml_pattern(R"(.+\.tp$)");

    if (!std::regex_search(file_path, toml_pattern)){
        return -1;
    }

    return 0;
}

int check_args (int argc, char* argv[]){
    // Check for any args
    if (argc == 1){
        throw std::logic_error("TP Roller: $ TP_Roller <name_of_tp_file>");
        return -1;
    }

    // Check for .toml file
    std::string transpile_file = argv[1];
    if (check_for_toml(transpile_file) == -1){
        throw std::logic_error("TP Roller: file given is not a .tp file.");
        return -1;
    }

    return 0;
}

std::string regex_tp_table_to_html_heading(std::string current_line) {
    std::regex h1_pattern(R"(\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\])");
    std::regex h2_pattern(R"(\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\])");
    std::regex h3_pattern(R"(\[\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\]\])");
    std::regex h4_pattern(R"(\[\[\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\]\]\])");
    std::regex h5_pattern(R"(\[\[\[\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\]\]\]\])");
    std::regex h6_pattern(R"(\[\[\[\[\[\[([^]\[]*([^u]|u[^r]|ur[^l])[^=]*[^a]|u[^r]|ur[^l]|r[^l]|l[^=])\]\]\]\]\]\])");

    std::smatch match;

    if (std::regex_match(current_line, match, h1_pattern)) {
        return "<h1>" + match[1].str() + "</h1>";
    } else if (std::regex_match(current_line, match, h2_pattern)) {
        return "<h2>" + match[1].str() + "</h2>";
    } else if (std::regex_match(current_line, match, h3_pattern)) {
        return "<h3>" + match[1].str() + "</h3>";
    } else if (std::regex_match(current_line, match, h4_pattern)) {
        return "<h4>" + match[1].str() + "</h4>";
    } else if (std::regex_match(current_line, match, h5_pattern)) {
        return "<h5>" + match[1].str() + "</h5>";
    } else if (std::regex_match(current_line, match, h6_pattern)) {
        return "<h6>" + match[1].str() + "</h6>";
    }

    return current_line;
}

std::string regex_tp_image_to_html_image_tag(std::string current_line){
    std::regex img_pattern(R"(\[img=\"([^\"]*[^\\])\"]$)");

    std::smatch match;
    if (std::regex_match(current_line, match ,img_pattern)){
        return "<img src=\"" + match[1].str() + "\">";
    }

    return current_line;
}

std::string regex_tp_video_to_html_video_tag(std::string current_line){
    std::regex video_pattern(R"(\[vid=\"([^\"]*[^\\])\"]$)");

    std::smatch match;
    if (std::regex_match(current_line, match, video_pattern)) {
        std::string video_src = match[1].str();
        std::string html_video_tag = "<video controls>\n<source src=\"" + video_src + "\">\nYour Browser does not support html <video> feature.\n</video>";

        return html_video_tag;
    }

    return current_line;
}

std::string regex_tp_audio_to_html_audio_tag(std::string current_line){
    std::regex audio_pattern(R"(\[aud=\"([^\"]*[^\\])\"]$)");

    std::smatch match;
    if (std::regex_match(current_line, match, audio_pattern)) {
        std::string video_src = match[1].str();
        std::string html_video_tag = "<audio controls>\n<source src=\"" + video_src + "\">\nYour Browser does not support html <audio> feature.\n</audio>";

        return html_video_tag;
    }

    return current_line;
}

std::string regex_tp_href_to_html_href_tag(std::string current_line){
    std::regex href_pattern(R"(\[url=\"([^\"]*[^\\])\"]$)");

    std::smatch match;
    if (std::regex_match(current_line, match ,href_pattern)){
        std::string href_src = match[1].str();
        std::string href_tag = "<a href=\"" + href_src + "\">";

        return href_tag;
    }

    return current_line;
}

std::string regex_tp_paragraph_to_html_paragraph_tag(std::string current_line) {
    std::regex paragraph_pattern(R"(^[^\[\]].*$)");

    std::smatch match;
    if(std::regex_match(current_line, match, paragraph_pattern)) {
        std::string para_src = match.str();
        std::string para_tag = "<p>" + para_src + "</p>";

        return para_tag;
    }

    return current_line;
}

std::string transpile_to_html(std::string current_line) {
    std::map<std::string, std::string> variables;

    // Define the variables and their corresponding regex functions
    // variables["heading"] = regex_tp_table_to_html_heading(current_line);
    variables["paragraph"] = regex_tp_paragraph_to_html_paragraph_tag(current_line);
    variables["image"]   = regex_tp_image_to_html_image_tag(current_line);
    variables["video"]   = regex_tp_video_to_html_video_tag(current_line);
    variables["audio"]   = regex_tp_audio_to_html_audio_tag(current_line);
    variables["href"]    = regex_tp_href_to_html_href_tag(current_line);

    // Check for changes and return the variable with changed data
    for (const auto& [var, value] : variables) {
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
