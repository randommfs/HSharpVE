#include "Compiler/Opcodes.hpp"
#include "Parser/Parser.hpp"
#include <vector>
#include <iostream>

#include <Compiler/Compiler_NoOptimizations.hpp>

HSharpParser::Value HSharpCompiler::Compiler_NoOpt::_transform_expression(std::vector<HSharpParser::Value>&& args) noexcept {
    NodeExpr *lhs, *rhs;
    if (args[0].type == HSharpParser::ParserValueType::TOKEN)
        lhs = allocator.emplace<NodeExpr>(_get_term(args[0]));
    else
        lhs = std::get<NodeExpr*>(args[0].value);

    if (args[2].type == HSharpParser::ParserValueType::TOKEN)
        rhs = allocator.emplace<NodeExpr>(_get_term(args[2]));
    else
        rhs = std::get<NodeExpr*>(args[2].value);

    switch (std::get<Token>(args[1].value).type) {
        case '+': return {.value=allocator.emplace<NodeExpr>(allocator.emplace<NodeBinExpr>(allocator.emplace<NodeBinExprAdd>(lhs, rhs)))};
        case '-': return {.value=allocator.emplace<NodeExpr>(allocator.emplace<NodeBinExpr>(allocator.emplace<NodeBinExprSub>(lhs, rhs)))};
        case '*': return {.value=allocator.emplace<NodeExpr>(allocator.emplace<NodeBinExpr>(allocator.emplace<NodeBinExprMul>(lhs, rhs)))};
        case '/': return {.value=allocator.emplace<NodeExpr>(allocator.emplace<NodeBinExpr>(allocator.emplace<NodeBinExprDiv>(lhs, rhs)))};
        default: exit(2);
    }
    return {};
}

