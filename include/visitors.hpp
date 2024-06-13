#pragma once

#include <parser/nodes.hpp>

namespace HSharp{
    enum class VariableType {
        INT,
        STRING
    };
    struct ValueInfo {
        VariableType type;
        void* value;
        uint32_t line;
        bool dealloc_required;
    };

    class IStatementVisitor{
    public:
        virtual void operator()(HSharpParser::NodeStmtInput* stmt) const = 0;
        virtual void operator()(HSharpParser::NodeStmtPrint* stmt) const = 0;
        virtual void operator()(HSharpParser::NodeStmtExit* stmt) const = 0;
        virtual void operator()(HSharpParser::NodeStmtVar* stmt) const = 0;
        virtual void operator()(HSharpParser::NodeStmtVarAssign* stmt) const = 0;
        virtual void operator()(HSharpParser::NodeScope* stmt) const = 0;
        virtual void operator()(HSharpParser::NodeStmtIf* stmt) const = 0;
        virtual ~IStatementVisitor() = default;
    };

    class IExpressionVisitor {
    public:
        virtual ValueInfo operator()(HSharpParser::NodeTerm* term) const = 0;
        virtual ValueInfo operator()(HSharpParser::NodeExpressionStrLit* expr) const = 0;
        virtual ValueInfo operator()(HSharpParser::NodeBinExpr* expr) const = 0;
        virtual ~IExpressionVisitor() = default;
    };

    class ITermVisitor {
    public:
        virtual ValueInfo operator()(HSharpParser::NodeTermIntLit* term) const = 0;
        virtual ValueInfo operator()(HSharpParser::NodeTermIdent* term) const = 0;
        virtual ~ITermVisitor() = default;
    };

    class IBinExprVisitor {
    public:
        virtual ValueInfo operator()(HSharpParser::NodeBinExprAdd* expr) const = 0;
        virtual ValueInfo operator()(HSharpParser::NodeBinExprSub* expr) const = 0;
        virtual ValueInfo operator()(HSharpParser::NodeBinExprMul* expr) const = 0;
        virtual ValueInfo operator()(HSharpParser::NodeBinExprDiv* expr) const = 0;
        virtual ~IBinExprVisitor() = default;
    };
}