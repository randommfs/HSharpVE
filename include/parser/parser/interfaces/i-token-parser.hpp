#pragma once

// STD
#include <memory>
#include <vector>

// Local
#include <parser/parser/nodes.hpp>
#include <parser/tokenizer/tokens.hpp>


namespace hsharp {

    class ITokenParser {
    public:

        class Error {};

        class WrappedResult {
        public:
            static WrappedResult wrapError(const std::string& what)
            { return WrappedResult(Error()); }
            static WrappedResult wrapResult(nodes::Node node)
            { return WrappedResult(std::move(node)); }

            bool isError() { return std::holds_alternative<Error>(result_); }
            Error& getError() { return std::get<Error>(result_); }
            nodes::Node& getResult() { return std::get<nodes::Node>(result_); }

        private:
            WrappedResult(std::variant<nodes::Node, Error> result) : result_(std::move(result)) {}

        private:
            std::variant<nodes::Node, Error> result_;
        };

        virtual WrappedResult process(
            std::vector<Token>::iterator& current, 
            std::vector<Token>::iterator end
        ) = 0;
        virtual ~ITokenParser() = default;

    };

    enum class EParsingField : std::int_fast8_t {
        STATEMENT = 0, 
        EXPRESION = 1, 
        TERM = 2
    };

    class ParserStorage 
        : std::enable_shared_from_this<ParserStorage> {
    private: struct Private { };
    public:
        std::shared_ptr<ParserStorage> create(std::initializer_list<EParsingField> fields);
        ParserStorage(std::initializer_list<EParsingField> fields, Private access);

        std::weak_ptr<ITokenParser> getParser(EParsingField field); 

    private:
        std::map<EParsingField, std::shared_ptr<EParsingField>> mapper_;

    };

}