#include "parser/expression/unary.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/unary.hpp"
#include "ast/expression/var_ref.hpp"
#include "lexer/token/token.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/var_ref.hpp"

shared_ptr<Expression> UnaryExpressionParser::parse() {
  Token op_token;

  shared_ptr<VarRefExpression> expression;

  if (left) {
    shared_ptr<VarRefExpression> _expression(
        dynamic_pointer_cast<VarRefExpression>(left));
    if (!_expression) {
      parser->get_logger()->error("Expected variable reference expression",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    expression = std::move(_expression);

    op_token = parser->current_token;
    parser->eat_token();  // eat postfix
  } else {
    op_token = parser->current_token;
    parser->eat_token();  // eat prefix

    expression = VarRefExpressionParser(parser).parse_var_ref();
    if (!expression) {
      parser->get_logger()->error("Expected variable reference expression",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  return make_shared<UnaryExpression>(*expression->get_position(), op_token,
                                      expression);
}
