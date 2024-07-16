// STD
#include <memory>
#include <cctype>
#include <utility>
#include <iterator>
#include <algorithm>

// Local
#include <parser/tokenizer/tokens.hpp>
#include <parser/tokenizer/common-token-readers.hpp>
#include <parser/tokenizer/interfaces/i-token-reader.hpp>

using namespace hsharp;


std::shared_ptr<SymbolTokenReader> SymbolTokenReader::create(const std::unordered_map<char, hsharp::EToken>* mappings) {
    return std::make_shared<SymbolTokenReader>(mappings, Private());
}

std::shared_ptr<KeywordTokenReader> KeywordTokenReader::create(const std::map<std::string, hsharp::EToken>* mappings) {
    return std::make_shared<KeywordTokenReader>(mappings, Private());
}

std::shared_ptr<IdentifierTokenReader> IdentifierTokenReader::create(const std::string* allowed, const std::string* firstExcluded) {
    return std::make_shared<IdentifierTokenReader>(allowed, firstExcluded, Private());
}

std::shared_ptr<LiteralTokenReader> LiteralTokenReader::create() {
    return std::make_shared<LiteralTokenReader>(Private());
}

SymbolTokenReader::SymbolTokenReader(
    const std::unordered_map<char, hsharp::EToken>* mappings, 
    Private access)
    : symbols_(mappings)
{}

KeywordTokenReader::KeywordTokenReader(
    const std::map<std::string, hsharp::EToken>* mappings, 
    Private access)
    : keywords_(mappings)
    , maxKeywordSize_(findMaxKey(mappings))
{}

IdentifierTokenReader::IdentifierTokenReader(
    const std::string* allowed,
    const std::string* firstExcluded,
    Private access)
    : allowed_(allowed)
    , firstExcluded_(firstExcluded)
{}

LiteralTokenReader::LiteralTokenReader(Private access) {}

ITokenReader::WrappedResult SymbolTokenReader::process(std::string::iterator position, std::string::iterator end) const {
    if (position != end) {
        char symbol = *position;
        if (auto match = symbols_->find(symbol); match != symbols_->end()) {
            auto& node = *match;
            std::string payload {node.first};
            return WrappedResult::wrapResult(makeToken(node.second, payload));
        }
        return WrappedResult::wrapError(Error::ETemplate::SYMBOL_VIOLATION, 0, symbol);
    }
    return WrappedResult::wrapError(Error::ETemplate::UNEXPECTED_EOL, 0);
}

ITokenReader::WrappedResult KeywordTokenReader::process(std::string::iterator position, std::string::iterator end) const {
    std::string keyword;
    while (true) {
        if (position == end) {
            return WrappedResult::wrapError(Error::ETemplate::UNEXPECTED_EOL, keyword.size());
        }

        keyword.push_back(*position);
        std::advance(position, 1);

        if (keyword.size() > maxKeywordSize_) {
            return WrappedResult::wrapError("Keyword length exceeding max, probably error in matcher?", keyword.size());
        }
        if (auto match = keywords_->find(keyword); match != keywords_->end()) {
            return WrappedResult::wrapResult(makeToken(match->second, match->first));
        }
    }
}

KeywordTokenReader::BoundariesTypeAlias KeywordTokenReader::tryMatch(const std::string& match, const BoundariesTypeAlias* hint) const {
    using ConstKeywordsIter = std::map<std::string, EToken>::const_iterator;
    ConstKeywordsIter left = keywords_->begin();
    ConstKeywordsIter right = keywords_->end();

    if (hint) {
        left = hint->first;
        right = hint->second;
    }

    ConstKeywordsIter new_left = keywords_->end();
    ConstKeywordsIter new_right = keywords_->end();

    bool found = false;
    for (auto i = left; i != right; ++i) {
        if (!found && i->first.starts_with(match)) {
            new_left = i;
            found = true;
        }
        if (found && !i->first.starts_with(match)) {
            new_right = i;
            break;
        }
    }

    return std::make_pair(new_left, new_right);
}

std::size_t KeywordTokenReader::findMaxKey(const std::map<std::string, hsharp::EToken>* words) {
    std::size_t max = 0;
    for (const auto& mapping : *words) {
        max = std::max(mapping.first.size(), max);
    }

    return max;
}

IdentifierTokenReader::WrappedResult IdentifierTokenReader::process(std::string::iterator position, std::string::iterator end) const {
    // read first
    std::string identifier;

    if (position == end) {
        return WrappedResult::wrapError(Error::ETemplate::UNEXPECTED_EOL, 0);
    }

    char first = *position;
    if (allowed_->contains(first) && !firstExcluded_->contains(first)) {
        identifier.push_back(first);
    } else {
        return WrappedResult::wrapError(Error::ETemplate::SYMBOL_VIOLATION, 0, first);
    }

    while (position != end && allowed_->contains(*position)) {
        identifier.push_back(*position);
        std::advance(position, 1);
    }

    return WrappedResult::wrapResult(makeToken(EToken::IDENTIFIER, identifier));
}

LiteralTokenReader::WrappedResult LiteralTokenReader::process(std::string::iterator position, std::string::iterator end) const {
    if (position == end) {
        return WrappedResult::wrapError(Error::ETemplate::UNEXPECTED_EOL, 0);
    }

    char first = *position;
    if (std::isdigit(first)) {
        return processInteger(position, end);
    }
    return processString(position, end);
}

LiteralTokenReader::WrappedResult LiteralTokenReader::processInteger(std::string::iterator position, std::string::iterator end) const {
    std::string literal;
    while (position != end && std::isdigit(*position)) {
        literal.push_back(*position);
        std::advance(position, 1);
    }

    return WrappedResult::wrapResult(makeToken(EToken::INTEGER_LITERAL, literal));
}

LiteralTokenReader::WrappedResult LiteralTokenReader::processString(std::string::iterator position, std::string::iterator end) const {
    std::string literal;
    char first = *position;

    if (first != '\"') {
        return WrappedResult::wrapError(Error::ETemplate::SYMBOL_VIOLATION, 0, first);
    }
    literal.push_back(first);

    while (position != end && *position != '\"') {
        literal.push_back(*position);
        std::advance(position, 1);
    }

    return WrappedResult::wrapResult(makeToken(EToken::STRING_LITERAL, literal));
}