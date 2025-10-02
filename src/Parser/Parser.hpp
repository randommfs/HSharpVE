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

  struct NodeTermIdent {
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
    std::variant<NodeTermIntLit*, NodeTermIdent*, NodeTermParen*> term;
  };

  struct NodeExpr {
    std::variant<NodeTerm*, NodeBinExpr*> var;
  };

  struct NodeVarAssign {
    Token name;
    NodeExpr* expr;
  };

  struct NodeStmt {
    std::variant<NodeVarAssign*> stmt;
  };

  struct NodeProgram {
    std::vector<NodeStmt*> stmts;
  };
  
  class Parser {
  public:
    Parser(std::vector<Token>&& tokens)
      : m_tokens(tokens)
      , m_alloc(1024 * 1024 * 16) {}

    TranslationUnit Parse();
  private:
    std::optional<NodeTerm*> ParseTerm();
    std::optional<NodeExpr*> ParseExpr(int min_prec = 0);

    std::optional<Token> TryConsume(TokenType);
    Token Peek(std::uint8_t offset = 0);
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
