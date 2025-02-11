#include "parser/parsers/expression/integer.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "ast/expression/integer.hpp"

unique_ptr<Expression> IntegerExpressionParser::parse() {
  return parse_integer();
}

unique_ptr<IntegerExpression> IntegerExpressionParser::parse_integer() {
  const auto position = *parser->get_lexer()->position;

  const auto integer_token = parser->current_token;
  parser->eat_token();  // eat integer

  return make_unique<IntegerExpression>(position, integer_token.literal);

  // try {
  //   long long value = stoll(integer_token.literal);
  //
  //   if (auto int_type = TypeHelper::is_int(type)) {
  //     if (int_type->is_unsigned) {
  //       if (value < 0) {
  //         parser->get_logger()->error("Negative value for unsigned integer",
  //                                     LogTypes::Error::SYNTAX);
  //         return nullptr;
  //       }
  //
  //       if ((int_type->size == 8 && value > numeric_limits<uint8_t>::max()) ||
  //           (int_type->size == 16 && value > numeric_limits<uint16_t>::max()) ||
  //           (int_type->size == 32 && value > numeric_limits<uint32_t>::max()) ||
  //           (int_type->size == 64 && value > numeric_limits<uint64_t>::max())) {
  //         parser->get_logger()->error("Unsigned integer too big",
  //                                     LogTypes::Error::SYNTAX);
  //         return nullptr;
  //       }
  //     } else {
  //       if (value > numeric_limits<int>::max() ||
  //           value < numeric_limits<int>::min() ||
  //           (int_type->size == 8 && (value < numeric_limits<int8_t>::min() ||
  //                                    value > numeric_limits<int8_t>::max())) ||
  //           (int_type->size == 16 &&
  //            (value < numeric_limits<int16_t>::min() ||
  //             value > numeric_limits<int16_t>::max())) ||
  //           (int_type->size == 32 &&
  //            (value < numeric_limits<int32_t>::min() ||
  //             value > numeric_limits<int32_t>::max())) ||
  //           (int_type->size == 64 &&
  //            (value < numeric_limits<int64_t>::min() ||
  //             value > numeric_limits<int64_t>::max()))) {
  //         parser->get_logger()->error("Signed integer too big or out of range",
  //                                     LogTypes::Error::SYNTAX);
  //         return nullptr;
  //       }
  //     }
  //
  //     return make_unique<IntegerExpression>(position, int_type,
  //                                           static_cast<int>(value));
  //
  //   } else {
  //     parser->get_logger()->error("Invalid type for integer parsing",
  //                                 LogTypes::Error::SYNTAX);
  //     return nullptr;
  //   }
  // } catch (const std::invalid_argument& e) {
  //   parser->get_logger()->error("Invalid integer", LogTypes::Error::SYNTAX);
  //   return nullptr;
  // } catch (const std::out_of_range& e) {
  //   parser->get_logger()->error("Invalid integer", LogTypes::Error::SYNTAX);
  //   return nullptr;
  // }
}
