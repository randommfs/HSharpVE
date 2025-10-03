#include "Tokenizer.hpp"
#include <stdexcept>
#include "gtest/gtest.h"
#include <variant>

#define private public
#include "Parser.hpp"

TEST(Parser, ParseTermIntLit) {
  std::string src{"5"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  
  auto term = parser.ParseTerm();

  ASSERT_TRUE(term.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term.value()->term));
  HVE::Parser::NodeTermIntLit& lit = *std::get<HVE::Parser::NodeTermIntLit*>(term.value()->term);
  ASSERT_EQ(lit.int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit.int_lit.lexeme, "5");
}

TEST(Parser, ParseTermIdent) {
  std::string src{"test"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  
  auto term = parser.ParseTerm();

  ASSERT_TRUE(term.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIdent*>(term.value()->term));
  HVE::Parser::NodeTermIdent& lit = *std::get<HVE::Parser::NodeTermIdent*>(term.value()->term);
  ASSERT_EQ(lit.ident.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(lit.ident.lexeme, "test");
}

TEST(Parser, ParseReflectedIdent) {
  std::string src{"^^test"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));

  auto term = parser.ParseTerm();

  ASSERT_TRUE(term.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermReflectIdent*>(term.value()->term));
  HVE::Parser::NodeTermReflectIdent& lit = *std::get<HVE::Parser::NodeTermReflectIdent*>(term.value()->term);
  ASSERT_EQ(lit.ident.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(lit.ident.lexeme, "test");
}
