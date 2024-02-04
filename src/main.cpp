#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <stdexcept>

int check_for_toml (std::string file_path);
int check_args (int argc, char* argv[]);
std::string regex_toml_table_to_html_heading(std::string current_line);
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

std::string regex_toml_table_to_html_heading(std::string current_line) {
    std::regex h1_pattern(R"(\[([^\]]*)\])");
    std::regex h2_pattern(R"(\[\[([^\]]*)\]\])");
    std::regex h3_pattern(R"(\[\[\[([^\]]*)\]\]\])");
    std::regex h4_pattern(R"(\[\[\[\[([^\]]*)\]\]\]\])");
    std::regex h5_pattern(R"(\[\[\[\[\[([^\]]*)\]\]\]\]\])");
    std::regex h6_pattern(R"(\[\[\[\[\[\[([^\]]*)\]\]\]\]\]\])");

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

std::string transpile_to_html(std::string current_line){
    std::string heading = regex_toml_table_to_html_heading(current_line);
    return heading;
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
