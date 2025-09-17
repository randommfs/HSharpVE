#include "gtest/gtest.h"

#define private public
#include "Tokenizer.hpp"

TEST(Tokenizer, SemicolonToken) {
  std::string test{";"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::SEMICOLON);
}

TEST(Tokenizer, ColonToken) {
  std::string test{":"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::COLON);
}

TEST(Tokenizer, OpenParenthesisToken) {
  std::string test{"("};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::OPEN_PARENTHESIS);
}

TEST(Tokenizer, CloseParenthesisToken) {
  std::string test{")"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::CLOSE_PARENTHESIS);
}

TEST(Tokenizer, OpenBraceToken) {
  std::string test{"{"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::OPEN_BRACE);
}

TEST(Tokenizer, CloseBraceToken) {
  std::string test{"}"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::CLOSE_BRACE);
}

TEST(Tokenizer, OpenBracketToken) {
  std::string test{"["};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::OPEN_BRACKET);
}

TEST(Tokenizer, CloseBracketToken) {
  std::string test{"]"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::CLOSE_BRACKET);
}

TEST(Tokenizer, OpenAngleBracketToken) {
  std::string test{"<"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::OPEN_ANGLE_BRACKET);
}

TEST(Tokenizer, CloseAngleBracketToken) {
  std::string test{">"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::CLOSE_ANGLE_BRACKET);
}

TEST(Tokenizer, ReferenceToken) {
  std::string test{"&"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::REFERENCE);
}

TEST(Tokenizer, PlusToken) {
  std::string test{"+"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::PLUS);
}

TEST(Tokenizer, MinusToken) {
  std::string test{"-"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::MINUS);
}

TEST(Tokenizer, StarToken) {
  std::string test{"*"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::STAR);
}

TEST(Tokenizer, SlashToken) {
  std::string test{"/"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::SLASH);
}

TEST(Tokenizer, BackslashToken) {
  std::string test{"\\"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::BACKSLASH);
}

TEST(Tokenizer, DotToken) {
  std::string test{"."};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::DOT);
}

TEST(Tokenizer, CommaToken) {
  std::string test{","};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::COMMA);
}

TEST(Tokenizer, UnknownToken) {
  std::string test{"/*  hagsgs */ ass"};
  HVE::Tokenizer tokenizer(test);

  auto tok = tokenizer.GetNextToken();

  ASSERT_EQ(tok.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(tok.lexeme, "ass");
}

TEST(Tokenizer, ExportToken_WithWhitespace) {
  std::string test{"   \n\texport"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::EXPORT);
}

TEST(Tokenizer, ImportToken_WithWhitespace) {
  std::string test{"   \n\timport"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::IMPORT);
}

TEST(Tokenizer, PrivateToken_WithWhitespace) {
  std::string test{"   \n\tpriv"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::PRIVATE);
}

TEST(Tokenizer, PublicToken_WithWhitespace) {
  std::string test{"   \n\tpub"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::PUBLIC);
}

TEST(Tokenizer, StaticToken_WithWhitespace) {
  std::string test{"   \n\tstat"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::STATIC);
}

TEST(Tokenizer, DynamicToken_WithWhitespace) {
  std::string test{"   \n\tdyn"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::DYNAMIC);
}

TEST(Tokenizer, FunctionDefToken_WithWhitespace) {
  std::string test{"   \n\tfunc"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::FUNCTION_DEF);
}

TEST(Tokenizer, TypenameToken_WithWhitespace) {
  std::string test{"   \n\ttypename"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::TYPENAME);
}

TEST(Tokenizer, IfToken_WithWhitespace) {
  std::string test{"   \n\tif"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::IF);
}

TEST(Tokenizer, ElifToken_WithWhitespace) {
  std::string test{"   \n\telif"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::ELIF);
}

TEST(Tokenizer, OtherwiseToken_WithWhitespace) {
  std::string test{"   \n\totherwise"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::OTHERWISE);
}

TEST(Tokenizer, CycleToken_WithWhitespace) {
  std::string test{"   \n\tcycle"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::CYCLE);
}

TEST(Tokenizer, ForToken_WithWhitespace) {
  std::string test{"   \n\tfor"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::FOR);
}

TEST(Tokenizer, NonetypeToken_WithWhitespace) {
  std::string test{"   \n\tnone"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::NONETYPE);
}

TEST(Tokenizer, AutotypeToken_WithWhitespace) {
  std::string test{"   \n\teasy"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_EQ(tokenizer.GetNextToken().type, HVE::TokenType::AUTOTYPE);
}

TEST(Tokenizer, StringLit_WithWhitespace) {
  std::string test{"   \n\t\"test\""};
  HVE::Tokenizer tokenizer(test);

  auto tok = tokenizer.GetNextToken();

  ASSERT_EQ(tok.type, HVE::TokenType::STRING_LITERAL);
  ASSERT_EQ(tok.lexeme, "test");
}

TEST(Tokenizer, IntLit_WithWhitespace) {
  std::string test{"   \n\t42069"};
  HVE::Tokenizer tokenizer(test);

  auto tok = tokenizer.GetNextToken();

  ASSERT_EQ(tok.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(tok.lexeme, "42069");
}

TEST(Tokenizer, FloatLit_WithWhitespace) {
  std::string test{"   \n\t420.69"};
  HVE::Tokenizer tokenizer(test);

  auto tok = tokenizer.GetNextToken();

  ASSERT_EQ(tok.type, HVE::TokenType::FLOAT_LITERAL);
  ASSERT_EQ(tok.lexeme, "420.69");
}

TEST(Tokenizer, FloatLitMultipleDots_WithWhitespace) {
  std::string test{"   \n\t420.6.9"};
  HVE::Tokenizer tokenizer(test);

  ASSERT_THROW({
      try {
          tokenizer.GetNextToken();
        } catch (std::runtime_error& err) {
          ASSERT_STREQ(err.what(), "Multiple dots in a float literal");
          throw;
        }
      }, std::runtime_error);
}
