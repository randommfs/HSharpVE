#pragma once

// STD
#include <vector>
#include <variant>
#include <cstdint>

// Local
#include <parser/tokenizer/tokens.hpp>


namespace hsharp {

    namespace nodes {

        struct Node {
            std::uint32_t line;
        };

        /* Base nodes declarations */
        struct TerminatingIntegerLiteral : Node {
            Token literal;
        };

        struct TerminatingIdentification : Node {
            Token ident;
        };

        struct TerminatingStringLiteral : Node {
            Token literal;
        };

        /* Basic expression node, includes all possible expressions */
        struct Expression {
            std::variant<NodeTerm*, NodeExpressionStrLit*, NodeBinExpr*> expr;
            uint32_t line;
        };

        /* Binary expressions */
        struct BinaryAddition {
            NodeExpression *lhs{}, *rhs{};
        };

        struct BinarySubstraction {
            NodeExpression *lhs{}, *rhs{};
        };

        struct BinaryMultiplication {
            NodeExpression *lhs{}, *rhs{};
        };

        struct BinaryDivision {
            NodeExpression *lhs{}, *rhs{};
        };

        struct BinaryExpression : Node {
            std::variant<
                BinaryAddition, 
                BinarySubstraction, 
                BinaryMultiplication, 
                BinaryDivision
            > underlying;
        };

        struct Terminator : Node {
            std::variant<
                TerminatingIntegerLiteral, 
                TerminatingStringLiteral, 
                TerminatingIdentification
            > terminator;
        };


        struct NodeTermParen{
            NodeExpression* expr;
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
                    NodeStmtVarAssign*> statement;
            uint32_t line;
        };

        /* Start of AST */
        struct Program {
            std::vector<NodeStmt*> statements;
        };

    }
}