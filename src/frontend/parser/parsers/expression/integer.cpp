#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expression/integer.hpp"
#include "frontend/parser/parsers/expression/integer.hpp"

IntegerExpressionParser::IntegerExpressionParser(
    const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<Expression> IntegerExpressionParser::parse() {
  const auto integer_token = parser->current_token;
  parser->eat_token(); // eat integer

  return make_unique<IntegerExpression>(*parser->get_lexer()->position,
                                        stoi(integer_token.literal));
}
