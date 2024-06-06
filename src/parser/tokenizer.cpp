#include <vector>
#include <string>

#include <parser/parser.hpp>
#include <main/file.hpp>
#include <ve/exceptions.hpp>

using std::uint32_t;
using HSharpParser::Token;

bool HSharpParser::is_bin_operator(HSharpParser::TokenType ttype) {
    switch(ttype){
        case TOK_PLUS:
        case TOK_MINUS:
        case TOK_MUL_SIGN:
        case TOK_FSLASH:
            return true;
        default:
            return false;
    }
}

std::optional<int> HSharpParser::bin_precedence(HSharpParser::TokenType ttype){
    switch(ttype){
        case TOK_PLUS:
        case TOK_MINUS:
            return 0;
        case TOK_MUL_SIGN:
        case TOK_FSLASH:
            return 1;
        default:
            return {};
    }
}

std::vector<Token> HSharpParser::Tokenizer::tokenize() {
    std::string buf = {};
    std::vector<Token> tokens = {};
    uint32_t line = 1;

    while (peek().has_value()) {
        if (std::isalpha(peek().value())) {
            buf.push_back(consume());
            while (peek().has_value() && std::isalnum(peek().value()))
                buf.push_back(consume());

            if (buf == "exit") {
                tokens.push_back({.line = line, .ttype = TokenType::TOK_EXIT});
                buf.clear();
            } else if (buf == "var") {
                tokens.push_back({.line = line, .ttype = TokenType::TOK_VAR});
                buf.clear();
            } else if (buf == "print") {
                tokens.push_back({.line = line, .ttype = TokenType::TOK_PRINT});
                buf.clear();
            } else if (buf == "input") {
                tokens.push_back({.line = line, .ttype = TokenType::TOK_INPUT});
                buf.clear();
            } else {
                tokens.push_back({.line = line, .ttype = TokenType::TOK_IDENT, .value = buf});
                buf.clear();
            }
        } else if (peek().value() == '"') {
            skip();
            while (peek().has_value() && peek().value() != '"')
                buf.push_back(consume());
            skip();
            tokens.push_back({.line = line, .ttype = TokenType::TOK_STR_LIT, .value = buf});
            buf.clear();
        } else if (std::isdigit(peek().value())) {
            buf.push_back(consume());
            while (peek().has_value() && std::isdigit(peek().value()))
                buf.push_back(consume());
            tokens.push_back({.line = line, .ttype = TokenType::TOK_INT_LIT, .value = buf});
            buf.clear();
        } else if (peek().value() == ';') {
            tokens.push_back({.line = line, .ttype = TokenType::TOK_SEMICOLON});
            skip();
        } else if (peek().value() == '(') {
            tokens.push_back({.line = line, .ttype = TokenType::TOK_PAREN_OPEN});
            skip();
        } else if (peek().value() == ')') {
            tokens.push_back({.line = line, .ttype = TokenType::TOK_PAREN_CLOSE});
            skip();
        } else if (peek().value() == '=') {
            tokens.push_back({.line = line, .ttype = TokenType::TOK_EQUALITY_SIGN});
            skip();
        } else if (peek().value() == '+') {
            tokens.push_back({.line = line, .ttype = TokenType::TOK_PLUS});
            skip();
        } else if (peek().value() == '-') {
            tokens.push_back({.line = line, .ttype = TokenType::TOK_MINUS});
            skip();
        } else if (peek().value() == '/') {
            tokens.push_back({.line = line, .ttype = TokenType::TOK_FSLASH});
            skip();
        } else if (peek().value() == '*') {
            tokens.push_back({.line = line, .ttype = TokenType::TOK_MUL_SIGN});
            skip();
        } else if (peek().value() == '.'){
            tokens.push_back({.line = line, .ttype = TokenType::TOK_DOT});
            skip();
        } else if (peek().value() == '/' && peek(1).has_value() && peek(1).value() == '/') {
            skip(2);
            while (peek().has_value() && peek().value() != '\n')
                skip();
        } else if (peek().value() == '/' && peek(1).has_value() && peek(1).value() == '*') {
            skip(2);
            while (peek().has_value()) {
                if (peek().value() == '*' && peek(1).has_value() && peek(1).value() == '/')
                    break;
                skip();
            }
            if (peek().has_value())
                skip();
            if (peek().has_value())
                skip();
        } else if (peek().value() == '\n'){
            ++line;
            skip();
        } else if (std::isspace(peek().value())) {
            skip();
        } else {
            error(HSharpVE::EExceptionSource::TOKENIZER, HSharpVE::EExceptionReason::UNEXPECTED_TOKEN, "invalid token");
        }
    }

    index = 0;
    return tokens;
}
