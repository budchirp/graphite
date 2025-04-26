#include "ast/expression/array.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/array.hpp"
#include "parser/statement/expression.hpp"

shared_ptr<Expression> ArrayExpressionParser::parse() { return parse_array(); }

shared_ptr<ArrayExpression> ArrayExpressionParser::parse_array() const {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat [

  auto expression_statement_parser = ExpressionStatementParser(parser);

  vector<shared_ptr<Expression>> values;
  while (parser->current_token.type != TOKEN_RIGHT_BRACKET) {
    auto expression =
        expression_statement_parser.parse_expression(Precedence::Value::LOWEST);
    if (!expression) {
      parser->get_logger()->error("Failed to parse expression",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    values.push_back(expression);

    if (parser->current_token.type == TOKEN_COMMA) {
      parser->eat_token();  // eat ','
    } else if (parser->current_token.type != TOKEN_RIGHT_BRACKET) {
      parser->get_logger()->error("Expected right bracket or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat ]

  return make_shared<ArrayExpression>(position, std::move(values));
}
