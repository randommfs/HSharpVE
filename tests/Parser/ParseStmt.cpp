#include "gtest/gtest.h"
#include <variant>

#define private public
#include "../../src/Parser/AST/ASTBuilder.hpp"

TEST(Parser, ParseExpr) {
  std::string src{"test();"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::ASTBuilder parser(std::move(tokens));
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
  HVE::Parser::ASTBuilder parser(std::move(tokens));
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

TEST(Parser, ParseImport) {
  std::string src{"import stl.foo;"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::ASTBuilder parser(std::move(tokens));
  auto result = parser.ParseImport();

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value()->import_target.size(), 2);
  ASSERT_EQ(result.value()->import_target[0], "stl");
  ASSERT_EQ(result.value()->import_target[1], "foo");
}

TEST(Parser, ParseStmtAssignmentInit) {
  std::string src{"foo: i32 = 5;"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::ASTBuilder parser(std::move(tokens));

  auto decl = parser.ParseStatement();

  ASSERT_TRUE(decl.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeVarDeclaration*>(decl.value()->stmt));
  auto var = std::get<HVE::Parser::NodeVarDeclaration*>(decl.value()->stmt);
  ASSERT_EQ(var->decl.ident.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(var->decl.ident.lexeme, "foo");
  ASSERT_EQ(var->decl.type.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(var->decl.type.lexeme, "i32");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(var->expr->var));
}
