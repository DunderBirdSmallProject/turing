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
                std::vector<std::string> lines;
                std::string buff;

                while (std::getline(file, buff)) {
                    lines.push_back(buff);
                }

                auto machine = Turing::getTuringMachine(lines, cli.isVerbose());
                if (machine == nullptr) {
                    return -1;
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
                if (cli.isVerbose()) {
                    std::cerr << cli.getInputFile() + " not found\n";
                } else {
                    std::cerr << "illegal input\n";
                }
                return -1;
            }
        }
    }
    return 0;
}
