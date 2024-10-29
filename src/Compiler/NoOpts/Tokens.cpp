#include "Compiler/Compiler_NoOptimizations.hpp"
#include "Parser/Parser.hpp"
#include <vector>

#include <Compiler/Compiler_NoOptimizations.hpp>

HSharpParser::Value HSharpCompiler::Compiler_NoOpt::_parse_operator(std::vector<HSharpParser::Value>&& args) noexcept {
    return std::move(args[0]);
}

HSharpParser::Value HSharpCompiler::Compiler_NoOpt::_parse_literal(std::vector<HSharpParser::Value>&& args) noexcept {
    return args[0];
}

HSharpParser::Value HSharpCompiler::Compiler_NoOpt::_parse_ident(std::vector<HSharpParser::Value>&& args) noexcept {
    return std::move(args[0]);
}