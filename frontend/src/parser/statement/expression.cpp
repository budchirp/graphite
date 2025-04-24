#include "ast/expression.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <memory>

#include "ast/statement/expression.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/array.hpp"
#include "parser/expression/binary.hpp"
#include "parser/expression/boolean.hpp"
#include "parser/expression/call.hpp"
#include "parser/expression/group.hpp"
#include "parser/expression/if.hpp"
#include "parser/expression/index.hpp"
#include "parser/expression/integer.hpp"
#include "parser/expression/parser.hpp"
#include "parser/expression/string.hpp"
#include "parser/expression/unary.hpp"
#include "parser/expression/var_ref.hpp"
#include "parser/precedence.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<ExpressionStatement> ExpressionStatementParser::parse() {
  auto expression = parse_expression(Precedence::LOWEST);

  return make_shared<ExpressionStatement>(*expression->get_position(),
                                          std::move(expression));
}

map<TokenType, function<shared_ptr<ExpressionParser>(shared_ptr<Parser>)>>
    prefix_parse_fns = {
        {TOKEN_IDENTIFIER,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<VarRefExpressionParser>(parser);
         }},
        {TOKEN_INT,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<IntegerExpressionParser>(parser);
         }},
        {TOKEN_STRING,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<StringExpressionParser>(parser);
         }},
        {TOKEN_LEFT_BRACKET,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<ArrayExpressionParser>(parser);
         }},
        {TOKEN_TRUE,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<BooleanExpressionParser>(parser);
         }},
        {TOKEN_FALSE,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<BooleanExpressionParser>(parser);
         }},
        {TOKEN_BANG,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_ASTERISK,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_AMPERSAND,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_PLUS,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_MINUS,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_LEFT_PARENTHESES,
         [](const shared_ptr<Parser> &parser) {
           return make_shared<GroupExpressionParser>(parser);
         }},
        {TOKEN_IF, [](const shared_ptr<Parser> &parser) {
           return make_shared<IfExpressionParser>(parser);
         }}};

map<TokenType, function<shared_ptr<ExpressionParser>(shared_ptr<Parser>,
                                                     shared_ptr<Expression> &)>>
    binary_parse_fns = {
        {TOKEN_PLUSPLUS,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<UnaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_MINUSMINUS,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<UnaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_BANG_BANG,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<UnaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_LEFT_BRACKET,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<IndexExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_PLUS,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_MINUS,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_SLASH,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_ASTERISK,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_ASSIGN,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_EQUAL,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_NOT_EQUAL,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_LESS_THAN,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_GREATER_THAN,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_AS,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_LEFT_PARENTHESES,
         [](const shared_ptr<Parser> &parser, shared_ptr<Expression> &left) {
           return make_shared<CallExpressionParser>(parser, std::move(left));
         }}};

shared_ptr<Expression> ExpressionStatementParser::parse_expression(
    Precedence::Value precedence) {
  auto unary_parser_it = prefix_parse_fns.find(parser->current_token.type);
  if (unary_parser_it == prefix_parse_fns.end()) {
    parser->get_logger()->error(
        "Unknown prefix / postfix: " + parser->current_token.to_string(),
        LogTypes::Error::SYNTAX);
  }

  auto prefix_parser = unary_parser_it->second(parser);
  auto expression = prefix_parser->parse();

  while (Precedence::precedence_for(parser->current_token.type) > precedence) {
    auto binary_parser_it = binary_parse_fns.find(parser->current_token.type);
    if (binary_parser_it == binary_parse_fns.end()) {
      break;
    }

    auto binary_parser = binary_parser_it->second(parser, expression);
    expression = binary_parser->parse();
  }

  return expression;
}
