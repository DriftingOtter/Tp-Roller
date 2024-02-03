#include <iostream>
#include <string>
#include </home/daksh/Documents/Tp-Roller/toml11-3.8.1/toml.hpp>
#include <regex>

int checkfor_toml (std::string file_path);

int main(int argc, char* argv[]){

    // Check for any args 
    if (argc == 1){
        std::cout << "TP_Roller: $ TP_Roller <name_of_toml_file>" << std::endl;
        return -1;
    }

    // Check for .toml file
    std::string transpile_file = argv[1];
    if (checkfor_toml(transpile_file) == -1){
        std::cout << "TP_Roller: file given is not a .toml file." << std::endl;
        return -1;
    }

    // TODO: Start Making The Transpile Code

    return 0;
}

int checkfor_toml (std::string file_path){
    std::regex toml_pattern("\\.toml$", std::regex_constants::icase);

    if (!std::regex_search(file_path, toml_pattern)){
        return -1;
    }

    return 0;
}
