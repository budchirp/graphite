#include "parser/statement/type.hpp"

#include <memory>

#include "ast/statement/type.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/identifier.hpp"
#include "parser/expression/type.hpp"
#include "semantic/visibilty.hpp"

shared_ptr<TypeStatement> TypeStatementParser::parse() {
  return parse(TOKEN_PRIVATE);
}

shared_ptr<TypeStatement> TypeStatementParser::parse(
    const TokenType &visibility) const {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat type

  if (parser->current_token.type != TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after type",
                                LogTypes::Error::SYNTAX);
  }

  auto name_expression = IdentifierExpressionParser(parser).parse_identifier();
  if (!name_expression) {
    parser->get_logger()->error(
        "Failed to parse identifier",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TOKEN_ASSIGN) {
    parser->get_logger()->error("Expected = after identifier",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat :

  auto type_expression = TypeExpressionParser(parser).parse_type();
  if (!type_expression) {
    parser->get_logger()->error("Failed to parse type",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_shared<TypeStatement>(
      position, SymbolVisibility::from_token_type(visibility), name_expression,
      type_expression);
}
