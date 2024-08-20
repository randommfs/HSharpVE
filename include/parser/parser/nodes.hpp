#pragma once

// STD
#include <memory>
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
        struct Literal 
            : public Node {
            Token literal;
        };

        struct Identificator 
            : public Node {
            Token identificator;
        };

        struct Expression;

        struct BinaryExpressionMixin {
            Expression* lhs;
            Expression* rhs;
        };

        /* Binary expressions */
        struct BinaryAddition       : public BinaryExpressionMixin { };
        struct BinarySubstraction   : public BinaryExpressionMixin { };
        struct BinaryMultiplication : public BinaryExpressionMixin { };
        struct BinaryDivision       : public BinaryExpressionMixin { };

        struct BinaryExpression : Node {
            std::variant<
                BinaryAddition*, 
                BinarySubstraction*, 
                BinaryMultiplication*, 
                BinaryDivision*
            > expression;
        };

        struct Term : Node {
            std::variant<
                Literal*, 
                Identificator*
            > term;
        };

        /* Basic expression node, includes all possible expressions */
        struct Expression 
            : public Node {
            std::variant<
                Term*, 
                BinaryExpression*
            > expression;
        };

        struct Parent {
            Expression* expression;
        };


        namespace builtin {

            /* Statement nodes */
            struct Exit {
                Expression* expression;
            };

            struct Print {
                Expression* expression;
            };

            struct Input {
                Expression* expression;
            };

            struct Var {
                Expression* expression;
            };

            struct Statement 
                : public Node {
                std::variant<
                    Exit*,
                    Print*,
                    Input*,
                    Var*
                > statement;
            };

        }

        /* Start of AST */
        struct Program {
            std::vector<builtin::Statement> statements;
        };

    }
}