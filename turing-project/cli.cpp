#include "cli.h"
#include <cstring>
#include <iostream>
#include <string>

inline bool equal(const char *str1, const char *str2) {
    return strcmp(str1, str2) == 0;
}

int Turing::Cli::parse(int argc, const char *argv[]) {
    if (argc <= 1) {
        return -1;
    }
    bool is_help = false;
    bool is_verbose = false;
    int i_filename = -1;
    int i_input = -1;

    for (int i = 1; i < argc; i++) {
        if (equal(argv[i], "-h") || equal(argv[i], "--help")) {
            is_help = true;
        } else if (equal(argv[i], "-v") || equal(argv[i], "--verbose")) {
            is_verbose = true;
        } else {
            if (i_filename <= 0) {
                i_filename = i;
            } else if (i_input <= 0) {
                i_input = i;
            } else {
                return -1;
            }
        }
    }
    if (i_filename > 0 && i_input > 0) {
        this->help = is_help;
        this->verbose = is_verbose;
        this->filename = std::string(argv[i_filename]);
        this->input = std::string(argv[i_input]);
        return 0;
    } else if (i_filename == -1 && i_input == -1 && is_help) {
        this->help = is_help;
        return 0;
    }
    return -1;
}
