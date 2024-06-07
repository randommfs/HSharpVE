#include <optional>
#include <iostream>
#include <functional>

#include <parser/parser.hpp>
#include <ve/exceptions.hpp>

static constexpr char parser_fail_msg[] = "Parser failed to parse expression.\n";

std::optional<HSharpParser::NodeBinExpr *> HSharpParser::Parser::parse_bin_expr() {
    std::string msg{};
    HSharpVE::EExceptionReason exc_type;
    std::optional<NodeExpression*> lhs;
    NodeBinExpr* bin_expr;
    if (!(lhs = parse_expression())) {
        msg.append(parser_fail_msg);
        exc_type = HSharpVE::EExceptionReason::UNEXPECTED_TOKEN;
        //goto error;
    }

    bin_expr = allocator.alloc<NodeBinExpr>();
    if (!peek().has_value()){
        msg.append(parser_fail_msg);
        exc_type = HSharpVE::EExceptionReason::EARLY_EOF;
        //goto error;
    }
    TokenType ttype = peek()->ttype;
    switch (ttype){
        case TOK_PLUS:{
            auto bin_expr_add = allocator.alloc<NodeBinExprAdd>();
            bin_expr_add->lhs = lhs.value();
            skip();
            bin_expr_add->rhs = parse_expression().value();
            bin_expr->var = bin_expr_add;
            return bin_expr;
        }
        case TOK_MINUS:{
            auto bin_expr_sub = allocator.alloc<NodeBinExprSub>();
            bin_expr_sub->lhs = lhs.value();
            skip();
            bin_expr_sub->rhs = parse_expression().value();
            bin_expr->var = bin_expr_sub;
            return bin_expr;
        }
        case TOK_MUL_SIGN:{
            auto bin_expr_mul = allocator.alloc<NodeBinExprMul>();
            bin_expr_mul->lhs = lhs.value();
            skip();
            bin_expr_mul->rhs = parse_expression().value();
            bin_expr->var = bin_expr_mul;
            return bin_expr;
        }
        case TOK_FSLASH:{
            auto bin_expr_div = allocator.alloc<NodeBinExprDiv>();
            bin_expr_div->lhs = lhs.value();
            skip();
            bin_expr_div->rhs = parse_expression().value();
            bin_expr->var = bin_expr_div;
            return bin_expr;
        }
        default:
            msg.append(parser_fail_msg);
            exc_type = HSharpVE::EExceptionReason::UNEXPECTED_TOKEN;
            goto error;
    }

    error:
    error(HSharpVE::EExceptionSource::TOKENIZER, exc_type, msg);
}

std::optional<HSharpParser::NodeTerm*> HSharpParser::Parser::parse_term() {
    TokenType type = peek()->ttype;
    switch (type){
        case TOK_INT_LIT:{
            std::optional<Token> int_lit = consume();
            auto term_int_lit = allocator.alloc<NodeTermIntLit>();
            auto expr = allocator.alloc<NodeTerm>();
            term_int_lit->int_lit = int_lit.value();
            expr->term = term_int_lit;
            expr->line = int_lit.value().line;
            return expr;
        }
        case TOK_IDENT:{
            std::optional<Token> ident = consume();
            auto term_ident = allocator.alloc<NodeTermIdent>();
            auto expr = allocator.alloc<NodeTerm>();
            term_ident->ident = ident.value();
            expr->term = term_ident;
            expr->line = ident.value().line;
            return expr;
        }
        default:
            return {};
    }
}


std::optional<HSharpParser::NodeExpression *> HSharpParser::Parser::parse_expression(int min_prec) {
    if (auto term_lhs = parse_term()){
        NodeExpression* expr_lhs = allocator.alloc<NodeExpression>();
        expr_lhs->expr = term_lhs.value();

        while (true){
            std::optional<Token> cur_tok = peek();
            std::optional<int> prec;
            NodeBinExpr* expr;
            NodeExpression* expr_lhs2 = allocator.alloc<NodeExpression>();
            Token op;
            if (!cur_tok.has_value()) break;

            prec = bin_precedence(cur_tok->ttype);

            if (!prec.has_value() || prec < min_prec) break;

            op = consume();
            int next_min_prec = prec.value() + 1;
            expr = allocator.alloc<NodeBinExpr>();
            auto expr_rhs = parse_expression(next_min_prec);
            if (!expr_rhs.has_value())
                HSharpVE::error(HSharpVE::EExceptionSource::PARSER,
                                HSharpVE::EExceptionReason::PARSE_ERROR,
                                "Failed to parse expression");

            expr_lhs2->expr = expr_lhs->expr;
            switch(op.ttype){
                case TOK_PLUS:{
                    auto node = allocator.alloc<NodeBinExprAdd>();
                    node->lhs = expr_lhs2;
                    node->rhs = expr_rhs.value();
                    expr->var = node;
                    break;
                }
                case TOK_MINUS:{
                    auto node = allocator.alloc<NodeBinExprSub>();
                    node->lhs = expr_lhs2;
                    node->rhs = expr_rhs.value();
                    expr->var = node;
                    break;
                }
                case TOK_MUL_SIGN:{
                    auto node = allocator.alloc<NodeBinExprMul>();
                    node->lhs = expr_lhs2;
                    node->rhs = expr_rhs.value();
                    expr->var = node;
                    break;
                }
                case TOK_FSLASH:{
                    auto node = allocator.alloc<NodeBinExprMul>();
                    node->lhs = expr_lhs2;
                    node->rhs = expr_rhs.value();
                    expr->var = node;
                    break;
                }
                default:
                    break;
            }
            expr_lhs->expr = expr;
            return expr_lhs;
        }
        return expr_lhs;
    } else if (auto str_lit = try_consume(TokenType::TOK_STR_LIT)) {
        auto expr_str_lit = allocator.alloc<NodeExpressionStrLit>();
        auto expr = allocator.alloc<NodeExpression>();
        expr_str_lit->str_lit = str_lit.value();
        expr->expr = expr_str_lit;
        expr->line = str_lit->line;
        return expr;
    }
    return {};
}

std::optional<HSharpParser::NodeStmt *> HSharpParser::Parser::parse_statement() {
    if (peek().has_value() && peek().value().ttype == TokenType::TOK_EXIT &&
        peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_PAREN_OPEN) {
        auto stmt_exit = allocator.alloc<NodeStmtExit>();
        auto node_stmt = allocator.alloc<NodeStmt>();
        std::optional<NodeExpression*> node_expr;
        stmt_exit->line = consume().line;
        skip();

        if (!(node_expr = parse_expression())) goto error;

        stmt_exit->expr = node_expr.value();
        stmt_exit->line = node_expr.value()->line;

        try_consume(TokenType::TOK_PAREN_CLOSE, 1);
        try_consume(TokenType::TOK_SEMICOLON, 1);

        node_stmt->statement = stmt_exit;
        node_stmt->line = stmt_exit->line;
        return node_stmt;
    } else if (peek().has_value() && peek().value().ttype == TokenType::TOK_PRINT &&
               peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_PAREN_OPEN) {
        skip(2);
        auto stmt_print = allocator.alloc<NodeStmtPrint>();
        if (auto expr = parse_expression())
            stmt_print->expr = expr.value();

        try_consume(TokenType::TOK_PAREN_CLOSE, 1);
        try_consume(TokenType::TOK_SEMICOLON, 1);

        auto node_stmt = allocator.alloc<NodeStmt>();
        node_stmt->statement = stmt_print;
        return node_stmt;
    } else if (peek().has_value() && peek().value().ttype == TokenType::TOK_INPUT &&
               peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_PAREN_OPEN) {
        skip(2);
        auto node_input = allocator.alloc<NodeStmtInput>();
        if (auto expr = parse_expression())
            node_input->expr = expr.value();

        try_consume(TokenType::TOK_PAREN_CLOSE, 1);
        try_consume(TokenType::TOK_SEMICOLON, 1);

        auto node_stmt = allocator.alloc<NodeStmt>();
        node_stmt->statement = node_input;
        return node_stmt;
    } else if (peek().has_value() && peek().value().ttype == TokenType::TOK_VAR &&
               peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_IDENT &&
               peek(2).has_value() && peek(2).value().ttype == TokenType::TOK_EQUALITY_SIGN) {
        skip();
        auto node_stmt_var = allocator.alloc<NodeStmtVar>();
        auto node_stmt = allocator.alloc<NodeStmt>();
        std::optional<NodeExpression*> expr;
        node_stmt_var->ident = consume();
        skip();
        if (!(expr = parse_expression())) goto error;

        node_stmt_var->expr = expr.value();

        try_consume(TokenType::TOK_SEMICOLON, 1);

        node_stmt->statement = node_stmt_var;
        return node_stmt;
    } else if (peek().has_value() && peek().value().ttype == TokenType::TOK_IDENT &&
                peek(1).has_value() && peek(1).value().ttype == TokenType::TOK_EQUALITY_SIGN) {
        auto node_stmt = allocator.alloc<NodeStmtVarAssign>();
        auto stmt = allocator.alloc<NodeStmt>();
        std::optional<NodeExpression*> expr;
        node_stmt->ident = consume();
        skip();
        if (!(expr = parse_expression())) goto error;

        node_stmt->expr = expr.value();

        try_consume(TokenType::TOK_SEMICOLON, 1);

        stmt->statement = node_stmt;
        return stmt;
    }

    error:
    error(HSharpVE::EExceptionSource::PARSER, HSharpVE::EExceptionReason::SYNTAX_ERROR, parser_fail_msg);
}


std::optional<HSharpParser::NodeProgram> HSharpParser::Parser::parse_program() {
    NodeProgram program;
    while (peek().has_value()) {
        std::optional<NodeStmt*> stmt = parse_statement();
        if (!stmt.has_value())
            error(HSharpVE::EExceptionSource::PARSER, HSharpVE::EExceptionReason::SYNTAX_ERROR, "Parser failed to parse statement");

        program.statements.push_back(stmt.value());
    }
    return program;
}