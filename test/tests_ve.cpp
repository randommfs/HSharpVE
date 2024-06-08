#include <variant>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <parser/parser.hpp>
#include <parser/nodes.hpp>
#include <ve/ve.hpp>
#include <tests.hpp>

using HSharpParser::Tokenizer;
using HSharpParser::Parser;
using HSharpVE::VirtualEnvironment;
using std::string;
using std::vector;

TEST(VE, EXIT_INT){
    File file("exit(50);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(50), "");
}

TEST(VE, EXIT_INT_VAR){
    File file("var code = 50; exit(code);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(50), "");
}

TEST(VE, EXIT_STR){
    File file("exit(\"50\");");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(50), "");
}

TEST(VE, EXIT_STR_VAR){
    File file("var code = \"50\"; exit(code);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(50), "");
}

TEST(VE, PRINT_INT){
    File file("print(69); exit(0);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    ::testing::internal::CaptureStdout();
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(0), "");
    EXPECT_EQ(::testing::internal::GetCapturedStdout(), "69\n");
}

TEST(VE, PRINT_INT_VAR){
    File file("var str = 69; print(str); exit(0);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    ::testing::internal::CaptureStdout();
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(0), "");
    EXPECT_EQ(::testing::internal::GetCapturedStdout(), "69\n");
}

TEST(VE, PRINT_STR){
    File file("print(\"str\"); exit(0);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    ::testing::internal::CaptureStdout();
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(0), "");
    EXPECT_EQ(::testing::internal::GetCapturedStdout(), "str\n");
}

TEST(VE, PRINT_STR_VAR){
    File file("var str = \"test lol\"; print(str); exit(0);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    ::testing::internal::CaptureStdout();
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(0), "");
    EXPECT_EQ(::testing::internal::GetCapturedStdout(), "test lol\n");
}

TEST(VE, EXIT_INT_BIN_EXPR){
    File file("exit(50 * 50 / 10 - 5 * 10);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(200), "");
}

TEST(VE, EXIT_INT_VAR_BIN_EXPR){
    File file("var code = 50 * 50 / 10 - 5 * 10; exit(code);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(200), "");
}

TEST(VE, CHANGE_VAR_TYPE){
    File file("var num = \"str\"; num = 50; exit(num);");
    vector<string> lines{};

    lines.push_back(file.contents.value());
    Tokenizer tokenizer(file);
    auto tokens = tokenizer.tokenize();
    Parser parser(tokens, lines);
    auto nodes = parser.parse_program();
    VirtualEnvironment virtualenv{nodes.value(), lines, true};
    EXPECT_EXIT(virtualenv.run(), ::testing::ExitedWithCode(50), "");
}