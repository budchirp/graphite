#include "parser/parsers/statement/return.hpp"

#include <memory>

#include "logger/log_types.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "parser/precedence.hpp"
#include "types/void.hpp"

unique_ptr<ReturnStatement> ReturnStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat return

  auto expression =
      ExpressionStatementParser(parser).parse_expression(Precedence::LOWEST);
  if (!expression) {
    parser->get_logger()->error(
        "Failed to parse expression of the return statement",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_unique<ReturnStatement>(position, make_shared<VoidType>(),
                                      std::move(expression));
}
