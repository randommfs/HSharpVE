#include <variant>

#include <Compiler/ICompiler.hpp>

namespace HSharpCompiler {
    using HSharpParser::Token;

    struct NodeTermIntLit {
        Token value;
    };

    struct NodeTermString {
        Token value;
    };

    struct NodeTermFloat {
        Token value;
    };

    struct NodeTermIdent {
        Token value;
    };

    struct NodeExpr;

    struct NodeBinExprAdd {
        NodeExpr *lhs, *rhs;
    };

    struct NodeBinExprSub {
        NodeExpr *lhs, *rhs;
    };

    struct NodeBinExprMul {
        NodeExpr *lhs, *rhs;
    };

    struct NodeBinExprDiv {
        NodeExpr *lhs, *rhs;
    };

    struct NodeBinExpr {
        std::variant<
            NodeBinExprAdd*,
            NodeBinExprSub*,
            NodeBinExprMul*,
            NodeBinExprDiv*> expr;
    };

    struct NodeTerm {
        std::variant<
            NodeTermFloat*,
            NodeTermIntLit*,
            NodeTermIdent*,
            NodeTermString*> term;
    };

    struct NodeExpr {
        std::variant<
            NodeTerm*,
            NodeBinExpr*> expr;
    };
}