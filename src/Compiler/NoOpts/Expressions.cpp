#include "Compiler/Opcodes.hpp"
#include "Parser/Parser.hpp"
#include <vector>
#include <iostream>

#include <Compiler/Compiler_NoOptimizations.hpp>

HSharpParser::Value HSharpCompiler::Compiler_NoOpt::_compile_expression(std::vector<HSharpParser::Value>&& args) noexcept {
    HSharpParser::Token& tok1 = std::get<HSharpParser::Token>(args[0]);
    HSharpParser::Token& tok3 = std::get<HSharpParser::Token>(args[2]);


    if (tok1.type != tok3.type && tok1.type != HSharpParser::TokenType::INT_LIT) {
        std::cout << "Expression could not be compiled!\n";
        exit(1);
    }

    try {
        int lit1 = std::stoi({tok1.str.begin()});
        int lit2 = std::stoi({tok3.str.begin()});
        emit_opcode(HSharpCompiler::Opcode::PUSH_CONST, lit1);
        emit_opcode(HSharpCompiler::Opcode::PUSH_CONST, lit2);
        emit_opcode([args]() -> HSharpCompiler::Opcode {
            switch(std::get<HSharpParser::Token>(args[1]).type) {
            case HSharpParser::TokenType::OP_ADD: return HSharpCompiler::Opcode::ADD_BINARY;
            case HSharpParser::TokenType::OP_SUB: return HSharpCompiler::Opcode::SUB_BINARY;
            case HSharpParser::TokenType::OP_MUL: return HSharpCompiler::Opcode::MUL_BINARY;
            case HSharpParser::TokenType::OP_DIV: return HSharpCompiler::Opcode::DIV_BINARY;
            default: throw std::runtime_error("nuh uh stfu");
        }}(), 0);
    } catch (std::invalid_argument exc) {

    } catch (std::out_of_range exc ) {

    }
    return HSharpParser::Token{HSharpParser::TokenType::EXPR};
}

