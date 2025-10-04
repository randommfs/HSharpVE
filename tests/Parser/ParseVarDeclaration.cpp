#include <stdexcept>
#include "gtest/gtest.h"
#include <variant>

#define private public
#include "Parser.hpp"

TEST(Parser, ParseVarDeclNoExpr) {
  std::string src{"foo: i32;"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));

  auto decl = parser.ParseVarDeclaration();

  ASSERT_TRUE(decl.has_value());
  ASSERT_EQ(decl.value()->expr, nullptr);
  ASSERT_EQ(decl.value()->ident.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->ident.lexeme, "foo");
  ASSERT_EQ(decl.value()->type.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->type.lexeme, "i32");
}

TEST(Parser, ParseVarDeclAssignmentInit) {
  std::string src{"foo: i32 = 5;"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));

  auto decl = parser.ParseVarDeclaration();

  ASSERT_TRUE(decl.has_value());
  ASSERT_EQ(decl.value()->ident.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->ident.lexeme, "foo");
  ASSERT_EQ(decl.value()->type.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->type.lexeme, "i32");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(decl.value()->expr->var));
}

TEST(Parser, ParseVarDeclBraceInit) {
  std::string src{"foo: i32{5};"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));

  auto decl = parser.ParseVarDeclaration();

  ASSERT_TRUE(decl.has_value());
  ASSERT_EQ(decl.value()->ident.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->ident.lexeme, "foo");
  ASSERT_EQ(decl.value()->type.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->type.lexeme, "i32");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(decl.value()->expr->var));
}

TEST(Parser, ParseVarDeclParenInit) {
  std::string src{"foo: i32{5};"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));

  auto decl = parser.ParseVarDeclaration();

  ASSERT_TRUE(decl.has_value());
  ASSERT_EQ(decl.value()->ident.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->ident.lexeme, "foo");
  ASSERT_EQ(decl.value()->type.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->type.lexeme, "i32");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(decl.value()->expr->var));
}