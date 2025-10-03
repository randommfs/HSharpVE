#include "gtest/gtest.h"
#include <variant>

#define private public
#include "Parser.hpp"

TEST(Parser, ParseVarAssign) {
  std::string src{"test: i32 = 5;"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseStatement();

  ASSERT_TRUE(result.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeVarAssign*>(result.value()->stmt));
  auto var_assign = std::get<HVE::Parser::NodeVarAssign*>(result.value()->stmt);
  ASSERT_EQ(var_assign->name.lexeme, "test");
  ASSERT_EQ(var_assign->type, "i32");
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(var_assign->expr->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(var_assign->expr->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");
}

TEST(Parser, ParseFuncCall) {
  std::string src{"test();"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseStatement();

  ASSERT_TRUE(result.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncCall*>(result.value()->stmt));
  auto call = std::get<HVE::Parser::NodeFuncCall*>(result.value()->stmt);
  ASSERT_EQ(call->name.lexeme, "test");
}
