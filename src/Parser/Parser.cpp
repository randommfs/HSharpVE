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
  case TokenType::IDENTIFIER: {
    auto int_tok = Consume();
    auto* lit = m_alloc.Emplace<NodeTermIdent>(std::move(int_tok));
    return m_alloc.Emplace<NodeTerm>(lit);
  }
  case TokenType::OPEN_PARENTHESIS: {
    Consume();
    //auto expr = ParseExpression();
    //if (!expr.value()) {
    //  throw std::runtime_error("Expected expression"):
    //}
    //return m_alloc.Emplace<NodeTermParen>(expr.value());
    std::abort();
    break;
  }
  default:
    std::abort();
  }
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
