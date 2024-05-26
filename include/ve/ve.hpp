#pragma once

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>

#include <parser/parser.hpp>
#include <ve/exceptions.hpp>

using HSharpParser::NodeStmtExit;
using HSharpParser::NodeStmtVar;
using HSharpParser::NodeExpressionStrLit;
using HSharpParser::NodeExpressionStrLit;

namespace HSharpVE {
    enum class VariableType {
        INT,
        STRING
    };
    struct Variable {
        VariableType vtype;
        void* value;
    };
    struct Scope {
        std::unordered_map<std::string, Variable> variables;
    };
    struct ExpressionVisitorRetPair {
        VariableType type;
        void* value;
    };

    class VirtualEnvironment{
    private:
        struct StatementVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit StatementVisitor(VirtualEnvironment* parent) : parent(parent) {}
            void operator()(HSharpParser::NodeStmtInput* stmt) const {
                throwFatalVirtualEnvException("Not implemented: input()");
            }
            void operator()(HSharpParser::NodeStmtPrint* stmt) const {
                parent->StatementVisitor_StatementPrint(stmt);
            }
            void operator()(NodeStmtExit* stmt) const {
                parent->StatementVisitor_StatementExit(stmt);
            }
            void operator()(NodeStmtVar* stmt) const {
                parent->StatementVisitor_StatementVar(stmt);
            }
            void operator()(HSharpParser::NodeStmtVarAssign* stmt) const {
                parent->StatementVisitor_StatementVarAssign(stmt);
            }
        };
        struct ExpressionVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit ExpressionVisitor(VirtualEnvironment* parent) : parent(parent) {}
            ExpressionVisitorRetPair operator()(HSharpParser::NodeTerm* term) const {
                return std::visit(parent->termvisitor, term->term);
            }
            ExpressionVisitorRetPair operator()(const HSharpParser::NodeExpressionStrLit* expr) const {
                auto str = new std::string;
                *str = {expr->str_lit.value.value()};
                return ExpressionVisitorRetPair{
                    .type = VariableType::STRING,
                    .value = str
                };
            }
            ExpressionVisitorRetPair operator()(HSharpParser::NodeBinExpr* expr) const {
                return std::visit(parent->binexprvisitor, expr->var);
            }
        };
        struct TermVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit TermVisitor(VirtualEnvironment* parent) : parent(parent) {}
            ExpressionVisitorRetPair operator()(const HSharpParser::NodeTermIntLit* term) const {
                if (!is_number(term->int_lit.value.value())) {
                    std::cerr << "Expression is not valid integer!" << std::endl;
                    exit(1);
                } else {
                    auto num = new int64_t;
                    *num = std::stol(term->int_lit.value.value());
                    return {.type = VariableType::INT, .value = num};
                }
            }
            ExpressionVisitorRetPair operator()(const HSharpParser::NodeTermIdent* term) const {
                if (!parent->is_variable(const_cast<char*>(term->ident.value.value().c_str()))){
                    std::cerr << "Invalid identifier" << std::endl;
                    exit(1);
                }
                return {
                    parent->global_scope.variables.at(term->ident.value.value()).vtype,
                    parent->global_scope.variables.at(term->ident.value.value()).value
                };
            }
        };
        struct BinExprVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit BinExprVisitor(VirtualEnvironment* parent) : parent(parent){}
            ExpressionVisitorRetPair operator()(const HSharpParser::NodeBinExprAdd* expr) const {
                auto value = parent->integers_pool.malloc();
                *value = *static_cast<int64_t*>(std::visit(parent->exprvisitor, expr->lhs->expr).value);
                *value += *static_cast<int64_t*>(std::visit(parent->exprvisitor, expr->rhs->expr).value);
                return ExpressionVisitorRetPair{.type = VariableType::INT, .value = value};
            }
            ExpressionVisitorRetPair operator()(const HSharpParser::NodeBinExprSub* expr) const {
                auto value = parent->integers_pool.malloc();
                *value = *static_cast<int64_t*>(std::visit(parent->exprvisitor, expr->lhs->expr).value);
                *value -= *static_cast<int64_t*>(std::visit(parent->exprvisitor, expr->rhs->expr).value);
                return ExpressionVisitorRetPair{.type = VariableType::INT, .value = value};
            }
            ExpressionVisitorRetPair operator()(const HSharpParser::NodeBinExprMul* expr) const {
                auto value = parent->integers_pool.malloc();
                *value = *static_cast<int64_t*>(std::visit(parent->exprvisitor, expr->lhs->expr).value);
                *value *= *static_cast<int64_t*>(std::visit(parent->exprvisitor, expr->rhs->expr).value);
                return ExpressionVisitorRetPair{.type = VariableType::INT, .value = value};
            }
            ExpressionVisitorRetPair operator()(const HSharpParser::NodeBinExprDiv* expr) const {
                auto value = parent->integers_pool.malloc();
                *value = *static_cast<int64_t*>(std::visit(parent->exprvisitor, expr->lhs->expr).value);
                *value /= *static_cast<int64_t*>(std::visit(parent->exprvisitor, expr->rhs->expr).value);
                return ExpressionVisitorRetPair{.type = VariableType::INT, .value = value};
            }
        };
        HSharpParser::NodeProgram root;
        Scope global_scope;
        boost::object_pool<std::int64_t> integers_pool;
        boost::pool<> strings_pool;
        ExpressionVisitor exprvisitor{this};
        StatementVisitor stmtvisitor{this};
        TermVisitor termvisitor{this};
        BinExprVisitor binexprvisitor{this};
        bool verbose;

        void StatementVisitor_StatementPrint(HSharpParser::NodeStmtPrint* stmt);
        void StatementVisitor_StatementExit(HSharpParser::NodeStmtExit* stmt);
        void StatementVisitor_StatementVar(HSharpParser::NodeStmtVar* stmt);
        void StatementVisitor_StatementVarAssign(HSharpParser::NodeStmtVarAssign* stmt);
        ExpressionVisitorRetPair ExpressionVisitor_ExprStrLit(NodeExpressionStrLit* expr);
        ExpressionVisitorRetPair ExpressionVisitor_ExprIntLit(HSharpParser::NodeTermIntLit* expr);
        ExpressionVisitorRetPair ExpressionVisitor_ExprIdent(HSharpParser::NodeTermIdent* expr) const;
        ExpressionVisitorRetPair ExpressionVisitor_BinExpr(HSharpParser::NodeBinExpr* expr) const;

        void exec_expression(const HSharpParser::NodeExpression* expr);
        void exec_statement(const HSharpParser::NodeStmt* stmt);

        void delete_variables();
        bool is_variable_value(void* value);
        bool is_variable(char* name);
        void dispose_value(void* value, VariableType valuetype);

        static bool is_number(const std::string& s);
    public:
        explicit VirtualEnvironment(HSharpParser::NodeProgram root, const bool verbose)
            : root(std::move(root)),
            verbose(verbose),
            strings_pool(sizeof(std::string)){}
        ~VirtualEnvironment() {
            delete_variables();
        }
        void run();
    };
}