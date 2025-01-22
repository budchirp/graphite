#include "parser/parsers/expression/unary.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/type.hpp"
#include "ast/expression/unary.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "token/token.hpp"
#include "token/token_type.hpp"
#include "types/pointer.hpp"
#include "types/type.hpp"

unique_ptr<Expression> UnaryExpressionParser::parse() {
  const auto op_token = parser->current_token;

  unique_ptr<Expression> expression;
  shared_ptr<Type> type;

  if (left) {
    expression = std::move(left);
    type = expression->get_type();

    parser->eat_token();  // eat postfix
  } else {
    parser->eat_token();  // eat prefix

    expression = ExpressionStatementParser(parser).parse_expression(
        PrecedenceHelper::precedence_for(op_token.type));
    if (!expression) {
      parser->get_logger()->error("Failed to parse the expression",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    type = expression->get_type();

    switch (op_token.type) {
      case TOKEN_ASTERISK: {
        if (dynamic_cast<TypeExpression *>(expression.get())) {
          break;
        }

        type = dynamic_pointer_cast<PointerType>(type)->type;
        break;
      }

      case TOKEN_AMPERSAND:
        type = make_shared<PointerType>(type);
        break;

      default:
        break;
    }
  }

  return make_unique<UnaryExpression>(*expression->get_position(), type,
                                      op_token, std::move(expression));
}
