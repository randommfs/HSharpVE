#pragma once

#include <vector>

#include <Parser/Parser.hpp>
#include <Compiler/Opcodes.hpp>

namespace HSharpCompiler {
    struct Instruction {
        Opcode opc;
        std::uint8_t arg;
    };

    struct CompilerState {
        std::vector<Instruction> instructions;
        std::unordered_map<std::string_view, std::uint64_t> funcs;
    };

    using ParserCallbackType = std::function<HSharpParser::Value(std::vector<HSharpParser::Value>&&)>;
    class ICompiler {
    public:
        virtual void emit_opcode(Opcode op, std::uint8_t arg = 0) = 0;

        /* Handlers for basic expression parts - operators and tokens*/
        virtual ParserCallbackType get__parse_operator() noexcept = 0;
        virtual ParserCallbackType get__parse_literal() noexcept = 0;
        virtual ParserCallbackType get__parse_ident() noexcept = 0;

        virtual ParserCallbackType get__compile_expression() noexcept = 0;
    };
}