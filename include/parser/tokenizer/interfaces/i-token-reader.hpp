#pragma once

// STD
#include <format>
#include <string>
#include <variant>

// Local
#include <parser/tokenizer/tokens.hpp>
#include <ve/exceptions.hpp>

namespace hsharp {

    class ITokenReader {
    public:

        class Error {
        public:

            enum class ETemplate {
                CUSTOM,             // placeholder for custom errors
                UNEXPECTED_EOL,     // in case we want input
                SYMBOL_VIOLATION    // found symbol is not permitted in current context
            };

            // error template strings
            static constexpr char tCustomError[] = "error: {}";
            static constexpr char tSymbolViolation[] = "unexpected symbol found: {}";
            static constexpr char tUnexpectedEOL[] = "unexpected end of line";

            Error(const std::string& what, std::size_t where)
            : description_(std::vformat(tCustomError, std::make_format_args(what)))
            , position_(where)
            {}

            template<typename... WrapperPayload>
            Error(ETemplate error, std::size_t where, const WrapperPayload&... args)
            : description_(std::vformat(templateMatcher(error), std::make_format_args(args...)))
            , position_(where)
            {}

            std::string describe() { return description_ + ", at position: " + std::to_string(position_); }

        private:
            const char* templateMatcher(ETemplate error); 

        private:
            std::string description_;
            std::size_t position_;
        };

        class WrappedResult {
        public:
            // result builders
            template<typename... WrapperPayload>
            static WrappedResult wrapError(Error::ETemplate error, std::size_t where, const WrapperPayload&... args)
            { return WrappedResult(Error(error, where, args...)); }

            static WrappedResult wrapError(const std::string& what, std::size_t where)
            { return WrappedResult(Error(what, where)); }
            static WrappedResult wrapResult(Token token)
            { return WrappedResult(std::move(token)); }
            // result state
            bool isError() { return std::holds_alternative<Error>(result_); }
            Error& getError() { return std::get<Error>(result_); }
            Token& getResult() { return std::get<Token>(result_); }

        private:
            WrappedResult(std::variant<Token, Error> result);

        private:
            std::variant<Token, Error> result_;
        };


        virtual WrappedResult process(std::string::iterator position, std::string::iterator end) const = 0;
        virtual ~ITokenReader() = default;
    };

    inline const char* ITokenReader::Error::templateMatcher(ETemplate error) {
        switch (error) {
            case ETemplate::CUSTOM: return tCustomError;
            case ETemplate::SYMBOL_VIOLATION: return tSymbolViolation;
            case ETemplate::UNEXPECTED_EOL: return tUnexpectedEOL;
            HSHARP_NOT_IMPLEMENTED(hsharp::EExceptionSource::TOKENIZER, "enum value out of range");
        }
    }

}



