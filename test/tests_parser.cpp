#include <variant>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <parser/parser.hpp>
#include <parser/nodes.hpp>
#include <tests.hpp>

using HSharpParser::Tokenizer;
using HSharpParser::Parser;
using HSharpParser::TokenType;
using HSharpParser::Token;
using std::vector;
using std::string;


TEST(PARSER, VARIABLE_CREATION_STR){
    File file("var str = \"50\";");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR,NodeType::STR_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_ASSIGN_VALUE_STR){
    File file("str = \"50\";");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR_ASSIGN,NodeType::STR_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_CREATION_INT){
    File file("var num = 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_ASSIGN_VALUE_INT){
    File file("num = 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR_ASSIGN,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_CREATION_BIN_EXPR_ADD_INT){
    File file("var num = 50 + 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR,NodeType::BIN_EXPR,
            NodeType::ADD,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_CREATION_BIN_EXPR_SUB_INT){
    File file("var num = 50 - 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR,NodeType::BIN_EXPR,
            NodeType::SUB,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_CREATION_BIN_EXPR_MUL_INT){
    File file("var num = 50 * 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR,NodeType::BIN_EXPR,
            NodeType::MUL,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_CREATION_BIN_EXPR_DIV_INT){
    File file("var num = 50 / 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR,NodeType::BIN_EXPR,
            NodeType::DIV,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_ASSIGN_VALUE_BIN_EXPR_ADD_INT){
    File file("num = 50 + 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR_ASSIGN,NodeType::BIN_EXPR,
            NodeType::ADD,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_ASSIGN_VALUE_BIN_EXPR_SUB_INT){
    File file("num = 50 - 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR_ASSIGN,NodeType::BIN_EXPR,
            NodeType::SUB,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_ASSIGN_VALUE_BIN_EXPR_MUL_INT){
    File file("num = 50 * 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR_ASSIGN,NodeType::BIN_EXPR,
            NodeType::MUL,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_ASSIGN_VALUE_BIN_EXPR_DIV_INT){
    File file("num = 50 / 50;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR_ASSIGN,NodeType::BIN_EXPR,
            NodeType::DIV,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_CREATION_BIN_EXPR_MUL_ADD_MULTIPLE_OPERATORS){
    File file("var num = 50 * 50 + 10;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR,NodeType::BIN_EXPR,
            NodeType::ADD,NodeType::BIN_EXPR,
            NodeType::MUL,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT,NodeType::BIN_EXPR,
            NodeType::MUL,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, VARIABLE_CREATION_BIN_EXPR_MUL_ADD_MUL_DIV_MULTIPLE_OPERATORS){
    File file("var num = 50 * 50 + 10 * 5 / 10;");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR,NodeType::BIN_EXPR,
            NodeType::ADD,NodeType::BIN_EXPR,
            NodeType::MUL,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT,NodeType::BIN_EXPR,
            NodeType::MUL,NodeType::TERM,
            NodeType::INT_LIT,NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, PRINT_STR){
    File file("print(\"str\");");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::PRINT, NodeType::STR_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, PRINT_STR_VAR){
    File file("var str = \"str\";\nprint(str);");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR, NodeType::STR_LIT,
            NodeType::PRINT, NodeType::TERM,
            NodeType::IDENT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 2);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, EXIT_INT){
    File file("exit(50);");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::EXIT, NodeType::TERM,
            NodeType::INT_LIT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 1);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}

TEST(PARSER, EXIT_INT_VAR){
    File file("var code = 50; exit(code);");
    vector<NodeType> visitors_activated{};
    vector<string> lines{};
    Visitor svisitor{visitors_activated};
    NodeType expected[] = {
            NodeType::VAR, NodeType::TERM,
            NodeType::INT_LIT, NodeType::EXIT,
            NodeType::TERM, NodeType::IDENT
    };

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    for (uint32_t i = 0; i < nodes->statements.size(); ++i)
        std::visit(svisitor, nodes->statements[i]->statement);

    ASSERT_TRUE(nodes.has_value());
    EXPECT_EQ(nodes.value().statements.size(), 2);
    EXPECT_EQ(nodes.value().statements[0]->line, 0);
    EXPECT_EQ(visitors_activated.size(), sizeof(expected) / sizeof(NodeType));
    EXPECT_THAT(visitors_activated, ::testing::ElementsAreArray(expected));
}