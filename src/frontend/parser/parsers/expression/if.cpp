#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expression/if.hpp"
#include "frontend/parser/parsers/expression/group.hpp"
#include "frontend/parser/parsers/expression/if.hpp"
#include "frontend/parser/parsers/statement/block.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

IfExpressionParser::IfExpressionParser(const shared_ptr<Parser> &parser) {
  this->parser = parser;
}

unique_ptr<Expression> IfExpressionParser::parse() {
  parser->eat_token(); // eat if

  if (parser->current_token.type != TokenType::TOKEN_LEFT_PARENTHESES) {
    parser->logger->error("Expected left parentheses after if",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto condition = GroupExpressionParser(parser).parse();

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    parser->logger->error("Expected left brace after condition",
                          LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto block_statement_parser = BlockStatementParser(parser);
  auto consequence = block_statement_parser.parse();

  unique_ptr<BlockStatement> alternative = nullptr;
  if (parser->current_token.type == TokenType::TOKEN_ELSE) {
    parser->eat_token(); // eat else

    if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
      parser->logger->error("Expected left brace after else",
                            LogTypes::Error::SYNTAX);
      return nullptr;
    }

    alternative = block_statement_parser.parse();
  }

  return make_unique<IfExpression>(std::move(condition), std::move(consequence),
                                   std::move(alternative));
}
