#include "parser/expression/unary.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/unary.hpp"
#include "lexer/token/token.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/identifier.hpp"

shared_ptr<Expression> UnaryExpressionParser::parse() {
  Token op_token;

  shared_ptr<IdentifierExpression> expression;
  if (left) {
        expression = dynamic_pointer_cast<IdentifierExpression>(left);
    if (!expression) {
      parser->get_logger()->error("Expected identifier expression",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    op_token = parser->current_token;
    parser->eat_token();  // eat postfix
  } else {
    op_token = parser->current_token;
    parser->eat_token();  // eat prefix

    expression = IdentifierExpressionParser(parser).parse_identifier();
    if (!expression) {
      parser->get_logger()->error("Expected variable reference expression",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  return make_shared<UnaryExpression>(*expression->get_position(), op_token,
                                      expression);
}
