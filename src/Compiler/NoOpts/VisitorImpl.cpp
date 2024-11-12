#include "Compiler/AST/Nodes.hpp"
#include "Compiler/Opcodes.hpp"
#include <Compiler/Compiler_NoOptimizations.hpp>
#include <stdexcept>

using namespace HSharpParser;

void HSharpCompiler::Compiler_NoOpt::TermVisitor::operator()(NodeTermFloat* term) {

}

void HSharpCompiler::Compiler_NoOpt::TermVisitor::operator()(NodeTermIdent* term) {

}

void HSharpCompiler::Compiler_NoOpt::TermVisitor::operator()(NodeTermString* term) {

}

void HSharpCompiler::Compiler_NoOpt::TermVisitor::operator()(NodeTermBoolLit* term) {

}

void HSharpCompiler::Compiler_NoOpt::TermVisitor::operator()(NodeExpr* term) {

}

void HSharpCompiler::Compiler_NoOpt::TermVisitor::operator()(NodeTermIntLit* term) {
  std::uint64_t lit = std::stoull(term->value.str.begin());
  std::uint32_t id = compiler.state->push_const(lit);
  compiler.emit_opcode(Opcode::PUSH_CONST, id & 0xFF);
  if (id > 0xFF)
    compiler.emit_opcode(Opcode::EXT_OPCODE, (id >> 8) & 0xFF);
  else return;
  if (id > 0xFFFF)
    compiler.emit_opcode(Opcode::EXT_OPCODE, (id >> 16) & 0xFF);
  else return;
  if (id > 0xFFFFFF)
    compiler.emit_opcode(Opcode::EXT_OPCODE, (id >> 24) & 0xFF);
}

void HSharpCompiler::Compiler_NoOpt::ExprVisitor::operator()(NodeTerm* term) {
  std::visit(compiler.termvisitor, term->term);
}

void HSharpCompiler::Compiler_NoOpt::ExprVisitor::operator()(HSharpCompiler::NodeBinExpr* binexpr) {
  std::visit(compiler.binexprvisitor, binexpr->expr);
}

void HSharpCompiler::Compiler_NoOpt::BinExprVisitor::operator()(HSharpCompiler::NodeBinExprAdd* binexpr) {
  std::visit(compiler.exprvisitor, binexpr->lhs->expr);
  std::visit(compiler.exprvisitor, binexpr->rhs->expr);
  compiler.emit_opcode(Opcode::ADD_BINARY);
}

void HSharpCompiler::Compiler_NoOpt::BinExprVisitor::operator()(HSharpCompiler::NodeBinExprSub* binexpr) {
  std::visit(compiler.exprvisitor, binexpr->lhs->expr);
  std::visit(compiler.exprvisitor, binexpr->rhs->expr);
  compiler.emit_opcode(Opcode::SUB_BINARY);
}

void HSharpCompiler::Compiler_NoOpt::BinExprVisitor::operator()(HSharpCompiler::NodeBinExprMul* binexpr) {
  std::visit(compiler.exprvisitor, binexpr->lhs->expr);
  std::visit(compiler.exprvisitor, binexpr->rhs->expr);
  compiler.emit_opcode(Opcode::MUL_BINARY);
}

void HSharpCompiler::Compiler_NoOpt::BinExprVisitor::operator()(HSharpCompiler::NodeBinExprDiv* binexpr) {
  std::visit(compiler.exprvisitor, binexpr->lhs->expr);
  std::visit(compiler.exprvisitor, binexpr->rhs->expr);
  compiler.emit_opcode(Opcode::DIV_BINARY);
}