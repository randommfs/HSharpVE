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
        PLUS = '+',
        MINUS = '-',
        STAR = '*',
        SLASH = '/',
        BACKSLASH = '\\',
        DOT = '.',
        COMMA = ',',
        SINGLE_QUOTE = '\'',
        DOUBLE_QUOTE = '"',
        ELLIPSIS,
        END_OF_FILE,

        IDENTIFIER,
        INT_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
    };

    enum class TokenizerState {
      START,
      END_OF_FILE,
      IDENT,
      STRING_LIT,
      INT_LIT,
      FLOAT_LIT
    };

    struct SourceLocation {
    public:
      SourceLocation(std::uint32_t _line, std::uint32_t _offset, std::uint64_t _raw_offset)
        : line(_line)
        , offset(_offset)
        , raw_offset(_raw_offset) {}
      std::uint32_t GetLine();
      std::uint32_t GetOffset();
      std::uint64_t GetRawOffset();

    private:
      std::uint32_t line;
      std::uint32_t offset;
      std::uint64_t raw_offset;
    };

    struct Token {
        TokenType type;
        SourceLocation source_loc;
        std::string lexeme;
    };

    class Tokenizer {
    public:
        Tokenizer(std::string& str) : source(str) {}
        std::vector<Token> Tokenize();
    private:
        Token GetNextToken();
        Token CreateToken(TokenType type, std::string lexeme);
        SourceLocation CreateSourceLocation();
        bool SkipWhitespace();
        bool SkipComments();

        char Peek(std::uint8_t offset = 1);
        void Advance();
        std::uint64_t GetCurOffset();

        std::string& source;
        std::uint64_t cur_raw_offset = 0;
        std::uint32_t cur_line = 1;
        std::uint32_t cur_offset = 0;
    };
};
