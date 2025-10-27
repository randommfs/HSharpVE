#include "gtest/gtest.h"
#include <variant>

#define private public
#include "../../src/Parser/AST/ASTBuilder.hpp"

TEST(Parser, ParseTLSImport) {
    std::string src{"import stl.foo;"};
    HVE::Tokenizer tokenizer(src);
    auto tokens = tokenizer.Tokenize();
    HVE::Parser::ASTBuilder parser(std::move(tokens));
    auto result = parser.ParseTopLevelStmt();

    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeImport*>(result.value()->stmt));
    auto import = std::get<HVE::Parser::NodeImport*>(result.value()->stmt);
    ASSERT_EQ(import->import_target.size(), 2);
    ASSERT_EQ(import->import_target[0], "stl");
    ASSERT_EQ(import->import_target[1], "foo");
}

TEST(Parser, ParseTLSFuncDef) {
    std::string src{"pub foo(): i32 {}"};
    HVE::Tokenizer tokenizer(src);
    auto tokens = tokenizer.Tokenize();
    HVE::Parser::ASTBuilder parser(std::move(tokens));
    auto result = parser.ParseTopLevelStmt();

    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(std::holds_alternative<HVE::Parser::NodeFuncDef*>(result.value()->stmt));
    auto func_def = std::get<HVE::Parser::NodeFuncDef*>(result.value()->stmt);
    ASSERT_EQ(func_def->access.type, HVE::TokenType::PUBLIC);
    ASSERT_EQ(func_def->name.lexeme, "foo");
    ASSERT_EQ(func_def->ret_type.lexeme, "i32");
    ASSERT_EQ(func_def->args.size(), 0);
    ASSERT_EQ(func_def->stmts->stmts.size(), 0);
}