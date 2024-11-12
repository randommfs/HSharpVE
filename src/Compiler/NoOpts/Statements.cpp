#include "Parser/Parser.hpp"
#include <vector>
#include <iostream>

#include <Compiler/Compiler_NoOptimizations.hpp>

HSharpParser::Value HSharpCompiler::Compiler_NoOpt::_compile_var_creation(std::vector<HSharpParser::Value>&& args) noexcept {
  HSharpParser::Token& tok = std::get<HSharpParser::Token>(args[1].value);
  _compile_expression(args[3]);
  emit_opcode(Opcode::STORE_FAST, state->allocate_var_name(tok.str));
  return {};
}