#include "parser/statement/proto.hpp"

#include <memory>

#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/identifier.hpp"
#include "parser/expression/type.hpp"

unique_ptr<ProtoStatement> ProtoStatementParser::parse() {
  if (parser->current_token.type != TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier as prototype name",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  const auto position = *parser->get_lexer()->position;

  auto identifier_expression_parser = IdentifierExpressionParser(parser);

  auto name = identifier_expression_parser.parse_identifier();
  if (!name) {
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

  vector<pair<unique_ptr<IdentifierExpression>, unique_ptr<TypeExpression>>>
      parameters;
  while (parser->current_token.type != TOKEN_RIGHT_PARENTHESES) {
    if (parser->current_token.type != TOKEN_IDENTIFIER) {
      parser->get_logger()->error("Expected identifier as prototype parameter",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    auto parameter = identifier_expression_parser.parse_identifier();
    if (!parameter) {
      parser->get_logger()->error(
          "Failed to parse one of the parameter in proto",
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
      parser->get_logger()->error(
          "Failed to parse the parameter type of the proto",
          LogTypes::Error::INTERNAL);
      return nullptr;
    }

    parameters.emplace_back(std::move(parameter), std::move(type_expression));

    if (parser->current_token.type == TOKEN_COMMA) {
      parser->eat_token();  // eat ,
    } else if (parser->current_token.type !=
               TOKEN_RIGHT_PARENTHESES) {
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

  unique_ptr<TypeExpression> return_type_expression(
      dynamic_cast<TypeExpression *>(type_expression_parser.parse().release()));
  if (!return_type_expression) {
    parser->get_logger()->error("Failed to parse the return type of the proto",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_unique<ProtoStatement>(position, std::move(name),
                                     std::move(parameters),
                                     std::move(return_type_expression));
}
