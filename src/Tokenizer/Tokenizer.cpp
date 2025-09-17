#include "Tokenizer.hpp"
#include <cctype>
#include <cstdint>

#include "mapbox/eternal.hpp"

static constexpr auto ident_assoc = mapbox::eternal::map<mapbox::eternal::string, HVE::TokenType>({
    {"import", HVE::TokenType::IMPORT},
    {"export", HVE::TokenType::EXPORT},
    {"pub", HVE::TokenType::PUBLIC},
    {"priv", HVE::TokenType::PRIVATE},
    {"stat", HVE::TokenType::STATIC},
    {"dyn", HVE::TokenType::DYNAMIC},
    {"func", HVE::TokenType::FUNCTION_DEF},
    {"typename", HVE::TokenType::TYPENAME},
    {"if", HVE::TokenType::IF},
    {"elif", HVE::TokenType::ELIF},
    {"otherwise", HVE::TokenType::OTHERWISE},
    {"cycle", HVE::TokenType::CYCLE},
    {"for", HVE::TokenType::FOR},
    {"none", HVE::TokenType::NONETYPE},
    {"easy", HVE::TokenType::AUTOTYPE}
});

char HVE::Tokenizer::Peek(std::uint8_t offset) {
  return source[cur_raw_offset + offset];
}

void HVE::Tokenizer::Advance() {
  char ch = Peek();
  if (source[cur_raw_offset++] == '\n') {
    ++cur_line;
    cur_offset = 0;
  } else {
    ++cur_offset;
  }
}

bool HVE::Tokenizer::SkipWhitespace() {
  bool has_effect = false;
  while (true) {
    switch (Peek(0)) {
    case '\n':
    case ' ':
    case '\t':
      Advance();
      has_effect = true;
      continue;
    case '\0':
    default:
      break;
    }
    break;
  }
  return has_effect;
}

bool HVE::Tokenizer::SkipComments() {
  char ch = Peek(0);
  bool has_effect = false;
  bool comment = true;

  if (!ch) {
    return false;
  }
  if (ch == '/' && Peek() == '*') {
    Advance();
    Advance();

    while (comment) {
      switch (Peek(0)) {
      case '*':
        if (Peek() == '/' || Peek() == '\0') {
          Advance();
          Advance();
          comment = false;
        }
        break;
      case '\0':
        comment = false;
        break;
      default:
        Advance();
        has_effect = true;
        continue;
      }
    }
    return true;
  } else if (ch == '/' && Peek() == '/') {
    Advance();
    Advance();

    while (comment) {
      switch (Peek(0)) {
      case '\0':
        comment = false;
        break;
      case '\n':
        Advance();
        comment = false;
        break;
      default:
        Advance();
        has_effect = true;
        continue;
      }
      break;
    }
  }
  return false;
}

HVE::Token HVE::Tokenizer::GetNextToken() {
  TokenizerState state = TokenizerState::START;
  std::string lexeme;
  bool running = true;

  while (SkipComments() || SkipWhitespace());

  while (running) {
    char ch = Peek(0);
    switch (state) {
    case TokenizerState::START:
      if (std::isalpha(ch)) {
        state = TokenizerState::IDENT;
        lexeme.push_back(ch);
        Advance();
        continue;
      } else if (std::isdigit(ch)) {
        state = TokenizerState::INT_LIT;
        lexeme.push_back(ch);
        Advance();
        continue;
      }
      switch (ch) {
      case '"':
        state = TokenizerState::STRING_LIT;
        Advance();
        continue;
      case '\'':
        state = TokenizerState::STRING_LIT;
        Advance();
        continue;
      case '\0':
        return CreateToken(TokenType::END_OF_FILE, {});
      default:
        Advance();
        return CreateToken(static_cast<TokenType>(ch), {});
      }
      break;
    case TokenizerState::IDENT:
      if (ch == '_' || std::isalnum(ch)) {
        lexeme.push_back(ch);
        Advance();
      } else {
        if (ident_assoc.contains(lexeme.c_str())) {
          return CreateToken(ident_assoc.at(lexeme.c_str()), lexeme);
        }
        return CreateToken(TokenType::IDENTIFIER, lexeme);
      }
      break;
    case TokenizerState::STRING_LIT:
      if (ch != '"') {
        lexeme.push_back(ch);
        Advance();
      } else {
        Advance();
        return CreateToken(TokenType::STRING_LITERAL, lexeme);
      }
      break;
    case TokenizerState::INT_LIT:
      if (ch == '.') {
        lexeme.push_back(ch);
        state = TokenizerState::FLOAT_LIT;
        Advance();
      } else if (std::isdigit(ch)) {
        lexeme.push_back(ch);
        Advance();
      } else {
        return CreateToken(TokenType::INT_LITERAL, lexeme);
      }
      break;
    case TokenizerState::FLOAT_LIT:
      if (ch == '.') {
        throw std::runtime_error("Multiple dots in a float literal");
      } else if (std::isdigit(ch)) {
        lexeme.push_back(ch);
        Advance();
      } else {
        return CreateToken(TokenType::FLOAT_LITERAL, lexeme);
      }
      break;
    default:
      std::abort();
    }
  }
}

HVE::Token HVE::Tokenizer::CreateToken(TokenType type, std::string lexeme) {
  return Token{type, CreateSourceLocation(), std::move(lexeme)};
}

HVE::SourceLocation HVE::Tokenizer::CreateSourceLocation() {
  return SourceLocation{cur_line, cur_offset, cur_raw_offset};
}

std::vector<HVE::Token> HVE::Tokenizer::Tokenize() {
  
}
