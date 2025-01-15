#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/binary.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/binary.hpp"
#include "parser/parsers/statement/expression.hpp"

unique_ptr<Expression> BinaryExpressionParser::parse() {
  const auto operator_token = parser->current_token;
  parser->eat_token(); // eat operator

  auto right = ExpressionStatementParser(parser).parse_expression(
      PrecedenceHelper::precedence_for(operator_token.type));
  if (!right) {
    parser->get_logger()->error(
        "Failed to parse the right hand side of the binary expression",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_unique<BinaryExpression>(*right->get_position(), left->get_type(),
                                       operator_token, std::move(left),
                                       std::move(right));
}
