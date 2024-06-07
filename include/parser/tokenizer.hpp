#pragma once

#include <map>
#include <cstdint>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include <parser/nodes.hpp>
#include <ve/exceptions.hpp>
#include <parser/tokens.hpp>
#include <arena_alloc/arena.hpp>

namespace HSharpParser {

    class Tokenizer {
    public:
        Tokenizer();
        std::vector<Token> tokenize(std::fstream& file);

    private:

        struct LineSpecialization {
            std::string line;
            std::int32_t num;
            std::int32_t pos;
        };
        
        struct ErrorSpecialization {
            HSharpVE::EExceptionReason reason;
            std::string message;
        };

        enum class EBet : std::int_fast8_t {
            IDENTIFIER,
            LITERAL,
            SPECIAL
        };

        void checkStreamFlags(std::fstream& ifs);
        [[noreturn]] void fallback(std::string error, std::optional<LineSpecialization> lineSpec = std::nullopt);

        // formatting
        std::string formatError(LineSpecialization lineSpec, ErrorSpecialization errorSpec);
        std::fstream& getline(std::fstream& ifs, std::string& line);
        void strip(std::string& line, bool stripBothSides = false);

        // reading
        EBet bet(std::string::iterator position, std::string::iterator end);
        char peekChar(std::string::iterator position, std::string::iterator end);
        Token read(std::string::iterator& position, std::string::iterator end, EBet bet);
        Token readIdentifier(std::string::iterator& position, std::string::iterator end);
        Token readLiteral(std::string::iterator& position, std::string::iterator end);
        Token readSpecial(std::string::iterator& position, std::string::iterator end);

        // char checks
        bool isIdentifierChar(char ch);

    private:
        
        struct State {
            std::string line;
            std::int32_t num;
        } state_;
    };

}