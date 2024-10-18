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
}
