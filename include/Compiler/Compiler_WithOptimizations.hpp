#pragma once

#include <vector>

#include <cstdint>

#include <Compiler/ICompiler.hpp>

namespace HSharpCompiler {
        class Compiler_WithOpts : public ICompiler {
    private:
        std::unique_ptr<CompilerState> state;

        HSharpParser::Value _parse_operator(std::vector<HSharpParser::Value>) noexcept;
        HSharpParser::Value _parse_literal(std::vector<HSharpParser::Value>) noexcept;
        HSharpParser::Value _parse_ident(std::vector<HSharpParser::Value>) noexcept;

        HSharpParser::Value _compile_expression(std::vector<HSharpParser::Value>) noexcept;
    public:
        Compiler_WithOpts() : state(std::make_unique<CompilerState>()) { }

        void emit_opcode(Opcode op, std::uint8_t arg = 0) override;

        /* Handlers for basic expression parts - operators and tokens*/
        ParserCallbackType get__parse_operator() noexcept override;
        ParserCallbackType get__parse_literal() noexcept override;
        ParserCallbackType get__parse_ident() noexcept override;

        ParserCallbackType get__compile_expression() noexcept override;
    };
}