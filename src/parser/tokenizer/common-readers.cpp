#include "parser/tokenizer/interfaces/i-token-reader.hpp"
#include "parser/tokenizer/tokens.hpp"
#include <memory>
#include <parser/tokenizer/common-token-readers.hpp>

using namespace hsharp;

std::shared_ptr<SymbolTokenReader> SymbolTokenReader::create(const std::unordered_map<char, hsharp::EToken>* mappings) {
    return std::make_shared<SymbolTokenReader>(mappings, Private());
}

std::shared_ptr<KeywordTokenReader> KeywordTokenReader::create(const std::unordered_map<std::string, hsharp::EToken>* mappings) {
    return std::make_shared<KeywordTokenReader>(mappings, Private());
}

std::shared_ptr<IdentifierTokenReader> IdentifierTokenReader::create(const std::string* allowed) {
    return std::make_shared<IdentifierTokenReader>(allowed, Private());
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
    const std::unordered_map<std::string, hsharp::EToken>* mappings, 
    Private access)
    : keywords_(mappings)
{}

IdentifierTokenReader::IdentifierTokenReader(
    const std::string* allowed, 
    Private access)
    : allowed_(allowed)
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
