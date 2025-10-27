#include "gtest/gtest.h"

#define private public
#include "Analyzer/SemanticAnalyzer.hpp"
#undef private

TEST(SemanticAnalyzer, BuildTU) {
    std::string source = R"(
import stl;
import stl.io;
import stl.math;

pub main(): i32 {}
pub foo(): void {}
)";
    HVE::Tokenizer tokenizer(source);
    auto tokens = tokenizer.Tokenize();
    HVE::Parser::ASTBuilder builder(std::move(tokens));
    auto ast = builder.BuildAST();
    HVE::SemanticAnalyzer::Analyzer analyzer(ast);
    auto tu = analyzer.BuildTU();

    ASSERT_EQ(tu.imports.size(), 3);
    ASSERT_EQ(tu.funcs.size(), 2);
}