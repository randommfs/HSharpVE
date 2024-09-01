#pragma once

// STD
#include <stack>
#include <memory>

// Local
#include <common/arena.hpp>
#include <parser/parser/nodes.hpp>
#include <parser/tokenizer/common-token-readers.hpp>
#include <parser/parser/interfaces/i-token-parser.hpp>


namespace hsharp {

    class StatementParser 
        : public ITokenParser 
        , public NonCopyable
        , public std::enable_shared_from_this<StatementParser> {
    private: struct Private { };
    public:

        std::shared_ptr<StatementParser> create(std::weak_ptr<ParserStorage> storage);
        StatementParser(std::weak_ptr<ParserStorage> storage, Private access);

        // ITokenParser implementation
        virtual WrappedResult process(
            std::vector<Token>::iterator& current, 
            std::vector<Token>::iterator end
        ) override;

    private:
        hsharp::arena_ptr<nodes::builtin::Statement> parseBuiltinFunction(
            std::vector<Token>::iterator& current, 
            std::vector<Token>::iterator end
        );

    private:
        std::weak_ptr<ParserStorage> storage_;

    };

    class TermParser 
        : public ITokenParser 
        , public NonCopyable
        , public std::enable_shared_from_this<StatementParser> {
    private: struct Private { };
    public:

        std::shared_ptr<TermParser> create(std::weak_ptr<ParserStorage> storage);
        TermParser(std::weak_ptr<ParserStorage> storage, Private access);

        // ITokenParser implementation
        virtual WrappedResult process(
            std::vector<Token>::iterator& current, 
            std::vector<Token>::iterator end
        ) override;

    private:
        std::weak_ptr<ParserStorage> storage_;

    };

    class ExpressionParser 
        : public ITokenParser 
        , public NonCopyable
        , public std::enable_shared_from_this<StatementParser> {
    private: struct Private { };
    public:

        std::shared_ptr<ExpressionParser> create(std::weak_ptr<ParserStorage> storage);
        ExpressionParser(std::weak_ptr<ParserStorage> storage, Private access);

        // ITokenParser implementation
        virtual WrappedResult process(
            std::vector<Token>::iterator& current, 
            std::vector<Token>::iterator end
        ) override;

    private:
        std::stack<int> minPrecedence_;
        std::weak_ptr<ParserStorage> storage_;

    };

}