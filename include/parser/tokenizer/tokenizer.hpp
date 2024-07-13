#pragma once

// STD
#include <vector>
#include <string>
#include <cstdint>
#include <istream>
#include <utility>
#include <optional>
#include <unordered_map>

// Local
#include <ve/exceptions.hpp>
#include <parser/tokenizer/tokens.hpp>
#include <parser/tokenizer/interfaces/i-token-reader.hpp>

namespace hsharp {

    class Tokenizer {
    public:

        Tokenizer();
        std::vector<Token> tokenize(std::istream& is);

    private:

        inline const static std::unordered_map<char, EToken> mReservedSymbols = {
            std::make_pair(';', EToken::STATEMENT_TERMINATOR),
            std::make_pair('+', EToken::ADDITION_SIGN),
            std::make_pair('-', EToken::SUBTRACTION_SIGN),
            std::make_pair('*', EToken::STAR_SIGN),
            std::make_pair('=', EToken::ASSIGNMENT_SIGN),
            std::make_pair('(', EToken::PARENTHESIS_OPEN_SIGN),
            std::make_pair(')', EToken::PARENTHESIS_CLOSE_SIGN),
            std::make_pair('{', EToken::CONTEXT_OPEN_SIGN),
            std::make_pair('}', EToken::CONTEXT_CLOSE_SIGN)
        };

        inline const static std::unordered_map<std::string, EToken> mKeywords = {
            std::make_pair("if", EToken::BRANCHING_START),
            std::make_pair("else", EToken::BRANCHING_ALT),
            std::make_pair("elif", EToken::BRANCHING_LIMITED_ALT)
        };

        struct LineSpecialization {
            std::string line;
            std::int32_t num;
            std::int32_t pos;
        };
        
        struct ErrorSpecialization {
            hsharp::EExceptionReason reason;
            std::string message;
        };

        enum class ETokenGroup : std::int_fast8_t {
            IDENTIFIER, // var name, function etc...
            LITERAL,    // string or integer constant value
            SYMBOL,     // special punc signs
            KEYWORD     // if, else, etc...
        };

        [[noreturn]] void fallback(std::string error, std::optional<LineSpecialization> lineSpec = std::nullopt);

        // formatting
        std::string formatError(LineSpecialization lineSpec, ErrorSpecialization errorSpec);
        std::istream& getline(std::istream& ifs, std::string& line);
        void strip(std::string& line, bool stripBothSides = false);

        // reading
        ETokenGroup indetify(std::string::iterator position, std::string::iterator end);
        char peek(std::string::iterator position, std::string::iterator end);
        Token read(std::string::iterator& position, std::string::iterator end, ETokenGroup bet);
        Token readIdentifier(std::string::iterator& position, std::string::iterator end);
        Token readLiteral(std::string::iterator& position, std::string::iterator end);
        Token readSpecial(std::string::iterator& position, std::string::iterator end);

        // char checks
        bool isIdentifierChar(char ch, bool isFirst = false);

    private:
        
        struct State {
            std::string line;
            std::int32_t num;
        } state_;
    };

}