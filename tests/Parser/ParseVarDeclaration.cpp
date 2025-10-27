#include <stdexcept>
#include "gtest/gtest.h"
#include <variant>

#define private public
#include "../../src/Parser/AST/ASTBuilder.hpp"

TEST(Parser, ParseVarDeclNoExpr) {
  std::string src{"foo: i32;"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::ASTBuilder parser(std::move(tokens));

  auto decl = parser.ParseVarDeclaration();

  ASSERT_TRUE(decl.has_value());
  ASSERT_EQ(decl.value()->expr, nullptr);
  ASSERT_EQ(decl.value()->decl.ident.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->decl.ident.lexeme, "foo");
  ASSERT_EQ(decl.value()->decl.type.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(decl.value()->decl.type.lexeme, "i32");
  ASSERT_EQ(decl.value()->expr, nullptr);
}
