#pragma once

#include "Tokenizer.hpp"

namespace HVE::Parser {
  struct TranslationUnit {
    std::vector<std::string> referenced_tus;
  };

  enum class ParserState {
    START,
    IMPORT
  };

  class Parser {
  public:
    Parser(std::vector<Token>&& tokens) : m_tokens(tokens) {}

    TranslationUnit Parse();
  private:
    Token Peek(std::uint8_t offset = 1);
    void Advance();

    std::vector<Token> m_tokens;
    std::vector<Token> m_tmp_buf;
    std::uint32_t m_current_token = 0;
    ParserState m_state = ParserState::START;
  };
};