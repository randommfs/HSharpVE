#pragma once

#include <vector>

#include <cstdint>

#include <Compiler/ICompiler.hpp>

namespace HSharpCompiler {
    using ParserCallbackType = std::function<HSharpParser::Value(std::vector<HSharpParser::Value>)>;
    
    class Compiler_WithOpts : public ICompiler {
    private:
        std::unique_ptr<CompilerState> state;

        HSharpParser::Value _compile_expression(std::vector<HSharpParser::Value>) noexcept;
    public:
        Compiler_WithOpts() : state(std::make_unique<CompilerState>()) { }

        void emit_opcode(Opcode op, std::uint8_t arg = 0);

        ParserCallbackType get__compile_expression() noexcept;
    };
}