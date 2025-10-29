#include <stdexcept>
#include "gtest/gtest.h"
#include <variant>

#define private public
#include "Parser/AST/ASTBuilder.hpp"

TEST(Parser, ParseClassDef) {
  std::string src{R"(class Foo {
    pub Method(): i32 {  }
    priv m_var: u64;
  })"};
  HVE::Tokenizer tokenizer(src);
  std::vector<HVE::Token> tokens = tokenizer.Tokenize();
  HVE::Parser::ASTBuilder parser(std::move(tokens));

  auto decl = parser.ParseClassDef();

  ASSERT_TRUE(decl.has_value());
  ASSERT_EQ(decl.value().member_vars.size(), 1);
  ASSERT_EQ(decl.value().member_methods.size(), 1);
}
