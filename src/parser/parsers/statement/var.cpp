#include "parser/parsers/statement/var.hpp"

#include <memory>

#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/identifier.hpp"
#include "parser/parsers/expression/type.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "parser/precedence.hpp"
#include "token/token_type.hpp"

unique_ptr<VarStatement> VarStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat var

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after var",
                                LogTypes::Error::SYNTAX);
  }

  auto name = IdentifierExpressionParser(parser).parse_identifier();
  if (!name) {
    parser->get_logger()->error(
        "Failed to parse the name of the variable statement",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TokenType::TOKEN_COLON) {
    parser->get_logger()->error("Expected : after identifier",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat :

  auto type_expression = TypeExpressionParser(parser).parse();
  unique_ptr<TypeExpression> type(
      dynamic_cast<TypeExpression *>(type_expression.release()));
  if (!type) {
    parser->get_logger()->error(
        "Failed to parse the type of the variable statement",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TokenType::TOKEN_ASSIGN) {
    parser->get_logger()->error("Expected = after type",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat =

  auto expression =
      ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);
  if (!expression) {
    parser->get_logger()->error(
        "Failed to parse the expression of the variable statement",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  parser->get_program()->get_env()->set_symbol(name->get_value(),
                                               type->get_type());

  return make_unique<VarStatement>(position, 
                                   std::move(name), std::move(type),
                                   std::move(expression));
}
