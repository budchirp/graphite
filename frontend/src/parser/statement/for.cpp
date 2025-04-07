#include "parser/statement/for.hpp"

#include <memory>

#include "parser/precedence.hpp"
#include "parser/statement/block.hpp"
#include "parser/statement/expression.hpp"
#include "parser/statement/var.hpp"

shared_ptr<ForStatement> ForStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat for

  if (parser->current_token.type != TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error("Expected left parentheses after for keyword",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat (

  auto init_statement = VarStatementParser(parser).parse();

  if (parser->current_token.type != TOKEN_SEMICOLON) {
    parser->get_logger()->error("Expected semicolon after initializer",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat ;

  auto expression_statement_parser = ExpressionStatementParser(parser);
  auto condition_expression =
      expression_statement_parser.parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TOKEN_SEMICOLON) {
    parser->get_logger()->error("Expected semicolon after condition",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat ;

  auto increment_expression =
      expression_statement_parser.parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TOKEN_RIGHT_PARENTHESES) {
    parser->get_logger()->error("Expected right parentheses after expression",
                                LogTypes::Error::SYNTAX);
  }

  parser->eat_token();  // eat )

  if (parser->current_token.type != TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected left brace after expressions",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto body_statement = BlockStatementParser(parser).parse();
  if (!body_statement) {
    parser->get_logger()->error("Failed to parse the body of the for statement",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_shared<ForStatement>(
      position, init_statement, condition_expression,
      increment_expression, body_statement);
}
