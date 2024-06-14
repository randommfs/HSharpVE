#include "parser/nodes.hpp"
#include <visitors.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <format>

#include <cstring>

#include <ve/exceptions.hpp>
#include <parser/parser.hpp>
#include <ve/ve.hpp>

using std::uint32_t;
using std::string;

/* All internal function definitions */
void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeStmtInput *stmt) const {
    error(EExceptionSource::VIRTUAL_ENV,EExceptionReason::NOT_IMPLEMENTED,"input() is not implemented");
}

void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeStmtPrint *stmt) const {
    ValueInfo pair = std::visit(parent->exprvisitor, stmt->expr->expr);
    std::string result;
    switch (pair.type){
        case VariableType::INT:
            result = std::to_string(*static_cast<int64_t*>(pair.value));
            break;
        case VariableType::STRING:
            result = *static_cast<std::string*>(pair.value);
            break;
        default:
            error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "print(): conversion failed: unknown type");
    }
    std::puts(result.c_str());
    parent->dispose_value(pair);
}

void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeStmtExit *stmt) const {
    int64_t exitcode;
    ValueInfo pair = std::visit(parent->exprvisitor, stmt->expr->expr);
    switch(pair.type){
        case VariableType::INT:
            exitcode = *static_cast<int64_t*>(pair.value);
            break;
        case VariableType::STRING:{
            std::string* ptr = static_cast<std::string*>(pair.value);
            if (!is_number(*ptr)){
                std::string msg{};
                msg.append("exit(): conversion failed: string is not convertable to number\n");
                msg.append(std::format("\tLine {}: {}", stmt->line, parent->lines.at(stmt->line - 1)));
                error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, msg);
            }
            exitcode = std::stol(*ptr);
            break;
        }
        default:
            error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "exit(): conversion failed: unknown type");
    }
    parent->dispose_value(pair);
    parent->delete_variables();
    exit(exitcode);
}

void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeStmtVar *stmt) const {
    if (parent->variable_exists(stmt->ident.value.value())) {
        std::cerr << "Variable reinitialization is not allowed\n";
        exit(1);
    } else {
        ValueInfo pair = std::visit(parent->exprvisitor, stmt->expr->expr);
        parent->dispose_value(pair);
        parent->create_variable(stmt->ident.value.value(), pair.type).value = pair.value;
    }
}

void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeStmtVarAssign *stmt) const {
    if (!parent->variable_exists(stmt->ident.value.value()))
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Cannot assign value to immediate value");
    auto variable = parent->get_variable(stmt->ident.value.value());
    Variable temp;
    ValueInfo info = std::visit(parent->exprvisitor, stmt->expr->expr);
    void* addr = parent->allocate(info.type);
    switch(info.type){
        case VariableType::INT:
            memcpy(addr, info.value, sizeof(int64_t));
            break;
        case VariableType::STRING:
            addr = new(addr)std::string(static_cast<std::string*>(info.value)->c_str());
            break;
        default:
            error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Cannot assign value: invalid type");
    }
    parent->dispose_value(info);
    parent->set_variable(stmt->ident.value.value(), info.type, addr);
}

void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeScope *stmt) const {
    parent->create_scope();
    for (auto statement : stmt->statements)
        std::visit(parent->stmtvisitor, statement->statement);
    parent->destroy_scope();
}

void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeStmtIf *stmt) const {
    ValueInfo vi = std::visit(parent->exprvisitor, stmt->expr->expr);
    if (static_cast<bool>(*static_cast<int64_t*>(vi.value))){
        parent->create_scope();
        for (auto statement : stmt->scope->statements)
            std::visit(parent->stmtvisitor, statement->statement);
        parent->destroy_scope();
    }
    
}

HSharp::ValueInfo HSharpVE::VirtualEnvironment::ExpressionVisitor::operator()(HSharpParser::NodeTerm *term) const {
    return std::visit(parent->termvisitor, term->term);
}

HSharp::ValueInfo HSharpVE::VirtualEnvironment::ExpressionVisitor::operator()(HSharpParser::NodeExpressionStrLit *expr) const {
    auto str = static_cast<std::string*>(parent->strings_pool.allocate());
    str = new(str)std::string(expr->str_lit.value.value());
    return ValueInfo{
            .type = VariableType::STRING,
            .value = str,
            .line = expr->line,
            .dealloc_required = true
    };
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::ExpressionVisitor::operator()(HSharpParser::NodeBinExpr *expr) const {
    return std::visit(parent->binexprvisitor, expr->var);
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::TermVisitor::operator()(HSharpParser::NodeTermIdent *term) const {
    if (!parent->variable_exists(term->ident.value.value())){
        std::cerr << "Invalid identifier" << std::endl;
        exit(1);
    }
    auto var = parent->get_variable(term->ident.value.value());
    return {
            .type = var.value()->vtype,
            .value = var.value()->value,
            .line = term->line,
            .dealloc_required = false
    };
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::TermVisitor::operator()(HSharpParser::NodeTermIntLit *term) const {
    if (!is_number(term->int_lit.value.value())) {
        std::cerr << "Expression is not valid integer!" << std::endl;
        exit(1);
    }
    auto num = parent->integers_pool.allocate();
    *num = std::stol(term->int_lit.value.value());
    return {.type = VariableType::INT,
            .value = num,
            .line = term->line,
            .dealloc_required = true};
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::TermVisitor::operator()(HSharpParser::NodeTermParen *term) const {
    return std::visit(parent->exprvisitor, term->expr->expr);
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(HSharpParser::NodeBinExprAdd *expr) const {
    auto result = parent->integers_pool.allocate();
    ValueInfo lhs, rhs;
    if ((lhs = std::visit(parent->exprvisitor, expr->lhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    if ((rhs = std::visit(parent->exprvisitor, expr->rhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    *result = *static_cast<int64_t*>(lhs.value) + *static_cast<int64_t*>(rhs.value);
    parent->dispose_value(lhs);
    parent->dispose_value(rhs);
    return ValueInfo{.type = VariableType::INT, .value = result, .dealloc_required = true};
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(HSharpParser::NodeBinExprSub *expr) const {
    auto result = parent->integers_pool.allocate();
    ValueInfo lhs, rhs;
    if ((lhs = std::visit(parent->exprvisitor, expr->lhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    if ((rhs = std::visit(parent->exprvisitor, expr->rhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    *result = *static_cast<int64_t*>(lhs.value) - *static_cast<int64_t*>(rhs.value);
    parent->dispose_value(lhs);
    parent->dispose_value(rhs);
    return ValueInfo{.type = VariableType::INT, .value = result, .dealloc_required = true};
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(HSharpParser::NodeBinExprMul *expr) const {
    auto result = parent->integers_pool.allocate();
    ValueInfo lhs, rhs;
    if ((lhs = std::visit(parent->exprvisitor, expr->lhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    if ((rhs = std::visit(parent->exprvisitor, expr->rhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    *result = *static_cast<int64_t*>(lhs.value) * *static_cast<int64_t*>(rhs.value);
    parent->dispose_value(lhs);
    parent->dispose_value(rhs);
    return ValueInfo{.type = VariableType::INT, .value = result, .dealloc_required = true};
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(HSharpParser::NodeBinExprDiv *expr) const {
    auto result = parent->integers_pool.allocate();
    ValueInfo lhs, rhs;
    if ((lhs = std::visit(parent->exprvisitor, expr->lhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    if ((rhs = std::visit(parent->exprvisitor, expr->rhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    *result = *static_cast<int64_t*>(lhs.value) / *static_cast<int64_t*>(rhs.value);
    parent->dispose_value(lhs);
    parent->dispose_value(rhs);
    return ValueInfo{.type = VariableType::INT, .value = result, .dealloc_required = true};
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(HSharpParser::NodeBinExprEq* expr) const {
    auto result = parent->integers_pool.allocate();
    ValueInfo lhs, rhs;
    if ((lhs = std::visit(parent->exprvisitor, expr->lhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    if ((rhs = std::visit(parent->exprvisitor, expr->rhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    *result = *static_cast<int64_t*>(lhs.value) == *static_cast<int64_t*>(rhs.value);
    parent->dispose_value(lhs);
    parent->dispose_value(rhs);
    return ValueInfo{.type = VariableType::INT, .value = result, .dealloc_required = true};
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(HSharpParser::NodeBinExprLess *expr) const {
    auto result = parent->integers_pool.allocate();
    ValueInfo lhs, rhs;
    if ((lhs = std::visit(parent->exprvisitor, expr->lhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    if ((rhs = std::visit(parent->exprvisitor, expr->rhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    *result = *static_cast<int64_t*>(lhs.value) < *static_cast<int64_t*>(rhs.value);
    parent->dispose_value(lhs);
    parent->dispose_value(rhs);
    return ValueInfo{.type = VariableType::INT, .value = result, .dealloc_required = true};
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(HSharpParser::NodeBinExprBig* expr) const {
    auto result = parent->integers_pool.allocate();
    ValueInfo lhs, rhs;
    if ((lhs = std::visit(parent->exprvisitor, expr->lhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    if ((rhs = std::visit(parent->exprvisitor, expr->rhs->expr)).type != VariableType::INT)
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Binary expression evaluation impossible: invalid literal type");
    *result = *static_cast<int64_t*>(lhs.value) > *static_cast<int64_t*>(rhs.value);
    parent->dispose_value(lhs);
    parent->dispose_value(rhs);
    return ValueInfo{.type = VariableType::INT, .value = result, .dealloc_required = true};
}

void HSharpVE::VirtualEnvironment::PredVisitor::operator()(HSharpParser::NodeIfPredOr* or_) {
    ValueInfo vi = std::visit(parent->exprvisitor, or_->expr->expr);
    if (!static_cast<bool>(*static_cast<int64_t*>(vi.value)))
        return;
    parent->create_scope();
    for (auto statement : or_->scope->statements)
        std::visit(parent->stmtvisitor, statement->statement);
    parent->destroy_scope();
}
void HSharpVE::VirtualEnvironment::PredVisitor::operator()(HSharpParser::NodeIfPredElse* else_) {
    
}

void HSharpVE::VirtualEnvironment::delete_var_value(const HSharpVE::Variable &variable) {
    switch(variable.vtype){
        case VariableType::INT:
            integers_pool.free(static_cast<int64_t*>(variable.value));
            break;
        case VariableType::STRING:
            static_cast<string*>(variable.value)->~string();
            strings_pool.free(static_cast<string*>(variable.value));
            break;
        default:
            error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Cannot delete value: invalid type");
    }
}
void* HSharpVE::VirtualEnvironment::allocate(HSharp::VariableType vtype) {
    switch(vtype){
        case VariableType::INT:
            return integers_pool.allocate();
        case VariableType::STRING:
            return strings_pool.allocate();
        default:
            error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Cannot allocate value: invalid type");
    }
}
void HSharpVE::VirtualEnvironment::exec_statement(const HSharpParser::NodeStmt* stmt) {
    std::visit(stmtvisitor, stmt->statement);
}
void HSharpVE::VirtualEnvironment::delete_variables() {
    for (auto scope : global_scopes) {
        std::for_each(std::begin(scope), std::end(scope), [](std::pair<std::string, Variable> f){
                if (f.second.vtype == HSharp::VariableType::STRING)
                    static_cast<std::string*>(f.second.value)->~string();
            });
    }
    while (!function_scopes.empty()) {
        FunctionScope& ptr = function_scopes.top();
        function_scopes.pop();
        for (auto scope : ptr) {
            std::for_each(std::begin(scope), std::end(scope), [](std::pair<std::string, Variable> f){
                if (f.second.vtype == HSharp::VariableType::STRING)
                    static_cast<std::string*>(f.second.value)->~string();
            });
        }
        ptr.clear();
    }
}
void HSharpVE::VirtualEnvironment::dispose_value(ValueInfo& data) {
    if (!data.dealloc_required) return;
    switch (data.type) {
        case VariableType::INT:
            integers_pool.free(static_cast<int64_t*>(data.value));
            break;
        case VariableType::STRING: 
            strings_pool.free(static_cast<std::string*>(data.value));
            break;
        default:
            error(EExceptionSource::VIRTUAL_ENV,
                    EExceptionReason::DEALLOC_ERROR,
                    "Failed to deallocate variable");
    }
}
void HSharpVE::VirtualEnvironment::run() {
    global_scopes.push_back({});
    for (const HSharpParser::NodeStmt* stmt : root.statements)
        exec_statement(stmt);
}

bool HSharpVE::VirtualEnvironment::is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}