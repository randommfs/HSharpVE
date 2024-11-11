#pragma once

#include <cstdint>

#include <pog/pog.h>
#include <variant>

namespace HSharpCompiler {
    class ICompiler;
    struct NodeExpr;
}

namespace HSharpParser {
    enum TokenType : std::uint_fast8_t {
        UNDEFINED = 0,
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
        BOOL_LIT,
        FLOAT_LIT,
        EXPR
    };
    
    struct Token {
        TokenType type;
        std::string_view str;
    };

    enum ParserValueType{
        EXPRESSION,
        TOKEN
    };

    struct ParserValue {
        ParserValueType type;
        std::variant<Token, HSharpCompiler::NodeExpr*> value;
    };

    using Value = ParserValue;

    class Parser {
    private:
        pog::Parser<Value> parser;
        HSharpCompiler::ICompiler* compiler;

        void _apply_parser_rules() noexcept;
    public:
        Parser(HSharpCompiler::ICompiler*);

        pog::ParserReport<Value> prepare();
        std::optional<Value> parse(std::string contents);
    };
}
