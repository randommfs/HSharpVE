#include "gtest/gtest.h"
#include <variant>

#define private public
#include "Parser.hpp"

TEST(Parser, ParseExpr) {
  std::string src{"test();"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseStatement();

  ASSERT_TRUE(result.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeExpr*>(result.value()->stmt));
  auto expr = std::get<HVE::Parser::NodeExpr*>(result.value()->stmt);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(expr->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(expr->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncCall*>(term->term));
  auto func = std::get<HVE::Parser::NodeFuncCall*>(term->term);
  ASSERT_EQ(func->name.lexeme, "test");
  ASSERT_TRUE(func->args.empty());
  ASSERT_TRUE(func->template_args.empty());
}

TEST(Parser, ParseAssignment) {
  std::string src{"5 = 7;"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseStatement();

  ASSERT_TRUE(result.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeAssignment*>(result.value()->stmt));
  auto assign = std::get<HVE::Parser::NodeAssignment*>(result.value()->stmt);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(assign->lhs->var));
  auto lhs = std::get<HVE::Parser::NodeTerm*>(assign->lhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(lhs->term));
  auto lhs_term = std::get<HVE::Parser::NodeTermIntLit*>(lhs->term);
  ASSERT_EQ(lhs_term->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(assign->lhs->var));
  auto rhs = std::get<HVE::Parser::NodeTerm*>(assign->rhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(lhs->term));
  auto rhs_term = std::get<HVE::Parser::NodeTermIntLit*>(rhs->term);
  ASSERT_EQ(rhs_term->int_lit.lexeme, "7");
}