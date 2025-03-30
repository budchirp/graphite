#include "ast/expression.hpp"
#include "ast/statement/expression.hpp"
#include "parser/statement/expression.hpp"

#include <functional>
#include <map>
#include <memory>

#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "parser/precedence.hpp"
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

unique_ptr<ExpressionStatement> ExpressionStatementParser::parse() {
  auto expression = parse_expression(Precedence::LOWEST);

  return make_unique<ExpressionStatement>(*expression->get_position(),
                                          std::move(expression));
}

map<TokenType, function<unique_ptr<ExpressionParser>(shared_ptr<Parser>)>>
    prefix_parse_fns = {
        {TOKEN_IDENTIFIER,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<VarRefExpressionParser>(parser);
         }},
        {TOKEN_INT,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<IntegerExpressionParser>(parser);
         }},
        {TOKEN_STRING,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<StringExpressionParser>(parser);
         }},
        {TOKEN_LEFT_BRACKET,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<ArrayExpressionParser>(parser);
         }},
        {TOKEN_TRUE,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<BooleanExpressionParser>(parser);
         }},
        {TOKEN_FALSE,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<BooleanExpressionParser>(parser);
         }},
        {TOKEN_BANG,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_ASTERISK,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_AMPERSAND,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_PLUS,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_MINUS,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<UnaryExpressionParser>(parser, nullptr);
         }},
        {TOKEN_LEFT_PARENTHESES,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<GroupExpressionParser>(parser);
         }},
        {TOKEN_IF, [](const shared_ptr<Parser> &parser) {
           return make_unique<IfExpressionParser>(parser);
         }}};

map<TokenType, function<unique_ptr<ExpressionParser>(shared_ptr<Parser>,
                                                     unique_ptr<Expression> &)>>
    binary_parse_fns = {
        {TOKEN_PLUSPLUS,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<UnaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_MINUSMINUS,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<UnaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_BANG_BANG,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<UnaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_LEFT_BRACKET,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<IndexExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_PLUS,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_MINUS,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_SLASH,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_ASTERISK,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_ASSIGN,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_EQUAL,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_NOT_EQUAL,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_LESS_THAN,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_GREATER_THAN,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_AS,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TOKEN_LEFT_PARENTHESES,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<CallExpressionParser>(parser, std::move(left));
         }}};

unique_ptr<Expression> ExpressionStatementParser::parse_expression(
    Precedence::Value precedence) {
  auto prefix_parser_it = prefix_parse_fns.find(parser->current_token.type);
  if (prefix_parser_it == prefix_parse_fns.end()) {
    parser->get_logger()->error(
        "Unknown prefix / postfix: " + parser->current_token.to_string(),
        LogTypes::Error::SYNTAX);
  }

  auto prefix_parser = prefix_parser_it->second(parser);
  auto expression = prefix_parser->parse();

  while (Precedence::precedence_for(parser->current_token.type) >
         precedence) {
    auto unary_parser_it = binary_parse_fns.find(parser->current_token.type);
    if (unary_parser_it == binary_parse_fns.end()) {
      break;
    }

    auto unary_parser = unary_parser_it->second(parser, expression);
    expression = unary_parser->parse();
  }

  return expression;
}
