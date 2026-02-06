#include "Tokenizer.hpp"
#include <stdexcept>
#include "gtest/gtest.h"
#include <variant>

#define private public
#include "AST/ASTBuilder.hpp"

TEST(Parser, ParseComplexTermIdentIdent) {
    std::string src{"test.foo"};
    HVE::Tokenizer tokenizer(src);
    std::vector<HVE::Token> tokens = tokenizer.Tokenize();
    HVE::Parser::ASTBuilder parser(std::move(tokens));

    auto term = parser.ParseComplexTerm();

    ASSERT_TRUE(term.has_value());
    ASSERT_EQ(term.value()->terms.size(), 2);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIdent*>(term.value()->terms[0]->term));
    auto term1 = std::get<HVE::Parser::NodeTermIdent*>(term.value()->terms[0]->term);
    ASSERT_EQ(term1->ident.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(term1->ident.lexeme, "test");

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIdent*>(term.value()->terms[1]->term));
    auto term2 = std::get<HVE::Parser::NodeTermIdent*>(term.value()->terms[1]->term);
    ASSERT_EQ(term2->ident.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(term2->ident.lexeme, "foo");
}

TEST(Parser, ParseComplexTermIdentFunc) {
    std::string src{"test.foo()"};
    HVE::Tokenizer tokenizer(src);
    std::vector<HVE::Token> tokens = tokenizer.Tokenize();
    HVE::Parser::ASTBuilder parser(std::move(tokens));

    auto term = parser.ParseComplexTerm();

    ASSERT_TRUE(term.has_value());
    ASSERT_EQ(term.value()->terms.size(), 2);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIdent*>(term.value()->terms[0]->term));
    auto term1 = std::get<HVE::Parser::NodeTermIdent*>(term.value()->terms[0]->term);
    ASSERT_EQ(term1->ident.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(term1->ident.lexeme, "test");

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncCall*>(term.value()->terms[1]->term));
    auto call = std::get<HVE::Parser::NodeFuncCall*>(term.value()->terms[1]->term);
    ASSERT_EQ(call->name.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(call->name.lexeme, "foo");
    ASSERT_EQ(call->args.size(), 0);
    ASSERT_EQ(call->template_args.size(), 0);
}

TEST(Parser, ParseComplexTermFuncFunc) {
    std::string src{"test().foo()"};
    HVE::Tokenizer tokenizer(src);
    std::vector<HVE::Token> tokens = tokenizer.Tokenize();
    HVE::Parser::ASTBuilder parser(std::move(tokens));

    auto term = parser.ParseComplexTerm();

    ASSERT_TRUE(term.has_value());
    ASSERT_EQ(term.value()->terms.size(), 2);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncCall*>(term.value()->terms[0]->term));
    auto call1 = std::get<HVE::Parser::NodeFuncCall*>(term.value()->terms[0]->term);
    ASSERT_EQ(call1->name.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(call1->name.lexeme, "test");
    ASSERT_EQ(call1->args.size(), 0);
    ASSERT_EQ(call1->template_args.size(), 0);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncCall*>(term.value()->terms[1]->term));
    auto call2 = std::get<HVE::Parser::NodeFuncCall*>(term.value()->terms[1]->term);
    ASSERT_EQ(call2->name.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(call2->name.lexeme, "foo");
    ASSERT_EQ(call2->args.size(), 0);
    ASSERT_EQ(call2->template_args.size(), 0);
}

TEST(Parser, ParseComplexTermExprFunc) {
    std::string src{"(test.bar(5, 4).baz()).foo()"};
    HVE::Tokenizer tokenizer(src);
    std::vector<HVE::Token> tokens = tokenizer.Tokenize();
    HVE::Parser::ASTBuilder parser(std::move(tokens));

    auto term = parser.ParseComplexTerm();

    ASSERT_TRUE(term.has_value());
    ASSERT_EQ(term.value()->terms.size(), 2);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermParen*>(term.value()->terms[0]->term));
    auto cmp_paren = std::get<HVE::Parser::NodeTermParen*>(term.value()->terms[0]->term);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTerm*>(cmp_paren->expr->var));
    auto inner_term = std::get<HVE::Parser::NodeTerm*>(cmp_paren->expr->var);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeComplexTerm*>(inner_term->term));
    auto inner_cmp_term = std::get<HVE::Parser::NodeComplexTerm*>(inner_term->term);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeTermIdent*>(inner_cmp_term->terms[0]->term));
    auto inner_ident1 = std::get<HVE::Parser::NodeTermIdent*>(inner_cmp_term->terms[0]->term);
    ASSERT_EQ(inner_ident1->ident.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(inner_ident1->ident.lexeme, "test");

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncCall*>(inner_cmp_term->terms[1]->term));
    auto inner_call2 = std::get<HVE::Parser::NodeFuncCall*>(inner_cmp_term->terms[1]->term);
    ASSERT_EQ(inner_call2->name.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(inner_call2->name.lexeme, "bar");
    ASSERT_EQ(inner_call2->args.size(), 2);
    ASSERT_EQ(inner_call2->template_args.size(), 0);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncCall*>(inner_cmp_term->terms[2]->term));
    auto inner_call3 = std::get<HVE::Parser::NodeFuncCall*>(inner_cmp_term->terms[2]->term);
    ASSERT_EQ(inner_call3->name.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(inner_call3->name.lexeme, "baz");
    ASSERT_EQ(inner_call3->args.size(), 0);
    ASSERT_EQ(inner_call3->template_args.size(), 0);

    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncCall*>(term.value()->terms[1]->term));
    auto outer_call = std::get<HVE::Parser::NodeFuncCall*>(term.value()->terms[1]->term);
    ASSERT_EQ(outer_call->name.type, HVE::TokenType::IDENTIFIER);
    ASSERT_EQ(outer_call->name.lexeme, "foo");
    ASSERT_EQ(outer_call->args.size(), 0);
    ASSERT_EQ(outer_call->template_args.size(), 0);
}