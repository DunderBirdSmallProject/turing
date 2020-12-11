#include "cli.h"
#include "turing.h"
#include <fstream>
#include <iostream>

int main(int argc, char const *argv[]) {
    Turing::Cli cli;
    if (cli.parse(argc, argv) < 0) {
        std::cerr << "illegal input\n";
    } else {
        if (cli.isHelp()) {
            std::cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n";
        } else {
            std::ifstream file;
            file.open(cli.getInputFile());
            if (file.is_open()) {
                std::string error_info;
                Turing::TuringMachine *machine = Turing::getTuringMachine(file, error_info);
                if (machine == nullptr) {
                    if (cli.isVerbose()) {
                        std::cerr << error_info << "\n";
                    } else {
                        std::cerr << "syntax error";
                    }
                } else {
                    bool success = true;
                    std::string res = machine->run(cli.getInput(), cli.isVerbose(), success);
                    delete machine;
                    if (success) {
                        std::cout << res;
                    } else {
                        return -1;
                    }
                }
                file.close();
            } else {
                std::cerr << cli.getInputFile() + " not found\n";
            }
        }
    }
    return 0;
}
