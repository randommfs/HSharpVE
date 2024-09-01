#include "parser/tokenizer/common-token-readers.hpp"
#include "parser/tokenizer/interfaces/i-token-reader.hpp"
#include <cctype>
#include <cstring>
#include <format>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include <parser/tokenizer/tokens.hpp>
#include <parser/tokenizer/tokenizer.hpp>
#include <ve/exceptions.hpp>

using namespace hsharp;


std::vector<Token> Tokenizer::tokenize(std::istream& is) {
    state_.num = 0;
    state_.line.clear();
    std::vector<Token> tokens;

    while (getline(is, state_.line)) {
        auto position = state_.line.begin();
        auto end = state_.line.end();
        while (position != state_.line.end()) {
            ETokenGroup guess = identify(position, end);
            std::shared_ptr<const ITokenReader> reader = readers_.getReaderByGroup<ITokenReader>(guess);
            ITokenReader::WrappedResult result = reader->process(position, end);

            if (result.isError()) {
                fallback(result.getError().describe());
            } else {
                tokens.push_back(result.getResult());
            }
            // post-processing?
            // probably work for parser
            ++state_.num;
        }
    }

    return tokens;
}

Tokenizer::ETokenGroup Tokenizer::identify(std::string::iterator position, std::string::iterator end) {
    char first = peek(position, end);
    // simplest case
    if (std::isdigit(first) || first == '\"') {
        return ETokenGroup::LITERAL;
    }
    // harder...
    for (const auto& mSymbol : mReservedSymbols) {
        if (mSymbol.first == first) {
            return ETokenGroup::SYMBOL;
        }
    }
    // now check all keywords
    std::string::iterator window = std::next(position, 1);
    std::string match {first};

    std::shared_ptr<const KeywordTokenReader> keywordsReader = readers_.getReaderByGroup<hsharp::KeywordTokenReader>(ETokenGroup::KEYWORD);
    auto boundaries = keywordsReader->tryMatch(match);

    while (window != end && boundaries.first != boundaries.second) {
        match.push_back(*window);
        boundaries = keywordsReader->tryMatch(match, &boundaries);

        if (std::distance(boundaries.first, boundaries.second) == 1) {
            // exact match?
            if (boundaries.first->first == match) {
                return ETokenGroup::KEYWORD;
            }
        }
        std::advance(window, 1);
    }

    return ETokenGroup::IDENTIFIER;
}

char Tokenizer::peek(std::string::iterator position, std::string::iterator end) {
    if (position != end) {
        return *position;
    }
    fallback("unexpected EOL", LineSpecialization(state_.line, state_.num, position - state_.line.begin()));
}

std::string hsharp::toString(EToken token) {
    switch (token) {
        HSHARP_ENUM_CASE(hsharp::EToken, BUILTIN_FUNCTION);
        HSHARP_ENUM_CASE(hsharp::EToken, IDENTIFIER);
        HSHARP_ENUM_CASE(hsharp::EToken, INTEGER_LITERAL);
        HSHARP_ENUM_CASE(hsharp::EToken, STRING_LITERAL);
        HSHARP_ENUM_CASE(hsharp::EToken, STATEMENT_TERMINATOR);
        HSHARP_ENUM_CASE(hsharp::EToken, ADDITION_SIGN);
        HSHARP_ENUM_CASE(hsharp::EToken, SUBTRACTION_SIGN);
        HSHARP_ENUM_CASE(hsharp::EToken, STAR_SIGN);
        HSHARP_ENUM_CASE(hsharp::EToken, ASSIGNMENT_SIGN);
        HSHARP_ENUM_CASE(hsharp::EToken, PARENTHESIS_OPEN_SIGN);
        HSHARP_ENUM_CASE(hsharp::EToken, PARENTHESIS_CLOSE_SIGN);
        HSHARP_ENUM_CASE(hsharp::EToken, CONTEXT_OPEN_SIGN);
        HSHARP_ENUM_CASE(hsharp::EToken, CONTEXT_CLOSE_SIGN);
        HSHARP_ENUM_CASE(hsharp::EToken, OWNING_SIGN);
        HSHARP_ENUM_CASE(hsharp::EToken, BRANCHING_START);
        HSHARP_ENUM_CASE(hsharp::EToken, BRANCHING_ALT);
        HSHARP_ENUM_CASE(hsharp::EToken, BRANCHING_LIMITED_ALT);
        default: HSHARP_NOT_IMPLEMENTED(hsharp::EExceptionSource::UTILITY, "enum value out of range");
    }
}

Token hsharp::makeToken(EToken type, std::string payload) {
    Token token;
    token.payload = std::move(payload);
    token.type = type;
    return token;
}

std::string Tokenizer::formatError(LineSpecialization lineSpec, ErrorSpecialization errorSpec) {
    static std::string formatBase = "\n{}\n";
    std::string formatted = std::vformat(formatBase, std::make_format_args(lineSpec.line));
    
    for (int i = 0; i < std::max(0, lineSpec.num - 5); ++i) {
        formatted.push_back(' ');
    }

    for (int i = std::max(lineSpec.num - 5, 0); i < std::min<int>(lineSpec.line.size(), lineSpec.num); ++i) {
        formatted.push_back('^');
    }

    for (int i = std::min<int>(lineSpec.line.size(), lineSpec.num); i < lineSpec.line.size(); ++i) {
        formatted.push_back(' ');
    }

    auto reason = hsharp::toString(errorSpec.reason);
    formatted += std::vformat("line {}: {}: {}", std::make_format_args(lineSpec.line, reason, errorSpec.message));
    return formatted;
}

std::istream& Tokenizer::getline(std::istream& is, std::string& line) {
    std::getline(is, line);

    while (is && line.back() == '\\') {
        std::string unformatted;
        if (!std::getline(is, unformatted)) {
            fallback("error reading line", LineSpecialization(state_.line, state_.num, 0));
        }

        strip(unformatted);
        line.pop_back();
        line.append(unformatted);
    }

    return is;
}