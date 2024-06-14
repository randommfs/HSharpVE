#pragma once

#include "parser/nodes.hpp"
#include <unordered_map>
#include <stack>

#include <cassert>


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
using HSharpParser::NodeScope;
using HSharpParser::NodeStmtIf;
using HSharpParser::NodeExpressionStrLit;
using HSharp::VariableType;
using HSharp::ValueInfo;

namespace HSharpVE {
    struct Variable {
        VariableType vtype;
        void* value;
    };

    typedef std::unordered_map<std::string, Variable> Scope;
    typedef std::vector<Scope> FunctionScope;

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
            void operator()(NodeScope* stmt) const override;
            void operator()(NodeStmtIf* stmt) const override;
        };

        struct ExpressionVisitor : HSharp::IExpressionVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit ExpressionVisitor(VirtualEnvironment* parent) : parent(parent) {}
            ValueInfo operator()(HSharpParser::NodeTerm* term) const override;
            ValueInfo operator()(HSharpParser::NodeExpressionStrLit* expr) const override;
            ValueInfo operator()(HSharpParser::NodeBinExpr* expr) const override;
        };
        struct TermVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit TermVisitor(VirtualEnvironment* parent) : parent(parent) {}
            ValueInfo operator()(HSharpParser::NodeTermIntLit* term) const;
            ValueInfo operator()(HSharpParser::NodeTermIdent* term) const;
            ValueInfo operator()(HSharpParser::NodeTermParen* term) const;
        };
        struct BinExprVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit BinExprVisitor(VirtualEnvironment* parent) : parent(parent){}
            ValueInfo operator()(HSharpParser::NodeBinExprAdd* expr) const;
            ValueInfo operator()(HSharpParser::NodeBinExprSub* expr) const;
            ValueInfo operator()(HSharpParser::NodeBinExprMul* expr) const;
            ValueInfo operator()(HSharpParser::NodeBinExprDiv* expr) const;
            ValueInfo operator()(HSharpParser::NodeBinExprEq* expr) const;
            ValueInfo operator()(HSharpParser::NodeBinExprLess* expr) const;
            ValueInfo operator()(HSharpParser::NodeBinExprBig* expr) const;
        };
        struct PredVisitor {
        private:
            VirtualEnvironment* parent;
        public:
            explicit PredVisitor(VirtualEnvironment* parent) : parent(parent){}
            void operator()(HSharpParser::NodeIfPredOr* or_);
            void operator()(HSharpParser::NodeIfPredElse* else_);
        };
        HSharpParser::NodeProgram root;
        std::vector<std::string>& lines;
        std::vector<Scope> global_scopes;
        std::stack<FunctionScope> function_scopes;
        FunctionScope* fscopes_top;
        hpool::HPool<std::int64_t> integers_pool;
        hpool::HPool<std::string> strings_pool;
        ExpressionVisitor exprvisitor{this};
        StatementVisitor stmtvisitor{this};
        TermVisitor termvisitor{this};
        BinExprVisitor binexprvisitor{this};
        bool verbose;
        bool is_current_scope_global;
        bool is_in_function;

        void exec_statement(const HSharpParser::NodeStmt* stmt);

        Variable& create_variable(std::string& name, VariableType vtype);
        bool variable_exists(std::string& name);
        void set_variable(std::string& name, void* value);
        Variable& get_variable(std::string& name);
        void create_scope();
        void destroy_scope();
        void delete_variables();
        bool is_variable(char* name);
        void dispose_value(ValueInfo& data);
        void delete_var_value(Variable& variable);
        void* allocate(VariableType vtype);

        static bool is_number(const std::string& s);
    public:
        explicit VirtualEnvironment(HSharpParser::NodeProgram root, std::vector<std::string>& lines, const bool verbose)
            : root(std::move(root)),
              lines(lines),
              global_scopes{},
              function_scopes{},
              integers_pool(16),
              strings_pool(16),
              verbose(verbose),
              is_current_scope_global(true),
              is_in_function(false){}

        ~VirtualEnvironment() {
            delete_variables();
        }
        void run();
    };
}