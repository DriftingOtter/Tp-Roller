#include "tp_transpiler.h"

int main(int argc, char* argv[]) {
    try {
        if (check_args(argc, argv) == -1) {
            return -1;
        }

        std::string transpile_file = argv[1];
        std::string file_data = read_file(transpile_file);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
