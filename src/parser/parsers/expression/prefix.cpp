#include <memory>

#include "ast/expression/prefix.hpp"
#include "parser/parsers/expression/prefix.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "token/token_type.hpp"
#include "types/pointer.hpp"
#include "types/type.hpp"

unique_ptr<Expression> PrefixExpressionParser::parse() {
  const auto prefix_token = parser->current_token;
  parser->eat_token(); // eat prefix

  auto right = ExpressionStatementParser(parser).parse_expression(
      PrecedenceHelper::precedence_for(prefix_token.type));  if (!right) {
    parser->get_logger()->error(
        "Failed to parse the right hand side of the prefix expression",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  shared_ptr<Type> type;
  switch (prefix_token.type) {
    case TOKEN_ASTERISK:
      type = dynamic_pointer_cast<PointerType>(type)->type;
      break;

    case TOKEN_AMPERSAND:
      type = make_shared<PointerType>(type);
      break;

    default:
      type = right->get_type();
      break;
  }

  return make_unique<PrefixExpression>(*right->get_position(), type, prefix_token,
                                       std::move(right));
}
