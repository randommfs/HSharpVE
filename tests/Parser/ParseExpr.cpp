#include "Tokenizer.hpp"
#include "gtest/gtest.h"
#include <variant>

#define private public
#include "Parser.hpp"

TEST(Parser, ParseIntLit) {
  std::string src = "5";
  auto tokens = HVE::Tokenizer{src}.Tokenize();
  auto parser = HVE::Parser::Parser(std::move(tokens));

  std::optional<HVE::Parser::NodeExpr*> expr = parser.ParseExpr();
  ASSERT_TRUE(expr.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(expr.value()->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(expr.value()->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  HVE::Parser::NodeTermIntLit* lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");
}

TEST(Parser, ParseBinExprAdd) {
  std::string src = "5 + 4";
  auto tokens = HVE::Tokenizer{src}.Tokenize();
  auto parser = HVE::Parser::Parser(std::move(tokens));

  std::optional<HVE::Parser::NodeExpr*> expr = parser.ParseExpr();
  ASSERT_TRUE(expr.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExpr*>(expr.value()->var));
  auto bexpr = std::get<HVE::Parser::NodeBinExpr*>(expr.value()->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExprAdd*>(bexpr->var));
  HVE::Parser::NodeBinExprAdd* bexpradd = std::get<HVE::Parser::NodeBinExprAdd*>(bexpr->var);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexpradd->lhs->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(bexpradd->lhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexpradd->rhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexpradd->rhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "4");
}

TEST(Parser, ParseBinExprSub) {
  std::string src = "5 - 4";
  auto tokens = HVE::Tokenizer{src}.Tokenize();
  auto parser = HVE::Parser::Parser(std::move(tokens));

  std::optional<HVE::Parser::NodeExpr*> expr = parser.ParseExpr();
  ASSERT_TRUE(expr.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExpr*>(expr.value()->var));
  auto bexpr = std::get<HVE::Parser::NodeBinExpr*>(expr.value()->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExprSub*>(bexpr->var));
  HVE::Parser::NodeBinExprSub* bexprsub = std::get<HVE::Parser::NodeBinExprSub*>(bexpr->var);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprsub->lhs->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(bexprsub->lhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprsub->rhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexprsub->rhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "4");
}

TEST(Parser, ParseBinExprMul) {
  std::string src = "5 * 4";
  auto tokens = HVE::Tokenizer{src}.Tokenize();
  auto parser = HVE::Parser::Parser(std::move(tokens));

  std::optional<HVE::Parser::NodeExpr*> expr = parser.ParseExpr();
  ASSERT_TRUE(expr.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExpr*>(expr.value()->var));
  auto bexpr = std::get<HVE::Parser::NodeBinExpr*>(expr.value()->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExprMulti*>(bexpr->var));
  HVE::Parser::NodeBinExprMulti* bexprmul = std::get<HVE::Parser::NodeBinExprMulti*>(bexpr->var);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprmul->lhs->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(bexprmul->lhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprmul->rhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexprmul->rhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "4");
}

TEST(Parser, ParseBinExprDiv) {
  std::string src = "5 / 4";
  auto tokens = HVE::Tokenizer{src}.Tokenize();
  auto parser = HVE::Parser::Parser(std::move(tokens));

  std::optional<HVE::Parser::NodeExpr*> expr = parser.ParseExpr();
  ASSERT_TRUE(expr.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExpr*>(expr.value()->var));
  auto bexpr = std::get<HVE::Parser::NodeBinExpr*>(expr.value()->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExprDiv*>(bexpr->var));
  HVE::Parser::NodeBinExprDiv* bexprdiv = std::get<HVE::Parser::NodeBinExprDiv*>(bexpr->var);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprdiv->lhs->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(bexprdiv->lhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprdiv->rhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexprdiv->rhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "4");
}

TEST(Parser, ParseBinExprDivParen) {
  std::string src = "(5 / 4)";
  auto tokens = HVE::Tokenizer{src}.Tokenize();
  auto parser = HVE::Parser::Parser(std::move(tokens));

  std::optional<HVE::Parser::NodeExpr*> expr = parser.ParseExpr();
  ASSERT_TRUE(expr.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(expr.value()->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(expr.value()->var);
  auto termparen = std::get<HVE::Parser::NodeTermParen*>(term->term);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExpr*>(termparen->expr->var));
  auto binexpr = std::get<HVE::Parser::NodeBinExpr*>(termparen->expr->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExprDiv*>(binexpr->var));
  auto bexprdiv = std::get<HVE::Parser::NodeBinExprDiv*>(binexpr->var);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprdiv->lhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexprdiv->lhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprdiv->rhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexprdiv->rhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "4");
}

TEST(Parser, ParseBinExprDiffPrec) {
  std::string src = "(5 - 4) * 8";
  auto tokens = HVE::Tokenizer{src}.Tokenize();
  auto parser = HVE::Parser::Parser(std::move(tokens));

  std::optional<HVE::Parser::NodeExpr*> expr = parser.ParseExpr();
  ASSERT_TRUE(expr.has_value());

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExpr*>(expr.value()->var));
  auto bexpr= std::get<HVE::Parser::NodeBinExpr*>(expr.value()->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExprMulti*>(bexpr->var));
  auto bexprmul = std::get<HVE::Parser::NodeBinExprMulti*>(bexpr->var);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprmul->lhs->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(bexprmul->lhs->var);
  auto termparen = std::get<HVE::Parser::NodeTermParen*>(term->term);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExpr*>(termparen->expr->var));
  auto binexpr = std::get<HVE::Parser::NodeBinExpr*>(termparen->expr->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExprSub*>(binexpr->var));
  auto bexprdiv = std::get<HVE::Parser::NodeBinExprSub*>(binexpr->var);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprdiv->lhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexprdiv->lhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprdiv->rhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexprdiv->rhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "4");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexprmul->rhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexprmul->rhs->var);
  ASSERT_TRUE(std::get<HVE::Parser::NodeTermIntLit*>(term->term));
  lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "8");
}

TEST(Parser, ParseBinExprAddWithFunc) {
  std::string src = "5 + test()";
  auto tokens = HVE::Tokenizer{src}.Tokenize();
  auto parser = HVE::Parser::Parser(std::move(tokens));

  std::optional<HVE::Parser::NodeExpr*> expr = parser.ParseExpr();
  ASSERT_TRUE(expr.has_value());
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExpr*>(expr.value()->var));
  auto bexpr = std::get<HVE::Parser::NodeBinExpr*>(expr.value()->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeBinExprAdd*>(bexpr->var));
  HVE::Parser::NodeBinExprAdd* bexpradd = std::get<HVE::Parser::NodeBinExprAdd*>(bexpr->var);

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexpradd->lhs->var));
  auto term = std::get<HVE::Parser::NodeTerm*>(bexpradd->lhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIntLit*>(term->term));
  auto lit = std::get<HVE::Parser::NodeTermIntLit*>(term->term);
  ASSERT_EQ(lit->int_lit.type, HVE::TokenType::INT_LITERAL);
  ASSERT_EQ(lit->int_lit.lexeme, "5");

  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(bexpradd->rhs->var));
  term = std::get<HVE::Parser::NodeTerm*>(bexpradd->rhs->var);
  ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncCall*>(term->term));
  auto call = std::get<HVE::Parser::NodeFuncCall*>(term->term);
  ASSERT_EQ(call->name.lexeme, "test");
  ASSERT_EQ(call->args.size(), 0);
  ASSERT_EQ(call->template_args.size(), 0);
}

