#include <cctype>
#include <cstdint>
#include <cstring>
#include <format>
#include <utility>
#include <vector>
#include <string>

#include <parser/tokens.hpp>
#include <parser/tokenizer.hpp>
#include <ve/exceptions.hpp>

using namespace HSharpParser;


void Tokenizer::checkStreamFlags(std::fstream& ifs) {
    if (!ifs.binary || !ifs.in || !ifs) {
        fallback("ICE");
    }
}

std::vector<Token> Tokenizer::tokenize(std::fstream& file) {
    checkStreamFlags(file);

    state_.num = 0;
    state_.line.clear();
    std::vector<Token> tokens;

    while (getline(file, state_.line)) {
        auto position = state_.line.begin();
        while (position != state_.line.end()) {
            EBet guess = bet(position, state_.line.end());
            Token token = read(position, state_.line.end(), guess);
            // post-processing?
            ++state_.num;
        }
    }

    return tokens;
}

Tokenizer::EBet Tokenizer::bet(std::string::iterator position, std::string::iterator end) {
    char first = peekChar(position, end);
    if (std::isalpha(first)) {
        return EBet::IDENTIFIER;
    }
    if (std::isdigit(first) || first == '\"') {
        return EBet::LITERAL;
    }
    return EBet::SPECIAL;
}

Token Tokenizer::read(std::string::iterator& position, std::string::iterator end, EBet bet) {
    switch (bet) {
        case EBet::IDENTIFIER: return readLiteral(position, end);
        case EBet::LITERAL: return readLiteral(position, end);
        case EBet::SPECIAL: return readSpecial(position, end);
        default: HSHARP_NOT_IMPLEMENTED(HSharpVE::EExceptionSource::TOKENIZER, "enum value out of range");
    }
}

Token Tokenizer::readIdentifier(std::string::iterator& position, std::string::iterator end) {
    std::string identifier;
    while (position != end && isIdentifierChar(*position)) {
        identifier.push_back(*(position++));
    }

    if (identifier.empty()) {
        fallback("expected identifier", LineSpecialization(state_.line, state_.num, position - state_.line.begin()));
    }

    return makeToken(EToken::IDENTIFIER, identifier);
}

char Tokenizer::peekChar(std::string::iterator position, std::string::iterator end) {
    if (position != end) {
        return *position;
    }
    fallback("unexpected EOL", LineSpecialization(state_.line, state_.num, position - state_.line.begin()));
}

Token Tokenizer::readLiteral(std::string::iterator& position, std::string::iterator end) {
    std::string literal;
    EToken type;

    if (peekChar(position, end) == '\"') {
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
    static std::map<char, EToken> typings = {
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

    if (position != end) {
        auto type = typings.find(*position);
        if (type == typings.end()) {
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

std::fstream& Tokenizer::getline(std::fstream& ifs, std::string& line) {
    std::getline(ifs, line);

    again:
    if (!ifs) {
        fallback("error reading line", LineSpecialization(state_.line, state_.num, 0));
    }

    if (strip(line); !line.empty() && line.back() == '\\') {
        std::string temp;
        std::getline(ifs, temp);
        line.pop_back();
        line.append(temp);
        goto again;
    }

    return ifs;
}

bool Tokenizer::isIdentifierChar(char ch) {
    return std::isalpha(ch); // might add smth else here
}