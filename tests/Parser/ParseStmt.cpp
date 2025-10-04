#include "gtest/gtest.h"
#include <variant>

#define private public
#include "Parser.hpp"

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
