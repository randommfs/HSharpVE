#pragma once

#include <vector>
#include <variant>
#include <optional>

#include <cstdint>

#include <parser/tokens.hpp>

namespace HSharpParser{
    /* Forward declarations to allow any order of regular declarations */
    struct NodeExpression;
    struct NodeExpressionIdent;
    struct NodeTermParen;
    struct NodeBinExpr;
    struct NodeBinExprAdd;
    struct NodeBinExprSub;
    struct NodeBinExprMul;
    struct NodeBinExprDiv;
    struct NodeBinExprEq;
    struct NodeBinExprLess;
    struct NodeBinExprBig;
    struct NodeStmt;
    struct NodeStmtExit;
    struct NodeStmtInput;
    struct NodeStmtPrint;
    struct NodeStmtVar;
    struct NodeScope;
    struct NodeStmtIf;
    struct NodeTerm;
    struct NodeTermIdent;
    struct NodeTermIntLit;
    struct NodeIfPred;
    struct NodeProgram;

    /* Base nodes declarations */
    struct NodeTermIntLit {
        Token int_lit;
        uint32_t line;
    };
    struct NodeTermIdent {
        Token ident;
        uint32_t line;
    };
    struct NodeExpressionStrLit {
        Token str_lit;
        uint32_t line;
    };

    struct NodeBinExpr {
        std::variant<NodeBinExprAdd*,
                NodeBinExprSub*,
                NodeBinExprMul*,
                NodeBinExprDiv*,
                NodeBinExprEq*,
                NodeBinExprLess*,
                NodeBinExprBig*> var;
        uint32_t line;
    };

    struct NodeTerm {
        std::variant<NodeTermIntLit*, NodeTermIdent*, NodeTermParen*> term;
        uint32_t line;
    };

    /* Basic expression node, includes all possible expressions */
    struct NodeExpression {
        std::variant<NodeTerm*, NodeExpressionStrLit*, NodeBinExpr*> expr;
        uint32_t line;
    };

    struct NodeTermParen{
        NodeExpression* expr;
    };

    /* Binary expressions */
    struct NodeBinExprAdd {
        NodeExpression *lhs{}, *rhs{};
    };
    struct NodeBinExprSub {
        NodeExpression *lhs{}, *rhs{};
    };
    struct NodeBinExprMul {
        NodeExpression *lhs{}, *rhs{};
    };
    struct NodeBinExprDiv {
        NodeExpression *lhs{}, *rhs{};
    };
    struct NodeBinExprEq {
        NodeExpression *lhs{}, *rhs{};
    };
    struct NodeBinExprLess {
        NodeExpression *lhs{}, *rhs{};
    };
    struct NodeBinExprBig {
        NodeExpression *lhs{}, *rhs{};
    };

    /* Statement nodes */
    struct NodeExit {
        NodeExpression* expr;
    };
    struct NodeStmtExit {
        NodeExpression* expr;
        uint32_t line;
    };
    struct NodeStmtPrint {
        NodeExpression* expr;
    };
    struct NodeStmtInput {
        NodeExpression* expr;
    };
    struct NodeStmtVar {
        Token ident{};
        NodeExpression* expr{};
    };
    struct NodeStmtVarAssign {
        Token ident{};
        NodeExpression* expr{};
    };
    struct NodeStmt {
        std::variant<NodeStmtExit*,
                NodeStmtPrint*,
                NodeStmtInput*,
                NodeStmtVar*,
                NodeStmtVarAssign*,
                NodeScope*,
                NodeStmtIf*> statement;
        uint32_t line;
    };

    struct NodeScope{
        std::vector<NodeStmt*> statements;
    };
    struct NodeIfPredOr{
        NodeExpression* expr;
        NodeScope* scope;
        std::optional<NodeIfPred*> pred;
    };
    struct NodeIfPredElse{
        NodeScope* scope;
    };
    struct NodeIfPred{
        std::variant<NodeIfPredOr*, NodeIfPredElse*> var;
    };
    struct NodeStmtIf{
        NodeExpression* expr;
        NodeScope* scope;
        std::optional<NodeIfPred*> pred;
    };

    /* Start of AST */
    struct NodeProgram {
        std::vector<NodeStmt*> statements;
    };
}