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
    std::variant<NodeTermIntLit*, NodeTermFloatLit*, NodeTermIdent*, NodeTermParen*, NodeTermReflectIdent*> term;
  };

  struct NodeExpr {
    std::variant<NodeTerm*, NodeBinExpr*> var;
  };

  struct NodeVarAssign {
    Token name;
    std::string type;
    NodeExpr* expr;
  };

  struct NodeStmt {
    std::variant<NodeVarAssign*> stmt;
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
    std::optional<NodeStmt*> ParseStatement();
    std::optional<NodeTerm*> ParseTerm();
    std::optional<NodeExpr*> ParseExpr(int min_prec = 0);
    std::optional<NodeScope*> ParseScope();

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
