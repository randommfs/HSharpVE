#pragma once

#include <vector>

#include <cstdint>

#include <Compiler/ICompiler.hpp>
#include <Allocator/HPoolWrapper.hpp>
#include <Compiler/AST/Nodes.hpp>

namespace HSharpCompiler {
    using NodeAllocator = HSharpAllocator::Allocator<
        NodeExpr,
        NodeBinExpr,
        NodeBinExprAdd,
        NodeBinExprSub,
        NodeBinExprMul,
        NodeBinExprDiv,
        NodeTerm,
        NodeTermFloat,
        NodeTermIdent,
        NodeTermIntLit,
        NodeTermString>;

    class Compiler_NoOpt : public ICompiler {
    private:
        struct TermVisitor {
            void operator()(NodeTermFloat*);
            void operator()(NodeTermIntLit*);
            void operator()(NodeTermIdent*);
            void operator()(NodeTermString*);
            void operator()(NodeTermBoolLit*);
            void operator()(NodeExpr*);
        };

        std::unique_ptr<CompilerState> state;
        NodeAllocator allocator;

        HSharpParser::Value _parse_operator(std::vector<HSharpParser::Value>&&) noexcept;
        HSharpParser::Value _parse_literal(std::vector<HSharpParser::Value>&&) noexcept;
        HSharpParser::Value _parse_ident(std::vector<HSharpParser::Value>&&) noexcept;

        void _compile_expression(HSharpParser::Value&) noexcept;
        HSharpParser::Value _transform_expression(std::vector<HSharpParser::Value>&&) noexcept;
        HSharpParser::Value _compile_var_creation(std::vector<HSharpParser::Value>&&) noexcept;

        NodeTerm* _get_term(HSharpParser::Value) noexcept;
    public:
        Compiler_NoOpt() : state(std::make_unique<CompilerState>()) { }

        void emit_opcode(Opcode op, std::uint8_t arg = 0) override;

        /* Handlers for basic expression parts - operators and tokens*/
        ParserCallbackType get__parse_operator() noexcept override;
        ParserCallbackType get__parse_literal() noexcept override;
        ParserCallbackType get__parse_ident() noexcept override;

        ParserCallbackType get__compile_expression() noexcept override;
        ParserCallbackType get__compile_var_creation() noexcept override;
    };
}