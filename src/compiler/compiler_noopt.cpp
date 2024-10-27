#include "parser/parser.hpp"
#include <iostream>

#include <compiler/compiler.hpp>

void HSharpCompiler::Compiler::emit_opcode(HSharpCompiler::Opcode op, std::uint8_t arg) {
    std::cout << "Emitted: [ " << OPCODE_NAMES.at(op).c_str() << " ], arg: [ " << static_cast<std::uint16_t>(arg) << " ]\n";
}

HSharpCompiler::ParserCallbackType HSharpCompiler::Compiler::get__compile_expression() noexcept {
    return std::bind(&Compiler::_compile_expression, this, std::placeholders::_1);
}

HSharpParser::Value HSharpCompiler::Compiler::_compile_expression(std::vector<HSharpParser::Value> args) noexcept {
    HSharpParser::Token& tok1 = std::get<HSharpParser::Token>(args[0]);
    HSharpParser::Token& tok3 = std::get<HSharpParser::Token>(args[2]);

    std::cout << "Compiling expression\n";

    if (tok1.type != tok3.type && tok1.type != HSharpParser::TokenType::INT_LIT) {
        std::cout << "Expression could not be compiled!\n";
        exit(1);
    }

    try {
        int lit1 = std::stoi({tok1.str.begin()});
        int lit2 = std::stoi({tok3.str.begin()});
        emit_opcode(HSharpCompiler::Opcode::PUSH_CONST, lit1);
        emit_opcode(HSharpCompiler::Opcode::PUSH_CONST, lit2);
        emit_opcode(HSharpCompiler::Opcode::ADD_BINARY);
    } catch (std::invalid_argument exc) {

    } catch (std::out_of_range exc ) {

    }
    return {};
}