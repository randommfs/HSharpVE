#include "Compiler/AST/Nodes.hpp"
#include "Compiler/ICompiler.hpp"
#include <iostream>

#include <Compiler/Compiler_NoOptimizations.hpp>
#include <Compiler/Opcodes.hpp>
#include <Parser/Parser.hpp>
#include <stdexcept>
#include <string>

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
HSharpCompiler::ParserCallbackType HSharpCompiler::Compiler_NoOpt::get__transform_expression() noexcept {
    return std::bind(&Compiler_NoOpt::_transform_expression, this, std::placeholders::_1);
}

HSharpCompiler::ParserCallbackType HSharpCompiler::Compiler_NoOpt::get__compile_var_creation() noexcept {
    return std::bind(&Compiler_NoOpt::_compile_var_creation, this, std::placeholders::_1);
}

HSharpCompiler::NodeTerm* HSharpCompiler::Compiler_NoOpt::_get_term(HSharpParser::Value value) noexcept {
    const HSharpParser::Token& tok = std::get<Token>(value.value);
    switch (tok.type) {
        case HSharpParser::INT_LIT:
            return allocator.emplace<NodeTerm>(allocator.emplace<NodeTermIntLit>(tok));
        case HSharpParser::BOOL_LIT:
            return allocator.emplace<NodeTerm>(allocator.emplace<NodeTermBoolLit>(tok));
        case HSharpParser::STR_LIT:
            return allocator.emplace<NodeTerm>(allocator.emplace<NodeTermString>(tok));
        case HSharpParser::FLOAT_LIT:
            return allocator.emplace<NodeTerm>(allocator.emplace<NodeTermFloat>(tok));
        default: break;
    }
    return nullptr;
}