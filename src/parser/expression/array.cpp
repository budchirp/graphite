

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/array.hpp"
#include "parser/expression/array.hpp"

#include "parser/statement/expression.hpp"

unique_ptr<Expression> ArrayExpressionParser::parse() { return parse_array(); }

unique_ptr<ArrayExpression> ArrayExpressionParser::parse_array() const {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat [

  auto expression_statement_parser = ExpressionStatementParser(parser);

  vector<unique_ptr<Expression>> values;
  while (parser->current_token.type != TOKEN_RIGHT_BRACKET) {
    values.push_back(
        expression_statement_parser.parse_expression(Precedence::Value::LOWEST));

    if (parser->current_token.type == TOKEN_COMMA) {
      parser->eat_token();  // eat ','
    } else if (parser->current_token.type != TOKEN_RIGHT_BRACKET) {
      parser->get_logger()->error("Expected right bracket or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat ]

  return make_unique<ArrayExpression>(position, std::move(values));
}
