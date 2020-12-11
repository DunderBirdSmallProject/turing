#ifndef TURING_CLI_H_
#define TURING_CLI_H_

#include <string>

namespace Turing {
class Cli {
  private:
    bool verbose;
    bool help;
    std::string filename;
    std::string input;

  public:
    Cli() {
        verbose = false;
        help = false;
    }
    bool isHelp() const {
        return help;
    }
    bool isVerbose() const {
        return verbose;
    }
    const std::string &getInputFile() const {
        return filename;
    }
    const std::string &getInput() const {
        return input;
    }
    void reset() {
        verbose = false;
        help = false;
        filename = "";
        input = "";
    }
    int parse(int argc, const char *argv[]);
};
} // namespace Turing

#endif