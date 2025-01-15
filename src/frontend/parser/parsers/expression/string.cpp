#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expression/string.hpp"
#include "frontend/parser/parsers/expression/string.hpp"

StringExpressionParser::StringExpressionParser(
    const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<Expression> StringExpressionParser::parse() {
  const auto string_token = parser->current_token;
  parser->eat_token(); // eat string

  return make_unique<StringExpression>(*parser->get_lexer()->position,
                                       string_token.literal);
}
