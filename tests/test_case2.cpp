#include <fstream>
#include <iostream>
#include <set>

#include "turing.h"
#include "util.h"

using namespace Turing;

std::ifstream openfile(const std::string filename) {
    std::ifstream file;
    file.open(filename);
    ASSERT(file.is_open());
    return file;
}

void test_case2(const std::string input, bool expect, bool other=false) {
    std::string error_info;
    std::ifstream file = openfile("programs/case2.tm");
    ASSERT(file.is_open());
    std::string buff;
    std::vector<std::string> lines;

    while (std::getline(file, buff)) {
        lines.push_back(buff);
    }
    auto machine = getTuringMachine(lines, false);
    ASSERT(machine != nullptr);
    bool success = true;
    auto res = machine->run(input, false, success);
    if (expect) {
        ASSERT(res == "true");
    } else {
        if(!other)
            ASSERT(res == "false");
    }
    ASSERT(success == expect);
    file.close();
}

int main() {
    std::cout.rdbuf(NULL);
    test_case2("1x1=a", false, true);
    test_case2("11x1=1", false);
    test_case2("x11=1", true);
    test_case2("1x111", false);

    test_case2("1x11=11", true);
    test_case2("11x1=11", true);
    test_case2("1x1=1", true);
    test_case2("11x11=1111", true);
    test_case2("11x111=111111", true);
    return 0;
}
