#include "Compiler/Compiler_WithOptimizations.hpp"
#include <iostream>

#include <Compiler/Compiler_WithOptimizations.hpp>
#include <Compiler/Opcodes.hpp>
#include <Parser/Parser.hpp>

void HSharpCompiler::Compiler_WithOpts::emit_opcode(HSharpCompiler::Opcode op, std::uint8_t arg) {
    std::cout << "Emitted: [ " << OPCODE_NAMES.at(op).c_str() << " ], arg: [ " << static_cast<std::uint16_t>(arg) << " ]\n";
}

HSharpCompiler::ParserCallbackType HSharpCompiler::Compiler_WithOpts::get__transform_expression() noexcept {
    return std::bind(&Compiler_WithOpts::_transform_expression, this, std::placeholders::_1);
}

HSharpParser::Value HSharpCompiler::Compiler_WithOpts::_transform_expression(std::vector<HSharpParser::Value> args) noexcept {
    return {};
}