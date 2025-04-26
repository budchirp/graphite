#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/field.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/identifier.hpp"
#include "parser/expression/field.hpp"

shared_ptr<Expression> FieldExpressionParser::parse() {
  auto identifier_expression(dynamic_pointer_cast<IdentifierExpression>(left));
  if (!identifier_expression) {
    parser->get_logger()->error("Expected identifier expression",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat .

  if (parser->current_token.type != TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after dot",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto field_expression = IdentifierExpressionParser(parser).parse_identifier();
  if (!field_expression) {
    parser->get_logger()->error("Failed to parse identifier", LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_shared<FieldExpression>(*identifier_expression->get_position(),
                                      identifier_expression, field_expression);
}
