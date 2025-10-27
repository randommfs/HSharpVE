#include <unordered_map>

#include "AST/ASTBuilder.hpp"

namespace HVE::SemanticAnalyzer {
    struct TranslationUnit {
        Parser::NodeProgram* program;
        std::vector<Parser::NodeImport*> imports;
        std::unordered_map<std::string, Parser::NodeFuncDef*> funcs;
    };
    class Analyzer {
    public:
        Analyzer(Parser::NodeProgram*);

        TranslationUnit BuildTU();

        void AnalyzeTU(TranslationUnit&);
    private:
        Parser::NodeProgram* m_prog;
    };
}