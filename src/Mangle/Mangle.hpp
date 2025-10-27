#include <string>

#include "AST/ASTBuilder.hpp"

namespace HVE::Mangle {
    constexpr auto return_type_label = "__HVrt_";
    constexpr auto name_label = "__HVn_";
    constexpr auto arg_label = "__HVa_";

    std::string Mangle(Parser::NodeFuncDef*);
    std::string Demangle(const std::string&);
}