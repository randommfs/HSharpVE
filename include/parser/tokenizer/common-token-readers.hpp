#pragma once

// STD
#include <cstdint>
#include <map>
#include <memory>
#include <unordered_map>

// Local
#include <parser/tokenizer/tokens.hpp>
#include <parser/tokenizer/interfaces/i-token-reader.hpp>


namespace hsharp {

    class NonCopyable {
    public:
        NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };

    class SymbolTokenReader 
        : public ITokenReader
        , public NonCopyable
        , public std::enable_shared_from_this<SymbolTokenReader> {
    
    private: struct Private {};
    public:
        static std::shared_ptr<SymbolTokenReader> create(const std::unordered_map<char, hsharp::EToken>* mappings);
        SymbolTokenReader(const std::unordered_map<char, hsharp::EToken>* mappings, Private access);
        // ITokenReader
        virtual WrappedResult process(std::string::iterator position, std::string::iterator end) const override;

    private:
        const std::unordered_map<char, hsharp::EToken>* symbols_;

    };

    class KeywordTokenReader 
        : public ITokenReader
        , public NonCopyable
        , public std::enable_shared_from_this<SymbolTokenReader> {
    
    private: struct Private {};
    public:

        static std::shared_ptr<KeywordTokenReader> create(const std::map<std::string, hsharp::EToken>* mappings);
        KeywordTokenReader(const std::map<std::string, hsharp::EToken>* mappings, Private access);
        // ITokenReader
        virtual WrappedResult process(std::string::iterator position, std::string::iterator end) const override;
        // methods to distinguish keywords from identifers
        using BoundariesTypeAlias = std::pair<
            std::map<std::string, hsharp::EToken>::const_iterator, 
            std::map<std::string, hsharp::EToken>::const_iterator
        >;
        BoundariesTypeAlias tryMatch(const std::string& match, const BoundariesTypeAlias* hint = nullptr) const;

    private:
        std::size_t findMaxKey(const std::map<std::string, hsharp::EToken>* words);

    private:
        const std::map<std::string, hsharp::EToken>* keywords_;
        const std::size_t maxKeywordSize_;

    };

    class IdentifierTokenReader 
        : public ITokenReader
        , public NonCopyable
        , public std::enable_shared_from_this<SymbolTokenReader> {
    
    private: struct Private {};
    public:

        static std::shared_ptr<IdentifierTokenReader> create(const std::string* allowed, const std::string* firstExcluded);
        IdentifierTokenReader(const std::string* allowed, const std::string* firstExcluded, Private access);
        // ITokenReader
        virtual WrappedResult process(std::string::iterator position, std::string::iterator end) const override;

    private:
        const std::string* allowed_;
        const std::string* firstExcluded_;

    };

    class LiteralTokenReader 
        : public ITokenReader   
        , public NonCopyable
        , public std::enable_shared_from_this<SymbolTokenReader> {
    
    private: struct Private {};
    public:

        static std::shared_ptr<LiteralTokenReader> create();
        LiteralTokenReader(Private access);
        // ITokenReader
        virtual WrappedResult process(std::string::iterator position, std::string::iterator end) const override;

    private:
        WrappedResult processInteger(std::string::iterator position, std::string::iterator end) const;
        WrappedResult processString(std::string::iterator position, std::string::iterator end) const;

    };

}