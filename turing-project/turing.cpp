#include "turing.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>

namespace Turing {
inline bool isWhiteSpace(char c) { return c == ' ' || c == '\r' || c == '\n'; }

inline bool isStateChar(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') || c == '_';
}

inline bool isTapeChar(char c) {
    return isprint(c) && (c != ' ' && c != ',' && c != ';' && c != '{' &&
                          c != '}' && c != '*');
}

inline bool isInputChar(char c) { return isTapeChar(c) && c != '_'; }

inline void eatWhiteSpace(string::size_type &cur, std::string &str) {
    std::string::size_type len = str.length();
    while (cur < len && isWhiteSpace(str[cur])) {
        cur++;
    }
}

inline int inputAssert(string::size_type &cur, std::string &str,
                       const char *expect, string::size_type expect_len) {
    std::string::size_type str_len = str.length();
    if (cur + expect_len > str_len) {
        return -1;
    }
    for (string::size_type i = 0; i < expect_len; i++) {
        if (str[cur + i] != expect[i]) {
            return -1;
        }
    }
    cur += expect_len;
    return 0;
}

int readState(State &state, std::string &str, string::size_type &cur) {
    const std::string::size_type len = str.length();
    state = "";
    eatWhiteSpace(cur, str);
    if (cur >= len || !isStateChar(str[cur])) {
        return -1;
    }
    string::size_type end = cur + 1;
    while (end < len) {
        if (isStateChar(str[end])) {
            end++;
        } else {
            if (str[end] != ' ') {
                return -1;
            } else {
                break;
            }
        }
    }
    state = str.substr(cur, end - cur);
    cur = end;
    return 0;
}

int readStateSet(StateSet &state_sets, std::string &str,
                 string::size_type &cur) {
    const std::string::size_type len = str.length();
    eatWhiteSpace(cur, str);
    if (inputAssert(cur, str, "{", 1) < 0) {
        return -1;
    }
    while (cur < len) {
        eatWhiteSpace(cur, str);
        string::size_type end = cur;
        while (end < len) {
            if (str[end] == ',' || str[end] == '}') {
                break;
            } else {
                end++;
            }
        }
        if (end == len) {
            return -1;
        }
        state_sets.insert(str.substr(cur, end - cur));
        cur = end + 1;
        if (str[end] == '}') {
            break;
        }
    }
    eatWhiteSpace(cur, str);
    if (cur != len && str[cur] != ';') {
        return -1;
    }
    return 0;
}

int readTapeAlphabet(Alphabet &alphabet, std::string &str,
                     string::size_type &cur) {
    const std::string::size_type len = str.length();
    eatWhiteSpace(cur, str);
    if (inputAssert(cur, str, "{", 1) < 0) {
        return -1;
    }
    while (cur < len) {
        eatWhiteSpace(cur, str);
        if (str[cur] == '}') {
            cur += 1;
            break;
        }
        if (!isTapeChar(str[cur])) {
            return -1;  // input alphabet must be valid
        }
        alphabet.insert(str[cur]);
        cur += 1;
        eatWhiteSpace(cur, str);
        if (str[cur] != '}') {
            if (str[cur] != ',') {
                return -1;
            }
            cur += 1;
        }
    }
    return 0;
}

int readInputAlphabet(Alphabet &alphabet, std::string &str,
                      string::size_type &cur) {
    const std::string::size_type len = str.length();
    eatWhiteSpace(cur, str);
    if (inputAssert(cur, str, "{", 1) < 0) {
        return -1;
    }
    while (cur < len) {
        eatWhiteSpace(cur, str);
        if (str[cur] == '}') {
            cur += 1;
            break;
        }
        if (!isInputChar(str[cur])) {
            return -1;  // input alphabet must be valid
        }
        alphabet.insert(str[cur]);
        cur += 1;
        eatWhiteSpace(cur, str);
        if (str[cur] != '}') {
            if (str[cur] != ',') {
                return -1;
            }
            cur += 1;
        }
    }
    return 0;
}

int readInt(int &target, std::string &str, string::size_type &cur) {
    const std::string::size_type len = str.length();
    eatWhiteSpace(cur, str);
    target = 0;
    if (cur >= len) {
        return -1;
    }
    while (cur < len) {
        if (str[cur] == '\n' || str[cur] == ' ') {
            break;
        }
        if (str[cur] >= '0' && str[cur] <= '9') {
            target = target * 10 + (str[cur] - '0');
        } else {
            return -1;
        }
        cur++;
    }
    eatWhiteSpace(cur, str);
    return 0;
}

int readSymSet(string &target, std::string &str, string::size_type &cur) {
    const string::size_type len = str.length();
    eatWhiteSpace(cur, str);
    if (cur >= len) {
        return -1;
    }
    target = "";
    string::size_type end = cur;
    while (end < len) {
        if (str[end] == ' ') {
            break;
        }
        end++;
    }
    target = str.substr(cur, end - cur);
    cur = end;
    return 0;
}

int readTrans(TransFunc &trans, std::string &str, string::size_type &cur) {
    State old_state, new_state;
    string old_tape_content, new_tape_content;
    string dir;
    if (readState(old_state, str, cur) < 0) {
        return -1;
    }
    if (readSymSet(old_tape_content, str, cur) < 0) {
        return -1;
    }
    if (readSymSet(new_tape_content, str, cur) < 0) {
        return -1;
    }
    if (readSymSet(dir, str, cur) < 0) {
        return -1;
    }
    if (readState(new_state, str, cur) < 0) {
        return -1;
    }

    // check whether direction is valid
    for (std::string::size_type i = 0; i < dir.length(); i++) {
        if (dir[i] != '*' && dir[i] != 'r' && dir[i] != 'l') {
            return -1;
        }
    }

    FullState old_full_state = FullState(old_state, old_tape_content);
    Transition transition = Transition(new_state, new_tape_content, dir);
    if (trans.find(old_full_state) != trans.end()) {
        return -1;
    } else {
        trans[old_full_state] = transition;
    }
    return 0;
}

std::unique_ptr<TuringMachine> getTuringMachine(
    std::vector<std::string> &lines) {
    int tape_cnt = 0;
    Alphabet input_alphabet, tape_alphabet;
    StateSet state_sets;
    StateSet fin_states;
    State init_state;
    TransFunc trans;
    char blank_char = '_';

    for (auto &line : lines) {
        const string::size_type len = line.length();
        string::size_type cur = 0;
        eatWhiteSpace(cur, line);

        /*
         * parse_state: 0: nothing input, 1: '#' read, 2: end of line or
         * comments
         */
        int parse_state = 0;
        while (cur < len && parse_state >= 0) {
            eatWhiteSpace(cur, line);
            if (parse_state == 0) {
                if (line[cur] == ';') {
                    break;
                } else if (line[cur] == '#') {
                    parse_state = 1;
                    cur++;
                } else {
                    if (readTrans(trans, line, cur) < 0) {
                        parse_state = -1;
                        break;
                    }
                    parse_state = 2;
                }
            } else if (parse_state == 1) {
                switch (line[cur]) {
                    case 'Q':
                        cur += 1;
                        eatWhiteSpace(cur, line);
                        if (inputAssert(cur, line, "=", 1) < 0) {
                            parse_state = -1;
                            break;
                        }
                        if (readStateSet(state_sets, line, cur) < 0) {
                            parse_state = -1;
                            break;
                        }
                        parse_state = 2;
                        break;
                    case 'S':
                        cur += 1;
                        eatWhiteSpace(cur, line);
                        if (inputAssert(cur, line, "=", 1) < 0) {
                            parse_state = -1;
                            break;
                        }
                        if (readInputAlphabet(input_alphabet, line, cur) < 0) {
                            parse_state = -1;
                            break;
                        }
                        parse_state = 2;
                        break;
                    case 'G':
                        cur += 1;
                        eatWhiteSpace(cur, line);
                        if (inputAssert(cur, line, "=", 1) < 0) {
                            parse_state = -1;
                            break;
                        }
                        if (readTapeAlphabet(tape_alphabet, line, cur) < 0) {
                            parse_state = -1;
                            break;
                        }
                        parse_state = 2;
                        break;
                    case 'q':
                        cur += 1;
                        if (inputAssert(cur, line, "0", 1) < 0) {
                            parse_state = -1;
                            break;
                        }
                        eatWhiteSpace(cur, line);
                        if (inputAssert(cur, line, "=", 1) < 0) {
                            parse_state = -1;
                            break;
                        }
                        if (readState(init_state, line, cur) < 0) {
                            parse_state = -1;
                            break;
                        }
                        parse_state = 2;
                        break;
                    case 'B':
                        cur += 1;
                        eatWhiteSpace(cur, line);
                        if (inputAssert(cur, line, "=", 1) < 0) {
                            parse_state = -1;
                            break;
                        }
                        eatWhiteSpace(cur, line);
                        if (cur >= len) {
                            parse_state = -1;
                            break;
                        }
                        blank_char = line[cur];
                        if (!isTapeChar(blank_char)) {
                            parse_state = -1;
                            break;
                        }
                        cur++;
                        parse_state = 2;
                        break;
                    case 'F':
                        cur += 1;
                        eatWhiteSpace(cur, line);
                        if (inputAssert(cur, line, "=", 1) < 0) {
                            parse_state = -1;
                            break;
                        }
                        if (readStateSet(fin_states, line, cur) < 0) {
                            parse_state = -1;
                            break;
                        }
                        parse_state = 2;
                        break;
                    case 'N':
                        cur += 1;
                        eatWhiteSpace(cur, line);
                        if (inputAssert(cur, line, "=", 1) < 0) {
                            parse_state = -1;
                            break;
                        }
                        if (readInt(tape_cnt, line, cur) < 0) {
                            parse_state = -1;
                            break;
                        }
                        parse_state = 2;
                        break;
                    default:
                        parse_state = -1;
                        break;
                }
            } else if (parse_state == 2) {
                if (cur != line.length() && line[cur] != ';') {
                    parse_state = -1;
                }
                break;
            }
        }
        if (parse_state == 1) {
            parse_state = -1;
        }
        if (parse_state < 0) {
            return nullptr;
        }
    }
    // TODO: check whether all information is inputed. (Q, S, G, q0, B, F, N,
    // delta)
    // TODO: check whether all states is in state set
    return std::unique_ptr<TuringMachine>(
        new TuringMachine(tape_cnt, blank_char, input_alphabet, tape_alphabet,
                          state_sets, fin_states, init_state, trans));
}

char TuringMachine::getChar(size_t ith, size_t index) const {
    assert(ith < tapes.size());
    assert(index >= 0);
    auto &cur_tape = tapes[ith];
    assert(index >= 0 && index < cur_tape.size());
    return cur_tape[index];
}

void TuringMachine::writeChar(size_t ith, size_t &index, char new_char,
                              char dir) {
    assert(ith < tapes.size());
    auto &cur_tape = tapes[ith];
    assert(index >= 0 && index < cur_tape.size());
    cur_tape[index] = new_char;
    assert(dir == 'r' || dir == 'l' || dir == '*');
    if (dir == 'r') {
        index += 1;
        if (index >= cur_tape.size()) {
            cur_tape.push_back(blank_char);
        }
    } else if (dir == 'l') {
        if (index == 0) {
            cur_tape.push_front(blank_char);
            _bias[ith] += 1;
        } else {
            index -= 1;
        }
    }
}

void TuringMachine::outputState(int step, const std::string &state,
                                const std::vector<size_t> &index) const {
    std::cout << std::left << std::setw(7) << "Step";
    std::cout << ": " << step << '\n';

    for (int i = 0; i < n; i++) {
        std::vector<char> c_index, c_tape, c_head;
        auto &cur_tape = tapes[i];
        auto sz = (int)cur_tape.size();
        assert(sz >= 1);
        int left_bound = 0, right_bound = sz - 1;
        while (left_bound < sz && cur_tape[left_bound] == blank_char) {
            left_bound += 1;
        }
        while (right_bound >= 0 && cur_tape[right_bound] == blank_char) {
            right_bound -= 1;
        }
        left_bound = std::min(left_bound, (int)index[i]);
        right_bound = std::max(right_bound, (int)index[i]);

        for (int j = left_bound; j <= right_bound; j++) {
            int real_index = j - (int)_bias[i];
            if (real_index < 0) {
                real_index = -real_index;
            }

            int tmp = real_index;
            std::vector<char> tmp_buff;
            do {
                int base = tmp % 10;
                tmp /= 10;
                tmp_buff.push_back('0' + base);
            } while (tmp > 0);

            for (auto it = tmp_buff.rbegin(); it != tmp_buff.rend(); ++it) {
                c_index.push_back(*it);
            }
            int width = (int)tmp_buff.size();
            c_tape.push_back(getChar(i, j));
            if (j == (int)index[i]) {
                c_head.push_back('^');
            } else {
                c_head.push_back(' ');
            }

            for (int k = 0; k < width - 1; k++) {
                c_tape.push_back(' ');
                c_head.push_back(' ');
            }

            if (j != right_bound) {
                c_index.push_back(' ');
                c_tape.push_back(' ');
                c_head.push_back(' ');
            }
        }

        std::cout << std::left << "Index" << std::setw(2) << i;
        std::cout << ": ";
        for (auto c : c_index) {
            std::cout << c;
        }
        std::cout << '\n';
        std::cout << std::left << "Tape" << std::setw(3) << i;
        std::cout << ": ";
        for (auto c : c_tape) {
            std::cout << c;
        }
        std::cout << '\n';
        std::cout << std::left << "Head" << std::setw(3) << i;
        std::cout << ": ";
        for (auto c : c_head) {
            std::cout << c;
        }
        std::cout << '\n';
    }

    std::cout << std::left << "State  : ";
    std::cout << state << '\n';
    std::cout << "---------------------------------------------\n";
}

void TuringMachine::run(std::string input, bool verbose, bool &success) {
    // judge the input is valid or not
    for (auto i = 0u; i < input.size(); i++) {
        char c = input[i];
        if (input_alphabet.count(c) == 0) {
            if (verbose) {
                std::cerr << "Input: " << input << "\n";
                std::cerr << "==================== ERR ====================\n";
                std::cerr << "error: '" << c << "'"
                          << " was not declared in the set of input symbols\n";
                std::cerr << "Input: " << input << "\n";
                for (int j = 0; j < 7; j++) {
                    std::cerr << ' ';
                }
                for (auto j = 0u; j < i; j++) {
                    std::cerr << ' ';
                }
                std::cerr << "^\n";
                std::cerr << "==================== END ====================";
            } else {
                std::cerr << "illegal input";
            }
            success = false;
            return;
        }
    }

    clearTapes();
    State cur_state = init_state;
    std::vector<size_t> cur_index(n);
    for (auto c : input) {
        tapes[0].push_back(c);
    }
    cur_index[0] = 0;
    if (tapes[0].size() == 0) {
        tapes[0].push_back(blank_char);
    }
    for (int i = 1; i < n; i++) {
        tapes[i].push_back(blank_char);
        cur_index[i] = 0;
    }

    int step_cnt = 0;
    while (true) {
        // finish state?
        if (verbose) {
            outputState(step_cnt, cur_state, cur_index);
        }
        std::string cur_tape_content;
        for (int i = 0; i < n; i++) {
            cur_tape_content += getChar(i, cur_index[i]);
        }
        auto cur_full_state = FullState(cur_state, cur_tape_content);
        if (trans.count(cur_full_state) == 0) {
            // dead state
            break;
        }
        const Transition &next_trans = trans[cur_full_state];
        for (int i = 0; i < n; i++) {
            char cur_dir = next_trans.dir[i];
            char to_write = next_trans.new_tape_content[i];
            writeChar(i, cur_index[i], to_write, cur_dir);
        }
        cur_state = next_trans.state;
        step_cnt += 1;
    }

    const auto sz = tapes[0].size();
    if (sz == 0) {
        return;
    }
    auto left_bound = 0u;
    auto right_bound = sz - 1;
    while (left_bound < sz) {
        if (tapes[0][left_bound] != blank_char) {
            break;
        }
        left_bound++;
    }
    while (right_bound >= 0) {
        if (tapes[0][right_bound] != blank_char) {
            break;
        }
        right_bound--;
    }
    std::string ans = "";
    for (auto i = left_bound; i <= right_bound; i++) {
        ans += tapes[0][i];
    }

    if (verbose) {
        std::cout << "Result: " << ans << '\n';
        std::cout << "==================== END ====================";
    } else {
        std::cout << ans;
    }
    clearTapes();
}
}  // namespace Turing
