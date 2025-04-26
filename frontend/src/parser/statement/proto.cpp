#include "parser/statement/proto.hpp"

#include <memory>

#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/identifier.hpp"
#include "parser/expression/type.hpp"

shared_ptr<ProtoStatement> ProtoStatementParser::parse() {
  if (parser->current_token.type != TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier as prototype name",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  const auto position = *parser->get_lexer()->position;

  auto identifier_expression_parser = IdentifierExpressionParser(parser);

  auto namen_expression = identifier_expression_parser.parse_identifier();
  if (!namen_expression) {
    parser->get_logger()->error("Failed to parse the name of the proto",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error(
        "Expected left parentheses after prototype name",
        LogTypes::Error::SYNTAX);
  }

  parser->eat_token();  // eat (

  auto type_expression_parser = TypeExpressionParser(parser);

  vector<pair<shared_ptr<IdentifierExpression>, shared_ptr<TypeExpression>>>
      parameters;
  while (parser->current_token.type != TOKEN_RIGHT_PARENTHESES) {
    if (parser->current_token.type != TOKEN_IDENTIFIER) {
      parser->get_logger()->error("Expected identifier as prototype parameter",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    auto parameter_expression = identifier_expression_parser.parse_identifier();
    if (!parameter_expression) {
      parser->get_logger()->error("Failed to parse parameter",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    if (parser->current_token.type != TOKEN_COLON) {
      parser->get_logger()->error("Expected : after parameter name",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    parser->eat_token();  // eat :

    auto type_expression = type_expression_parser.parse_type();
    if (!type_expression) {
      parser->get_logger()->error("Failed to parse type",
                                  LogTypes::Error::INTERNAL);
      return nullptr;
    }

    parameters.emplace_back(parameter_expression, type_expression);

    if (parser->current_token.type == TOKEN_COMMA) {
      parser->eat_token();  // eat ,
    } else if (parser->current_token.type != TOKEN_RIGHT_PARENTHESES) {
      parser->get_logger()->error("Expected right parentheses or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat )

  if (parser->current_token.type != TOKEN_ARROW) {
    parser->get_logger()->error("Expected -> after )", LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat ->

  auto return_type_expression = type_expression_parser.parse_type();
  if (!return_type_expression) {
    parser->get_logger()->error("Failed to parse type",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_shared<ProtoStatement>(position, namen_expression, parameters,
                                     return_type_expression);
}
