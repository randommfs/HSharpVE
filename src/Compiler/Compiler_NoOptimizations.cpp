#include <iostream>

#include <Compiler/Compiler_NoOptimizations.hpp>
#include <Compiler/Opcodes.hpp>
#include <Parser/Parser.hpp>

void HSharpCompiler::Compiler_NoOpt::emit_opcode(HSharpCompiler::Opcode op, std::uint8_t arg) {
    std::cout << "Emitted: [ " << OPCODE_NAMES.at(op).c_str() << " ], arg: [ " << static_cast<std::uint16_t>(arg) << " ]\n";
}

/* Basic expression parts - literals and operators */
HSharpCompiler::ParserCallbackType HSharpCompiler::Compiler_NoOpt::get__parse_operator() noexcept {
    return std::bind(&Compiler_NoOpt::_parse_operator, this, std::placeholders::_1);
}

HSharpCompiler::ParserCallbackType HSharpCompiler::Compiler_NoOpt::get__parse_literal() noexcept {
    return std::bind(&Compiler_NoOpt::_parse_literal, this, std::placeholders::_1);
}

HSharpCompiler::ParserCallbackType HSharpCompiler::Compiler_NoOpt::get__parse_ident() noexcept {
    return std::bind(&Compiler_NoOpt::_parse_ident, this, std::placeholders::_1);
}

/* Expression handler */
HSharpCompiler::ParserCallbackType HSharpCompiler::Compiler_NoOpt::get__compile_expression() noexcept {
    return std::bind(&Compiler_NoOpt::_compile_expression, this, std::placeholders::_1);
}

