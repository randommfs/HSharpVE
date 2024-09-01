// STD
#include <memory>

// Local
#include <parser/parser/token-parsers.hpp>
#include <parser/parser/interfaces/i-token-parser.hpp>

using namespace hsharp;


std::shared_ptr<ExpressionParser> ExpressionParser::create(std::weak_ptr<ParserStorage> storage) {
    return std::make_shared<ExpressionParser>(std::move(storage), Private());
}

std::shared_ptr<TermParser> TermParser::create(std::weak_ptr<ParserStorage> storage) {
    return std::make_shared<TermParser>(std::move(storage), Private());
}

std::shared_ptr<StatementParser> StatementParser::create(std::weak_ptr<ParserStorage> storage) {
    return std::make_shared<StatementParser>(std::move(storage), Private());
}

ExpressionParser::ExpressionParser(std::weak_ptr<ParserStorage> storage, Private access) 
: storage_(std::move(storage))
{}

TermParser::TermParser(std::weak_ptr<ParserStorage> storage, Private access) 
: storage_(std::move(storage))
{}

StatementParser::StatementParser(std::weak_ptr<ParserStorage> storage, Private access) 
: storage_(std::move(storage))
{}

ExpressionParser::WrappedResult ExpressionParser::process(
    std::vector<Token>::iterator& current, 
    std::vector<Token>::iterator end
) {

}

StatementParser::WrappedResult StatementParser::process(
    std::vector<Token>::iterator& current, 
    std::vector<Token>::iterator end
) {
    if (current == end) {
        return WrappedResult::wrapError("Expected token");
    }

    Token first = std::move(*current);
}

hsharp::arena_ptr<nodes::builtin::Statement> StatementParser::parseBuiltinFunction(
    std::vector<Token>::iterator& current, 
    std::vector<Token>::iterator end
) {
    
}