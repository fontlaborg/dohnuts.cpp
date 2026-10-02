// this_file: tests/side_helpers.cpp
#include "dohnuts/side/common.hpp"
#include "dohnuts/side/jpt_prompt.hpp"
#include <stdexcept>
#include <iostream>

int main(int argc, char ** argv) {
    if (argc == 3) {
        std::cout << dohnuts::side::jpt_chat_prompt(argv[1], argv[2]);
        return 0;
    }
    const auto prompt = dohnuts::side::jpt_chat_prompt(" U\n", "Question: Q\nOptions:\nA. yes\nB. no");
    if (prompt.find("<|im_start|>user\nU<|im_end|>\n<|im_start|>user\n") != 0
        || prompt.find("Question: Q\nOptions:\nA. yes\nB. no<|im_end|>\n<|im_start|>assistant\n<think>\n\n</think>\n\nAnswer:") == std::string::npos)
        throw std::runtime_error("JPT must place the question inside the user turn and open exactly one assistant turn");
    using dohnuts::side::noul_probability;
    if (noul_probability({"false", "true"}, {0.2, 0.8}) != 0.8)
        throw std::runtime_error("false-first noul must read P(true)");
    if (noul_probability({"true", "false"}, {0.8, 0.2}) != 0.8)
        throw std::runtime_error("true-first JPT noul must read P(true)");
    if (noul_probability({"no", "yes"}, {0.2, 0.8}) != 0.8)
        throw std::runtime_error("Jet noul must read P(yes)");
    try {
        noul_probability({"A", "B"}, {0.2, 0.8});
    } catch (const std::invalid_argument &) {
        return 0;
    }
    throw std::runtime_error("missing affirmative label must be rejected");
}
