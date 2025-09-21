#include "ast/expression/field.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/field.hpp"
#include "parser/expression/identifier.hpp"

shared_ptr<Expression> FieldExpressionParser::parse() {
  parser->eat_token();  // eat .

  if (parser->current_token.type != TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after dot",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto field_expression = IdentifierExpressionParser(parser).parse_identifier();
  if (!field_expression) {
    parser->get_logger()->error("Failed to parse identifier",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_shared<FieldExpression>(*left->get_position(), left,
                                      field_expression);
}
