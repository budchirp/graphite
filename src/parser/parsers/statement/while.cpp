
#include "parser/parsers/statement/while.hpp"

#include <memory>

#include "parser/parsers/statement/block.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "parser/precedence.hpp"

unique_ptr<WhileStatement> WhileStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat for

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    parser->get_logger()->error("Expected left parentheses after for keyword",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  parser->eat_token();  // eat (

  auto condition =
      ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);

  if (parser->current_token.type != TokenType::TOKEN_RIGHT_PARENTHESES) {
    parser->get_logger()->error("Expected right parentheses after condition",
                                LogTypes::Error::SYNTAX);
  }

  parser->eat_token();  // eat )

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected left brace after right parantheses",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto body = BlockStatementParser(parser).parse();
  if (!body) {
    parser->get_logger()->error("Failed to parse the body of the for statement",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_unique<WhileStatement>(position, std::move(condition),
                                     std::move(body));
}
