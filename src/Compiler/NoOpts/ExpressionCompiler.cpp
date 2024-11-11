#include "Compiler/AST/Nodes.hpp"
#include "Compiler/Compiler_NoOptimizations.hpp"
#include "Compiler/ICompiler.hpp"
#include "Parser/Parser.hpp"
#include <Compiler/Compiler_NoOptimizations.hpp>

using namespace HSharpCompiler;

template<typename... T>
struct overloaded : T... {
  using T::operator()...;
};

void HSharpCompiler::Compiler_NoOpt::_compile_expression(HSharpParser::Value& val) noexcept {
  if (val.type == HSharpParser::ParserValueType::TOKEN) {
    HSharpParser::Token token = std::get<HSharpParser::Token>(val.value);
    switch (token.type) {
      case HSharpParser::TokenType::INT_LIT:
        state->push_const(std::stoull(std::string{token.str}));
        break;
      case HSharpParser::TokenType::FLOAT_LIT:
        state->push_const(std::stod(std::string{token.str}));
        break;
      case HSharpParser::TokenType::STR_LIT:
        state->push_const(token.str);
        break;
      case HSharpParser::TokenType::BOOL_LIT:
        state->push_const(token.str == "true");
        break;
      default:
        throw std::runtime_error("Unknown token type - can't push constant and compile expression");
    }
  }

  auto parse_expr = overloaded{
    [](NodeTerm* term) {

    },
    [](NodeBinExpr* expr) {

    },
  }
}