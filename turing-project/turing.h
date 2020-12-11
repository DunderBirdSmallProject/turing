#ifndef TURING_TURING_H
#define TURING_TURING_H

#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace Turing {
using std::deque;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

typedef deque<char> TapeType;
typedef string State;
typedef set<char> Alphabet;
typedef set<State> StateSet;

struct FullState {
    State state;
    string tape_content;
    bool operator==(const FullState &other) const {
        return state == other.state && tape_content == other.tape_content;
    }
    bool operator<(const FullState &other) const {
        if (state == other.state) {
            return tape_content < other.tape_content;
        }
        return state < other.state;
    }
    FullState(State &state, string &tape_content) : state(state), tape_content(tape_content) {}
};

struct Transition {
    State state;
    string new_tape_content;
    string dir;
    Transition() {}
    Transition(State &state, string &new_tape_content, string dir) : state(state), new_tape_content(new_tape_content), dir(dir) {}
};
typedef map<FullState, Transition> TransFunc;

class TuringMachine {
  private:
    int n; // number of tapes
    char blank_char;

    Alphabet input_alphabet;
    Alphabet tape_alphabet;
    StateSet state_sets;
    StateSet fin_state;
    State init_state;
    TransFunc trans;

    vector<TapeType> tapes;

    State cur_state;

  public:
    TuringMachine(int tapecnt, char blank_char, Alphabet input_alphabet, Alphabet tape_alphabet, StateSet state_sets,
                  State q0, TransFunc trans) : n(tapecnt), input_alphabet(input_alphabet),
                                               tape_alphabet(tape_alphabet), state_sets(state_sets), init_state(q0), trans(trans) {}
    int getNumberOfTapes() const {
        return n;
    }
    const vector<TapeType> &getTapes() const {
        return tapes;
    }
    const bool isAccepted() const {
        return fin_state.find(cur_state) != fin_state.end();
    }
};

TuringMachine *getTuringMachine(std::ifstream &in, std::string &error_info);

} // namespace Turing

#endif