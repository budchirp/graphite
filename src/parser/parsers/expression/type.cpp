#include "ast/expression/type.hpp"

#include <memory>

#include "ast/expression.hpp"
#include "parser/parsers/expression/type.hpp"
#include "token/token_type.hpp"
#include "types/pointer.hpp"
#include "types/type.hpp"

unique_ptr<Expression> TypeExpressionParser::parse() {
  return make_unique<TypeExpression>(*parser->get_lexer()->position,
                                     parse_type());
}

shared_ptr<Type> TypeExpressionParser::parse_type() {
  if (parser->current_token.type == TokenType::TOKEN_ASTERISK) {
    parser->eat_token();  // eat *
    return make_shared<PointerType>(parse_type());
  }

  auto type_name = parser->current_token.literal;
  if (auto type = parser->get_program()->get_env()->get_type(type_name)) {
    parser->eat_token();
    return type;
  } else {
    return nullptr;
  }
}
