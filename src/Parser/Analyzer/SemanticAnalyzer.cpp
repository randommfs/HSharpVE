#include "SemanticAnalyzer.hpp"

#include "../../Mangle/Mangle.hpp"

#include <iostream>

HVE::SemanticAnalyzer::Analyzer::Analyzer(Parser::NodeProgram* prog)
    : m_prog(prog) { }

HVE::SemanticAnalyzer::TranslationUnit HVE::SemanticAnalyzer::Analyzer::BuildTU() {
    TranslationUnit tu;

    for (auto stmt : m_prog->stmts) {
        std::visit([&tu]<typename T_>(T_&& arg) -> void {
          using T = std::remove_cvref_t<T_>;
            if constexpr (std::is_same_v<T, Parser::NodeImport*>) {
                tu.imports.push_back(arg);
            } else if constexpr (std::is_same_v<T, Parser::NodeFuncDef*>) {
                auto mangled = Mangle::Mangle(arg);
                if (tu.funcs.contains(mangled)) {
                    throw std::runtime_error("Function '" + mangled + "' redefinition");
                }
                tu.funcs[mangled] = arg;
            }
        }, stmt->stmt);
    }

    return tu;
}

void HVE::SemanticAnalyzer::Analyzer::AnalyzeTU(TranslationUnit& tu) {
    for (auto stmt : m_prog->stmts) {
        std::visit([&tu]<typename T>(T&& arg) -> void {
            if constexpr (std::is_same_v<T, Parser::NodeImport*>) {

            } else if constexpr (std::is_same_v<T, Parser::NodeFuncDef*>) {

            }
        }, stmt->stmt);
    }
}
