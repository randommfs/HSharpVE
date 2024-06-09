#include <iostream>
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
    if (variables.exists(stmt->ident.value.value())) {
        std::cerr << "Variable reinitialization is not allowed\n";
        exit(1);
    } else {
        const ValueInfo pair = std::visit(exprvisitor, stmt->expr->expr);
        variables[stmt->ident.value.value(), depth] = {.vtype = pair.type, .value = pair.value};
    }
}

void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeStmtVarAssign *stmt) const {
    if (!is_variable(const_cast<char*>(stmt->ident.value.value().c_str())))
        error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Cannot assign value to immediate value");
    auto variable = &variables[stmt->ident.value.value().c_str(), depth];
    ValueInfo info = std::visit(exprvisitor, stmt->expr->expr);
    delete_var_value(*variable);
    variable->vtype = info.type;
    variable->value = allocate(info.type);
    switch(info.type){
        case VariableType::INT:
            memcpy(variable->value, info.value, sizeof(int64_t));
            break;
        case VariableType::STRING:
            variable->value = new(variable->value)std::string(*static_cast<std::string*>(info.value));
            break;
        default:
            error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Cannot assign value: invalid type");
    }
    dispose_value(info);
}

void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeScope *stmt) const {

}

void HSharpVE::VirtualEnvironment::StatementVisitor::operator()(HSharpParser::NodeStmtIf *stmt) const {

}

HSharp::ValueInfo HSharpVE::VirtualEnvironment::ExpressionVisitor::operator()(HSharpParser::NodeTerm *term) const {
    return std::visit(parent->termvisitor, term->term);
}

HSharp::ValueInfo HSharpVE::VirtualEnvironment::ExpressionVisitor::operator()(const HSharpParser::NodeExpressionStrLit *expr) const {
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
HSharp::ValueInfo HSharpVE::VirtualEnvironment::TermVisitor::operator()(const HSharpParser::NodeTermIdent *term) const {
    if (!parent->is_variable(const_cast<char*>(term->ident.value.value().c_str()))){
        std::cerr << "Invalid identifier" << std::endl;
        exit(1);
    }
    return {
            .type = parent->variables.at(term->ident.value.value()).value()->vtype,
            .value = parent->variables.at(term->ident.value.value()).value()->value,
            .line = term->line,
            .dealloc_required = false
    };
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::TermVisitor::operator()(const HSharpParser::NodeTermIntLit *term) const {
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
HSharp::ValueInfo HSharpVE::VirtualEnvironment::TermVisitor::operator()(const HSharpParser::NodeTermParen *term) const {
    return std::visit(parent->exprvisitor, term->expr->expr);
}
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(const HSharpParser::NodeBinExprAdd *expr) const {
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
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(const HSharpParser::NodeBinExprSub *expr) const {
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
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(const HSharpParser::NodeBinExprMul *expr) const {
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
HSharp::ValueInfo HSharpVE::VirtualEnvironment::BinExprVisitor::operator()(const HSharpParser::NodeBinExprDiv *expr) const {
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

void HSharpVE::VirtualEnvironment::delete_var_value(HSharpVE::Variable &variable) {
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
            error(EExceptionSource::VIRTUAL_ENV, EExceptionReason::TYPE_ERROR, "Cannot delete value: invalid type");
    }
}
void HSharpVE::VirtualEnvironment::exec_statement(const HSharpParser::NodeStmt* stmt) {
    std::visit(stmtvisitor, stmt->statement);
}
void HSharpVE::VirtualEnvironment::delete_variables() {
    variables.clean(integers_pool, strings_pool);
}
bool HSharpVE::VirtualEnvironment::is_variable(char* name) {
    return variables.exists(name);
}
void HSharpVE::VirtualEnvironment::dispose_value(ValueInfo& data) {
    if (!data.dealloc_required) return;
    switch (data.type) {
        case VariableType::INT: integers_pool.free(static_cast<int64_t*>(data.value)); break;
        case VariableType::STRING: strings_pool.free(static_cast<std::string*>(data.value)); break;
        default: std::terminate();
    }
}
void HSharpVE::VirtualEnvironment::run() {
    variables = {};
    for (const HSharpParser::NodeStmt* stmt : root.statements)
        exec_statement(stmt);
}

bool HSharpVE::VirtualEnvironment::is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}