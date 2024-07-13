#include <cctype>
#include <cstring>
#include <format>
#include <iterator>
#include <string>
#include <vector>
#include <utility>

#include <parser/tokenizer/tokens.hpp>
#include <parser/tokenizer/tokenizer.hpp>
#include <ve/exceptions.hpp>

using namespace HSharpParser;


std::vector<Token> Tokenizer::tokenize(std::istream& is) {
    state_.num = 0;
    state_.line.clear();
    std::vector<Token> tokens;

    while (getline(is, state_.line)) {
        auto position = state_.line.begin();
        while (position != state_.line.end()) {
            ETokenGroup guess = indetify(position, state_.line.end());
            Token token = read(position, state_.line.end(), guess);
            // post-processing?
            // probably work for parser
            ++state_.num;
        }
    }

    return tokens;
}

Tokenizer::ETokenGroup Tokenizer::indetify(std::string::iterator position, std::string::iterator end) {
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
    std::string::iterator window = position;
    std::string match {first};
    while (true) {
        if (const auto found = std::find_if(mKeywords.begin(), mKeywords.end(), [match](const auto& current) -> bool {
                return current.first.starts_with(match);
        }); found != mKeywords.end()) {
            if (!std::strcmp(found->first.c_str(), match.c_str())) {
                return ETokenGroup::KEYWORD;
            }
            std::advance(window, 1);
            if (window == end) {
                return ETokenGroup::IDENTIFIER;
            }
        } else {
            break;
        }
    }
    return ETokenGroup::IDENTIFIER;
}

Token Tokenizer::read(std::string::iterator& position, std::string::iterator end, ETokenGroup bet) {
    switch (bet) {
        case ETokenGroup::IDENTIFIER: return readLiteral(position, end);
        case ETokenGroup::LITERAL: return readLiteral(position, end);
        case ETokenGroup::SPECIAL: return readSpecial(position, end);
        default: HSHARP_NOT_IMPLEMENTED(HSharpVE::EExceptionSource::TOKENIZER, "enum value out of range");
    }
}

Token Tokenizer::readIdentifier(std::string::iterator& position, std::string::iterator end) {
    std::string identifier;

    if (char head = peek(position, end); isIdentifierChar(head, true)) {
        identifier.push_back(head);
    }

    while (position != end && isIdentifierChar(*position)) {
        identifier.push_back(*(position++));
    }

    if (identifier.empty()) {
        fallback("expected identifier", LineSpecialization(state_.line, state_.num, position - state_.line.begin()));
    }

    return makeToken(EToken::IDENTIFIER, identifier);
}

char Tokenizer::peek(std::string::iterator position, std::string::iterator end) {
    if (position != end) {
        return *position;
    }
    fallback("unexpected EOL", LineSpecialization(state_.line, state_.num, position - state_.line.begin()));
}

Token Tokenizer::readLiteral(std::string::iterator& position, std::string::iterator end) {
    std::string literal;
    EToken type;

    if (peek(position, end) == '\"') {
        // read string
        type = EToken::STRING_LITERAL;
        while (position != end && *position != '\"') {
            literal.push_back(*(position++));
        }
        literal.push_back(*(position++));
    } else {
        type = EToken::INTEGER_LITERAL;
        while (position != end && std::isdigit(*position)) {
            literal.push_back(*(position++));
        }
    }

    if (literal.empty()) {
        fallback("expected literal", LineSpecialization(state_.line, state_.num, position - state_.line.begin()));
    }

    return makeToken(type, literal);
}

Token Tokenizer::readSpecial(std::string::iterator& position, std::string::iterator end) {
    if (position != end) {
        auto type = mReservedSymbols.find(*position);
        if (type == mReservedSymbols.end()) {
            fallback(std::format("{}", *position));
        } else {
            std::string payload; payload.push_back(*position);
            return makeToken(type->second, payload);
        }
    } else {
        fallback("unexpected EOL", LineSpecialization(state_.line, state_.num, position - state_.line.begin()));
    }
}

std::string HSharpParser::toString(EToken token) {
    switch (token) {
        HSHARP_ENUM_CASE(HSharpParser::EToken, BUILTIN_FUNCTION);
        HSHARP_ENUM_CASE(HSharpParser::EToken, IDENTIFIER);
        HSHARP_ENUM_CASE(HSharpParser::EToken, INTEGER_LITERAL);
        HSHARP_ENUM_CASE(HSharpParser::EToken, STRING_LITERAL);
        HSHARP_ENUM_CASE(HSharpParser::EToken, STATEMENT_TERMINATOR);
        HSHARP_ENUM_CASE(HSharpParser::EToken, ADDITION_SIGN);
        HSHARP_ENUM_CASE(HSharpParser::EToken, SUBTRACTION_SIGN);
        HSHARP_ENUM_CASE(HSharpParser::EToken, STAR_SIGN);
        HSHARP_ENUM_CASE(HSharpParser::EToken, ASSIGNMENT_SIGN);
        HSHARP_ENUM_CASE(HSharpParser::EToken, PARENTHESIS_OPEN_SIGN);
        HSHARP_ENUM_CASE(HSharpParser::EToken, PARENTHESIS_CLOSE_SIGN);
        HSHARP_ENUM_CASE(HSharpParser::EToken, CONTEXT_OPEN_SIGN);
        HSHARP_ENUM_CASE(HSharpParser::EToken, CONTEXT_CLOSE_SIGN);
        HSHARP_ENUM_CASE(HSharpParser::EToken, OWNING_SIGN);
        HSHARP_ENUM_CASE(HSharpParser::EToken, CASCADE_BRANCHING_OPERATOR);
        default: HSHARP_NOT_IMPLEMENTED(HSharpVE::EExceptionSource::UTILITY, "enum value out of range");
    }
}

Token HSharpParser::makeToken(EToken type, std::string payload) {
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

    auto reason = HSharpVE::toString(errorSpec.reason);
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

bool Tokenizer::isIdentifierChar(char ch, bool /* isFirst */) {
    return std::isalpha(ch); // might add something else here
}