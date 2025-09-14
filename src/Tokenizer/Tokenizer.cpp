#include "Tokenizer.hpp"

std::vector<HVE::Token> HVE::Tokenizer::Tokenize(std::string source) {
    std::vector<Token> tokens;
    std::uint32_t line = 1;
    std::int32_t offset = 0;
    std::string cur_tok;
    TokenType guess;
    while (char ch = Peek()) {
        switch (ch) {
            case ' ':
                Consume();
                continue;
            case '\n':
                Consume();
                ++line;
                offset = 0;
            case '.':
                if (guess == TokenType::INT_LITERAL) {
                    guess = TokenType::FLOAT_LITERAL;
                    cur_tok.push_back(Consume());
                    continue;
                }
            case ';':
            case ':':
            case '(':
            case ')':
            case '{':
            case '}':
            case '[':
            case ']':
            case '<':
            case '>':

            case ',':
            case '&':
                tokens.push_back(Token { static_cast<TokenType>(Consume()), line, static_cast<std::uint32_t>(offset), 1, i, "" });
                break;
            default:
                if (isalpha(ch)) {
                    cur_tok.push_back(Consume());
                    continue;
                } else if (isdigit(ch)) {
                    cur_tok.push_back(Consume());
                    guess = TokenType::INT_LITERAL;
                }
        }
    }
    return tokens;
}

char HVE::Tokenizer::Peek(std::uint8_t offset) {
    if (cur_offset + offset - 1 >= source.length()) {
        return 0;
    }
    return source[cur_offset + offset];
}

char HVE::Tokenizer::Consume() {
    return source[cur_offset++];
}

std::uint64_t HVE::Tokenizer::GetCurOffset() {
    return cur_offset;
}