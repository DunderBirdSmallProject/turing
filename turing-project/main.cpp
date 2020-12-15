#include <fstream>
#include <iostream>

#include "cli.h"
#include "turing.h"

int main(int argc, char const *argv[]) {
    Turing::Cli cli;
    if (cli.parse(argc, argv) < 0) {
        std::cerr << "illegal input\n";
    } else {
        if (cli.isHelp()) {
            std::cout
                << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n";
        } else {
            std::ifstream file;
            file.open(cli.getInputFile());
            if (file.is_open()) {
                bool success = true;
                std::string error_info;
                auto machine = Turing::getTuringMachine(file, error_info);
                if (machine == nullptr) {
                    if (cli.isVerbose()) {
                        std::cerr << error_info << "\n";
                    } else {
                        std::cerr << "syntax error";
                    }
                } else {
                    machine->run(cli.getInput(), cli.isVerbose(), success);
                }
                file.close();
                if (success) {
                    return 0;
                } else {
                    return -1;
                }
            } else {
                std::cerr << cli.getInputFile() + " not found\n";
            }
        }
    }
    return 0;
}
