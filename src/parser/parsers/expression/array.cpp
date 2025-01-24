#include "parser/parsers/expression/array.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/array.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "token/token_type.hpp"
#include "types/array.hpp"
#include "types/null.hpp"

unique_ptr<Expression> ArrayExpressionParser::parse() {
  return parse_array();
}

unique_ptr<ArrayExpression> ArrayExpressionParser::parse_array() {
  return parse_array(nullptr);
}
unique_ptr<ArrayExpression> ArrayExpressionParser::parse_array(
    const shared_ptr<Type> &type) {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat [

  auto expression_statement_parser = ExpressionStatementParser(parser);

  vector<unique_ptr<Expression>> values;
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_BRACKET) {
    values.push_back(
        expression_statement_parser.parse_expression(Precedence::LOWEST));

    if (parser->current_token.type == TokenType::TOKEN_COMMA) {
      parser->eat_token();  // eat ','
    } else if (parser->current_token.type != TokenType::TOKEN_RIGHT_BRACKET) {
      parser->get_logger()->error("Expected right bracket or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token(); // eat ]

  return make_unique<ArrayExpression>(
      position,
      type ? dynamic_pointer_cast<ArrayType>(type)
           : make_shared<ArrayType>(values[0] ? values[0]->get_type()
                                              : make_shared<NullType>(nullptr),
                                    values.size()),
      std::move(values));
}
