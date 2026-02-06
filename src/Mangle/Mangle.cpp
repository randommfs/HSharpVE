#include "Mangle.hpp"

#include <cstring>

std::string HVE::Mangle::Mangle(HVE::Parser::NodeFuncDef* func_def) {
    std::string res;
    res += return_type_label;
    res += func_def->ret_type.lexeme;
    res += name_label;
    res += func_def->name.lexeme;
    for (const auto& arg : func_def->args) {
        res += arg_label;
        res += arg.type.lexeme;
    }
    return res;
}

std::string HVE::Mangle::Demangle(const std::string& src) {
    std::string res;
    res += src.substr(src.find(return_type_label) + strlen(return_type_label),
        src.find(name_label) - strlen(return_type_label));
    res += ' ';
    if (src.find(arg_label) != std::string::npos) {
        res += src.substr(src.find(name_label) + strlen(name_label),
            src.find(arg_label) - (src.find(name_label) + strlen(name_label)));
        res += "(";
        std::uint64_t arg = 0;
        std::string_view substr = src;
        while (true) {
            arg = substr.find(arg_label) + strlen(arg_label);
            substr = substr.substr(arg);
            if (substr.find(arg_label) == std::string::npos) {
                res += substr;
                break;
            } else {
                res += substr.substr(0, substr.find(arg_label));
            }
            res += ',';
        }
    } else {
        res += src.substr(src.find(name_label) + strlen(name_label));
        res += '(';
    }
    res += ')';
    return res;
}
