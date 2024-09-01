#pragma once

// STD
#include <vector>
#include <variant>
#include <cstdint>

// Local
#include <common/arena.hpp>
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
<<<<<<< Updated upstream
            Expression* lhs;
            Expression* rhs;
=======
            hsharp::arena_ptr<Expression> lhs;
            hsharp::arena_ptr<Expression> rhs;
>>>>>>> Stashed changes
        };

        /* Binary expressions */
        struct BinaryAddition       : public BinaryExpressionMixin { };
        struct BinarySubtraction    : public BinaryExpressionMixin { };
        struct BinaryMultiplication : public BinaryExpressionMixin { };
        struct BinaryDivision       : public BinaryExpressionMixin { };

        struct BinaryExpression : Node {
            std::variant<
<<<<<<< Updated upstream
                BinaryAddition*, 
                BinarySubstraction*, 
                BinaryMultiplication*, 
                BinaryDivision*
=======
                hsharp::arena_ptr<BinaryAddition>, 
                hsharp::arena_ptr<BinarySubtraction>, 
                hsharp::arena_ptr<BinaryMultiplication>, 
                hsharp::arena_ptr<BinaryDivision>
>>>>>>> Stashed changes
            > expression;
        };

        struct Term : Node {
            std::variant<
<<<<<<< Updated upstream
                Literal*, 
                Identificator*
=======
                hsharp::arena_ptr<Literal>, 
                hsharp::arena_ptr<Identificator>
>>>>>>> Stashed changes
            > term;
        };

        /* Basic expression node, includes all possible expressions */
        struct Expression 
            : public Node {
            std::variant<
<<<<<<< Updated upstream
                Term*, 
                BinaryExpression*
=======
                hsharp::arena_ptr<Term>, 
                hsharp::arena_ptr<BinaryExpression>
>>>>>>> Stashed changes
            > expression;
        };

        struct Parent {
<<<<<<< Updated upstream
            Expression* expression;
=======
            hsharp::arena_ptr<Expression> expression;
>>>>>>> Stashed changes
        };


        namespace builtin {

            /* Statement nodes */
            struct Exit {
<<<<<<< Updated upstream
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
=======
                hsharp::arena_ptr<Expression> expression;
            };

            struct Print {
                hsharp::arena_ptr<Expression> expression;
            };

            struct Input {
                hsharp::arena_ptr<Expression> expression;
            };

            struct Var {
                hsharp::arena_ptr<Expression> expression;
>>>>>>> Stashed changes
            };

            struct Statement 
                : public Node {
                std::variant<
<<<<<<< Updated upstream
                    Exit*,
                    Print*,
                    Input*,
                    Var*
=======
                    hsharp::arena_ptr<Exit>,
                    hsharp::arena_ptr<Print>,
                    hsharp::arena_ptr<Input>,
                    hsharp::arena_ptr<Var>
>>>>>>> Stashed changes
                > statement;
            };

        }

        /* Start of AST */
        struct Program {
<<<<<<< Updated upstream
            std::vector<builtin::Statement> statements;
=======
            std::vector<hsharp::arena_ptr<builtin::Statement>> statements;
>>>>>>> Stashed changes
        };

    }
}