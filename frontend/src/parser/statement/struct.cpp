#include "parser/statement/struct.hpp"

#include <memory>

#include "ast/statement/struct.hpp"
#include "lexer/token/token_type.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/identifier.hpp"
#include "parser/expression/type.hpp"

shared_ptr<StructStatement> StructStatementParser::parse() {
  return parse(TOKEN_PRIVATE);
}

shared_ptr<StructStatement> StructStatementParser::parse(
    const TokenType &visibility) {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat struct

  if (parser->current_token.type != TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after struct",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto identifier_expression_parser = IdentifierExpressionParser(parser);
  auto name_expression = identifier_expression_parser.parse_identifier();
  if (!name_expression) {
    parser->get_logger()->error("Failed to parse identifier",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected { after identifier",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat {

  auto type_expression_parser = TypeExpressionParser(parser);

  vector<pair<shared_ptr<IdentifierExpression>, shared_ptr<TypeExpression>>>
      fields;
  while (parser->current_token.type != TOKEN_RIGHT_BRACE) {
    if (parser->current_token.type != TOKEN_IDENTIFIER) {
      parser->get_logger()->error("Expected identifier as field name",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    auto field_name_expression =
        identifier_expression_parser.parse_identifier();
    if (!field_name_expression) {
      parser->get_logger()->error("Failed to parse identifier",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    if (parser->current_token.type != TOKEN_COLON) {
      parser->get_logger()->error("Expected : after field name",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    parser->eat_token();  // eat :

    auto field_type_expression = type_expression_parser.parse_type();
    if (!field_type_expression) {
      parser->get_logger()->error("Failed to parse type",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    fields.emplace_back(field_name_expression, field_type_expression);
  }

  parser->eat_token();  // eat }

  return make_shared<StructStatement>(
      position, SymbolVisibility::from_token_type(visibility), name_expression,
      fields);
}
