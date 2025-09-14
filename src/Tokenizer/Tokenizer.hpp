#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace HVE {
    enum class TokenType : std::uint8_t {
        /* Keywords */
        IMPORT,
        EXPORT,
        PRIVATE,
        PUBLIC,
        STATIC,
        DYNAMIC,
        FUNCTION_DEF,
        TYPENAME,
        IF,
        ELIF,
        OTHERWISE,
        CYCLE,
        FOR,
        NONETYPE,
        AUTOTYPE,

        /* Characters */
        SEMICOLON = ';',
        COLON = ':',
        OPEN_PARENTHESIS = '(',
        CLOSE_PARENTHESIS = ')',
        OPEN_BRACE = '{',
        CLOSE_BRACE = '}',
        OPEN_BRACKET = '[',
        CLOSE_BRACKET = ']',
        OPEN_ANGLE_BRACKET = '<',
        CLOSE_ANGLE_BRACKET = '>',
        REFERENCE = '&',
        DOT = '.',
        COMMA = ',',
        ELLIPSIS,

        IDENTIFIER,
        INT_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
    };

    struct Token {
        TokenType type;
        std::uint32_t line;
        std::uint32_t offset;
        std::uint32_t length;
        std::uint64_t raw_offset;
        std::string lexeme;
    };

    class Tokenizer {
    public:
        std::vector<Token> Tokenize(std::string source);
    private:
        char Peek(std::uint8_t offset = 1);
        char Consume();
        std::uint64_t GetCurOffset();
        std::string source;
        std::uint64_t cur_offset = 0;
    };
};