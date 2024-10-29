#include <vector>

#include <Compiler/Compiler_WithOptimizations.hpp>

HSharpParser::Value HSharpCompiler::Compiler_WithOpts::_parse_operator(std::vector<HSharpParser::Value> args) noexcept {
    return std::move(args[0]);
}

HSharpParser::Value HSharpCompiler::Compiler_WithOpts::_parse_literal(std::vector<HSharpParser::Value> args) noexcept {
    return std::move(args[0]);
}

HSharpParser::Value HSharpCompiler::Compiler_WithOpts::_parse_ident(std::vector<HSharpParser::Value> args) noexcept {
    return std::move(args[0]);
}