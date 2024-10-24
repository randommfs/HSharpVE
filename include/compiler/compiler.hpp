#pragma once

#include <unordered_map>
#include <string_view>
#include <vector>

#include <cstdint>

#include <compiler/opcodes.hpp>
#include <parser/parser.hpp>

namespace HSharpCompiler {
    struct Instruction {
        Opcode opc;
        std::uint8_t arg;
    };

    struct CompilerState {
        std::vector<Instruction> instructions;
        std::unordered_map<std::string_view, std::uint64_t> funcs;
    };
    using ParserCallbackType = std::function<HSharpParser::Value(std::vector<HSharpParser::Value>)>;
    class Compiler {
    private:
        std::unique_ptr<CompilerState> state;

        HSharpParser::Value _compile_expression(std::vector<HSharpParser::Value>) noexcept;
    public:
        Compiler() : state(std::make_unique<CompilerState>()) { }

        void emit_opcode(Opcode op, std::uint8_t arg = 0);

        ParserCallbackType get__compile_expression() noexcept;
    };
}