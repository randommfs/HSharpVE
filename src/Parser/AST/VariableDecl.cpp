#include "ASTBuilder.hpp"

std::optional<HVE::Parser::NodeVarDeclaration*> HVE::Parser::ASTBuilder::ParseVarDeclaration(bool with_access_modifier) {
  if (with_access_modifier) {
    if ((!TryPeek(TokenType::PUBLIC) && !TryPeek(TokenType::PRIVATE)) && !TryPeek(TokenType::IDENTIFIER, 1) || !TryPeek(TokenType::COLON, 2) || !TryPeek(TokenType::IDENTIFIER, 3)) {
      return std::nullopt;
    }
  } else {
    if (!TryPeek(TokenType::IDENTIFIER) || !TryPeek(TokenType::COLON, 1) || !TryPeek(TokenType::IDENTIFIER, 2)) {
      return std::nullopt;
    }
  }

  std::optional<Token> access_modifier;
  if (with_access_modifier) {
    access_modifier = Consume();
  }
  
  auto name = Consume();
  Consume();
  auto type = Consume();
  auto var = m_alloc.Emplace<NodeVarDeclaration>(name, type, std::move(access_modifier));

  return var; 
}
