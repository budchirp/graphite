#include <functional>
#include <map>
#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/parser/parsers/expression/binary.hpp"
#include "frontend/parser/parsers/expression/call.hpp"
#include "frontend/parser/parsers/expression/group.hpp"
#include "frontend/parser/parsers/expression/identifier.hpp"
#include "frontend/parser/parsers/expression/integer.hpp"
#include "frontend/parser/parsers/expression/parser.hpp"
#include "frontend/parser/parsers/expression/prefix.hpp"
#include "frontend/parser/parsers/statement/expression.hpp"
#include "frontend/parser/precedence.hpp"
#include "utils/logger/logger.hpp"

ExpressionStatementParser::ExpressionStatementParser(
    shared_ptr<Parser> parser) {
  this->parser = parser;
}

unique_ptr<ExpressionStatement> ExpressionStatementParser::parse() {
  auto expression = parse_expression(Precedence::LOWEST);

  return make_unique<ExpressionStatement>(std::move(expression));
}

std::map<TokenType, std::function<std::unique_ptr<ExpressionParser>(
                        std::shared_ptr<Parser>)>>
    prefix_parse_fns = {
        {TokenType::TOKEN_IDENTIFIER,
         [](std::shared_ptr<Parser> parser) {
           return std::make_unique<IdentifierExpressionParser>(parser);
         }},
        {TokenType::TOKEN_INT,
         [](std::shared_ptr<Parser> parser) {
           return std::make_unique<IntegerExpressionParser>(parser);
         }},
        {TokenType::TOKEN_BANG,
         [](std::shared_ptr<Parser> parser) {
           return std::make_unique<PrefixExpressionParser>(parser);
         }},
        {TokenType::TOKEN_PLUS,
         [](std::shared_ptr<Parser> parser) {
           return std::make_unique<PrefixExpressionParser>(parser);
         }},
        {TokenType::TOKEN_MINUS,
         [](std::shared_ptr<Parser> parser) {
           return std::make_unique<PrefixExpressionParser>(parser);
         }},
        {TokenType::TOKEN_LEFT_PARENTHESES,
         [](std::shared_ptr<Parser> parser) {
           return std::make_unique<GroupExpressionParser>(parser);
         }},
};

std::map<TokenType,
         std::function<std::unique_ptr<ExpressionParser>(
             std::shared_ptr<Parser>, std::unique_ptr<Expression> &)>>
    binary_parse_fns = {
        {TokenType::TOKEN_PLUS,
         [](std::shared_ptr<Parser> parser, std::unique_ptr<Expression> &left) {
           return std::make_unique<BinaryExpressionParser>(parser,
                                                           std::move(left));
         }},
        {TokenType::TOKEN_MINUS,
         [](std::shared_ptr<Parser> parser, std::unique_ptr<Expression> &left) {
           return std::make_unique<BinaryExpressionParser>(parser,
                                                           std::move(left));
         }},
        {TokenType::TOKEN_SLASH,
         [](std::shared_ptr<Parser> parser, std::unique_ptr<Expression> &left) {
           return std::make_unique<BinaryExpressionParser>(parser,
                                                           std::move(left));
         }},
        {TokenType::TOKEN_ASTERISK,
         [](std::shared_ptr<Parser> parser, std::unique_ptr<Expression> &left) {
           return std::make_unique<BinaryExpressionParser>(parser,
                                                           std::move(left));
         }},
        {TokenType::TOKEN_EQUAL,
         [](std::shared_ptr<Parser> parser, std::unique_ptr<Expression> &left) {
           return std::make_unique<BinaryExpressionParser>(parser,
                                                           std::move(left));
         }},
        {TokenType::TOKEN_NOT_EQUAL,
         [](std::shared_ptr<Parser> parser, std::unique_ptr<Expression> &left) {
           return std::make_unique<BinaryExpressionParser>(parser,
                                                           std::move(left));
         }},
        {TokenType::TOKEN_LESS_THAN,
         [](std::shared_ptr<Parser> parser, std::unique_ptr<Expression> &left) {
           return std::make_unique<BinaryExpressionParser>(parser,
                                                           std::move(left));
         }},
        {TokenType::TOKEN_GREATER_THAN,
         [](std::shared_ptr<Parser> parser, std::unique_ptr<Expression> &left) {
           return std::make_unique<BinaryExpressionParser>(parser,
                                                           std::move(left));
         }},
        {TokenType::TOKEN_LEFT_PARENTHESES,
         [](std::shared_ptr<Parser> parser, std::unique_ptr<Expression> &left) {
           return std::make_unique<CallExpressionParser>(parser,
                                                         std::move(left));
         }}};

unique_ptr<Expression>
ExpressionStatementParser::parse_expression(Precedence precedence) {
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
