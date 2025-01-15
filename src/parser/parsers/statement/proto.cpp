#include "parser/parsers/statement/proto.hpp"

#include <memory>

#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/identifier.hpp"
#include "parser/parsers/expression/type.hpp"
#include "token/token_type.hpp"
#include "types/function.hpp"
#include "types/type.hpp"

unique_ptr<ProtoStatement> ProtoStatementParser::parse() {
  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier as prototype name",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  const auto position = *parser->get_lexer()->position;

  auto identifier_expression_parser = IdentifierExpressionParser(parser);

  auto name_expression = identifier_expression_parser.parse();
  unique_ptr<IdentifierExpression> name(
      dynamic_cast<IdentifierExpression *>(name_expression.release()));
  if (!name) {
    parser->get_logger()->error("Failed to parse the name of the proto",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error(
        "Expected left parentheses after prototype name",
        LogTypes::Error::SYNTAX);
  }

  parser->eat_token();  // eat (

  auto type_expression_parser = TypeExpressionParser(parser);

  vector<pair<unique_ptr<IdentifierExpression>, unique_ptr<TypeExpression>>>
      parameters;
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
      parser->get_logger()->error("Expected identifier as prototype parameter",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    auto parameter_expression = identifier_expression_parser.parse();
    unique_ptr<IdentifierExpression> parameter(
        dynamic_cast<IdentifierExpression *>(parameter_expression.release()));
    if (!parameter) {
      parser->get_logger()->error("Failed to parse the parameter of the proto");
      return nullptr;
    }

    if (parser->current_token.type != TokenType::TOKEN_COLON) {
      parser->get_logger()->error("Expected : after parameter name",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    parser->eat_token();  // eat :

    auto type_expression = type_expression_parser.parse();
    unique_ptr<TypeExpression> type(
        dynamic_cast<TypeExpression *>(type_expression.release()));
    if (!type) {
      parser->get_logger()->error(
          "Failed to parse the parameter type of the proto",
          LogTypes::Error::INTERNAL);
      return nullptr;
    }

    parameters.push_back(pair(std::move(parameter), std::move(type)));

    if (parser->current_token.type == TokenType::TOKEN_COMMA) {
      parser->eat_token();  // eat ,
    } else if (parser->current_token.type !=
               TokenType::TOKEN_RIGHT_PARENTHESES) {
      parser->get_logger()->error("Expected right parentheses or comma",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token();  // eat )

  if (parser->current_token.type != TokenType::TOKEN_ARROW) {
    parser->get_logger()->error("Expected -> after )", LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat ->

  auto type_expression = type_expression_parser.parse();
  unique_ptr<TypeExpression> type(
      dynamic_cast<TypeExpression *>(type_expression.release()));
  if (!type) {
    parser->get_logger()->error("Failed to parse the return type of the proto",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  auto parent = parser->get_env()->get_parent();
  auto env = parent ? parent : parser->get_env();

  vector<shared_ptr<Type>> parameter_types;
  for (const auto &[_, type] : parameters) {
    parameter_types.push_back(type->get_type());
  }

  env->set_type(name->get_value(),
                make_shared<FunctionType>(parameter_types, type->get_type()));
  env->set_symbol(name->get_value(), type->get_type());

  return make_unique<ProtoStatement>(position, std::move(name),
                                     std::move(parameters), std::move(type));
}
