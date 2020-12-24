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

void test_case1(const std::string input, bool expect, bool other=false) {
    std::string error_info;
    std::ifstream file = openfile("programs/case1.tm");
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
        if (!other)
            ASSERT(res == "false");
    }
    ASSERT(success == expect);
    file.close();
}

int main() {
    std::cout.rdbuf(NULL);
    test_case1("ab", false);
    test_case1("ba", false);
    test_case1("c", false, true);
    test_case1("", false);
    test_case1("aabbaab", false);
    test_case1("aabbaabbb", false);
    test_case1("aaaa", false);
    test_case1("abaa", false);
    test_case1("bb", false);
    test_case1("aa", false);

    test_case1("abab", true);
    test_case1("aabbaabb", true);
    test_case1("aabaab", true);
    test_case1("abbabb", true);
    test_case1("aaaabaaaab", true);
    test_case1("abbbbabbbb", true);
    return 0;
}
