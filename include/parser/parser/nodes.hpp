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
            std::unique_ptr<Expression> lhs;
            std::unique_ptr<Expression> rhs;
        };

        /* Binary expressions */
        struct BinaryAddition       : public BinaryExpressionMixin { };
        struct BinarySubstraction   : public BinaryExpressionMixin { };
        struct BinaryMultiplication : public BinaryExpressionMixin { };
        struct BinaryDivision       : public BinaryExpressionMixin { };

        struct BinaryExpression : Node {
            std::variant<
                std::unique_ptr<BinaryAddition>, 
                std::unique_ptr<BinarySubstraction>, 
                std::unique_ptr<BinaryMultiplication>, 
                std::unique_ptr<BinaryDivision>
            > expression;
        };

        struct Term : Node {
            std::variant<
                std::unique_ptr<Literal>, 
                std::unique_ptr<Identificator>
            > term;
        };

        /* Basic expression node, includes all possible expressions */
        struct Expression 
            : public Node {
            std::variant<
                std::unique_ptr<Term>, 
                std::unique_ptr<BinaryExpression>
            > expression;
        };

        struct Parent {
            std::unique_ptr<Expression> expression;
        };


        namespace builtin {

            /* Statement nodes */
            struct Exit {
                std::unique_ptr<Expression> expression;
            };

            struct Print {
                std::unique_ptr<Expression> expression;
            };

            struct Input {
                std::unique_ptr<Expression> expression;
            };

            struct Var {
                std::unique_ptr<Expression> expression;
            };

            struct Statement 
                : public Node {
                std::variant<
                    std::unique_ptr<Exit>,
                    std::unique_ptr<Print>,
                    std::unique_ptr<Input>,
                    std::unique_ptr<Var>
                > statement;
            };

        }

        /* Start of AST */
        struct Program {
            std::vector<std::unique_ptr<builtin::Statement>> statements;
        };

    }
}