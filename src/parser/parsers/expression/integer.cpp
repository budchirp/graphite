#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/integer.hpp"
#include "parser/parsers/expression/integer.hpp"
#include "types/int.hpp"

unique_ptr<Expression> IntegerExpressionParser::parse() {
  const auto position = *parser->get_lexer()->position;

  const auto integer_token = parser->current_token;
  parser->eat_token(); // eat integer

  return make_unique<IntegerExpression>(position,
                                        make_shared<IntType>(32, false),
                                        stoi(integer_token.literal));
}
