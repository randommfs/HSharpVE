#include "Parser.hpp"
#include "Tokenizer.hpp"

#include <optional>
#include <stdexcept>

HVE::Parser::TranslationUnit HVE::Parser::Parser::Parse() {
  
}

std::optional<HVE::Parser::NodeTerm*> HVE::Parser::Parser::ParseTerm() {
  switch (Peek().type) {
  case TokenType::INT_LITERAL: {
    auto int_tok = Consume();
    auto* lit = m_alloc.Emplace<NodeTermIntLit>(std::move(int_tok));
    return m_alloc.Emplace<NodeTerm>(lit);

  }
  case TokenType::FLOAT_LITERAL: {
    auto float_tok = Consume();
    auto* lit = m_alloc.Emplace<NodeTermFloatLit>(std::move(float_tok));
    return m_alloc.Emplace<NodeTerm>(lit);
  }
  case TokenType::IDENTIFIER: {
    auto int_tok = Consume();
    auto* lit = m_alloc.Emplace<NodeTermIdent>(std::move(int_tok));
    return m_alloc.Emplace<NodeTerm>(lit);
  }
  case TokenType::OPEN_PARENTHESIS: {
    Consume();
    auto expr = ParseExpr();
    if (!expr.value()) {
      throw std::runtime_error("Expected expression");
    }
    if (!TryConsume(TokenType::CLOSE_PARENTHESIS)) {
      throw std::runtime_error("No closing parenthesis for expression");
    }
    auto pexpr = m_alloc.Emplace<NodeTermParen>(expr.value());
    return m_alloc.Emplace<NodeTerm>(pexpr);
    break;
  }
  default:
    std::abort();
  }
}

std::optional<HVE::Parser::NodeExpr*> HVE::Parser::Parser::ParseExpr(int min_prec) {
  std::optional<NodeTerm*> term_lhs = ParseTerm();
  if (!term_lhs.has_value()) {
    return {};
  }
  auto expr_lhs = m_alloc.Emplace<NodeExpr>(std::move(term_lhs.value()));
  while (true) {
    std::optional<Token> tok = Peek();
    std::optional<int> prec;
    if (tok.has_value()) {
      prec = GetPrecedence(tok->type);
      if (!prec.has_value() || prec < min_prec) {
        break;
      }
    } else {
      break;
    }
    auto op = Consume();
    auto expr_rhs = ParseExpr(prec.value() + 1);
    if (!expr_rhs.has_value()) {
      throw std::runtime_error("Failed to parse expression");
    }
    auto expr = m_alloc.Emplace<NodeBinExpr>();
    auto expr_lhs2 = m_alloc.Emplace<NodeExpr>();
    switch (op.type) {
    case TokenType::PLUS:
      expr_lhs2->var = expr_lhs->var;
      expr->var = m_alloc.Emplace<NodeBinExprAdd>(expr_lhs2, expr_rhs.value());
      break;
    case TokenType::MINUS:
      expr_lhs2->var = expr_lhs->var;
      expr->var = m_alloc.Emplace<NodeBinExprSub>(expr_lhs2, expr_rhs.value());
      break;
    case TokenType::STAR:
      expr_lhs2->var = expr_lhs->var;
      expr->var = m_alloc.Emplace<NodeBinExprMulti>(expr_lhs2, expr_rhs.value());
      break;
    case TokenType::SLASH:
      expr_lhs2->var = expr_lhs->var;
      expr->var = m_alloc.Emplace<NodeBinExprDiv>(expr_lhs2, expr_rhs.value());
      break;
    default:
      std::abort();
    }
    expr_lhs->var = expr;
  }
  return expr_lhs;
}

std::optional<HVE::Token> HVE::Parser::Parser::TryConsume(TokenType type) {
  if (Peek().type == type) {
    return Consume();
  }
  return std::nullopt;
}

HVE::Token HVE::Parser::Parser::Peek(std::uint8_t offset) {
  return m_tokens[m_current_token + offset];
}

HVE::Token HVE::Parser::Parser::Consume() {
  return m_tokens[m_current_token++];
}
