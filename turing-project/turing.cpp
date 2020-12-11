#include "turing.h"
#include <cassert>
#include <cctype>
#include <fstream>

namespace Turing {
inline bool isWhiteSpace(char c) {
    return c == ' ' || c == '\r' || c == '\n';
}

inline bool isStateChar(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') || c == '_';
}

inline bool isTapeChar(char c) {
    return isprint(c) && (c != ' ' && c != ',' && c != ';' && c != '{' &&
                          c != '}' && c != '*');
}

inline bool isInputChar(char c) {
    return isTapeChar(c) && c != '_';
}

inline void eatWhiteSpace(string::size_type &cur, std::string &str) {
    std::string::size_type len = str.length();
    while (cur < len && isWhiteSpace(str[cur])) {
        cur++;
    }
}

inline int inputAssert(string::size_type &cur, std::string &str, const char *expect, string::size_type expect_len) {
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

int readStateSet(StateSet &state_sets, std::string &str, string::size_type &cur) {
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

int readTapeAlphabet(Alphabet &alphabet, std::string &str, string::size_type &cur) {
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
            return -1; // input alphabet must be valid
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

int readInputAlphabet(Alphabet &alphabet, std::string &str, string::size_type &cur) {
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
            return -1; // input alphabet must be valid
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

TuringMachine *getTuringMachine(std::ifstream &in, std::string &error_msg) {
    int tape_cnt = 0;
    Alphabet input_alphabet, tape_alphabet;
    StateSet state_sets;
    StateSet fin_states;
    State init_state;
    TransFunc trans;
    char blank_char = '_';

    std::string buff;
    while (std::getline(in, buff)) {
        const string::size_type len = buff.length();
        string::size_type cur = 0;
        eatWhiteSpace(cur, buff);

        /*
         * parse_state: 0: nothing input, 1: '#' read, 2: end of line or comments
         */
        int parse_state = 0;
        while (cur < len && parse_state >= 0) {
            eatWhiteSpace(cur, buff);
            if (parse_state == 0) {
                if (buff[cur] == ';') {
                    break;
                } else if (buff[cur] == '#') {
                    parse_state = 1;
                    cur++;
                } else {
                    if (readTrans(trans, buff, cur) < 0) {
                        parse_state = -1;
                        break;
                    }
                    parse_state = 2;
                }
            } else if (parse_state == 1) {
                switch (buff[cur]) {
                case 'Q':
                    cur += 1;
                    eatWhiteSpace(cur, buff);
                    if (inputAssert(cur, buff, "=", 1) < 0) {
                        parse_state = -1;
                        break;
                    }
                    if (readStateSet(state_sets, buff, cur) < 0) {
                        parse_state = -1;
                        break;
                    }
                    parse_state = 2;
                    break;
                case 'S':
                    cur += 1;
                    eatWhiteSpace(cur, buff);
                    if (inputAssert(cur, buff, "=", 1) < 0) {
                        parse_state = -1;
                        break;
                    }
                    if (readInputAlphabet(input_alphabet, buff, cur) < 0) {
                        parse_state = -1;
                        break;
                    }
                    parse_state = 2;
                    break;
                case 'G':
                    cur += 1;
                    eatWhiteSpace(cur, buff);
                    if (inputAssert(cur, buff, "=", 1) < 0) {
                        parse_state = -1;
                        break;
                    }
                    if (readTapeAlphabet(tape_alphabet, buff, cur) < 0) {
                        parse_state = -1;
                        break;
                    }
                    parse_state = 2;
                    break;
                case 'q':
                    cur += 1;
                    if (inputAssert(cur, buff, "0", 1) < 0) {
                        parse_state = -1;
                        break;
                    }
                    eatWhiteSpace(cur, buff);
                    if (inputAssert(cur, buff, "=", 1) < 0) {
                        parse_state = -1;
                        break;
                    }
                    if (readState(init_state, buff, cur) < 0) {
                        parse_state = -1;
                        break;
                    }
                    parse_state = 2;
                    break;
                case 'B':
                    cur += 1;
                    eatWhiteSpace(cur, buff);
                    if (inputAssert(cur, buff, "=", 1) < 0) {
                        parse_state = -1;
                        break;
                    }
                    eatWhiteSpace(cur, buff);
                    if (cur >= len) {
                        parse_state = -1;
                        break;
                    }
                    blank_char = buff[cur];
                    if (!isTapeChar(blank_char)) {
                        parse_state = -1;
                        break;
                    }
                    cur++;
                    parse_state = 2;
                    break;
                case 'F':
                    cur += 1;
                    eatWhiteSpace(cur, buff);
                    if (inputAssert(cur, buff, "=", 1) < 0) {
                        parse_state = -1;
                        break;
                    }
                    if (readStateSet(fin_states, buff, cur) < 0) {
                        parse_state = -1;
                        break;
                    }
                    parse_state = 2;
                    break;
                case 'N':
                    cur += 1;
                    eatWhiteSpace(cur, buff);
                    if (inputAssert(cur, buff, "=", 1) < 0) {
                        parse_state = -1;
                        break;
                    }
                    if (readInt(tape_cnt, buff, cur) < 0) {
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
                if (cur != buff.length() && buff[cur] != ';') {
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
    // TODO: check whether all information is inputed. (Q, S, G, q0, B, F, N, delta)
    // TODO: check whether all states is in state set
    return new TuringMachine(tape_cnt, blank_char, input_alphabet, tape_alphabet, state_sets, fin_states, init_state, trans);
}
} // namespace Turing
