#include "Tokenizer.hpp"
#include "gtest/gtest.h"
#include <variant>

#define private public
#include "Parser.hpp"

TEST(Parser, ParseFuncCallNoArgs) {
  std::string src{"test()"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseFuncCall();

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value()->name.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(result.value()->name.lexeme, "test");
  ASSERT_EQ(result.value()->args.size(), 0);
}

TEST(Parser, ParseFuncCallSingleArg) {
  std::string src{"test(5)"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseFuncCall();

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value()->name.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(result.value()->name.lexeme, "test");
  ASSERT_EQ(result.value()->args.size(), 1);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->args[0]->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(result.value()->args[0]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");
}

TEST(Parser, ParseFuncCallMultipleArgs) {
  std::string src{"test(5, 4, 6)"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseFuncCall();

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value()->name.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(result.value()->name.lexeme, "test");
  ASSERT_EQ(result.value()->args.size(), 3);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->args[0]->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(result.value()->args[0]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->args[1]->var));
  term = std::get<HVE::Parser::NodeTerm*>(result.value()->args[1]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "4");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->args[2]->var));
  term = std::get<HVE::Parser::NodeTerm*>(result.value()->args[2]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "6");
}

TEST(Parser, ParseTemplateFuncCallNoArgs) {
  std::string src{"test<>()"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseFuncCall();

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value()->name.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(result.value()->name.lexeme, "test");
  ASSERT_EQ(result.value()->args.size(), 0);
  ASSERT_EQ(result.value()->template_args.size(), 0);
}

TEST(Parser, ParseTemplateFuncCallSingleArg) {
  std::string src{"test<8>(5)"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseFuncCall();

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value()->name.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(result.value()->name.lexeme, "test");
  ASSERT_EQ(result.value()->args.size(), 1);
  ASSERT_EQ(result.value()->template_args.size(), 1);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->args[0]->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(result.value()->args[0]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->template_args[0]->var));
  term = std::get<HVE::Parser::NodeTerm*>(result.value()->template_args[0]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "8");

}

TEST(Parser, ParseTemplateFuncCallMultipleArgs) {
  std::string src{"test<2, 8>(5, 4, 6)"};
  HVE::Tokenizer tokenizer(src);
  auto tokens = tokenizer.Tokenize();
  HVE::Parser::Parser parser(std::move(tokens));
  auto result = parser.ParseFuncCall();

  ASSERT_TRUE(result.has_value());
  ASSERT_EQ(result.value()->name.type, HVE::TokenType::IDENTIFIER);
  ASSERT_EQ(result.value()->name.lexeme, "test");
  ASSERT_EQ(result.value()->args.size(), 3);
  ASSERT_EQ(result.value()->template_args.size(), 2);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->args[0]->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(result.value()->args[0]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->args[1]->var));
  term = std::get<HVE::Parser::NodeTerm*>(result.value()->args[1]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "4");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->args[2]->var));
  term = std::get<HVE::Parser::NodeTerm*>(result.value()->args[2]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "6");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->template_args[0]->var));
  term = std::get<HVE::Parser::NodeTerm*>(result.value()->template_args[0]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "2");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(result.value()->template_args[1]->var));
  term = std::get<HVE::Parser::NodeTerm*>(result.value()->template_args[1]->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "8");

}
