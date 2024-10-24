#pragma once

#include <cstdint>
#include <pog/pog.h>

namespace HSharpCompiler {
    class Compiler;
}

namespace HSharpParser {
    enum TokenType : std::uint_fast8_t {
        ASSIGN_ADD,
        ASSIGN_SUB,
        ASSIGN_MUL,
        ASSIGN_DIV,
        OP_ADD = '+',
        OP_SUB = '-',
        OP_MUL = '*',
        OP_DIV = '/',
        TYPE,
        IDENT,
        INT_LIT,
        STR_LIT,
        BOOL_LIT
    };
    struct Token {
        TokenType type;
        std::string_view str;
    };

    using Value = std::variant<Token>;

    class Parser {
    private:
        pog::Parser<Value> parser;
        HSharpCompiler::Compiler& compiler;

        std::function<Value(std::vector<Value>)> compile_expr;
        std::function<Value(std::vector<Value>)> type_to_tkn;

        void _apply_parser_rules() noexcept;
    public:
        Parser(HSharpCompiler::Compiler&);

        pog::ParserReport<Value> prepare();
        std::optional<Value> parse(std::ifstream& contents);
    };
}
