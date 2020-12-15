#ifndef TURING_TURING_H
#define TURING_TURING_H

#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
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
    FullState(const State &state, const string &tape_content)
        : state(state), tape_content(tape_content) {}
};

struct Transition {
    State state;
    string new_tape_content;
    string dir;
    Transition() {}
    Transition(State &state, string &new_tape_content, string dir)
        : state(state), new_tape_content(new_tape_content), dir(dir) {}
};
typedef map<FullState, Transition> TransFunc;

class TuringMachine {
   private:
    const int n;  // number of tapes
    const char blank_char;

    Alphabet input_alphabet;
    Alphabet tape_alphabet;
    StateSet state_sets;
    StateSet fin_states;
    State init_state;
    TransFunc trans;

    std::vector<TapeType> tapes;
    std::vector<size_t> _bias;

   private:
    char getChar(size_t ith, size_t index) const;
    void writeChar(size_t ith, size_t &index, char new_char, char dir);
    void outputState(int step, const std::string &state,
                     const std::vector<size_t> &index) const;

    void clearTapes() {
        assert((int)tapes.size() == n);
        for (int i = 0; i < n; i++) {
            tapes[i].clear();
            _bias[i] = 0;
        }
    }

   public:
    TuringMachine(int tapecnt, char blank_char, Alphabet input_alphabet,
                  Alphabet tape_alphabet, StateSet state_sets,
                  StateSet fin_states, State q0, TransFunc trans)
        : n(tapecnt),
          blank_char(blank_char),
          input_alphabet(input_alphabet),
          tape_alphabet(tape_alphabet),
          state_sets(state_sets),
          fin_states(fin_states),
          init_state(q0),
          trans(trans),
          tapes(n),
          _bias(n) {}
    int getNumberOfTapes() const { return n; }
    const Alphabet &getInputAlphabet() const { return input_alphabet; }
    const Alphabet &getTapeAlphabet() const { return tape_alphabet; }
    const StateSet &getStateSets() const { return state_sets; }
    const StateSet &getFinState() const { return fin_states; }
    const State &getInitState() const { return init_state; }
    const TransFunc &getTransFunc() const { return trans; }
    char getBlankChar() const { return blank_char; }

    void run(std::string input, bool verbose, bool &success);
};

std::unique_ptr<TuringMachine> getTuringMachine(std::vector<std::string> &lines);

}  // namespace Turing

#endif