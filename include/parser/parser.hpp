#pragma once

#include <pog/pog.h>

namespace HSharpParser {
    class Parser {
    private:
        using Value = std::variant<int, bool, std::string>;

        pog::Parser<Value> parser;

        void _apply_parser_rules() noexcept;
    public:
        Parser();

        pog::ParserReport<Value> prepare();
        std::optional<Value> parse(std::istream input);
    };
}
