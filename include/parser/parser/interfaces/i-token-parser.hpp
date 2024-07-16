#pragma once

// STD
#include <vector>

// Local
#include <parser/parser/nodes.hpp>
#include <parser/tokenizer/tokens.hpp>


namespace hsharp {

    template<typename NodeGroup>
    class ITokenParser {
    public:

        class Error {};

        class WrappedResult {
        public:
            static WrappedResult wrapError(const std::string& what)
            { return WrappedResult(Error()); }
            static WrappedResult wrapResult(NodeGroup node)
            { return WrappedResult(std::move(node)); }

            bool isError() { return std::holds_alternative<Error>(result_); }
            Error& getError() { return std::get<Error>(result_); }
            NodeGroup& getResult() { return std::get<NodeGroup>(result_); }

        private:
            WrappedResult(std::variant<NodeGroup, Error> result) : result_(std::move(result)) {}

        private:
            std::variant<NodeGroup, Error> result_;
        };

        virtual WrappedResult process(
            std::vector<Token>::iterator& current, 
            std::vector<Token>::iterator end
        ) = 0;
        virtual ~ITokenParser() = default;

    };

}