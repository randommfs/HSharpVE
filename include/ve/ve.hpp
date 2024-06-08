#pragma once

#include <algorithm>
#include <cassert>
#include <unordered_map>

#include <visitors.hpp>
#include <parser/parser.hpp>
#include <ve/exceptions.hpp>
#include <hpool/cpp/hpool.hpp>

/* Nodes */
using HSharpParser::NodeStmtInput;
using HSharpParser::NodeStmtExit;
using HSharpParser::NodeStmtPrint;
using HSharpParser::NodeStmtVar;
using HSharpParser::NodeStmtVarAssign;
using HSharpParser::NodeExpressionStrLit;
using HSharp::VariableType;
using HSharp::ValueInfo;

namespace HSharpVE {
    struct Variable {
        VariableType vtype;
        void* value;
    };
    struct Scope {
        std::unordered_map<std::string, Variable> variables;
    };


    class VirtualEnvironment{
    private:
        struct StatementVisitor : public HSharp::IStatementVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit StatementVisitor(VirtualEnvironment* parent) : parent(parent) {}
            void operator()(NodeStmtInput* stmt) const override;
            void operator()(NodeStmtPrint* stmt) const override;
            void operator()(NodeStmtExit* stmt) const override;
            void operator()(NodeStmtVar* stmt) const override;
            void operator()(NodeStmtVarAssign* stmt) const override;
        };

        struct ExpressionVisitor : HSharp::IExpressionVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit ExpressionVisitor(VirtualEnvironment* parent) : parent(parent) {}
            ValueInfo operator()(HSharpParser::NodeTerm* term) const override;
            ValueInfo operator()(const HSharpParser::NodeExpressionStrLit* expr) const override;
            ValueInfo operator()(HSharpParser::NodeBinExpr* expr) const override;
        };
        struct TermVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit TermVisitor(VirtualEnvironment* parent) : parent(parent) {}
            ValueInfo operator()(const HSharpParser::NodeTermIntLit* term) const;
            ValueInfo operator()(const HSharpParser::NodeTermIdent* term) const;
            ValueInfo operator()(const HSharpParser::NodeTermParen* term) const;
        };
        struct BinExprVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit BinExprVisitor(VirtualEnvironment* parent) : parent(parent){}
            ValueInfo operator()(const HSharpParser::NodeBinExprAdd* expr) const;
            ValueInfo operator()(const HSharpParser::NodeBinExprSub* expr) const;
            ValueInfo operator()(const HSharpParser::NodeBinExprMul* expr) const;
            ValueInfo operator()(const HSharpParser::NodeBinExprDiv* expr) const;
        };
        HSharpParser::NodeProgram root;
        std::vector<std::string>& lines;
        Scope global_scope;
        hpool::HPool<std::int64_t> integers_pool;
        hpool::HPool<std::string> strings_pool;
        ExpressionVisitor exprvisitor{this};
        StatementVisitor stmtvisitor{this};
        TermVisitor termvisitor{this};
        BinExprVisitor binexprvisitor{this};
        bool verbose;

        void StatementVisitor_StatementPrint(HSharpParser::NodeStmtPrint* stmt);
        void StatementVisitor_StatementExit(HSharpParser::NodeStmtExit* stmt);
        void StatementVisitor_StatementVar(HSharpParser::NodeStmtVar* stmt);
        void StatementVisitor_StatementVarAssign(HSharpParser::NodeStmtVarAssign* stmt);
        ValueInfo ExpressionVisitor_ExprStrLit(NodeExpressionStrLit* expr);
        ValueInfo ExpressionVisitor_ExprIntLit(HSharpParser::NodeTermIntLit* expr);
        ValueInfo ExpressionVisitor_ExprIdent(HSharpParser::NodeTermIdent* expr) const;
        ValueInfo ExpressionVisitor_BinExpr(HSharpParser::NodeBinExpr* expr) const;

        void exec_statement(const HSharpParser::NodeStmt* stmt);

        void delete_variables();
        bool is_variable(char* name);
        void dispose_value(ValueInfo& data);
        void delete_var_value(Variable& variable);
        void* allocate(VariableType vtype);

        template <typename... Args>
        std::string vformat_wrapper(const char* format, Args&&... args);
        static bool is_number(const std::string& s);
    public:
        explicit VirtualEnvironment(HSharpParser::NodeProgram root, std::vector<std::string>& lines, const bool verbose)
            : root(std::move(root)),
              lines(lines),
              integers_pool(16),
              strings_pool(16),
              verbose(verbose){}

        ~VirtualEnvironment() {
            delete_variables();
        }
        void run();
    };
}