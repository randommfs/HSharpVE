#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace HVE {
    enum class TokenType : std::uint8_t {
        /* Keywords */
        IMPORT = 1,
        EXPORT = 2,
        PRIVATE = 3,
        PUBLIC = 4,
        STATIC = 5,
        DYNAMIC = 6,
        FUNCTION_DEF = 7,
        TYPENAME = 8,
        IF = 9,
        ELIF = 10,
        OTHERWISE = 11,
        CYCLE = 12,
        FOR = 13,
        NONETYPE = 14,
        AUTOTYPE = 15,

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
        ELLIPSIS = 200,
        END_OF_FILE = 201,

        IDENTIFIER = 202,
        INT_LITERAL = 203,
        FLOAT_LITERAL = 204,
        STRING_LITERAL = 205,
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
      SourceLocation()
        : line(0)
        , offset(0)
        , raw_offset(0) {}
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
