#ifndef TP_TRANSPILER_H
#define TP_TRANSPILER_H

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <stdexcept>
#include <map>

int check_for_toml(std::string file_path);
int check_args(int argc, char* argv[]);
std::string regex_tp_heading_to_html_heading(std::string current_line);
std::string regex_tp_image_to_html_image_tag(std::string current_line);
std::string regex_tp_video_to_html_video_tag(std::string current_line);
std::string regex_tp_audio_to_html_audio_tag(std::string current_line);
std::string regex_tp_href_to_html_href_tag(std::string current_line);
std::string regex_tp_paragraph_to_html_paragraph_tag(std::string current_line);
std::string regex_tp_code_to_html_code_tag(std::string current_line);
std::string regex_tp_table_column_to_html_table_tag(std::string current_line);
std::string regex_tp_table_row_to_html(std::string current_line);
std::string regex_tp_unordered_list_to_html(std::string current_line, bool& in_unordered_list);
std::string regex_tp_numbered_list_to_html(std::string current_line, bool& in_numbered_list);
std::string regex_tp_alphabetical_list_to_html(std::string current_line, bool& in_alphabetical_list);
std::string transpile_to_html(std::string current_line);
std::string read_file(std::string file_path);

#endif

