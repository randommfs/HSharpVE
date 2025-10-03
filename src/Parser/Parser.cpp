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
  case TokenType::UP_ARROW: {
    Consume();
    if (!TryConsume(TokenType::UP_ARROW)) {
      throw std::runtime_error("Invalid up arrow usage");
    }
    auto ident = TryConsume(TokenType::IDENTIFIER);
    auto* lit = m_alloc.Emplace<NodeTermReflectIdent>(std::move(ident.value()));
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

std::optional<HVE::Parser::NodeStmt*> HVE::Parser::Parser::ParseStatement() {
  if (TryPeek(TokenType::IDENTIFIER) && TryPeek(TokenType::COMMA, 1) && TryPeek(TokenType::IDENTIFIER, 2) && TryPeek(TokenType::EQUAL)) {
    auto ident = Consume();
    Consume();
    auto type = ParseType();
    if (type.empty()) {
      throw std::runtime_error("Failed to parse type");
    }
    Consume();
    auto expr = ParseExpr();
    if (!expr.has_value()) {
      throw std::runtime_error("Expected expression");
    }
    if (!TryConsume(TokenType::SEMICOLON)) {
      throw std::runtime_error("Expected ';' after expression");
    }
    auto var_assign = m_alloc.Emplace<NodeVarAssign>(ident, type, expr.value());
    return m_alloc.Emplace<NodeStmt>(var_assign);
  }
}

std::optional<HVE::Parser::NodeScope*> HVE::Parser::Parser::ParseScope() {
  if (!TryConsume(TokenType::OPEN_BRACE)) {
    throw std::runtime_error("Expected '{' before scope");
  }
  auto scope = m_alloc.Emplace<NodeScope>();
  while (true) {
    std::optional<NodeStmt*> stmt = ParseStatement();
    if (!stmt.has_value()) {
      break;
    }
    scope->stmts.emplace_back(stmt.value());
  }
  if (!TryConsume(TokenType::CLOSE_BRACE)) {
    throw std::runtime_error("Expected '}' after scope");
  }
  return scope;
}

std::string HVE::Parser::Parser::ParseType() {
  if (TryPeek(TokenType::OPEN_BRACKET) && TryPeek(TokenType::IDENTIFIER) && TryPeek(TokenType::CLOSE_BRACKET)) {
    Consume();
    auto ident = Consume();
    Consume();
    return ident.lexeme;
  } else if (TryPeek(TokenType::IDENTIFIER)) {
    return Consume().lexeme;
  }
  return {};
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

bool HVE::Parser::Parser::TryPeek(TokenType type, std::uint8_t offset) {
  return !(m_current_token + offset >= m_tokens.size() || m_tokens[m_current_token + offset].type != type);
}


HVE::Token HVE::Parser::Parser::Consume() {
  return m_tokens[m_current_token++];
}
