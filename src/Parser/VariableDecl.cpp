#include "Parser.hpp"

std::optional<HVE::Parser::NodeVarDeclaration*> HVE::Parser::Parser::ParseVarDeclaration() {
  if (!(TryPeek(TokenType::IDENTIFIER) || TryPeek(TokenType::COLON, 1) || TryPeek(TokenType::IDENTIFIER, 2))) {
    return std::nullopt;
  }

  auto name = Consume();
  Consume();
  auto type = Consume();
  auto var = m_alloc.Emplace<NodeVarDeclaration>(name, type, nullptr);

  switch (Peek().type) {
  case TokenType::EQUAL: {
    Consume();
    auto expr = ParseExpr();
    if (!expr) {
      throw std::runtime_error("Failed to parse expression");
    }
    var->expr = expr.value();
    if (!TryConsume(TokenType::SEMICOLON)) {
      throw std::runtime_error("Expected semicolon at the end of a statement");
    }
    break;
  }
  case TokenType::OPEN_BRACE: {
    Consume();
    auto expr = ParseExpr();
    if (!expr) {
      throw std::runtime_error("Failed to parse expression");
    }
    var->expr = expr.value();
    if (!TryConsume(TokenType::CLOSE_BRACE)) {
      throw std::runtime_error("Expected closing brace at the end of a statement");
    }
    if (!TryConsume(TokenType::SEMICOLON)) {
      throw std::runtime_error("Expected semicolon at the end of a statement");
    }
    break;
  }
  case TokenType::OPEN_PARENTHESIS: {
    Consume();
    auto expr = ParseExpr();
    if (!expr) {
      throw std::runtime_error("Failed to parse expression");
    }
    var->expr = expr.value();
    if (!TryConsume(TokenType::CLOSE_PARENTHESIS)) {
      throw std::runtime_error("Expected closing parenthesis at the end of a statement");
    }
    if (!TryConsume(TokenType::SEMICOLON)) {
      throw std::runtime_error("Expected semicolon at the end of a statement");
    }
    break;
  }
  case TokenType::SEMICOLON: {
    break;
  }
  default:
    std::abort();
  }
  return var;
}
