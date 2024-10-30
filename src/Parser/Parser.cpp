#include "pog/html_report.h"
#include <iostream>

#include <Parser/Parser.hpp>
#include <Parser/Rules/Tokenization.hpp>
#include <Compiler/Compiler_NoOptimizations.hpp>

#include <mapbox/eternal.hpp>
#include <pog/pog.h>
#include <type_traits>

HSharpParser::Parser::Parser(HSharpCompiler::ICompiler* compiler) : compiler(compiler) {
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
};

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
        .production("type", "ident", "=", "string", ";", [](auto&& args)->Value{
            auto& tok1 = std::get<Token>(args[0]);
            auto& tok2 = std::get<Token>(args[1]);
            auto& tok3 = std::get<Token>(args[3]);

            std::cout << tok1.str << tok2.str << "=" << tok3.str << ";\n";
            return {};
        });
    parser.rule("var_assign")
        .production("ident", "op", "expr", ";", [](auto&& args)->Value{return {};});
    
    // Basic statement parts
    parser.rule("type")
        .production("ident", compiler->get__parse_ident());
    parser.rule("expr")
        .production("expr", "op", "expr", compiler->get__compile_expression())
        .production("(", "expr", ")")
        .production("int", compiler->get__parse_literal());
        
    parser.rule("op")
        .production("+", compiler->get__parse_operator())
        .production("-", compiler->get__parse_operator())
        .production("*", compiler->get__parse_operator())
        .production("/", compiler->get__parse_operator());
    
    
}

pog::ParserReport<HSharpParser::Value> HSharpParser::Parser::prepare() {
    auto rep = parser.prepare();
    pog::HtmlReport report(parser);
    report.save("report.html");
    return rep;
}

std::optional<HSharpParser::Value> HSharpParser::Parser::parse(std::string contents) {
    return parser.parse(contents);
}

