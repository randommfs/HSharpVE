#include "gtest/gtest.h"

#include "Analyzer/SemanticAnalyzer.hpp"
#define private public
#include "Mangle.hpp"
#undef private

TEST(Mangler, MangleFromFuncDef) {
    std::string source = R"(
pub main(arg1: u64, arg2: str): i32 {}
)";
    HVE::Tokenizer tokenizer(source);
    auto tokens = tokenizer.Tokenize();
    HVE::Parser::ASTBuilder builder(std::move(tokens));
    auto ast = builder.BuildAST();
    ASSERT_EQ(HVE::Mangle::Mangle(std::get<HVE::Parser::NodeFuncDef*>(ast->stmts[0]->stmt)), "__HVrt_i32__HVn_main__HVa_u64__HVa_str");
}

TEST(Mangler, DemangleFromStr) {
    std::string source = "__HVrt_i32__HVn_main__HVa_u64__HVa_str";
    ASSERT_EQ(HVE::Mangle::Demangle(source), "i32 main(u64,str)");
}