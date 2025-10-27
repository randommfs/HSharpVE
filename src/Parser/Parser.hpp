#pragma once

#include <optional>
#include <cstdint>
#include <variant>
#include <vector>

#include "Arena.hpp"
#include "Tokenizer.hpp"

namespace HVE::Parser {
  struct TranslationUnit {
    std::vector<std::string> referenced_tus;
  };

  enum class ValueType {
    LVALUE,
    RVALUE
  };

  struct NodeTermIntLit {
    Token int_lit;
  };

  struct NodeTermFloatLit {
    Token float_lit;
  };

  struct NodeTermIdent {
    Token ident;
  };

  struct NodeTermReflectIdent {
    Token ident;
  };

  struct NodeExpr;
  struct NodeFuncCall;
  struct NodeSubscriptOp;
  struct NodeComplexTerm;

  struct NodeTermParen {
    NodeExpr* expr;
  };

  struct NodeBinExprAdd {
    NodeExpr* lhs;
    NodeExpr* rhs;
  };

  struct NodeBinExprMulti {
    NodeExpr* lhs;
    NodeExpr* rhs;
  };

  struct NodeBinExprSub {
    NodeExpr* lhs;
    NodeExpr* rhs;
  };

  struct NodeBinExprDiv {
    NodeExpr* lhs;
    NodeExpr* rhs;
  };

  struct NodeBinExpr {
    std::variant<NodeBinExprAdd*, NodeBinExprMulti*, NodeBinExprSub*, NodeBinExprDiv*> var;
  };

  struct NodeTerm {
    std::variant<NodeTermIntLit*,
      NodeTermFloatLit*,
      NodeTermIdent*,
      NodeTermParen*,
      NodeTermReflectIdent*,
      NodeFuncCall*,
      NodeSubscriptOp*,
      NodeComplexTerm*> term;
  };

  struct NodeExpr {
    std::variant<NodeTerm*, NodeBinExpr*> var;
  };

  struct NodeAssignment {
    NodeExpr* lhs;
    NodeExpr* rhs;
    Token op;
  };

  struct NodeVarDeclaration {
    Token ident;
    Token type;
    NodeExpr* expr;
  };

  struct NodeStmt {
    std::variant<NodeAssignment*, NodeVarDeclaration*, NodeExpr*> stmt;
  };

  struct NodeScope {
    std::vector<NodeStmt*> stmts;
  };

  struct FuncArg {
    Token name;
    Token type;
  };

  struct NodeFuncDef {
    Token access;
    Token name;
    Token ret_type;
    std::vector<FuncArg> args;
    NodeScope stmts;
  };

  struct NodeFuncCall {
    Token name;
    std::vector<NodeExpr*> args;
    std::vector<NodeExpr*> template_args;
  };

  struct NodeSubscriptOp {
    Token ident;
    std::vector<NodeExpr*> subscript_args;
  };

  struct NodeComplexTerm {
    std::vector<NodeTerm*> terms;
  };

  struct NodeProgram {
    std::vector<std::variant<NodeFuncDef*>> stmts;
  };
  
  class Parser {
  public:
    Parser(std::vector<Token>&& tokens)
      : m_tokens(tokens)
      , m_alloc(1024 * 1024 * 64) {}

    TranslationUnit Parse();
  private:
    std::optional<NodeExpr*> ParseLValue();
    std::optional<NodeExpr*> ParseRValue();
    std::optional<NodeStmt*> ParseStatement();
    std::optional<NodeTerm*> ParseTerm();
    std::optional<NodeComplexTerm*> ParseComplexTerm();
    std::optional<NodeExpr*> ParseExpr(int min_prec = 0);
    std::optional<NodeScope*> ParseScope();
    std::optional<NodeFuncCall*> ParseFuncCall();
    std::optional<NodeFuncDef*> ParseFuncDef();
    std::optional<NodeSubscriptOp*> ParseSubscriptOp();

    std::optional<NodeAssignment*> ParseAssignment(NodeExpr* lhs = nullptr);

    /* Variable init */
    std::optional<NodeVarDeclaration*> ParseVarDeclaration();

    std::string ParseType();

    std::optional<Token> TryConsume(TokenType);
    Token Peek(std::uint8_t offset = 0);
    bool TryPeek(TokenType type, std::uint8_t offset = 0); // Is used only to verify if there is a specific token at specific offset
    Token Consume();

    inline std::optional<int> GetPrecedence(TokenType type) {
      switch (type) {
      case TokenType::MINUS:
      case TokenType::PLUS:
        return 0;
      case TokenType::SLASH:
      case TokenType::STAR:
        return 1;
      default:
        return {};
      }
    }


    std::vector<Token> m_tokens;
    std::vector<Token> m_tmp_buf;
    std::uint32_t m_current_token = 0;

    HVE::Alloc::ArenaAllocator m_alloc;
  };
};
