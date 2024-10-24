#include "pog/parser_report.h"
#include <parser/parser.hpp>
#include <parser/rules/tokenization.hpp>

#include <mapbox/eternal.hpp>

#include <compiler/compiler.hpp>

#include <pog/pog.h>
#include <iostream>
#include <stdexcept>

#define _VISIBILITY_HIDDEN [[gnu::visibility("hidden")]]

_VISIBILITY_HIDDEN
HSharpParser::Value op_to_token(std::vector<HSharpParser::Value> tok) {
    std::cout << "Parsed operator: " << std::get<HSharpParser::Token>(tok[0]).str << '\n';
    return {HSharpParser::Token{ static_cast<HSharpParser::TokenType>(std::get<HSharpParser::Token>(tok[0]).str[0]) }};
}

_VISIBILITY_HIDDEN
auto type_to_token = [](auto&& args) -> HSharpParser::Value {
    auto& tok = std::get<HSharpParser::Token>(args[0]).str;

    std::cout << "Parsed type: " << tok << '\n';
    return { HSharpParser::Token{ HSharpParser::TokenType::TYPE, tok } };
};

HSharpParser::Parser::Parser(HSharpCompiler::Compiler& compiler) : compiler(compiler) {
    // Init tokenizer rules
    for (const auto& rule : rules) {
        auto& builder = parser.token(rule.regex_);
        if (!rule.symbol_.empty())
            builder.symbol(rule.symbol_);
        if (!rule.description_.empty())
            builder.symbol(rule.description_);
        if (rule.action_)
            builder.action(rule.action_);
        if (rule.fullword_)
            builder.fullword();
    }
    parser.token(R"(\+)").symbol("+").precedence(1, pog::Associativity::Left);
    parser.end_token().action([](auto&& args) -> Value {return {};});

    _apply_parser_rules();

    this->type_to_tkn = type_to_token;
    this->compile_expr = [this](auto&& args) -> HSharpParser::Value {
    std::cout << "Compiling expression\n";
    HSharpParser::Token& tok1 = std::get<HSharpParser::Token>(args[0]);
    HSharpParser::Token& tok2 = std::get<HSharpParser::Token>(args[1]);
    HSharpParser::Token& tok3 = std::get<HSharpParser::Token>(args[2]);

    if (tok1.type != tok3.type && tok1.type != HSharpParser::TokenType::INT_LIT) {
        std::cout << "Expression could not be compiled!\n";
        exit(1);
    }

    try {
        int lit1 = std::stoi({tok1.str.begin()});
        int lit2 = std::stoi({tok1.str.begin()});
        this->compiler.emit_opcode(HSharpCompiler::Opcode::PUSH_CONST, lit1);
        this->compiler.emit_opcode(HSharpCompiler::Opcode::PUSH_CONST, lit2);
        this->compiler.emit_opcode(HSharpCompiler::Opcode::ADD_BINARY);
    } catch (std::invalid_argument exc) {

    } catch (std::out_of_range exc ) {

    }
};

}

void HSharpParser::Parser::_apply_parser_rules() noexcept {
    // Whole program consists out of statements
    parser.set_start_symbol("expr");

    // It's not one statement - amount of statements is unlimited
    parser.rule("statements")
        .production("statements", "statement", [](auto&& args)->Value{return {};})
        .production("statement", [](auto&& args)->Value{return {};});

    // Listing statement types
    parser.rule("statement")
        .production("var_create", [](auto&& args)->Value {
            std::cout << "Parsed variable\n";
            return {};
        })
        .production("var_assign", [](auto&& args)->Value{return {};});
    
    // Defining statement types
    parser.rule("var_create")
        .production("type", "ident", "=", "string", ";", [](auto&& args)->Value{return {};});
    parser.rule("var_assign")
        .production("ident", "op", "expr", ";", [](auto&& args)->Value{return {};});
    
    std::function<Value(std::vector<Value>)> expression_compiler = compile_expr;

    // Basic statement parts
    parser.rule("type")
        .production("ident", type_to_token);
    parser.rule("expr")
        .production("expr", "+", "expr", compiler.get__compile_expression())
        .production("int", [](auto&& args)->Value{return std::move(args[0]);});
    
    parser.rule("lit")
        .production("bool", [](auto&& args)->Value{return {};})
        .production("string", [](auto&& args)->Value{return {};})
        .production("int", [](auto&& args)->Value{return {};});
        
    parser.rule("op")
        .production("+", [](auto&& args) -> Value { return { }; })
        .production("-", op_to_token)
        .production("*", op_to_token)
        .production("/", op_to_token);
    
}

pog::ParserReport<HSharpParser::Value> HSharpParser::Parser::prepare() {
    return parser.prepare();
}

std::optional<HSharpParser::Value> HSharpParser::Parser::parse(std::ifstream& contents) {
    std::stringstream input{"5 + 4"};
    return parser.parse(input);
}

