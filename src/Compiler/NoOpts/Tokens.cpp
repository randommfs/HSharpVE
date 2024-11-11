#include "Compiler/Compiler_NoOptimizations.hpp"
#include "Parser/Parser.hpp"
#include <vector>
#include <iostream>

#include <Compiler/Compiler_NoOptimizations.hpp>

HSharpParser::Value HSharpCompiler::Compiler_NoOpt::_parse_operator(std::vector<HSharpParser::Value>&& args) noexcept {
    if (args[0].type != HSharpParser::TOKEN) return {};
    return {
        HSharpParser::ParserValueType::TOKEN,
        HSharpParser::Token {
            static_cast<HSharpParser::TokenType>(std::get<Token>(
                args[0].value
            ).str.at(0))
        }
    };
}

HSharpParser::Value HSharpCompiler::Compiler_NoOpt::_parse_literal(std::vector<HSharpParser::Value>&& args) noexcept {
    return args[0];
}

HSharpParser::Value HSharpCompiler::Compiler_NoOpt::_parse_ident(std::vector<HSharpParser::Value>&& args) noexcept {
    return args[0];
}