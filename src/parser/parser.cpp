#include "pog/parser_report.h"
#include <parser/parser.hpp>
#include <parser/rules/tokenization.hpp>

#include <pog/pog.h>

#define _VISIBILITY_HIDDEN __attribute__((visibility("hidden")))

HSharpParser::Parser::Parser() {
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

    _apply_parser_rules();
}


_VISIBILITY_HIDDEN
void HSharpParser::Parser::_apply_parser_rules() noexcept {
    // Whole program consists out of statements
    parser.set_start_symbol("statements");

    // It's not one statement - amount of statements is unlimited
    parser.rule("statements")
        .production("statements", "statement");

    // Listing statement types
    parser.rule("statement")
        .production("var_create")
        .production("var_assign");
    
    // Defining statement types
    parser.rule("var_create")
        .production("type", "id", "=", "expr", ";");
    parser.rule("var_assign")
        .production("id", "op", "expr", ";");
    
    // Basic statement parts
    parser.rule("type")
        .production("id");
    parser.rule("expr")
        .production("(", "expr", "op", "expr", ")");
    parser.rule("expr")
        .production("expr", "op", "expr");
    parser.rule("op")
        .production("+=")
        .production("-=")
        .production("*=")
        .production("/=");
    
}

pog::ParserReport<HSharpParser::Parser::Value> HSharpParser::Parser::prepare() {
    return parser.prepare();
}

std::optional<HSharpParser::Parser::Value> HSharpParser::Parser::parse(std::istream input) {
    return parser.parse(input);
}