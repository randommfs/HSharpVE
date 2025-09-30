#include "Tokenizer.hpp"
#include <stdexcept>
#include "gtest/gtest.h"
#include <variant>

#define private public
#include "Parser.hpp"

TEST(Parser, ParseTermIntLit) {
  std::string src{"5"};
  std::vector<HVE::Token> tokens = HVE::Tokenizer{src}.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  
  auto term = parser.ParseTerm();

  ASSERT_TRUE(term.has_value());
  //ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(term.value().term));
  HVE::Parser::NodeTermIntLit& lit = *std::get<HVE::Parser::NodeTermIntLit*>(term.value()->term);
  ASSERT_EQ(lit.int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit.int_lit.lexeme, "5");
}


