#include "ast/expression.hpp"

#include <functional>
#include <map>
#include <memory>

#include "logger/logger.hpp"
#include "parser/parsers/expression/binary.hpp"
#include "parser/parsers/expression/boolean.hpp"
#include "parser/parsers/expression/call.hpp"
#include "parser/parsers/expression/group.hpp"
#include "parser/parsers/expression/identifier.hpp"
#include "parser/parsers/expression/if.hpp"
#include "parser/parsers/expression/integer.hpp"
#include "parser/parsers/expression/parser.hpp"
#include "parser/parsers/expression/prefix.hpp"
#include "parser/parsers/expression/string.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "parser/precedence.hpp"

unique_ptr<ExpressionStatement> ExpressionStatementParser::parse() {
  auto expression = parse_expression(Precedence::LOWEST);

  return make_unique<ExpressionStatement>(*expression->get_position(),
                                          expression->get_type(),
                                          std::move(expression));
}

map<TokenType, function<unique_ptr<ExpressionParser>(shared_ptr<Parser>)>>
    prefix_parse_fns = {
        {TokenType::TOKEN_IDENTIFIER,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<IdentifierExpressionParser>(parser);
         }},
        {TokenType::TOKEN_INT,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<IntegerExpressionParser>(parser);
         }},
        {TokenType::TOKEN_STRING,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<StringExpressionParser>(parser);
         }},
        {TokenType::TOKEN_TRUE,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<BooleanExpressionParser>(parser);
         }},
        {TokenType::TOKEN_FALSE,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<BooleanExpressionParser>(parser);
         }},
        {TokenType::TOKEN_BANG,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<PrefixExpressionParser>(parser);
         }},
        {TokenType::TOKEN_ASTERISK,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<PrefixExpressionParser>(parser);
         }},
        {TokenType::TOKEN_AMPERSAND,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<PrefixExpressionParser>(parser);
         }},
        {TokenType::TOKEN_PLUS,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<PrefixExpressionParser>(parser);
         }},
        {TokenType::TOKEN_MINUS,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<PrefixExpressionParser>(parser);
         }},
        {TokenType::TOKEN_LEFT_PARENTHESES,
         [](const shared_ptr<Parser> &parser) {
           return make_unique<GroupExpressionParser>(parser);
         }},
        {TokenType::TOKEN_IF, [](const shared_ptr<Parser> &parser) {
           return make_unique<IfExpressionParser>(parser);
         }}};

map<TokenType, function<unique_ptr<ExpressionParser>(shared_ptr<Parser>,
                                                     unique_ptr<Expression> &)>>
    binary_parse_fns = {
        {TokenType::TOKEN_PLUS,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TokenType::TOKEN_MINUS,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TokenType::TOKEN_SLASH,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TokenType::TOKEN_ASTERISK,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TokenType::TOKEN_EQUAL,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TokenType::TOKEN_NOT_EQUAL,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TokenType::TOKEN_LESS_THAN,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TokenType::TOKEN_GREATER_THAN,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<BinaryExpressionParser>(parser, std::move(left));
         }},
        {TokenType::TOKEN_LEFT_PARENTHESES,
         [](const shared_ptr<Parser> &parser, unique_ptr<Expression> &left) {
           return make_unique<CallExpressionParser>(parser, std::move(left));
         }}};

unique_ptr<Expression> ExpressionStatementParser::parse_expression(
    Precedence precedence) {
  auto prefix_parser_it = prefix_parse_fns.find(parser->current_token.type);
  if (prefix_parser_it == prefix_parse_fns.end()) {
    Logger::error("Unknown prefix: " + parser->current_token.to_string());
  }

  auto prefix_parser = prefix_parser_it->second(parser);
  auto left = prefix_parser->parse();

  while (PrecedenceHelper::precedence_for(parser->current_token.type) >
         precedence) {
    auto infix_parser_it = binary_parse_fns.find(parser->current_token.type);
    if (infix_parser_it == binary_parse_fns.end()) {
      break;
    }

    auto infix_parser = infix_parser_it->second(parser, left);
    left = infix_parser->parse();
  }

  return left;
}
