#include "ast/expression/identifier.hpp"

#include <iostream>
#include <memory>

#include "ast/expression.hpp"
#include "parser/parsers/expression/identifier.hpp"

unique_ptr<Expression> IdentifierExpressionParser::parse() {
  return parse(true);
}

unique_ptr<Expression> IdentifierExpressionParser::parse(bool is_variable) {
  const auto position = *parser->get_lexer()->position;

  const auto identifier_token = parser->current_token;
  parser->eat_token();  // eat identifier

  shared_ptr<Type> type = nullptr;
  if (is_variable) {
    type =
        parser->get_program()->get_env()->get_symbol(identifier_token.literal);

    if (!type) {
      parser->get_logger()->error(
          "Undefined variable `" + identifier_token.literal + "`",
          LogTypes::Error::UNDEFINED);
      return nullptr;
    }
  }

  return make_unique<IdentifierExpression>(position, type,
                                           identifier_token.literal);
}
