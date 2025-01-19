#include "ast/expression/integer.hpp"

#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

#include "ast/expression.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/integer.hpp"
#include "types/int.hpp"

unique_ptr<Expression> IntegerExpressionParser::parse() {
  const auto position = *parser->get_lexer()->position;

  const auto integer_token = parser->current_token;
  parser->eat_token();  // eat integer

  try {
    long long value = stoll(integer_token.literal);

    if (value > numeric_limits<int>::max() ||
        value < numeric_limits<int>::min()) {
      parser->get_logger()->error("Integer too big", LogTypes::Error::SYNTAX);
    }

    return make_unique<IntegerExpression>(
        position, make_shared<IntType>(32, false), static_cast<int>(value));
  } catch (const std::invalid_argument& e) {
    parser->get_logger()->error("Invalid integer", LogTypes::Error::SYNTAX);
    return nullptr;
  } catch (const std::out_of_range& e) {
    parser->get_logger()->error("Invalid integer", LogTypes::Error::SYNTAX);
    return nullptr;
  }
}
