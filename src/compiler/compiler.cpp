#include "parser/parser.hpp"
#include <iostream>

#include <compiler/compiler.hpp>

void HSharpCompiler::Compiler::emit_opcode(HSharpCompiler::Opcode op, std::uint8_t arg) {
    std::cout << "Emitted: [ " << OPCODE_NAMES.at(op).c_str() << " ], arg: [ " << arg << " ]\n";
}

HSharpCompiler::ParserCallbackType HSharpCompiler::Compiler::get__compile_expression() noexcept {
    return std::bind(&Compiler::_compile_expression, this, std::placeholders::_1);
}

HSharpParser::Value HSharpCompiler::Compiler::_compile_expression(std::vector<HSharpParser::Value> args) noexcept {
    HSharpParser::Token& tok1 = std::get<HSharpParser::Token>(args[0]);
    HSharpParser::Token& tok2 = std::get<HSharpParser::Token>(args[1]);
    HSharpParser::Token& tok3 = std::get<HSharpParser::Token>(args[2]);

    std::cout << "Compiling expression\n";

    if (tok1.type != tok3.type && tok1.type != HSharpParser::TokenType::INT_LIT) {
    std::cout << "Expression could not be compiled!\n";
    exit(1);
    }

    try {
        int lit1 = std::stoi({tok1.str.begin()});
        int lit2 = std::stoi({tok1.str.begin()});
        std::cout << "Expression compiled!\n";
    } catch (std::invalid_argument exc) {

    } catch (std::out_of_range exc ) {

    }
    return {};
}