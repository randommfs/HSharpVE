#pragma once

// STD
#include <concepts>
#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <istream>
#include <utility>
#include <memory>
#include <optional>
#include <unordered_map>
#include <initializer_list>

// Local
#include <ve/exceptions.hpp>
#include <parser/tokenizer/tokens.hpp>
#include <parser/tokenizer/common-token-readers.hpp>
#include <parser/tokenizer/interfaces/i-token-reader.hpp>

namespace hsharp {

    class Tokenizer {
    public:

        Tokenizer();
        std::vector<Token> tokenize(std::istream& is);

    private:

        enum class ETokenGroup : std::int_fast8_t {
            IDENTIFIER, // var name, function etc...
            LITERAL,    // string or integer constant value
            SYMBOL,     // special punc signs
            KEYWORD     // if, else, etc...
        };

        class TokenReaderStorage {
        public: 

            TokenReaderStorage(std::initializer_list<ETokenGroup> groups) {
                for (const auto& group : groups) {
                    switch (group) {
                        case ETokenGroup::IDENTIFIER:
                            readers_[group] = IdentifierTokenReader::create(
                                &allowedIdentifierSymbols, 
                                &exludedFirstIdentifierSymbols
                            );
                            break;
                        case ETokenGroup::KEYWORD:
                            readers_[group] = KeywordTokenReader::create(&mKeywords);
                            break;
                        case ETokenGroup::LITERAL:
                            readers_[group] = LiteralTokenReader::create();
                            break;
                        case ETokenGroup::SYMBOL:
                            readers_[group] = SymbolTokenReader::create(&mReservedSymbols);
                            break;
                    }
                }
            }

            template<typename Reader>
            std::shared_ptr<Reader> getReaderByGroup(ETokenGroup group) {
                static_assert(std::derived_from<Reader, ITokenReader>, "Reader should be direvied from ITokenReader");
                return dynamic_cast<std::shared_ptr<Reader>>(readers_[group]);
            }

        private:
            std::unordered_map<ETokenGroup, std::shared_ptr<ITokenReader>> readers_;

        };

        constexpr static std::string yieldIdentifierSymbols() {
            std::string allowed;
            for (char curr = 'a'; curr < 'z'; ++curr) {
                allowed.push_back(curr);
            }
            return allowed;
        }

        inline const static std::string allowedIdentifierSymbols = yieldIdentifierSymbols();
        inline const static std::string exludedFirstIdentifierSymbols = "";

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

        inline const static std::map<std::string, EToken> mKeywords = {
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

        [[noreturn]] void fallback(std::string error, std::optional<LineSpecialization> lineSpec = std::nullopt);

        // formatting
        std::string formatError(LineSpecialization lineSpec, ErrorSpecialization errorSpec);
        std::istream& getline(std::istream& ifs, std::string& line);
        void strip(std::string& line, bool stripBothSides = false);

        // reading
        ETokenGroup indetify(std::string::iterator position, std::string::iterator end);
        char peek(std::string::iterator position, std::string::iterator end);

    private:
        
        struct State {
            std::string line;
            std::int32_t num;
        } state_;

        TokenReaderStorage readers_;

    };

}