#include "Parser.hpp"

#include <stdexcept>

HVE::Parser::TranslationUnit HVE::Parser::Parser::Parse() {
  for (auto& token : m_tokens) {
    switch (m_state) {
    case ParserState::START:
      switch (token.type) {
      case TokenType::IMPORT:
        m_state = ParserState::IMPORT;
        continue;
      }
      break;
    case ParserState::IMPORT:
      switch (token.type) {
      case TokenType::IDENTIFIER:
        if (m_tmp_buf.size() % 2) {
          m_tmp_buf.emplace_back(std::move(token));
        } else {
          throw std::runtime_error("Expected dot, got identifier");
        }
        break;
      case TokenType::DOT:
        if (m_tmp_buf.size() % 2 == 1) {
          m_tmp_buf.emplace_back(std::move(token));
        } else {
          throw std::runtime_error("Expected identifier, got dot");
        }
      }
      default:
        throw std::runtime_error("Unexpected token");
      break;
    }
  }
}

HVE::Token HVE::Parser::Parser::Peek(std::uint8_t offset) {
  return m_tokens[m_current_token + offset];
}

void HVE::Parser::Parser::Advance() {
  ++m_current_token;
}
