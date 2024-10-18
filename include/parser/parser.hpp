#pragma once

#include <pog/pog.h>

namespace HSharpParser {
    class Parser {
    private:
        using Value = std::variant<int, bool, std::string>;

        pog::Parser<Value> parser;
    public:
        Parser();
    };
}
