#pragma once

#include "pog/precedence.h"
#include <string_view>
#include <functional>
#include <variant>

#include <parser/parser.hpp>

namespace HSharpParser {
    using TokenValue = Value;

    template<typename ValueT>
    struct TokenizerRule {
        std::string regex_;
        std::string symbol_;
        std::string description_;
        std::function<ValueT(std::string_view)> action_;
        bool fullword_;
        int precedence_;
        pog::Associativity associativity_;

        TokenizerRule<ValueT>() : fullword_(false) { }
        
        constexpr TokenizerRule<ValueT> regex(std::string regex__) { regex_ = regex__; return *this; }
        constexpr TokenizerRule<ValueT> symbol(std::string symbol__) { symbol_ = symbol__; return *this; }
        constexpr TokenizerRule<ValueT> description(std::string description__) { description_ = description__; return *this; }
        constexpr TokenizerRule<ValueT> action(std::function<ValueT(std::string_view)> action__) { action_ = action__; return *this; }
        constexpr TokenizerRule<ValueT> fullword(bool fullword__) { fullword_ = fullword__; return *this; }
        constexpr TokenizerRule<ValueT> precedence(int prec, pog::Associativity assoc) { precedence_ = prec; associativity_ = assoc; return *this; }
    };

    TokenValue _parse_bool(std::string_view str) { return {Token{TokenType::BOOL_LIT, str}}; }
    TokenValue _parse_int(std::string_view str) { return {Token{TokenType::INT_LIT, str}}; }
    TokenValue _parse_str(std::string_view str) { return {Token{TokenType::STR_LIT, str}}; }
    TokenValue _parse_ident(std::string_view str) { return {Token{TokenType::IDENT, str}}; }

    using Rule = TokenizerRule<TokenValue>;

    static inline const TokenizerRule<TokenValue> rules[] = {
        Rule().regex("\\s+"),
        Rule().regex("=").symbol("="),
        //Rule().regex(R"(\+)").symbol("+").precedence(1, pog::Associativity::Left),
        Rule().regex("-").symbol("-"),
        Rule().regex("\\*").symbol("*"),
        Rule().regex("/").symbol("/"),
        Rule().regex(";").symbol(";"),
        Rule().regex("(true|false)").symbol("bool").fullword(true).action(_parse_bool),
        Rule().regex("[0-9]+").symbol("int").action(_parse_int),
        Rule().regex(R"("[^"]*")").symbol("string").action(_parse_str),
        Rule().regex("[a-zA-Z_][a-zA-Z0-9_]*").symbol("ident").action(_parse_ident)
    };
}
