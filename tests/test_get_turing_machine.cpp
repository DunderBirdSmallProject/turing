#include "turing.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <set>

using namespace Turing;

std::ifstream openfile(const std::string filename) {
    std::ifstream file;
    file.open(filename);
    ASSERT(file.is_open());
    return file;
}

void test_sample1() {
    std::string error_info;
    std::ifstream file = openfile("tests/sample1.tm");
    TuringMachine *machine = getTuringMachine(file, error_info);
    ASSERT(machine != nullptr);

    Alphabet input = machine->getInputAlphabet();
    Alphabet tape = machine->getTapeAlphabet();
    int n = machine->getNumberOfTapes();
    char blank_char = machine->getBlankChar();
    StateSet states = machine->getStateSets();
    StateSet fin_state = machine->getFinState();
    TransFunc trans = machine->getTransFunc();
    State init_state = machine->getInitState();

    EXPECT((input == std::set<char>{'0', '1'}));
    EXPECT((tape == std::set<char>{'0', '1', '_', 't', 'r', 'u', 'e', 'f', 'a', 'l', 's'}));
    EXPECT(init_state == "0");
    EXPECT(blank_char == '_');
    EXPECT((fin_state == std::set<std::string>{"halt_accept"}));
    EXPECT(n == 2);
    ASSERT(trans.size() == 1);

    FullState full_state("cmp", "01");
    ASSERT(trans.count(full_state));
    Transition transition = trans[full_state];
    EXPECT(transition.dir == "rl");
    EXPECT(transition.new_tape_content == "__");
    EXPECT(transition.state == "reject");

    delete machine;
}

int main() {
    test_sample1();
    return 0;
}
