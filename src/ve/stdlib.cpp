#include <iostream>
#include <cstring>

#include <ve/exceptions.hpp>
#include <parser/parser.hpp>
#include <ve/ve.hpp>
#include <hpool/cpp/hpool.hpp>

using HSharp::ValueInfo;


ValueInfo HSharpVE::VirtualEnvironment::ExpressionVisitor_ExprStrLit(NodeExpressionStrLit* expr) {
    auto str = static_cast<std::string*>(strings_pool.allocate());
    *str = std::string(expr->str_lit.value.value());
    return {VariableType::STRING, str};
}

ValueInfo HSharpVE::VirtualEnvironment::ExpressionVisitor_ExprIntLit(HSharpParser::NodeTermIntLit* expr) {
    if (!is_number(expr->int_lit.value.value())) {
        std::cerr << "Expression is not valid integer!" << std::endl;
        exit(1);
    } else {
        auto value = static_cast<int64_t*>(integers_pool.allocate());
        *value = std::stol(expr->int_lit.value.value());
        return {VariableType::INT, value};
    }
}

ValueInfo HSharpVE::VirtualEnvironment::ExpressionVisitor_ExprIdent(HSharpParser::NodeTermIdent* expr) {
    if (!variables.exists(expr->ident.value.value()))
        std::terminate();
    else {
        return {
            variables.at(expr->ident.value.value()).value()->vtype,
            variables.at(expr->ident.value.value()).value()->value
        };
    }
}

ValueInfo HSharpVE::VirtualEnvironment::ExpressionVisitor_BinExpr(HSharpParser::NodeBinExpr *expr) const {
    return {};
}
