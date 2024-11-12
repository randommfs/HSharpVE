#include "Compiler/AST/Nodes.hpp"
#include "Compiler/Compiler_NoOptimizations.hpp"
#include "Compiler/ICompiler.hpp"
#include "Parser/Parser.hpp"
#include <Compiler/Compiler_NoOptimizations.hpp>

using namespace HSharpCompiler;

void HSharpCompiler::Compiler_NoOpt::_compile_expression(HSharpParser::Value& val) noexcept {
  if (val.type == HSharpParser::ParserValueType::TOKEN) {
    HSharpParser::Token& token = std::get<HSharpParser::Token>(val.value);
    std::uint32_t id;
    switch (token.type) {
      case HSharpParser::TokenType::INT_LIT:
        id = state->push_const(std::stoull(std::string{token.str}));
        break;
      case HSharpParser::TokenType::FLOAT_LIT:
        id = state->push_const(std::stod(std::string{token.str}));
        break;
      case HSharpParser::TokenType::STR_LIT:
        id = state->push_const(token.str);
        break;
      case HSharpParser::TokenType::BOOL_LIT:
        id = state->push_const(token.str == "true");
        break;
      default:
        throw std::runtime_error("Unknown token type - can't push constant and compile expression");
    }
    emit_opcode(Opcode::LOAD_CONST, id);
  } else {
    NodeExpr* expr = std::get<NodeExpr*>(val.value);
    std::visit(exprvisitor, expr->expr);
  }

}