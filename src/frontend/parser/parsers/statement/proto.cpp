#include <memory>

#include "frontend/ast/expression/identifier.hpp"
#include "frontend/parser/parsers/expression/identifier.hpp"
#include "frontend/parser/parsers/statement/proto.hpp"
#include "frontend/token/token_type.hpp"
#include "logger/log_types.hpp"

ProtoStatementParser::ProtoStatementParser(const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<ProtoStatement> ProtoStatementParser::parse() {
  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->logger->error("Expected identifier as prototype name",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto identifier_expression_parser = IdentifierExpressionParser(parser);

  auto name_expression = identifier_expression_parser.parse();
  unique_ptr<IdentifierExpression> name(
      dynamic_cast<IdentifierExpression *>(name_expression.release()));

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    parser->logger->error("Expected left parentheses after prototype name",
                          LogTypes::Error::SYNTAX);
  }

  parser->eat_token(); // eat (

  vector<
      pair<unique_ptr<IdentifierExpression>, unique_ptr<IdentifierExpression>>>
      parameters;
  while (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
      parser->logger->error("Expected identifier as prototype parameter",
                            LogTypes::Error::SYNTAX);
      return nullptr;
    }

    auto parameter_expression = identifier_expression_parser.parse();
    unique_ptr<IdentifierExpression> parameter(
        dynamic_cast<IdentifierExpression *>(parameter_expression.release()));

    if (parser->current_token.type != TokenType::TOKEN_COLON) {
      parser->logger->error("Expected : after parameter name",
                            LogTypes::Error::SYNTAX);
      return nullptr;
    }

    parser->eat_token(); // eat :

    bool is_pointer = false;
    if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
      if (parser->current_token.type == TokenType::TOKEN_ASTERISK) {
        parser->eat_token(); // eat *
        is_pointer = true;
      } else {
        parser->logger->error("Expected type after :", LogTypes::Error::SYNTAX);
        return nullptr;
      }
    }

    auto type_expression = identifier_expression_parser.parse();
    unique_ptr<IdentifierExpression> type(
        dynamic_cast<IdentifierExpression *>(type_expression.release()));
    if (is_pointer) {
      type = make_unique<IdentifierExpression>("*" + type->get_value());
    }

    parameters.push_back(pair(std::move(parameter), std::move(type)));

    if (parser->current_token.type == TokenType::TOKEN_COMMA) {
      parser->eat_token(); // eat ,
    } else if (parser->current_token.type !=
               TokenType::TOKEN_RIGHT_PARENTHESES) {
      parser->logger->error("Expected right parentheses or comma",
                            LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  parser->eat_token(); // eat )

  if (parser->current_token.type != TokenType::TOKEN_ARROW) {
    parser->logger->error("Expected -> after )", LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token(); // eat ->

  bool is_pointer = false;
  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    if (parser->current_token.type == TokenType::TOKEN_ASTERISK) {
      parser->eat_token(); // eat *
      is_pointer = true;
    } else {
      parser->logger->error("Expected type identifier after :",
                            LogTypes::Error::SYNTAX);
      return nullptr;
    }
  }

  auto type_expression = identifier_expression_parser.parse();
  unique_ptr<IdentifierExpression> type(
      dynamic_cast<IdentifierExpression *>(type_expression.release()));
  if (is_pointer) {
    type = make_unique<IdentifierExpression>("*" + type->get_value());
  }

  return make_unique<ProtoStatement>(std::move(name), std::move(parameters),
                                     std::move(type));
}
